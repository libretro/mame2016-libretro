// license:BSD-3-Clause
// copyright-holders:Nicola Salmoria, Aaron Giles
/***************************************************************************

    mame.c

    Controls execution of the core MAME system.

***************************************************************************/

#include "emu.h"
#include "mame.h"
#include "emuopts.h"
#include "mameopts.h"
#include "pluginopts.h"
#include "osdepend.h"
#include "validity.h"
#include "clifront.h"
#include "drivenum.h"
#include "luaengine.h"
#include <time.h>
#include "ui/ui.h"
#include "ui/selgame.h"
#include "ui/simpleselgame.h"
#include "cheat.h"
#include "ui/datfile.h"
#include "ui/inifile.h"
#include "xmlfile.h"

//**************************************************************************
//  MACHINE MANAGER
//**************************************************************************

mame_machine_manager* mame_machine_manager::m_manager = nullptr;

mame_machine_manager* mame_machine_manager::instance(emu_options &options,osd_interface &osd)
{
	if(!m_manager)
	{
		m_manager = global_alloc(mame_machine_manager(options,osd));
	}
	return m_manager;
}

mame_machine_manager* mame_machine_manager::instance()
{
	return m_manager;
}

//-------------------------------------------------
//  mame_machine_manager - constructor
//-------------------------------------------------

mame_machine_manager::mame_machine_manager(emu_options &options,osd_interface &osd)
		: machine_manager(options, osd),
		m_plugins(std::make_unique<plugin_options>()),
		m_lua(global_alloc(lua_engine)),
		m_new_driver_pending(nullptr),
		m_firstrun(true),
		m_autoboot_timer(nullptr)
{
}


//-------------------------------------------------
//  ~mame_machine_manager - destructor
//-------------------------------------------------

mame_machine_manager::~mame_machine_manager()
{
	global_free(m_lua);
	m_manager = nullptr;
}


/***************************************************************************
    GLOBAL VARIABLES
***************************************************************************/

//-------------------------------------------------
//  mame_schedule_new_driver - schedule a new game to
//  be loaded
//-------------------------------------------------

void mame_machine_manager::schedule_new_driver(const game_driver &driver)
{
	m_new_driver_pending = &driver;
}


/***************************************************************************
    CORE IMPLEMENTATION
***************************************************************************/
void mame_machine_manager::update_machine()
{
	m_lua->set_machine(m_machine);
	m_lua->attach_notifiers();
}


std::vector<std::string> split(const std::string &text, char sep)
{
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
		std::string temp = text.substr(start, end - start);
		if (temp != "") tokens.push_back(temp);
		start = end + 1;
	}
	std::string temp = text.substr(start);
	if (temp != "") tokens.push_back(temp);
	return tokens;
}

void mame_machine_manager::start_luaengine()
{
	if (options().plugins())
	{
		path_iterator iter(options().plugins_path());
		std::string pluginpath;
		while (iter.next(pluginpath))
		{
			m_plugins->parse_json(pluginpath);
		}
		std::vector<std::string> include = split(options().plugin(),',');
		std::vector<std::string> exclude = split(options().no_plugin(),',');
		{
			// parse the file
			std::string error;
			// attempt to open the output file
			emu_file file(options().ini_path(), OPEN_FLAG_READ);
			if (file.open("plugin.ini") == osd_file::error::NONE)
			{
				bool result = m_plugins->parse_ini_file((util::core_file&)file, OPTION_PRIORITY_MAME_INI, OPTION_PRIORITY_DRIVER_INI, error);
				if (!result)
					osd_printf_error("**Error loading plugin.ini**");
			}
		}
		for (auto &curentry : *m_plugins)
		{
			if (!curentry.is_header())
			{
				if (std::find(include.begin(), include.end(), curentry.name()) != include.end())
				{
					std::string error_string;
					m_plugins->set_value(curentry.name(), "1", OPTION_PRIORITY_CMDLINE, error_string);
				}
				if (std::find(exclude.begin(), exclude.end(), curentry.name()) != exclude.end())
				{
					std::string error_string;
					m_plugins->set_value(curentry.name(), "0", OPTION_PRIORITY_CMDLINE, error_string);
				}
			}
		}
	}
	m_lua->initialize();

	{
		emu_file file(options().plugins_path(), OPEN_FLAG_READ);
		osd_file::error filerr = file.open("boot.lua");
		if (filerr == osd_file::error::NONE)
		{
			m_lua->load_script(file.fullpath());
		}
	}
}

#ifdef __LIBRETRO__

extern mame_machine_manager *retro_manager;
static running_machine *retro_global_machine;
static const machine_config *retro_global_config;

int ENDEXEC=0;

static bool firstgame = true;
bool started_empty = false;

#endif

/*-------------------------------------------------
    execute - run the core emulation
-------------------------------------------------*/

int mame_machine_manager::execute()
{
#ifndef __LIBRETRO__
	bool started_empty = false;

	bool firstgame = true;
#endif
	// loop across multiple hard resets
	bool exit_pending = false;
	int error = EMU_ERR_NONE;

	if (m_options.console()) {
		m_lua->start_console();
	}

	while (error == EMU_ERR_NONE && !exit_pending)
	{
		m_new_driver_pending = nullptr;

		// if no driver, use the internal empty driver
		const game_driver *system = mame_options::system(m_options);
		if (system == nullptr)
		{
			system = &GAME_NAME(___empty);
			if (firstgame)
				started_empty = true;
		}

		firstgame = false;

		// parse any INI files as the first thing
		if (m_options.read_config())
		{
			m_options.revert(OPTION_PRIORITY_INI);
			std::string errors;
			mame_options::parse_standard_inis(m_options,errors);
		}

		// otherwise, perform validity checks before anything else
		bool is_empty = (system == &GAME_NAME(___empty));
		if (!is_empty)
		{
			validity_checker valid(m_options);
			valid.set_verbose(false);
			valid.check_shared_source(*system);
		}

#ifdef __LIBRETRO__

		retro_global_config= global_alloc(machine_config(*system, m_options));

	        retro_global_machine=global_alloc(running_machine(*retro_global_config, *this));

		set_machine(&(*retro_global_machine));

		error = retro_global_machine->run(is_empty);
		m_firstrun = false;

		goto retro_handle;

#else
		// create the machine configuration
		machine_config config(*system, m_options);

		// create the machine structure and driver
		running_machine machine(config, *this);

		set_machine(&machine);

		// run the machine
		error = machine.run(is_empty);
		m_firstrun = false;

		// check the state of the machine
		if (m_new_driver_pending)
		{
			// set up new system name and adjust device options accordingly
			mame_options::set_system_name(m_options,m_new_driver_pending->name);
			m_firstrun = true;
		}
		else
		{
			if (machine.exit_pending()) mame_options::set_system_name(m_options,"");
		}

		if (machine.exit_pending() && (!started_empty || is_empty))
			exit_pending = true;

		// machine will go away when we exit scope
		set_machine(nullptr);
#endif
	}
	// return an error
	return error;

#ifdef __LIBRETRO__
retro_handle:
	return 1;
#endif
}

#ifdef __LIBRETRO__
extern int RLOOP,retro_pause;
extern void retro_loop(running_machine *machine);
extern void retro_execute();
extern core_options *retro_global_options;
int mfirst=0;
void mame_machine_manager::mmchange(){

	mfirst=0;
	// check the state of the machine
	if (m_new_driver_pending)
	{
	// set up new system name and adjust device options accordingly
		mame_options::set_system_name(m_options,m_new_driver_pending->name);
		m_firstrun = true;
		mfirst=1;
	}
	else
	{
		if (retro_global_machine->exit_pending()) mame_options::set_system_name(m_options,"");
	}

	//FIXME RETRO
	//if (retro_global_machine->exit_pending() && (!started_empty || (system == &GAME_NAME(___empty))))
	//exit_pending = true;
		

}

void free_machineconfig(){

		global_free(retro_global_machine);
		global_free(retro_global_config);

		retro_manager->set_machine(NULL);
}

extern void free_man();


void retro_finish(){
	printf("retro_finish begin\n");
	retro_global_machine->retro_machineexit();
	free_machineconfig();
	free_man();
	printf("retro_finish end\n");
}

void retro_main_loop()
{
	retro_global_machine->retro_loop();

	if(ENDEXEC==1){

		ENDEXEC=0;

		retro_manager->mmchange();

		if(mfirst == 1){
			//restart a new driver from UI
			retro_execute();
			return;
		}
		else{ 
			RLOOP=0;
			
			global_free(retro_global_machine);
			global_free(retro_global_config);
			retro_manager->set_machine(NULL);

			printf("exit scope, restart empty driver\n");
			//FIXME restart empty driver else it crash
			// we quit using retroarch (ESC or Menu)
			retro_execute();

		}

	}

}


#endif

TIMER_CALLBACK_MEMBER(mame_machine_manager::autoboot_callback)
{
	if (strlen(options().autoboot_script())!=0) {
		mame_machine_manager::instance()->lua()->load_script(options().autoboot_script());
	}
	else if (strlen(options().autoboot_command())!=0) {
		std::string cmd = std::string(options().autoboot_command());
		strreplace(cmd, "'", "\\'");
		std::string val = std::string("emu.keypost('").append(cmd).append("')");
		mame_machine_manager::instance()->lua()->load_string(val.c_str());
	}
}

void mame_machine_manager::reset()
{
	// setup autoboot if needed
	m_autoboot_timer->adjust(attotime(options().autoboot_delay(),0),0);
}

ui_manager* mame_machine_manager::create_ui(running_machine& machine)
{
	m_ui = std::make_unique<mame_ui_manager>(machine);
	m_ui->init();

	machine.add_notifier(MACHINE_NOTIFY_RESET, machine_notify_delegate(FUNC(mame_machine_manager::reset), this));

	m_ui->set_startup_text("Initializing...", true);

	return m_ui.get();
}

void mame_machine_manager::ui_initialize(running_machine& machine)
{
	m_ui->initialize(machine);

	// display the startup screens
	m_ui->display_startup_screens(m_firstrun);
}

void mame_machine_manager::create_custom(running_machine& machine)
{
	// start the inifile manager
	m_inifile = std::make_unique<inifile_manager>(machine, m_ui->options());

	// allocate autoboot timer
	m_autoboot_timer = machine.scheduler().timer_alloc(timer_expired_delegate(FUNC(mame_machine_manager::autoboot_callback), this));

	// start datfile manager
	m_datfile = std::make_unique<datfile_manager>(machine, m_ui->options());

	// start favorite manager
	m_favorite = std::make_unique<favorite_manager>(machine, m_ui->options());

	// set up the cheat engine
	m_cheat = std::make_unique<cheat_manager>(machine);
}

const char * emulator_info::get_bare_build_version() { return bare_build_version; }
const char * emulator_info::get_build_version() { return build_version; }

void emulator_info::display_ui_chooser(running_machine& machine)
{
	// force the UI to show the game select screen
	mame_ui_manager &mui = mame_machine_manager::instance()->ui();
	render_container *container = &machine.render().ui_container();
	if (machine.options().ui() == emu_options::UI_SIMPLE)
		ui::simple_menu_select_game::force_game_select(mui, container);
	else
		ui::menu_select_game::force_game_select(mui, container);
}

int emulator_info::start_frontend(emu_options &options, osd_interface &osd, int argc, char *argv[])
{
	cli_frontend frontend(options, osd);
	return frontend.execute(argc, argv);
}

void emulator_info::draw_user_interface(running_machine& machine)
{
	mame_machine_manager::instance()->ui().update_and_render(&machine.render().ui_container());
}

void emulator_info::periodic_check()
{
	mame_machine_manager::instance()->lua()->periodic_check();
}

bool emulator_info::frame_hook()
{
	return mame_machine_manager::instance()->lua()->frame_hook();
}

void emulator_info::layout_file_cb(xml_data_node &layout)
{
	xml_data_node *mamelayout = xml_get_sibling(layout.child, "mamelayout");
	if(mamelayout)
	{
		xml_data_node *script = xml_get_sibling(mamelayout->child, "script");
		if(script)
			mame_machine_manager::instance()->lua()->call_plugin(script->value, "layout");
	}
}

bool emulator_info::standalone() { return false; }

#include "options.h"
#include "osdepend.h"
#include "modules/lib/osdobj_common.h"

//============================================================
//  TYPE DEFINITIONS
//============================================================

class retro_osd_interface : public osd_common_t
{
public:
	// construction/destruction
	retro_osd_interface(osd_options &options);
	virtual ~retro_osd_interface();

	// general overridables
	virtual void init(running_machine &machine);
	virtual void update(bool skip_redraw);

	// input overridables
//	virtual void customize_input_type_list(input_type_desc *typelist);
	virtual void customize_input_type_list(simple_list<input_type_entry> &typelist);

private:
	virtual void osd_exit();
};

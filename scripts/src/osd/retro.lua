function maintargetosdoptions(_target)
end

function includeosd()
	includedirs {
		MAME_DIR .. "src/osd",
		MAME_DIR .. "src/osd/retro",
	}
end

forcedincludes {
	MAME_DIR .. "src/osd/retro/retroprefix.h"
}

newoption {
	trigger = "NO_USE_MIDI",
	description = "Disable MIDI I/O",
	allowed = {
	--	{ "0",  "Enable MIDI"  },
		{ "1",  "Disable MIDI" },
	},
}

configuration { "mingw*" }
		linkoptions {	
			"-shared ",
			"-Wl,--version-script=../../../../src/osd/retro/link.T",
			"-Wl,--no-undefined", 
		}

configuration { }

project ("osd_" .. _OPTIONS["osd"])
	uuid (os.uuid("osd_" .. _OPTIONS["osd"]))
	kind "StaticLib"

	removeflags {
		"SingleOutputDir",
	}

	options {
		"ForceCPP",
	}

   buildoptions {
 		"-fPIC"
	}

   linkoptions{
		"-shared -Wl,--version-script=" .. MAME_DIR .. "src/osd/retro/link.T -Wl,--no-undefined"
	}

	dofile("retro_cfg.lua")

	includedirs {
		MAME_DIR .. "3rdparty",
		MAME_DIR .. "3rdparty/bx/include",
		MAME_DIR .. "3rdparty/winpcap/Include",
		MAME_DIR .. "src/emu",
		MAME_DIR .. "src/lib",
		MAME_DIR .. "src/lib/util",
		MAME_DIR .. "src/osd",
		MAME_DIR .. "src/osd/modules/render",
		MAME_DIR .. "src/osd/retro",
		MAME_DIR .. "src/osd/retro/libretro-common/include",
	}

	files {
		MAME_DIR .. "src/osd/osdnet.cpp",
		MAME_DIR .. "src/osd/modules/netdev/taptun.cpp",
		MAME_DIR .. "src/osd/modules/netdev/pcap.cpp",
		MAME_DIR .. "src/osd/modules/netdev/none.cpp",
		MAME_DIR .. "src/osd/modules/debugger/debug_module.h",
		MAME_DIR .. "src/osd/modules/debugger/none.cpp",
		MAME_DIR .. "src/osd/modules/lib/osdobj_common.cpp",
		MAME_DIR .. "src/osd/modules/input/input_module.h",
		MAME_DIR .. "src/osd/modules/input/input_common.cpp",
		MAME_DIR .. "src/osd/modules/input/input_common.h",
		MAME_DIR .. "src/osd/modules/input/input_none.cpp",
		MAME_DIR .. "src/osd/modules/sound/none.cpp",
		MAME_DIR .. "src/osd/modules/sound/retro_sound.cpp",
		MAME_DIR .. "src/osd/retro/retromain.cpp",

		-- The public API in libretro.c is "unused" and tends to get
		-- stripped by the "helpful" linker, so we compile it into
		-- the top-level shared lib to avoid having to jump through
		-- quite as many hoops.
		-- MAME_DIR .. "src/osd/retro/libretro.c",
	}

	-- We don't support MIDI at present
	--if _OPTIONS["NO_USE_MIDI"]=="1" then
		defines {
			"NO_USE_MIDI",
		}

		files {
			MAME_DIR .. "src/osd/modules/midi/none.cpp",
		}
	--else
	--	files {
	--		MAME_DIR .. "src/osd/modules/midi/portmidi.c",
	--	}
	--end

	if _OPTIONS["NO_OPENGL"]=="1" then
		files {
			MAME_DIR .. "src/osd/retro/libretro-common/glsym/rglgen.c",
			MAME_DIR .. "src/osd/retro/libretro-common/glsym/glsym_gl.c",
		}
	end

project ("ocore_" .. _OPTIONS["osd"])
	uuid (os.uuid("ocore_" .. _OPTIONS["osd"]))
	kind "StaticLib"

	options {
		"ForceCPP",
	}

	removeflags {
		"SingleOutputDir",
	}

	dofile("retro_cfg.lua")

   buildoptions {
 		"-fPIC"
	}

   
	linkoptions{
		"-shared -Wl,--version-script=src/osd/retro/link.T -Wl,--no-undefined"
	}
		
	links {

	}

	includedirs {
		MAME_DIR .. "src/emu",
		MAME_DIR .. "src/osd",
		MAME_DIR .. "src/lib",
		MAME_DIR .. "src/lib/util",
		MAME_DIR .. "src/osd/retro",
		MAME_DIR .. "src/osd/retro/libretro-common/include",
	}

	if _OPTIONS["targetos"]=="linux" then
		BASE_TARGETOS = "unix"
		SDLOS_TARGETOS = "unix"
		SYNC_IMPLEMENTATION = "tc"
	end

	if _OPTIONS["targetos"]=="windows" then
		BASE_TARGETOS = "win32"
		SDLOS_TARGETOS = "win32"
		SYNC_IMPLEMENTATION = "windows"
	end

	if _OPTIONS["targetos"]=="macosx" then
		BASE_TARGETOS = "unix"
		SDLOS_TARGETOS = "macosx"
		SYNC_IMPLEMENTATION = "ntc"
	end

	files {
		MAME_DIR .. "src/osd/osdcore.cpp",
      MAME_DIR .. "src/osd/strconv.cpp",
      MAME_DIR .. "src/osd/strconv.h",
		MAME_DIR .. "src/osd/watchdog.cpp",
		MAME_DIR .. "src/osd/watchdog.h",
		MAME_DIR .. "src/osd/modules/font/font_none.cpp",
		MAME_DIR .. "src/osd/modules/lib/osdlib_retro.cpp",
		MAME_DIR .. "src/osd/modules/midi/none.cpp",
		MAME_DIR .. "src/osd/modules/osdmodule.cpp",
		MAME_DIR .. "src/osd/osdsync.cpp",
		MAME_DIR .. "src/osd/retro/retrodir.cpp",
		MAME_DIR .. "src/osd/modules/output/output_module.h",
		MAME_DIR .. "src/osd/modules/output/none.cpp",
		MAME_DIR .. "src/osd/modules/output/console.cpp",
	}

   if BASE_TARGETOS=="unix" then
		files {
			MAME_DIR .. "src/osd/modules/file/posixfile.cpp",
			MAME_DIR .. "src/osd/modules/file/posixfile.h",
			MAME_DIR .. "src/osd/modules/file/posixptty.cpp",
			MAME_DIR .. "src/osd/modules/file/posixsocket.cpp",
		}

	elseif BASE_TARGETOS=="win32" then
		includedirs {
			MAME_DIR .. "src/osd/windows",
		}
		files {
			MAME_DIR .. "src/osd/modules/file/winfile.cpp",
			MAME_DIR .. "src/osd/modules/file/winfile.h",
			MAME_DIR .. "src/osd/modules/file/winptty.cpp",
			MAME_DIR .. "src/osd/modules/file/winsocket.cpp",
			MAME_DIR .. "src/osd/windows/winutil.cpp", -- FIXME put the necessary functions somewhere more appropriate
		}
	else
		files {
			MAME_DIR .. "src/osd/modules/file/stdfile.cpp",
		}
	end



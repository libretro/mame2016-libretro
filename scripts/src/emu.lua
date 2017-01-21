-- license:BSD-3-Clause
-- copyright-holders:MAMEdev Team

---------------------------------------------------------------------------
--
--   emu.lua
--
--   Rules for building emu cores
--
---------------------------------------------------------------------------

project ("emu")
uuid ("e6fa15e4-a354-4526-acef-13c8e80fcacf")
kind (LIBTYPE)

addprojectflags()
precompiledheaders()
options {
	"ArchiveSplit",
}
includedirs {
	MAME_DIR .. "src/osd",
	MAME_DIR .. "src/emu",
	MAME_DIR .. "src/lib",
	MAME_DIR .. "src/lib/util",
	MAME_DIR .. "3rdparty",
	GEN_DIR  .. "emu",
	GEN_DIR  .. "emu/layout",
}

includedirs {
	ext_includedir("expat"),
	ext_includedir("lua"),
	ext_includedir("zlib"),
	ext_includedir("flac"),
}

files {
	MAME_DIR .. "src/emu/emu.h",
	MAME_DIR .. "src/emu/main.h",
	MAME_DIR .. "src/emu/gamedrv.h",
	MAME_DIR .. "src/emu/hashfile.cpp",
	MAME_DIR .. "src/emu/hashfile.h",
	MAME_DIR .. "src/emu/addrmap.cpp",
	MAME_DIR .. "src/emu/addrmap.h",
	MAME_DIR .. "src/emu/attotime.cpp",
	MAME_DIR .. "src/emu/attotime.h",
	MAME_DIR .. "src/emu/bookkeeping.cpp",
	MAME_DIR .. "src/emu/bookkeeping.h",
	MAME_DIR .. "src/emu/config.cpp",
	MAME_DIR .. "src/emu/config.h",
	MAME_DIR .. "src/emu/crsshair.cpp",
	MAME_DIR .. "src/emu/crsshair.h",
	MAME_DIR .. "src/emu/debugger.cpp",
	MAME_DIR .. "src/emu/debugger.h",
	MAME_DIR .. "src/emu/devdelegate.cpp",
	MAME_DIR .. "src/emu/devdelegate.h",
	MAME_DIR .. "src/emu/devcb.cpp",
	MAME_DIR .. "src/emu/devcb.h",
	MAME_DIR .. "src/emu/devcpu.cpp",
	MAME_DIR .. "src/emu/devcpu.h",
	MAME_DIR .. "src/emu/devfind.cpp",
	MAME_DIR .. "src/emu/devfind.h",
	MAME_DIR .. "src/emu/device.cpp",
	MAME_DIR .. "src/emu/device.h",
	MAME_DIR .. "src/emu/device.ipp",
	MAME_DIR .. "src/emu/didisasm.cpp",
	MAME_DIR .. "src/emu/didisasm.h",
	MAME_DIR .. "src/emu/diexec.cpp",
	MAME_DIR .. "src/emu/diexec.h",
	MAME_DIR .. "src/emu/digfx.cpp",
	MAME_DIR .. "src/emu/digfx.h",
	MAME_DIR .. "src/emu/diimage.cpp",
	MAME_DIR .. "src/emu/diimage.h",
	MAME_DIR .. "src/emu/dimemory.cpp",
	MAME_DIR .. "src/emu/dimemory.h",
	MAME_DIR .. "src/emu/dinetwork.cpp",
	MAME_DIR .. "src/emu/dinetwork.h",
	MAME_DIR .. "src/emu/dinvram.cpp",
	MAME_DIR .. "src/emu/dinvram.h",
	MAME_DIR .. "src/emu/dioutput.cpp",
	MAME_DIR .. "src/emu/dioutput.h",
	MAME_DIR .. "src/emu/dipty.cpp",
	MAME_DIR .. "src/emu/dipty.h",
	MAME_DIR .. "src/emu/dirtc.cpp",
	MAME_DIR .. "src/emu/dirtc.h",
	MAME_DIR .. "src/emu/diserial.cpp",
	MAME_DIR .. "src/emu/diserial.h",
	MAME_DIR .. "src/emu/dislot.cpp",
	MAME_DIR .. "src/emu/dislot.h",
	MAME_DIR .. "src/emu/disound.cpp",
	MAME_DIR .. "src/emu/disound.h",
	MAME_DIR .. "src/emu/dispatch.cpp",
	MAME_DIR .. "src/emu/dispatch.h",
	MAME_DIR .. "src/emu/distate.cpp",
	MAME_DIR .. "src/emu/distate.h",
	MAME_DIR .. "src/emu/divideo.cpp",
	MAME_DIR .. "src/emu/divideo.h",
	MAME_DIR .. "src/emu/divtlb.cpp",
	MAME_DIR .. "src/emu/divtlb.h",
	MAME_DIR .. "src/emu/drawgfx.cpp",
	MAME_DIR .. "src/emu/drawgfx.h",
	MAME_DIR .. "src/emu/drawgfxm.h",
	MAME_DIR .. "src/emu/driver.cpp",
	MAME_DIR .. "src/emu/driver.h",
	MAME_DIR .. "src/emu/drivenum.cpp",
	MAME_DIR .. "src/emu/drivenum.h",
	MAME_DIR .. "src/emu/emualloc.cpp",
	MAME_DIR .. "src/emu/emualloc.h",
	MAME_DIR .. "src/emu/emucore.cpp",
	MAME_DIR .. "src/emu/emucore.h",
	MAME_DIR .. "src/emu/emumem.cpp",
	MAME_DIR .. "src/emu/emumem.h",	
	MAME_DIR .. "src/emu/emuopts.cpp",
	MAME_DIR .. "src/emu/emuopts.h",
	MAME_DIR .. "src/emu/emupal.cpp",
	MAME_DIR .. "src/emu/emupal.h",
	MAME_DIR .. "src/emu/fileio.cpp",
	MAME_DIR .. "src/emu/fileio.h",
	MAME_DIR .. "src/emu/hash.cpp",
	MAME_DIR .. "src/emu/hash.h",
	MAME_DIR .. "src/emu/image.cpp",
	MAME_DIR .. "src/emu/image.h",
	MAME_DIR .. "src/emu/input.cpp",
	MAME_DIR .. "src/emu/input.h",
	MAME_DIR .. "src/emu/ioport.cpp",
	MAME_DIR .. "src/emu/ioport.h",
	MAME_DIR .. "src/emu/inpttype.h",
	MAME_DIR .. "src/emu/machine.cpp",
	MAME_DIR .. "src/emu/machine.h",
	MAME_DIR .. "src/emu/mconfig.cpp",
	MAME_DIR .. "src/emu/mconfig.h",
	MAME_DIR .. "src/emu/memarray.cpp",
	MAME_DIR .. "src/emu/memarray.h",
	MAME_DIR .. "src/emu/network.cpp",
	MAME_DIR .. "src/emu/network.h",
	MAME_DIR .. "src/emu/parameters.cpp",
	MAME_DIR .. "src/emu/parameters.h",
	MAME_DIR .. "src/emu/profiler.cpp",
	MAME_DIR .. "src/emu/profiler.h",
	MAME_DIR .. "src/emu/output.cpp",
	MAME_DIR .. "src/emu/output.h",
	MAME_DIR .. "src/emu/render.cpp",
	MAME_DIR .. "src/emu/render.h",
	MAME_DIR .. "src/emu/rendfont.cpp",
	MAME_DIR .. "src/emu/rendfont.h",
	MAME_DIR .. "src/emu/rendlay.cpp",
	MAME_DIR .. "src/emu/rendlay.h",
	MAME_DIR .. "src/emu/rendutil.cpp",
	MAME_DIR .. "src/emu/rendutil.h",
	MAME_DIR .. "src/emu/romload.cpp",
	MAME_DIR .. "src/emu/romload.h",
	MAME_DIR .. "src/emu/save.cpp",
	MAME_DIR .. "src/emu/save.h",
	MAME_DIR .. "src/emu/schedule.cpp",
	MAME_DIR .. "src/emu/schedule.h",
	MAME_DIR .. "src/emu/screen.cpp",
	MAME_DIR .. "src/emu/screen.h",
	MAME_DIR .. "src/emu/softlist.cpp",
	MAME_DIR .. "src/emu/softlist.h",
	MAME_DIR .. "src/emu/sound.cpp",
	MAME_DIR .. "src/emu/sound.h",
	MAME_DIR .. "src/emu/speaker.cpp",
	MAME_DIR .. "src/emu/speaker.h",
	MAME_DIR .. "src/emu/tilemap.cpp",
	MAME_DIR .. "src/emu/tilemap.h",
	MAME_DIR .. "src/emu/timer.cpp",
	MAME_DIR .. "src/emu/timer.h",
	MAME_DIR .. "src/emu/uiinput.cpp",
	MAME_DIR .. "src/emu/uiinput.h",
	MAME_DIR .. "src/emu/validity.cpp",
	MAME_DIR .. "src/emu/validity.h",
	MAME_DIR .. "src/emu/video.cpp",
	MAME_DIR .. "src/emu/video.h",
	MAME_DIR .. "src/emu/rendersw.hxx",
	MAME_DIR .. "src/emu/ui/uimain.h",
	MAME_DIR .. "src/emu/ui/cmdrender.h", -- TODO: remove
	MAME_DIR .. "src/emu/ui/cmddata.h",   -- TODO: remove
	MAME_DIR .. "src/emu/debug/debugcmd.cpp",
	MAME_DIR .. "src/emu/debug/debugcmd.h",
	MAME_DIR .. "src/emu/debug/debugcon.cpp",
	MAME_DIR .. "src/emu/debug/debugcon.h",
	MAME_DIR .. "src/emu/debug/debugcpu.cpp",
	MAME_DIR .. "src/emu/debug/debugcpu.h",
	MAME_DIR .. "src/emu/debug/debughlp.cpp",
	MAME_DIR .. "src/emu/debug/debughlp.h",
	MAME_DIR .. "src/emu/debug/debugvw.cpp",
	MAME_DIR .. "src/emu/debug/debugvw.h",
	MAME_DIR .. "src/emu/debug/dvdisasm.cpp",
	MAME_DIR .. "src/emu/debug/dvdisasm.h",
	MAME_DIR .. "src/emu/debug/dvmemory.cpp",
	MAME_DIR .. "src/emu/debug/dvmemory.h",
	MAME_DIR .. "src/emu/debug/dvbpoints.cpp",
	MAME_DIR .. "src/emu/debug/dvbpoints.h",
	MAME_DIR .. "src/emu/debug/dvwpoints.cpp",
	MAME_DIR .. "src/emu/debug/dvwpoints.h",
	MAME_DIR .. "src/emu/debug/dvstate.cpp",
	MAME_DIR .. "src/emu/debug/dvstate.h",
	MAME_DIR .. "src/emu/debug/dvtext.cpp",
	MAME_DIR .. "src/emu/debug/dvtext.h",
	MAME_DIR .. "src/emu/debug/express.cpp",
	MAME_DIR .. "src/emu/debug/express.h",
	MAME_DIR .. "src/emu/debug/textbuf.cpp",
	MAME_DIR .. "src/emu/debug/textbuf.h",
	MAME_DIR .. "src/emu/drivers/empty.cpp",
	MAME_DIR .. "src/emu/drivers/xtal.h",
	MAME_DIR .. "src/emu/video/generic.cpp",
	MAME_DIR .. "src/emu/video/generic.h",
	MAME_DIR .. "src/emu/video/resnet.cpp",
	MAME_DIR .. "src/emu/video/resnet.h",
	MAME_DIR .. "src/emu/video/rgbutil.h",
	MAME_DIR .. "src/emu/video/rgbgen.cpp",
	MAME_DIR .. "src/emu/video/rgbgen.h",
	MAME_DIR .. "src/emu/video/rgbsse.cpp",
	MAME_DIR .. "src/emu/video/rgbsse.h",
	MAME_DIR .. "src/emu/video/rgbvmx.cpp",
	MAME_DIR .. "src/emu/video/rgbvmx.h",
}

dependency {
	--------------------------------------------------
	-- additional dependencies
	--------------------------------------------------
	{ MAME_DIR .. "src/emu/rendfont.cpp", GEN_DIR .. "emu/uismall.fh" },
	{ MAME_DIR .. "src/emu/rendfont.cpp", GEN_DIR .. "emu/ui/uicmd14.fh" },
	-------------------------------------------------
	-- core layouts
	--------------------------------------------------
	{ MAME_DIR .. "src/emu/rendlay.cpp", GEN_DIR .. "emu/layout/dualhovu.lh" },
	{ MAME_DIR .. "src/emu/rendlay.cpp", GEN_DIR .. "emu/layout/dualhsxs.lh" },
	{ MAME_DIR .. "src/emu/rendlay.cpp", GEN_DIR .. "emu/layout/dualhuov.lh" },
	{ MAME_DIR .. "src/emu/rendlay.cpp", GEN_DIR .. "emu/layout/horizont.lh" },
	{ MAME_DIR .. "src/emu/rendlay.cpp", GEN_DIR .. "emu/layout/triphsxs.lh" },
	{ MAME_DIR .. "src/emu/rendlay.cpp", GEN_DIR .. "emu/layout/quadhsxs.lh" },
	{ MAME_DIR .. "src/emu/rendlay.cpp", GEN_DIR .. "emu/layout/vertical.lh" },
	{ MAME_DIR .. "src/emu/rendlay.cpp", GEN_DIR .. "emu/layout/lcd.lh" },
	{ MAME_DIR .. "src/emu/rendlay.cpp", GEN_DIR .. "emu/layout/lcd_rot.lh" },
	{ MAME_DIR .. "src/emu/rendlay.cpp", GEN_DIR .. "emu/layout/svg.lh" },
	{ MAME_DIR .. "src/emu/rendlay.cpp", GEN_DIR .. "emu/layout/noscreens.lh" },

	{ MAME_DIR .. "src/emu/video.cpp",   GEN_DIR .. "emu/layout/snap.lh" },

}

custombuildtask {
	{ MAME_DIR .. "src/osd/retro/ui.bdc", GEN_DIR .. "emu/uismall.fh",     {  MAME_DIR .. "scripts/build/file2str.py" }, {"@echo Converting ui.bdc...", PYTHON .. " $(1) $(<) $(@) font_uismall uint8_t" }},
}

custombuildtask {
	{ MAME_DIR .. "src/frontend/mame/ui/uicmd14.png"        , GEN_DIR .. "emu/ui/uicmd14.fh",  {  MAME_DIR.. "scripts/build/png2bdc.py",  MAME_DIR .. "scripts/build/file2str.py" }, {"@echo Converting uicmd14.png...", PYTHON .. " $(1) $(<) temp_cmd.bdc", PYTHON .. " $(2) temp_cmd.bdc $(@) font_uicmd14 UINT8" }},

	layoutbuildtask("emu/layout", "dualhovu"),
	layoutbuildtask("emu/layout", "dualhsxs"),
	layoutbuildtask("emu/layout", "dualhuov"),
	layoutbuildtask("emu/layout", "horizont"),
	layoutbuildtask("emu/layout", "triphsxs"),
	layoutbuildtask("emu/layout", "quadhsxs"),
	layoutbuildtask("emu/layout", "vertical"),
	layoutbuildtask("emu/layout", "lcd"),
	layoutbuildtask("emu/layout", "lcd_rot"),
	layoutbuildtask("emu/layout", "svg"),
	layoutbuildtask("emu/layout", "noscreens"),
	layoutbuildtask("emu/layout", "snap"),
}

project ("precompile")
uuid ("a6fb15d4-b123-4445-acef-13c8e80fcacf")
kind (LIBTYPE)

addprojectflags()
precompiledheaders()

includedirs {
	MAME_DIR .. "src/osd",
	MAME_DIR .. "src/emu",
	MAME_DIR .. "src/lib/util",
}
files {
	MAME_DIR .. "src/emu/drivers/empty.cpp",
}
dependency {
	{ "$(OBJDIR)/src/emu/drivers/empty.o", "$(GCH)", true  },
}



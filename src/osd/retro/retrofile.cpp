#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

// MAME headers
#include "osdcore.h"
#include "retroos.h"

//============================================================
//  osd_uchar_from_osdchar
//============================================================

int osd_uchar_from_osdchar(UINT32 /* unicode_char */ *uchar, const char *osdchar, size_t count)
{
	// we assume a standard 1:1 mapping of characters to the first 256 unicode characters
	*uchar = (UINT8)*osdchar;
	return 1;
}

/*
//============================================================
//  osd_openpty
//============================================================

osd_file::error osd_file::openpty(ptr &file, std::string &name)
{
	return error::FAILURE;
}
*/

//============================================================
//  osd_subst_env
//============================================================
void osd_subst_env(std::string &dst, const std::string &src)
{
	dst = src;
}

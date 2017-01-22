// license:BSD-3-Clause
// copyright-holders:Aaron Giles
//============================================================
//
//  retroprefix.h - Libretro prefix file, included by ALL files
//
//============================================================

#if defined(__ANDROID__)
#include <math.h>
#include <stdarg.h>
#ifdef __cplusplus
#include <cstddef>
#include <cstdlib>
#endif

#ifndef UINT64_C
#define UINT64_C(c) (c ## ULL)
#endif

//static inline double log2(double x) { return log(x) * M_LOG2E; }
#undef _C
#define SDLMAME_ARM 1
#undef PAGE_MASK
#undef si_status
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501 // Windows XP
#endif

#if defined(_WIN32)
#define SDLMAME_WIN32 1
#endif

#ifdef _MSC_VER
#include <malloc.h>
#if _MSC_VER < 1900 // < VS2015
#define snprintf _snprintf
#if _MSC_VER < 1800 // VS2013 or earlier
#define alloca _alloca
#define round(x) floor((x) + 0.5)
#define strtoll _strtoi64
#define _USE_MATH_DEFINES
#include <math.h>
static __inline double fmin(double x, double y){ return (x < y) ? x : y; }
static __inline double fmax(double x, double y){ return (x > y) ? x : y; }
static __inline double log2(double x) { return log(x) * M_LOG2E; }
#if _MSC_VER < 1500 // VS2008 or earlier
#define vsnprintf _vsnprintf
#endif // VS2008
#endif // VS2013
#else // VS2015
#pragma warning (disable: 4091)
#pragma warning (disable: 4267)
#pragma warning (disable: 4456 4457 4458 4459)
#pragma warning (disable: 4463)
#pragma warning (disable: 4838)
#pragma warning (disable: 5025 5026 5027)
#define _CRT_STDIO_LEGACY_WIDE_SPECIFIERS
#endif
#elif defined(ANDROID)
#include <math.h>
#endif

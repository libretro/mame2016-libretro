#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#ifdef SDLMAME_LINUX
#define __USE_LARGEFILE64
#endif
#ifndef SDLMAME_BSD
#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#endif
#define _XOPEN_SOURCE 500
#endif

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

// MAME headers
#include "../osdcore.h"
#include "../osdcomm.h"
#include "../modules/file/posixfile.h"
#include "retroos.h"


//============================================================
//  GLOBAL IDENTIFIERS
//============================================================

#if 0
extern const char *sdlfile_socket_identifier;
extern const char *sdlfile_ptty_identifier;
#endif

//============================================================
//  CONSTANTS
//============================================================

#if defined(_WIN32)
#define PATHSEPCH '\\'
#define INVPATHSEPCH '/'
#else
#define PATHSEPCH '/'
#define INVPATHSEPCH '\\'
#endif

#define NO_ERROR    (0)

//============================================================
//  Prototypes
//============================================================

static std::uint32_t create_path_recursive(char *path);

//============================================================
//  error_to_file_error
//  (does filling this out on non-Windows make any sense?)
//============================================================
//
osd_file::error osd_openpty(osd_file::ptr *file, char *name, size_t name_len)
{
   return osd_file::error::NONE;
}

osd_file::error error_to_file_error(std::uint32_t error)
{
   switch (error)
   {
      case ENOENT:
      case ENOTDIR:
         return osd_file::error::NOT_FOUND;

      case EACCES:
      case EROFS:
#ifndef _WIN32
      case ETXTBSY:
#endif
      case EEXIST:
      case EPERM:
      case EISDIR:
      case EINVAL:
         return osd_file::error::ACCESS_DENIED;

      case ENFILE:
      case EMFILE:
         return osd_file::error::TOO_MANY_FILES;
   }

   return osd_file::error::FAILURE;
}


//============================================================
//  osd_open
//============================================================

osd_file::error osd_file::open(const char *path, std::uint32_t openflags, osd_file::ptr *file, UINT64 *filesize)
{
   std::uint32_t access;
   const char *src;
   char *dst;
#if defined(__MACH__) || defined(_WIN32) ||  defined(SDLMAME_NO64BITIO) || defined(SDLMAME_ARM) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2) || defined(SDLMAME_HAIKU)
   struct stat st;
#else
   struct stat64 st;
#endif
   char *tmpstr, *envstr;
   int i, j;
   osd_file::error filerr = osd_file::error::NONE;

   tmpstr = NULL;

   // allocate a file object, plus space for the converted filename
   *file = (osd_file::ptr) osd_malloc_array(sizeof(**file) + sizeof(char) * strlen(path));
   if (*file == NULL)
   {
      filerr = osd_file::error::OUT_OF_MEMORY;
      goto error;
   }

#if 0
   if (sdl_check_socket_path(path))
   {
      (*file)->type = SDLFILE_SOCKET;
      filerr = sdl_open_socket(path, openflags, file, filesize);
      goto error;
   }

   if (strlen(sdlfile_ptty_identifier) > 0 && strncmp(path, sdlfile_ptty_identifier, strlen(sdlfile_ptty_identifier)) == 0)
   {
      (*file)->type = SDLFILE_PTTY;
      filerr = sdl_open_ptty(path, openflags, file, filesize);
      goto error;
   }
#endif

   (*file)->type = SDLFILE_FILE;

   /* convert the path into something compatible */
   dst = (*file)->filename;
   for (src = path; *src != 0; src++)
      *dst++ = (*src == INVPATHSEPCH) ? PATHSEPCH : *src;
   *dst++ = 0;

   /* select the file open modes */
   if (openflags & OPEN_FLAG_WRITE)
   {
      access = (openflags & OPEN_FLAG_READ) ? O_RDWR : O_WRONLY;
      access |= (openflags & OPEN_FLAG_CREATE) ? (O_CREAT | O_TRUNC) : 0;
   }
   else if (openflags & OPEN_FLAG_READ)
   {
      access = O_RDONLY;
   }
   else
   {
      filerr = osd_file::error::INVALID_ACCESS;
      goto error;
   }

   tmpstr = (char *) osd_malloc_array(strlen((*file)->filename)+1);
   strcpy(tmpstr, (*file)->filename);

   /* does path start with an environment variable? */
   if (tmpstr[0] == '$')
   {
      char *envval;
      envstr = (char *) osd_malloc_array(strlen(tmpstr)+1);

      strcpy(envstr, tmpstr);

      i = 0;
      while (envstr[i] != PATHSEPCH && envstr[i] != 0 && envstr[i] != '.')
      {
         i++;
      }

      envstr[i] = '\0';

      envval = (char*)osd_getenv(&envstr[1]);

      if (envval)
      {
         j = strlen(envval) + strlen(tmpstr) + 1;
         osd_free(tmpstr);
         tmpstr = (char *) osd_malloc_array(j);

         // start with the value of $HOME
         strcpy(tmpstr, envval);
         // replace the null with a path separator again
         envstr[i] = PATHSEPCH;
         // append it
         strcat(tmpstr, &envstr[i]);
      }
      else
         fprintf(stderr, "Warning: osd_file::open environment variable %s not found.\n", envstr);
      osd_free(envstr);
   }

#if defined(_WIN32) || defined(SDLMAME_OS2)
   access |= O_BINARY;
#endif

   // attempt to open the file
#if defined(__MACH__) || defined(_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_ARM) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2) || defined(SDLMAME_HAIKU)
   (*file)->handle = open(tmpstr, access, 0666);
#else
   (*file)->handle = open64(tmpstr, access, 0666);
#endif
   if ((*file)->handle == -1)
   {
      // create the path if necessary
      if ((openflags & OPEN_FLAG_CREATE) && (openflags & OPEN_FLAG_CREATE_PATHS))
      {
         char *pathsep = strrchr(tmpstr, PATHSEPCH);

         if (pathsep)
         {
            int error;

            // create the path up to the file
            *pathsep = 0;
            error = create_path_recursive(tmpstr);
            *pathsep = PATHSEPCH;

            // attempt to reopen the file
            if (error == NO_ERROR)
            {
#if defined(__MACH__) || defined(_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_ARM) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2) || defined(SDLMAME_HAIKU)
               (*file)->handle = open(tmpstr, access, 0666);
#else
               (*file)->handle = open64(tmpstr, access, 0666);
#endif
            }
         }
      }

      // if we still failed, clean up and osd_free
      if ((*file)->handle == -1)
      {
         osd_free(*file);
         *file = NULL;
         osd_free(tmpstr);
         return error_to_file_error(errno);
      }
   }

   // get the file size
#if defined(__MACH__) || defined(_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_ARM) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2) || defined(SDLMAME_HAIKU)
   fstat((*file)->handle, &st);
#else
   fstat64((*file)->handle, &st);
#endif

   *filesize = (UINT64)st.st_size;


error:
   // cleanup
   if (filerr != osd_file::error::NONE && *file != NULL)
   {
      osd_free(*file);
      *file = NULL;
   }
   if (tmpstr)
      osd_free(tmpstr);
   return filerr;
}


//============================================================
//  osd_read
//============================================================

osd_file::error osd_read(osd_file::ptr file, void *buffer, UINT64 offset, std::uint32_t count, std::uint32_t *actual)
{
   ssize_t result;

   switch (file->type)
   {
      case SDLFILE_FILE:
#if defined(__MACH__) || defined(SDLMAME_BSD)
         result = pread(file->handle, buffer, count, offset);
         if (result < 0)
#elif defined(_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_ARM) || defined(SDLMAME_OS2)
            lseek(file->handle, (std::uint32_t)offset&0xffffffff, SEEK_SET);
         result = read(file->handle, buffer, count);
         if (result < 0)
#elif defined(SDLMAME_UNIX)
            result = pread64(file->handle, buffer, count, offset);
         if (result < 0)
#else
#error Unknown SDL SUBARCH!
#endif
            return error_to_file_error(errno);

         if (actual != NULL)
            *actual = result;

         return osd_file::error::NONE;
         break;
#if 0
      case SDLFILE_SOCKET:
         return sdl_read_socket(file, buffer, offset, count, actual);
         break;

      case SDLFILE_PTTY:
         return sdl_read_ptty(file, buffer, offset, count, actual);
         break;
#endif
   }

   return osd_file::error::FAILURE;
}


//============================================================
//  osd_write
//============================================================

osd_file::error osd_write(osd_file::ptr file, const void *buffer, UINT64 offset, std::uint32_t count, std::uint32_t *actual)
{
   std::uint32_t result;

   switch (file->type)
   {
      case SDLFILE_FILE:
#if defined(__MACH__) || defined(SDLMAME_BSD)
         result = pwrite(file->handle, buffer, count, offset);
         if (!result)
#elif defined(_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_ARM) || defined(SDLMAME_OS2)
            lseek(file->handle, (std::uint32_t)offset&0xffffffff, SEEK_SET);
         result = write(file->handle, buffer, count);
         if (!result)
#elif defined(SDLMAME_UNIX)
            result = pwrite64(file->handle, buffer, count, offset);
         if (!result)
#else
#error Unknown SDL SUBARCH!
#endif
            return error_to_file_error(errno);

         if (actual != NULL)
            *actual = result;
         return osd_file::error::NONE;
         break;
#if 0
      case SDLFILE_SOCKET:
         return sdl_write_socket(file, buffer, offset, count, actual);
         break;

      case SDLFILE_PTTY:
         return sdl_write_ptty(file, buffer, offset, count, actual);
         break;
#endif
      default:
         return osd_file::error::FAILURE;
   }
}


//============================================================
//  osd_close
//============================================================

osd_file::error osd_close(osd_file::ptr file)
{
   // close the file handle and free the file structure
   switch (file->type)
   {
      case SDLFILE_FILE:
         close(file->handle);
         osd_free(file);
         return osd_file::error::NONE;
         break;
#if 0
      case SDLFILE_SOCKET:
         return sdl_close_socket(file);
         break;

      case SDLFILE_PTTY:
         return sdl_close_ptty(file);
         break;
#endif
      default:
         return osd_file::error::FAILURE;
   }
}

//============================================================
//  osd_rmfile
//============================================================

osd_file::error osd_rmfile(const char *filename)
{
	if (unlink(filename) == -1)
		return error_to_file_error(errno);

	return osd_file::error::NONE;
}

//============================================================
//  create_path_recursive
//============================================================

static std::uint32_t create_path_recursive(char *path)
{
	std::uint32_t filerr;
	struct stat st;
	char *sep = strrchr(path, PATHSEPCH);

	/* if there's still a separator, 
    * and it's not the root, nuke it and recurse. */
	if (sep && sep > path && sep[0] != ':' && sep[-1] != PATHSEPCH)
	{
		*sep = 0;
		filerr = create_path_recursive(path);
		*sep = PATHSEPCH;
		if (filerr != NO_ERROR)
			return filerr;
	}

	/* if the path already exists, we're done */
	if (!stat(path, &st))
		return NO_ERROR;

	/* create the path */
	#ifdef _WIN32
	if (mkdir(path) != 0)
	#else
	if (mkdir(path, 0777) != 0)
	#endif
		return error_to_file_error(errno);
	return NO_ERROR;
}

//============================================================
//  osd_get_physical_drive_geometry
//============================================================

int osd_get_physical_drive_geometry(const char *filename,
      std::uint32_t *cylinders, std::uint32_t *heads, std::uint32_t *sectors, std::uint32_t *bps)
{
	return FALSE;
}

//============================================================
//  osd_is_path_separator
//============================================================

static int osd_is_path_separator(char c)
{
	return (c == '/') || (c == '\\');
}

//============================================================
//  osd_is_absolute_path
//============================================================

int osd_is_absolute_path(const char *path)
{
	int result;

	if (osd_is_path_separator(path[0]))
		result = TRUE;
#if !defined(SDLMAME_WIN32) && !defined(SDLMAME_OS2)
	else if (path[0] == '.')
		result = TRUE;
#else
	#ifndef UNDER_CE
	else if (*path && path[1] == ':')
		result = TRUE;
	#endif
#endif
	else
		result = FALSE;

	return result;
}


int osd_uchar_from_osdchar(std::uint32_t /* unicode_char */ *uchar,
      const char *osdchar, size_t count)
{
	/* we assume a standard 1:1 mapping of characters 
    * to the first 256 unicode characters. */
	*uchar = (UINT8)*osdchar;
	return 1;
}

//============================================================
//  osd_stat
//============================================================

osd_directory_entry *osd_stat(const char *path)
{
	int err;
	osd_directory_entry *result = NULL;
#if defined(__MACH__) || defined(_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_ARM) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2) || defined(SDLMAME_HAIKU)
   struct stat st;
#else
   struct stat64 st;
#endif

#if defined(__MACH__) || defined(_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_ARM) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2) || defined(SDLMAME_HAIKU)
   err = stat(path, &st);
#else
   err = stat64(path, &st);
#endif

	if( err == -1)
      return NULL;

	/* create an osd_directory_entry; 
    * Be sure to make sure that the caller can
    * free all resources by just freeing the 
    * resulting osd_directory_entry. */
	result       = (osd_directory_entry *) osd_malloc_array(sizeof(*result) + strlen(path) + 1);

	strcpy(((char *) result) + sizeof(*result), path);

	result->name = ((char *) result) + sizeof(*result);
	result->type = S_ISDIR(st.st_mode) ? ENTTYPE_DIR : ENTTYPE_FILE;
	result->size = (UINT64)st.st_size;

	return result;
}

//============================================================
//  osd_get_volume_name
//============================================================

const char *osd_get_volume_name(int idx)
{
	if (idx != 0)
      return NULL;
	return "/";
}

//============================================================
//  osd_get_full_path
//============================================================

osd_file::error osd_get_full_path(char **dst, const char *path)
{
	osd_file::error err;
	char path_buffer[512];

	err = osd_file::error::NONE;

	if (getcwd(path_buffer, 511) == NULL)
	{
		printf("osd_get_full_path: failed!\n");
		err = osd_file::error::FAILURE;
	}
	else
	{
		*dst = (char *)osd_malloc_array(strlen(path_buffer)+strlen(path)+3);

		// if it's already a full path, just pass it through
		if (path[0] == '/')
			strcpy(*dst, path);
		else
			sprintf(*dst, "%s%s%s", path_buffer, PATH_SEPARATOR, path);
	}

	return err;
}

//============================================================
//  osd_truncate
//============================================================
osd_file::error osd_truncate(osd_file::ptr file, UINT64 offset)
{
	int result;

	result = ftruncate(file->handle, offset);
	if (!result)
		return error_to_file_error(errno);

	return osd_file::error::NONE;
}

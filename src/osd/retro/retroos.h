#define SDLMAME_UNIX

/*-----------------------------------------------------------------------------
    osd_getenv: return pointer to environment variable

    Parameters:

        name  - name of environment variable

    Return value:

        pointer to value
-----------------------------------------------------------------------------*/
const char *osd_getenv(const char *name);

/*-----------------------------------------------------------------------------
    osd_setenv: set environment variable

    Parameters:

        name  - name of environment variable
        value - value to write
        overwrite - overwrite if it exists

    Return value:

        0 on success
-----------------------------------------------------------------------------*/

int osd_setenv(const char *name, const char *value, int overwrite);


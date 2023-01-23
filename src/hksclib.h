/*
** hksclib.h
** A wrapper for the modded Lua library
** See Copyright Notice in lua.h
*/


#ifndef HKSC_LIB_H
#define HKSC_LIB_H

#include "hksclua.h"


typedef int (*hksc_DumpFunction) (hksc_State *H, void *ud);

/*
** API declarations
*/

/* Lua state constructor/destructor */
#ifdef HKSC_LOGGING
LUA_API hksc_State *hksI_newstate(int mode, hksc_LogContext *logctx);
#else /* !HKSC_LOGGING */
LUA_API hksc_State *hksI_newstate(int mode);
#endif /* HKSC_LOGGING */
LUA_API void hksI_close(hksc_State *H);


/* Custom parse & dump functions (user program must provide a custom dump
   function to call when dumping the bytecode) */
LUA_API int hksI_parser_file(hksc_State *H, const char *filename,
                             hksc_DumpFunction dumpf, void *ud);
LUA_API int hksI_parser_buffer(hksc_State *H, const char *buff, size_t size,
                               char *source, hksc_DumpFunction dumpf, void *ud);


/* extra error code for `luaL_load' */
#define LUA_ERRFILE     (LUA_ERRERR+1)

#endif /* HKSC_LIB_H */

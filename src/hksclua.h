/*
** $Id: lua.h $
** Lua - An Extensible Extension Language
** Lua.org, PUC-Rio, Brazil (http://www.lua.org)
** See Copyright Notice at the end of this file
*/


#ifndef lua_h
#define lua_h

#include <stdarg.h>
#include <stddef.h>


#include "hkscluaconf.h"


#define LUA_VERSION	"Lua 5.1"
#define LUA_VERSION_NUM	501
#define LUA_COPYRIGHT	"Copyright (C) 1994-2006 Lua.org, PUC-Rio"
#define LUA_AUTHORS 	"R. Ierusalimschy, H. H. de Figueiredo & W. Celes"


/* mark for precompiled code (`<esc>Lua') */
#define	LUA_SIGNATURE	"\033Lua"

/* option for multiple returns in `lua_pcall' and `lua_call' */
#define LUA_MULTRET	(-1)


/* thread status; 0 is OK */
#define LUA_YIELD	1
#define LUA_ERRRUN	2
#define LUA_ERRSYNTAX	3
#define LUA_ERRMEM	4
#define LUA_ERRERR	5


/*
** int literal options
*/
#define INT_LITERALS_NONE   0 /* disable all literals */
#define INT_LITERALS_LUD    1 /* enable LUD literal constants */
#define INT_LITERALS_32BIT  INT_LITERALS_LUD
#define INT_LITERALS_UI64   2 /* enable UI64 literal constants */
#define INT_LITERALS_64BIT  INT_LITERALS_UI64
#define INT_LITERALS_ALL    3 /* enable all literals */


/*
** bytecode sharing modes
*/
#define HKSC_SHARING_MODE_OFF  0
#define HKSC_SHARING_MODE_ON   1
#define HKSC_SHARING_MODE_SECURE 2


/*
** bytecode sharing formats
*/
#define HKSC_BYTECODE_DEFAULT  0
#define HKSC_BYTECODE_INPLACE  1
#define HKSC_BYTECODE_REFERENCED 2

/*
** bytecode endianness
*/
#define HKSC_DEFAULT_ENDIAN   0
#define HKSC_BIG_ENDIAN       1
#define HKSC_LITTLE_ENDIAN    2

/*
** hksc modes
*/
#define HKSC_MODE_DEFAULT   0 /* infer from content of first file */
#define HKSC_MODE_COMPILE   1 /* compiling source */
#define HKSC_MODE_DECOMPILE 2 /* decompiling bytecode */


/*
** bytecode stripping levels
*/
#define BYTECODE_STRIPPING_NONE 0
#define BYTECODE_STRIPPING_PROFILING 1
#define BYTECODE_STRIPPING_ALL 2
#ifdef LUA_COD /* Cod extensions */
#define BYTECODE_STRIPPING_DEBUG_ONLY 3
#define BYTECODE_STRIPPING_CALLSTACK_RECONSTRUCTION 4
#endif /* LUA_COD */


typedef struct hksc_State hksc_State;

typedef int (*lua_CFunction) (hksc_State *H);

#ifdef HKSC_LOGGING

#define LOG_PRIORITY_DEBUG     0
#define LOG_PRIORITY_INFO      1
#define LOG_PRIORITY_WARN      2
#define LOG_PRIORITY_ERROR     3
#define LOG_PRIORITY_FATAL     4

typedef int (*hksc_LogFunction) (hksc_State *H, const char *label, int priority,
                                 const char *msg, void *ud);

typedef struct hksc_LogContext {
  hksc_LogFunction f;
  void *ud;
  int priority;
} hksc_LogContext;

#define DEFAULT_LOGCTX {NULL, NULL, 0}
#endif /* HKSC_LOGGING */

/* user-defined callbacks for beginning/end of cycles */
typedef void (*hksc_CycleCallback)(hksc_State *H, const char *name);


/*
** functions that read/write blocks when loading/dumping Lua chunks
*/
typedef const char * (*lua_Reader) (hksc_State *H, void *ud, size_t *sz);

typedef int (*lua_Writer) (hksc_State *H, const void* p, size_t sz, void* ud);


/*
** prototype for memory-allocation functions
*/
typedef void * (*lua_Alloc) (void *ud, void *ptr, size_t osize, size_t nsize);


/*
** basic types
*/
#define LUA_TANY    (-2)
#define LUA_TNONE		(-1)

#define LUA_TNIL		0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		8
#define LUA_TIFUNCTION  9
#define LUA_TCFUNCTION  10
#define LUA_TUI64       11
#define LUA_TSTRUCT     12

#define LUA_NUM_TYPE_OBJECTS 14


/*
** generic extra include file
*/
#if defined(LUA_USER_H)
#include LUA_USER_H
#endif


/* type of numbers in Lua */
typedef LUA_NUMBER lua_Number;


/* type for integer functions */
typedef LUA_INTEGER lua_Integer;


/*
** state manipulation
*/
#ifdef HKSC_LOGGING
LUA_API hksc_State *(lua_newstate) (lua_Alloc f, void *ud,
                                    hksc_LogContext *logctx);
#else /* !HKSC_LOGGING */
LUA_API hksc_State *(lua_newstate) (lua_Alloc f, void *ud);
#endif /* HKSC_LOGGING */
LUA_API void       (lua_close) (hksc_State *H);

LUA_API lua_CFunction (lua_atpanic) (hksc_State *H, lua_CFunction panicf);
LUA_API hksc_CycleCallback (lua_onstartcycle) (hksc_State *H,
                                               hksc_CycleCallback f);
LUA_API hksc_CycleCallback (lua_onendcycle) (hksc_State *H,
                                             hksc_CycleCallback f);


/*
** miscellaneous functions
*/
LUA_API const char *(lua_newstring) (hksc_State *H, const char *str);
LUA_API const char *(lua_newlstring) (hksc_State *H, const char *str, size_t l);
LUA_API const char *(lua_newfstring) (hksc_State *H, const char *fmt, ...);
LUA_API const char *(lua_newvfstring) (hksc_State *H, const char *fmt,
                                       va_list argp);
LUA_API const char *(lua_geterror) (hksc_State *H);
LUA_API void (lua_seterror) (hksc_State *H, const char *s);
LUA_API void (lua_setferror) (hksc_State *H, const char *fmt, ...);
LUA_API void (lua_setvferror) (hksc_State *H, const char *fmt, va_list argp);
LUA_API void (lua_clearerror) (hksc_State *H);
LUA_API int (lua_getmode) (hksc_State *H);
LUA_API void (lua_setmode) (hksc_State *H, int mode);
LUA_API lua_Alloc (lua_getallocf) (hksc_State *H, void **ud);
LUA_API void (lua_setallocf) (hksc_State *H, lua_Alloc f, void *ud);

#ifdef HKSC_LOGGING
LUA_API hksc_LogFunction (lua_getlogf) (hksc_State *H, void **ud);
LUA_API void (lua_setlogf) (hksc_State *H, hksc_LogFunction f, void *ud);
LUA_API int (lua_getlogpriority) (hksc_State *H);
LUA_API void (lua_setlogpriority) (hksc_State *H, int priority);
#endif /* HKSC_LOGGING */

#if defined(LUA_COD) && defined(HKSC_DECOMPILER)
LUA_API const char *(lua_getDebugFile) (hksc_State *H);
LUA_API void (lua_setDebugFile) (hksc_State *H, const char *name);
#endif /* defined(LUA_COD) && defined(HKSC_DECOMPILER) */

/*
** Lua compiler/decompiler settings
*/
LUA_API int (lua_getEmitStruct) (hksc_State *H);
LUA_API void (lua_setEmitStruct) (hksc_State *H, int emit_struct);
LUA_API int (lua_getIntLiteralsEnabled) (hksc_State *H);
LUA_API void (lua_setIntLiteralsEnabled) (hksc_State *H,
                                          int enable_int_literals);
LUA_API int (lua_getBytecodeStrippingLevel) (hksc_State *H);
LUA_API void (lua_setBytecodeStrippingLevel) (hksc_State *H, int strip);
LUA_API int (lua_getIgnoreDebug) (hksc_State *H);
LUA_API void (lua_setIgnoreDebug) (hksc_State *H, int ignore_debug);
#ifdef HKSC_DECOMPILER
LUA_API int (lua_getMatchLineInfo) (hksc_State *H);
LUA_API void (lua_setMatchLineInfo) (hksc_State *H, int match_line_info);
#endif /* HKSC_DECOMPILER */

/*
** dump functions
*/

/* print a function to stdout */
LUA_API void (lua_print) (hksc_State *H, int full);

/* dump one chunk; from ldump.c */
LUA_API int (lua_dump) (hksc_State *H, lua_Writer w, void *data);

#ifdef HKSC_DECOMPILER
/* decompile one chunk; from ldecomp.c */
LUA_API int (lua_decompile) (hksc_State *H, lua_Writer w, void *data);
#endif /* HKSC_DECOMPILER */

/*
** compatibility macros and functions
*/

#define lua_Chunkreader		lua_Reader
#define lua_Chunkwriter		lua_Writer



/******************************************************************************
* Copyright (C) 1994-2006 Lua.org, PUC-Rio.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/


#endif

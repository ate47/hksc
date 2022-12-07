/*
** $Id: hkscconf.h $
** Configuration file for Hksc
** See Copyright Notice in lua.h
*/


#ifndef hconfig_h
#define hconfig_h

/*#include <assert.h>
#define lua_assert assert*/

#define HKSC_TEST_UI64_S

/* compatibility bits */
#define HKSC_COMPATIBILITY_BIT_MEMOIZATION  0
#define HKSC_COMPATIBILITY_BIT_STRUCTURES   1
#define HKSC_COMPATIBILITY_BIT_SELF         2
#define HKSC_COMPATIBILITY_BIT_DOUBLES      3
#define HKSC_COMPATIBILITY_BIT_NATIVEINT    4

#ifndef LUA_COD
#define LUA_COD /* general cod compatibility */
#endif
#ifndef LUA_CODT7
#define LUA_CODT7 /* support bitwise operations */
#endif

#ifndef HKSC_DECOMPILER
#define HKSC_DECOMPILER
#endif

/*
** NOTE for non-Call-of-Duty developers:
** This version of Hksc is only intended to work with Call of Duty. Because Call
** of Duty disables all compatibiliy flags, none of those features are
** implemented in the compiler. If the game you are developing for enables any
** of the compatibility features, you will need to find out which ones are
** enabled and provide implementations for them. Also make sure to undefine the
** macro `LUA_COD' if you are not developing for a Call of Duty game.
*/

/* #ifdef LUA_COD */
#define HKSC_GETGLOBAL_MEMOIZATION 0
#define HKSC_STRUCTURE_EXTENSION_ON 0
#define HKSC_SELF 0
#define HKSC_WITHDOUBLES 0
#define HKSC_WITHNATIVEINT 0
/* #endif */

#if !defined(LUA_COD) && defined(LUA_CODT7)
#undef LUA_CODT7
#endif

#if HKSC_GETGLOBAL_MEMOIZATION
#error "Global Memoization not implemented"
#endif

#if HKSC_STRUCTURE_EXTENSION_ON
#error "Structure extension not implemented"
#endif

#if HKSC_SELF
#error "Self extension not implemented"
#endif

#if HKSC_WITHNATIVEINT
#error "Native Int not implemented"
#endif

#endif

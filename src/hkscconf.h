/*
** $Id: hkscconf.h $
** Configuration file for Hksc
** See Copyright Notice in lua.h
*/


#ifndef hconfig_h
#define hconfig_h

/* compatibility bits */
#define HKSC_COMPATIBILITY_BIT_MEMOIZATION  0
#define HKSC_COMPATIBILITY_BIT_STRUCTURES   1
#define HKSC_COMPATIBILITY_BIT_SELF         2
#define HKSC_COMPATIBILITY_BIT_DOUBLES      3
#define HKSC_COMPATIBILITY_BIT_NATIVEINT    4

/* LUA_COMPAT_VARARG does vary, at least between TA and non-TA */
#if 1
#undef HKSC_NO_COMPAT_VARARG
#else
#define HKSC_NO_COMPAT_VARARG
#endif

/* UI64 type */
#if 1
#define HKSC_UI64API
#else
#undef HKSC_UI64API
#endif

/* UI64 emulation */
#if 0
#define HKSC_EMU_UI64
#else
#undef HKSC_EMU_UI64
#endif

/* Lua decompiler */
#if 0
#define HKSC_DECOMPILER
#else
#undef HKSC_DECOMPILER
#endif

/* IW6 `hdelete' extension */
#if 0
#define LUA_CODIW6
#else
#undef LUA_CODIW6
#endif

/* T7 compatibility */
#if 1
#define LUA_CODT7
#else
#undef LUA_CODT7
#endif

/* T6 compatibility */
#if 1
#define LUA_CODT6
#undef LUA_CODIW6
#else
#undef LUA_CODT6
#undef LUA_CODT7
#endif

/* matching table traversal order in the FromSoftware version */
#if 0
#define HKSC_FROMSOFT_TTABLES
#else
#undef HKSC_FROMSOFT_TTABLES
#endif

/* multi-platform targeting */
#if 0
#define HKSC_MULTIPLAT
#else
#undef HKSC_MULTIPLAT
#endif

/* compatibility flags */
#define HKSC_GETGLOBAL_MEMOIZATION 0
#define HKSC_STRUCTURE_EXTENSION_ON 0
#define HKSC_SELF 0
#define HKSC_WITHDOUBLES 0
#define HKSC_WITHNATIVEINT 0

/* whether this build is being used for tests */
#if 0
#define HKSC_TESTING
#else
#undef HKSC_TESTING
#endif

/* I have this option for testing regular Lua code patterns with the
   decompiler */
#if 0
#define HKSC_TEST_WITH_STANDARD_LUA
#else
#undef HKSC_TEST_WITH_STANDARD_LUA
#endif

#ifdef HKSC_DECOMPILER
/* set which decompiler pass to test */
#define HKSC_DEBUG_PASS 0
#else
#undef HKSC_DEBUG_PASS
#endif

/* put here any errors that need to happen for unsupported configurations */

#if HKSC_SELF
#error "Self not implemented"
#endif

#if HKSC_WITHNATIVEINT
#error "Native Int not implemented"
#endif

#endif

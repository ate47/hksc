/*
** $Id: lobject.h $
** Type definitions for Lua objects
** See Copyright Notice in lua.h
*/


#ifndef lobject_h
#define lobject_h


#include <stdarg.h>


#include "llimits.h"
#include "hksclua.h"


/* tags for values visible from Lua */
#define LAST_TAG  (LUA_NUM_TYPE_OBJECTS-1)

#define NUM_TAGS  (LAST_TAG+1)


/*
** Extra tags for non-values
*/
#define LUA_TPROTO  (LAST_TAG+1)
#define LUA_TUPVAL  (LAST_TAG+2)
#define LUA_TDEADKEY  (LAST_TAG+3)
#define LUA_TANALYZER  (LAST_TAG+4)


/*
** Union of all collectable objects
*/
typedef union GCObject GCObject;


/*
** Common Header for all collectable objects (in macro form, to be
** included in other objects)
*/
#define CommonHeader  GCObject *next; lu_byte tt; lu_byte marked


/*
** Common header in struct form
*/
typedef struct GCheader {
  CommonHeader;
} GCheader;


#ifdef LUA_UI64_S
struct lua_ui64_s { /* typedef'd as lu_int64 */
  lu_int32 hi; /* bits 33-64 */
  lu_int32 lo; /* bits 1-32 */
};
#endif /* LUA_UI64_S */


/*
** Union of all Lua values
*/
typedef union {
  GCObject *gc;
  void *p;
  lua_Number n;
  lu_int64 l;
  int b;
} Value;


/*
** Tagged Values
*/

#define TValuefields  Value value; int tt

typedef struct lua_TValue {
  TValuefields;
} TValue;


/* Macros to test type */
#define ttisnil(o)  (ttype(o) == LUA_TNIL)
#define ttisnumber(o)  (ttype(o) == LUA_TNUMBER)
#define ttisstring(o)  (ttype(o) == LUA_TSTRING)
#define ttistable(o)  (ttype(o) == LUA_TTABLE)
#define ttisfunction(o)  (ttype(o) == LUA_TFUNCTION)
#define ttisboolean(o)  (ttype(o) == LUA_TBOOLEAN)
#define ttisuserdata(o)  (ttype(o) == LUA_TUSERDATA)
#define ttisthread(o)  (ttype(o) == LUA_TTHREAD)
#define ttislightuserdata(o)  (ttype(o) == LUA_TLIGHTUSERDATA)
#define ttisui64(o)  (ttype(o) == LUA_TUI64)
#define ttisstruct(o)  (ttype(o) == LUA_TSTRUCT)
#define ttisifunction(o)  (ttype(o) == LUA_TIFUNCTION)
#define ttiscfunction(o)  (ttype(o) == LUA_TCFUNCTION)

/* Macros to access values */
#define ttype(o)  ((o)->tt)
#define gcvalue(o)  check_exp(iscollectable(o), (o)->value.gc)
#define pvalue(o)  check_exp(ttislightuserdata(o), (o)->value.p)
#define nvalue(o)  check_exp(ttisnumber(o), (o)->value.n)
#define ui64value(o)  check_exp(ttisui64(o), (o)->value.l)
#define rawtsvalue(o)  check_exp(ttisstring(o), &(o)->value.gc->ts)
#define tsvalue(o)  (&rawtsvalue(o)->tsv)
#define rawuvalue(o)  check_exp(ttisuserdata(o), &(o)->value.gc->u)
#define uvalue(o)  (&rawuvalue(o)->uv)
#define clvalue(o)  check_exp(ttisfunction(o), &(o)->value.gc->cl)
#define hvalue(o)  check_exp(ttistable(o), &(o)->value.gc->h)
#define bvalue(o)  check_exp(ttisboolean(o), (o)->value.b)
#define thvalue(o)  check_exp(ttisthread(o), &(o)->value.gc->th)

#define l_isfalse(o)  (ttisnil(o) || (ttisboolean(o) && bvalue(o) == 0))

/*
** for internal debug only
*/
#define checkconsistency(obj) \
  lua_assert(!iscollectable(obj) || (ttype(obj) == (obj)->value.gc->gch.tt))

#define checkliveness(g,obj) \
  lua_assert(!iscollectable(obj) || \
  ((ttype(obj) == (obj)->value.gc->gch.tt) && !isdead(g, (obj)->value.gc)))


/* Macros to set values */
#define setnilvalue(obj) do { (obj)->tt=LUA_TNIL; } while (0)

#define setnvalue(obj,x) do \
  { TValue *i_o=(obj); i_o->value.n=(x); i_o->tt=LUA_TNUMBER; } while(0)

#define setpvalue(obj,x) do \
  { TValue *i_o=(obj); i_o->value.p=(x); i_o->tt=LUA_TLIGHTUSERDATA; } while(0)

#define setbvalue(obj,x) do \
  { TValue *i_o=(obj); i_o->value.b=(x); i_o->tt=LUA_TBOOLEAN; } while(0)

#define setsvalue(obj,x) do \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TSTRING; } while(0)

#define setuvalue(obj,x) do \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TUSERDATA; } while(0)

#define setthvalue(obj,x) do \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TTHREAD; } while (0)

#define setclvalue(obj,x) do \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TFUNCTION; } while (0)

#define sethvalue(obj,x) do \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TTABLE; } while (0)

#define setptvalue(obj,x) do \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TPROTO; } while (0)

#define setui64value(obj,x) do \
  { TValue *i_o=(obj); i_o->value.l=(x); i_o->tt=LUA_TUI64; } while (0)




#define setobj(obj1,obj2) \
  { const TValue *o2=(obj2); TValue *o1=(obj1); \
    o1->value = o2->value; o1->tt=o2->tt; }


/*
** different types of sets, according to destination
*/

/* from stack to (same) stack */
#define setobjs2s  setobj
/* to stack (not from same stack) */
#define setobj2s  setobj
#define setsvalue2s  setsvalue
#define sethvalue2s  sethvalue
#define setptvalue2s  setptvalue
/* from table to same table */
#define setobjt2t  setobj
/* to table */
#define setobj2t  setobj
/* to new object */
#define setobj2n  setobj
#define setsvalue2n  setsvalue

#define setttype(obj, tt) (ttype(obj) = (tt))


#define iscollectable(o)  (ttype(o) >= LUA_TSTRING && !ttisui64(o))



typedef TValue *StkId;  /* index to stack elements */


/*
** String headers for string table
*/
typedef union TString {
  L_Umaxalign dummy;  /* ensures maximum alignment for strings */
  struct {
    CommonHeader;
    lu_byte reserved;
    unsigned int hash;
    size_t len;
  } tsv;
} TString;


#define getstr(ts)  cast(const char *, (ts) + 1)
#define svalue(o)       getstr(tsvalue(o))



typedef union Udata {
  L_Umaxalign dummy;  /* ensures maximum alignment for `local' udata */
  struct {
    CommonHeader;
    struct Table *metatable;
    struct Table *env;
    size_t len;
  } uv;
} Udata;




/*
** Function Prototypes
*/
typedef struct Proto {
  CommonHeader;
  TValue *k;  /* constants used by the function */
  Instruction *code;
  struct Proto **p;  /* functions defined inside the function */
  int *lineinfo;  /* map from opcodes to source lines */
  struct LocVar *locvars;  /* information about local variables */
  TString **upvalues;  /* upvalue names */
  TString  *source;
  TString  *name;
#ifdef LUA_COD
  lu_int32 hash;
#endif /* LUA_COD */
  int sizeupvalues;
  int sizek;  /* size of `k' */
  int sizecode;
  int sizelineinfo;
  int sizep;  /* size of `p' */
  int sizelocvars;
  int linedefined;
  int lastlinedefined;
  GCObject *gclist;
  lu_byte nups;  /* number of upvalues */
  lu_byte numparams;
  lu_byte is_vararg;
  lu_byte maxstacksize;
} Proto;


/* masks for new-style vararg */
#define VARARG_HASARG    1
#define VARARG_ISVARARG    2
#define VARARG_NEEDSARG    4


typedef struct LocVar {
  TString *varname;
  int startpc;  /* first point where variable is active */
  int endpc;    /* first point where variable is dead */
} LocVar;



/*
** Function analyzers
*/
#ifdef HKSC_DECOMPILER

/* flags defined in lanalyzer.h */
typedef lu_int32 InstructionFlags;
typedef lu_byte RegisterFlags;
struct BasicBlock;

typedef struct Analyzer {
  CommonHeader;
  InstructionFlags *insproperties;  /* instruction flags */
  RegisterFlags *regproperties;  /* register flags */
  int *lineinfo;  /* map from opcodes to decompiltion lines */
  struct LocVar *locvars;  /* information about local variables */
  TString **upvalues;  /* upvalue names */
  int sizeinsproperties;
  int sizeregproperties;
  int sizelineinfo;
  int sizelocvars;
  int sizeupvalues;
  struct {
    struct BasicBlock *first, *last;
  } bbllist;
} Analyzer;

#endif /* HKSC_DECOMPILER */



/*
** Upvalues
*/

typedef struct UpVal {
  CommonHeader;
  TValue *v;  /* points to stack or to its own value */
  union {
    TValue value;  /* the value (when closed) */
    struct {  /* double linked list (when open) */
      struct UpVal *prev;
      struct UpVal *next;
    } l;
  } u;
} UpVal;


/*
** Closures
*/

#define ClosureHeader \
  CommonHeader; lu_byte isC; lu_byte nupvalues; GCObject *gclist; \
  struct Table *env

typedef struct CClosure {
  ClosureHeader;
  lua_CFunction f;
  TValue upvalue[1];
} CClosure;


typedef struct LClosure {
  ClosureHeader;
  struct Proto *p;
  UpVal *upvals[1];
} LClosure;


typedef union Closure {
  CClosure c;
  LClosure l;
} Closure;


#define iscfunction(o)  (ttiscfunction(o))
#define isLfunction(o)  (ttisifunction(o))


/*
** Tables
*/

typedef union TKey {
  struct {
    TValuefields;
    struct Node *next;  /* for chaining */
  } nk;
  TValue tvk;
} TKey;


typedef struct Node {
  TValue i_val;
  TKey i_key;
} Node;


typedef struct Table {
  CommonHeader;
  lu_byte flags;  /* 1<<p means tagmethod(p) is not present */ 
  lu_byte lsizenode;  /* log2 of size of `node' array */
  struct Table *metatable;
  TValue *array;  /* array part */
  Node *node;
  Node *lastfree;  /* any free position is before this position */
  GCObject *gclist;
  int sizearray;  /* size of `array' array */
} Table;



/*
** `module' operation for hashing (size is always a power of 2)
*/
#define lmod(s,size) \
  (check_exp((size&(size-1))==0, (cast(int, (s) & ((size)-1)))))


#define twoto(x)  (1<<(x))
#define sizenode(t)  (twoto((t)->lsizenode))


#define luaO_nilobject    (&luaO_nilobject_)

LUAI_DATA const TValue luaO_nilobject_;

#define ceillog2(x)  (luaO_log2((x)-1) + 1)

LUAI_FUNC int luaO_log2 (unsigned int x);
LUAI_FUNC int luaO_int2fb (unsigned int x);
LUAI_FUNC int luaO_fb2int (int x);
LUAI_FUNC int luaO_rawequalObj (const TValue *t1, const TValue *t2);
LUAI_FUNC int luaO_str2d (const char *s, lua_Number *result);
#ifdef LUA_UI64_S /* special UI64 functions for C89 */
LUAI_FUNC struct lua_ui64_s luaO_str2ui64_s(const char *s, char **endptr,
                                          size_t n);
LUAI_FUNC int luaO_ui64_s_2str(char *str, struct lua_ui64_s literal);
#endif /* LUA_UI64_S */
LUAI_FUNC int luaO_str2ui64(const char *s,const char *suffix,lu_int64 *result);
LUAI_FUNC TString *luaO_kstring2print (hksc_State *H, TString *ts);
LUAI_FUNC const char *luaO_pushvfstring (hksc_State *H, const char *fmt,
                                                       va_list argp);
LUAI_FUNC const char *luaO_pushfstring (hksc_State *H, const char *fmt, ...);
LUAI_FUNC void luaO_chunkid (char *out, const char *source, size_t len);

#endif


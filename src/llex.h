/*
** $Id: llex.h,v 1.56 2005/12/07 15:33:27 roberto Exp roberto $
** Lexical Analyzer
** See Copyright Notice in lua.h
*/

#ifndef llex_h
#define llex_h

#include "hksc_begin_code.h"

#include "lobject.h"
#include "lzio.h"


#define FIRST_RESERVED  0x400001

#define DEFTOK1(name, text) name = FIRST_RESERVED,
#define DEFTOK(name, text) name,
enum RESERVED {
#include "ltoken.def"
};
#undef DEFTOK1
#undef DEFTOK

#define LAST_RESERVED LTOKENS_LAST_RESERVED_WORD

/* maximum length of a reserved word */
#define TOKEN_LEN (sizeof(LTOKENS_LONGEST_TOKEN)/sizeof(char))

/* number of reserved words */
#define NUM_RESERVED  (cast(int, LAST_RESERVED-FIRST_RESERVED+1))


/* array with token `names' */
LUAI_DATA const char *const luaX_tokens [];


typedef union {
  lua_Number r;
  lua_Literal l;
  TString *ts;
} SemInfo;  /* semantics information */


typedef struct Token {
  int token;
  SemInfo seminfo;
} Token;


typedef struct LexState {
  int current;  /* current character (charint) */
  int linenumber;  /* input line counter */
  int lastline;  /* line of last token `consumed' */
  Token t;  /* current token */
  Token lookahead;  /* look ahead token */
  struct FuncState *fs;  /* `FuncState' is private to the parser */
  struct hksc_State *H;
  ZIO *z;  /* input stream */
  Mbuffer *buff;  /* buffer for tokens */
  TString *source;  /* current source name */
  char decpoint;  /* locale decimal point */
} LexState;


LUAI_FUNC void luaX_init (hksc_State *H);
LUAI_FUNC void luaX_setinput (hksc_State *H, LexState *LS, ZIO *z,
                              TString *source);
LUAI_FUNC TString *luaX_newstring (LexState *LS, const char *str, size_t l);
LUAI_FUNC void luaX_next (LexState *ls);
LUAI_FUNC void luaX_lookahead (LexState *ls);
LUAI_FUNC void luaX_lexerror (LexState *ls, const char *msg, int token);
LUAI_FUNC void luaX_syntaxerror (LexState *ls, const char *s);
LUAI_FUNC const char *luaX_token2str (LexState *ls, int token);


#endif

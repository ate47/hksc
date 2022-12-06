/*
** $Id: print.c $
** print bytecodes
** See Copyright Notice in lua.h
*/

#include <stdio.h>

#define hksc_c
#define LUA_CORE

#include "lctype.h"
#include "ldebug.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lundump.h"

#define PrintFunction	luaU_print

#define Sizeof(x)	((int)sizeof(x))
#define VOID(p)		((const void*)(p))

static void PrintString(const TString *ts)
{
  const char *s=getstr(ts);
  size_t i,n=ts->tsv.len;
  putchar('"');
  for (i=0; i<n; i++)
  {
    int c=s[i];
    switch (c)
    {
      case '"': printf("\\\""); break;
      case '\\': printf("\\\\"); break;
      case '\a': printf("\\a"); break;
      case '\b': printf("\\b"); break;
      case '\f': printf("\\f"); break;
      case '\n': printf("\\n"); break;
      case '\r': printf("\\r"); break;
      case '\t': printf("\\t"); break;
      case '\v': printf("\\v"); break;
      default:
        if (lisprint((unsigned char)c))
          putchar(c);
        else
          printf("\\%03u",(unsigned char)c);
        break;
    }
  }
  putchar('"');
}

static void PrintUI64(const lu_int64 literal)
{
  char buff[LUAI_MAXUI642STR];
  lua_ui642str(buff, literal);
  printf("0x%shl", buff);
}

static void PrintConstant(const Proto *f, int i)
{
  const TValue *o=&f->k[i];
  switch (ttype(o))
  {
    case LUA_TNIL:
      printf("nil");
      break;
    case LUA_TBOOLEAN:
      printf(bvalue(o) ? "true" : "false");
      break;
    case LUA_TLIGHTUSERDATA:
      printf("%phi", pvalue(o));
      break;
    case LUA_TNUMBER:
      printf(LUA_NUMBER_FMT,nvalue(o));
      break;
    case LUA_TSTRING:
      PrintString(rawtsvalue(o));
      break;
    case LUA_TUI64:
      PrintUI64(hlvalue(o));
      break;
    default:				/* cannot happen */
      printf("? type=%d",ttype(o));
      break;
  }
}

static void PrintCode(const Proto *f)
{
  const Instruction *code=f->code;
  int pc,n=f->sizecode;
  for (pc=0; pc<n; pc++)
  {
    Instruction i=code[pc];
    OpCode o=GET_OPCODE(i);
    int a=GETARG_A(i);
    int b=GETARG_B(i);
    int c=GETARG_C(i);
    int bx=GETARG_Bx(i);
    int sbx=GETARG_sBx(i);
    int line=getline(f,pc);
    printf("\t%d\t",pc+1);
    if (line>0) printf("[%d]\t",line); else printf("[-]\t");
    printf("%-9s\t",luaP_opnames[o]);
    switch (getOpMode(o))
    {
      case iABC:
        printf("%d",a);
        if (getBMode(o)!=OpArgN) printf(" %d",ISK(b) ? (-1-INDEXK(b)) : b);
        if (getCMode(o)!=OpArgN) printf(" %d",ISK(c) ? (-1-INDEXK(c)) : c);
        break;
      case iABx:
        if (getBMode(o)==OpArgK) printf("%d %d",a,-1-bx);
        else printf("%d %d",a,bx);
        break;
      case iAsBx:
        if (o==OP_JMP) printf("%d",sbx); else printf("%d %d",a,sbx);
        break;
    }
    switch (o)
    {
      case OP_LOADK:
        printf("\t; "); PrintConstant(f,bx);
        break;
      case OP_GETUPVAL:
      case OP_SETUPVAL:
        printf("\t; %s", (f->sizeupvalues>0) ? getstr(f->upvalues[b]) : "-");
        break;
      case OP_GETGLOBAL:
      case OP_SETGLOBAL:
        printf("\t; %s",svalue(&f->k[bx]));
        break;
      case OP_GETTABLE:
      case OP_GETTABLE_S:
      case OP_GETTABLE_N:
      case OP_GETFIELD:
      case OP_SELF:
        if (ISK(c)) { printf("\t; "); PrintConstant(f,INDEXK(c)); }
        break;
      case OP_SETTABLE: case OP_SETTABLE_BK:
      case OP_SETTABLE_S: case OP_SETTABLE_S_BK:
      case OP_SETTABLE_N: case OP_SETTABLE_N_BK:
      case OP_SETFIELD:
      case OP_ADD: case OP_ADD_BK:
      case OP_SUB: case OP_SUB_BK:
      case OP_MUL: case OP_MUL_BK:
      case OP_DIV: case OP_DIV_BK:
      case OP_POW: case OP_POW_BK:
      case OP_EQ:
      case OP_LT: case OP_LT_BK:
      case OP_LE: case OP_LE_BK:
#ifdef LUA_CODT7
      case OP_LEFT_SHIFT: case OP_LEFT_SHIFT_BK:
      case OP_RIGHT_SHIFT: case OP_RIGHT_SHIFT_BK:
      case OP_BIT_AND: case OP_BIT_AND_BK:
      case OP_BIT_OR: case OP_BIT_OR_BK:
#endif /* LUA_CODT7 */
        if (ISK(b) || ISK(c))
        {
          printf("\t; ");
          if (ISK(b)) PrintConstant(f,INDEXK(b)); else printf("-");
          printf(" ");
          if (ISK(c)) PrintConstant(f,INDEXK(c)); else printf("-");
        }
        break;
      case OP_JMP:
      case OP_FORLOOP:
      case OP_FORPREP:
        printf("\t; to %d",sbx+pc+2);
        break;
      case OP_CLOSURE:
        printf("\t; %p",VOID(f->p[bx]));
        break;
      case OP_SETLIST:
        if (c==0) printf("\t; %d",(int)code[++pc]);
        else printf("\t; %d",c);
        break;
      default:
        break;
    }
    printf("\n");
  }
}

#define SS(x)	(x==1)?"":"s"
#define S(x)	x,SS(x)

static void PrintHeader(const Proto *f)
{
  const char *s=getstr(f->source);
  const char *n=f->name ? getstr(f->name) : "(anonymous)";
  if (*s=='@' || *s=='=')
    s++;
  else if (*s==LUA_SIGNATURE[0])
    s="(bstring)";
  else
    s="(string)";
  printf("\n%s <%s:%s:"
#ifdef LUA_COD /* print hash */
         "%" LUA_INT_FRMLEN "x:"
#endif /* LUA_COD */
         "%d,%d> (%d instruction%s, %d bytes at %p)\n",
  	(f->linedefined==0)?"main":"function",s,n,
#ifdef LUA_COD
    f->hash,
#endif /* LUA_COD */
    f->linedefined,f->lastlinedefined,
    S(f->sizecode),f->sizecode*Sizeof(Instruction),VOID(f));
  printf("%d%s param%s, %d slot%s, %d upvalue%s, ",
    f->numparams,f->is_vararg?"+":"",SS(f->numparams),
  S(f->maxstacksize),S(f->nups));
    printf("%d local%s, %d constant%s, %d function%s\n",
  S(f->sizelocvars),S(f->sizek),S(f->sizep));
}

static void PrintConstants(const Proto *f)
{
  int i,n=f->sizek;
  printf("constants (%d) for %p:\n",n,VOID(f));
  for (i=0; i<n; i++)
  {
    printf("\t%d\t",i+1);
    PrintConstant(f,i);
    printf("\n");
  }
}

static void PrintLocals(const Proto *f)
{
  int i,n=f->sizelocvars;
  printf("locals (%d) for %p:\n",n,VOID(f));
  for (i=0; i<n; i++)
  {
    printf("\t%d\t%s\t%d\t%d\n", i,
      getstr(f->locvars[i].varname),f->locvars[i].startpc+1,
             f->locvars[i].endpc+1);
  }
}

static void PrintUpvalues(const Proto *f)
{
  int i,n=f->sizeupvalues;
  printf("upvalues (%d) for %p:\n",n,VOID(f));
  if (f->upvalues==NULL) return;
  for (i=0; i<n; i++)
  {
    printf("\t%d\t%s\n",i,getstr(f->upvalues[i]));
  }
}

void PrintFunction(const Proto *f, int full)
{
  int i,n=f->sizep;
  PrintHeader(f);
  PrintCode(f);
  if (full)
  {
    PrintConstants(f);
    PrintLocals(f);
    PrintUpvalues(f);
  }
  for (i=0; i<n; i++) PrintFunction(f->p[i],full);
}

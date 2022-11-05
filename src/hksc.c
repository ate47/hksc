#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/* Must include before any non-system headers */
#include "hksc_begin_code.h"

#include "luaconf.h"

#include "lua.h"

#include "hksclib.h"

#define PROGNAME "hksc" /* default program name */
#define OUTPUT PROGNAME ".out" /* default output file */

static char Output[]={ OUTPUT };

static char *output_file = NULL; /* output file name */

static const char *progname=PROGNAME;

static const char *test_src=
"";

static void
usage(const char *msg)
{
  if (*msg == '-')
    fprintf(stderr, "%s: unrecognized option '%s'\n", progname, msg);
  else
    fprintf(stderr, "%s: %s\n", progname, msg);

  fprintf(stderr,
   "usage: %s [options] [filenames]\n"
   "Available options are:\n"
   "  -- stop handling options\n",
   progname);

  exit(EXIT_FAILURE);
}

static void
print_version(void)
{
  fprintf(stdout, PROGNAME " 0.0.0\n");
}

static int doargs(int argc, char **argv)
{
  if (argv[0] != NULL && *argv[0] != 0)
    progname = argv[0];

  return 1;
}

/* mention an error, but do not exit */
static void
cannot(const char *what, const char *name, int doexit)
{
  fprintf(stderr, "%s: cannot %s %s: %s\n",
    progname, what, name, strerror(errno));

  if (doexit) exit(EXIT_FAILURE);
}

static int
dofile(hksc_State *H, const char *filename)
{
  return hksc_parsefile(H, filename);
}

int
main(int argc, char **argv)
{
  /* parse args */
  int i = doargs(argc, argv);
  argc -= i; argv += 1;

  if (argc <= 0 && 0) usage("no input files given");
  /* warn about using -o with multiple input files */
  else if (argc > 1 &&
           output_file != NULL &&
           output_file != Output)
    usage("'-o' option used with multiple input files");

  if (output_file == NULL) output_file = Output;

  hksc_State *H = hksI_newstate();
  if (!H)
  {
    fprintf(stderr, "cannot allocate an hksc_State\n");
    exit(EXIT_FAILURE);
  }

  /* compile files */
  for (i = 0; i < argc; i++)
  {
    int status = dofile(H, argv[i]);
    if (status)
      fprintf(stderr, "%s\n", luaE_geterrormsg(H));
  }

  printf("hksc: closing hksc_State\n");

  /* TODO: close the state */
  hksc_close(H);

  printf("hksc: closed hksc_State\nExiting with code 0\n");
  return 0;
}

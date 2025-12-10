#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH_MAX 4096
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define abort(cond, msg)                                                       \
  do {                                                                         \
    if (cond) {                                                                \
      fprintf(stderr, msg);                                                    \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

typedef uint64_t u64;
typedef uint32_t u32;
typedef int64_t i64;
typedef int32_t i32;

u64 solve(const char *fname) {
  FILE *fp = fopen(fname, "r");
  abort(!fp, "Unable to open input file!");

  char lines[1024][BUFSIZ];
  u32 L = 0;
  while (fgets(lines[L++], BUFSIZ, fp));
  fclose(fp);

  u32 sum = 0, S = strlen(lines[0]) - 1; /* -1 for '\n' */
  for (u32 l = 1; l < L; l++) {
    for (u32 s = 0; s < S; s++) {
      if (lines[l][s] == '.') {
        /* beam starts or continues */
        if (lines[l - 1][s] == 'S' || lines[l - 1][s] == 'b') lines[l][s] = 'b';
      } else if (lines[l][s] == '^') {
        /* beam splits */
        if (lines[l - 1][s] == 'b') {
          if (s > 0) lines[l][s - 1] = 'b';
          if (s < S - 1) lines[l][s + 1] = 'b';
          sum++;
        }
      }
    }
  }

  return sum;
}

int main(int argc, char *argv[]) {
  char fname[PATH_MAX + 4];
  strncpy(fname, argv[0], PATH_MAX);

  i32 ex = (argc == 2) ? atoi(argv[1]) : 0;
  char *ext = ex ? ".ex" : ".in";
  printf("%llu\n", solve(strncat(fname, ext, 3)));

  return 0;
}

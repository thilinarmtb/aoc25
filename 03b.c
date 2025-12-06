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

u32 get_max(const char *str, u32 *s, u32 e) {
  u32 idx = *s, M = (u32)(str[idx] - '0'), M1;
  for (u32 i = idx + 1; i < e; i++)
    M1 = MAX(M, (u32)(str[i] - '0')), idx = (M != M1) ? i : idx, M = M1;
  *s = idx + 1;
  return M;
}

u64 solve(const char *fname) {
  FILE *fp = fopen(fname, "r");
  abort(!fp, "Unable to open input file!");

  char line[BUFSIZ];
  u64 sum = 0;
  while (fgets(line, BUFSIZ, fp)) {
    u64 J = 0;
    u32 s = 0, e = strlen(line) - 11 - 1; /* -1 for `\n' */
    for (u32 i = 0; i < 12; i++) J = (10 * J + get_max(line, &s, e)), e++;
    sum += J;
  }
  fclose(fp);

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

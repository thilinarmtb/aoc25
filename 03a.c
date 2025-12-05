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

  char line[BUFSIZ];
  u32 sum = 0;
  while (fgets(line, BUFSIZ, fp)) {
    u32 i = 0, M = 0, m = 0;
    for (; i < strlen(line) - 2; i++) {
      u32 M1 = MAX(M, (u32)(line[i] - '0'));
      m = MAX(m, (u32)(line[i] - '0')) * (M1 == M);
      M = M1;
    }
    m = MAX(m, (u32)(line[i] - '0'));
    sum += (10 * M + m);
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

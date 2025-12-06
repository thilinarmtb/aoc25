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
  u32 *G = 0, sz = 0, ln = 0, ls = 0;
  while (fgets(line, BUFSIZ, fp)) {
    ls = strlen(line) - 1; /* -1 for '\n' */
    G = realloc(G, (sz + ls) * sizeof(u32));
    for (u32 l = 0; l < ls; l++) G[sz++] = (line[l] == '@') ? 1 : 0;
    ln++;
  }
  fclose(fp);

#define V(n, s)                                                                \
  (((n) < 0 || (n) >= ln || (s) < 0 || (s) >= ls) ? 0 : G[(n) * ln + (s)])

  u32 sum = 0;
  for (i32 n = 0; n < ln; n++) {
    for (i32 s = 0; s < ls; s++) {
      u32 r = V(n - 1, s - 1) + V(n - 1, s) + V(n - 1, s + 1) + V(n, s - 1) +
              V(n, s + 1) + V(n + 1, s - 1) + V(n + 1, s) + V(n + 1, s + 1);
      sum += (r < 4) && V(n, s);
    }
  }

  free(G);
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

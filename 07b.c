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

  char G[1024][1024];
  u32 L = 0;
  while (fgets(G[L], 1024, fp)) L++;
  fclose(fp);

  u64 T[1024][1024] = {0};
  u32 S = strlen(G[0]) - 1; /* -1 for '\n' */
  for (u32 l = 1; l < L; l++) {
    char H[1024];
    strcpy(H, G[l]);
    for (u32 s = 0; s < S; s++) {
      /* beam starts or continues */
      if (H[s] == '.') {
        if (G[l - 1][s] == 'S') G[l][s] = 'b', T[l][s] = 1;
        if (G[l - 1][s] == 'b') G[l][s] = 'b', T[l][s] += T[l - 1][s];
      }
      /* beam splits */
      if (H[s] == '^' && G[l - 1][s] == 'b') {
        if (s > 0) G[l][s - 1] = 'b', T[l][s - 1] += T[l - 1][s];
        if (s < S - 1) G[l][s + 1] = 'b', T[l][s + 1] += T[l - 1][s];
      }
    }
  }

  u64 A = 0;
  for (u32 s = 0; s < S; s++) A += T[L - 1][s];

  return A;
}

int main(int argc, char *argv[]) {
  char fname[PATH_MAX + 4];
  strncpy(fname, argv[0], PATH_MAX);

  i32 ex = (argc == 2) ? atoi(argv[1]) : 0;
  char *ext = ex ? ".ex" : ".in";
  printf("%llu\n", solve(strncat(fname, ext, 3)));

  return 0;
}

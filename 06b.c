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

u32 space(char *s) {
  u32 l = strlen(s);
  u32 n = 0;
  while (n < l && s[n] != ' ') n++;
  return (strlen(s) != n) ? n : n - 1;
}

u32 width(char *s[], u32 L, u32 o) {
  u32 w = 0;
  for (u32 l = 0; l < (L - 1); l++) w = MAX(w, space(&s[l][o]));
  return w;
}

u64 do_problem(char *s[], u32 L, u32 *o) {
  i32 W = width(s, L, *o);
  char op = s[L - 1][*o];
  u64 sum = (op == '+') ? 0 : 1;
  for (i32 w = W - 1; w >= 0; w--) {
    u32 d = 0;
    for (u32 l = 0; l < (L - 1); l++) {
      char c = s[l][*o + w];
      if (c == ' ') continue;
      d = 10 * d + c - '0';
    }
    if (op == '+')
      sum += d;
    else
      sum *= d;
  }

  (*o) += (W + 1); /* + 1 is for the space */

  return sum;
}

u64 solve(const char *fname) {
  FILE *fp = fopen(fname, "r");
  abort(!fp, "Unable to open input file!");

  char line[8 * BUFSIZ], *delims = " \n";
  char *lines[1024];
  u64 L = 0;
  while (fgets(line, 8 * BUFSIZ, fp)) {
    u32 S = strlen(line);
    lines[L] = calloc(S + 1, sizeof(char));
    strncpy(lines[L], line, S);
    L++;
  }

  fclose(fp);

  u64 sum = 0;
  u32 o = 0;
  while (o < strlen(lines[0])) sum += do_problem(lines, L, &o);

  for (u32 l = 0; l < L; l++) free(lines[l]);

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

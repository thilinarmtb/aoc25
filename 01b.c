#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH_MAX 4096

#define abort(cond, msg)    \
  do {                      \
    if (cond) {             \
      fprintf(stderr, msg); \
      exit(EXIT_FAILURE);   \
    }                       \
  } while(0)

int solve(const char *fname) {
  FILE *fp = fopen(fname, "r");
  abort(!fp, "Unable to open input file!");

  char line[BUFSIZ], c;
  int n, s, t, p = 50, pw = 0;
  while (fgets(line, BUFSIZ, fp)) {
    sscanf(line, "%c%d\n", &c, &n);

    s = (c == 'L') ? -1 : 1;

    pw += (n / 100), n = n % 100;
    t = p + s * n;
    pw += (p * t < 0) || ((p - 100) * (t - 100) < 0);
    p = (t + 100) % 100, pw += (p == 0);
  }

  fclose(fp);

  return pw;
}

int main(int argc, char *argv[]) {
  char fname[PATH_MAX + 4];
  strncpy(fname, argv[0], PATH_MAX);

  int ex = (argc == 2) ? atoi(argv[1]) : 0;
  char *ext = ex ? ".ex" : ".in";
  printf("%d\n", solve(strncat(fname, ext, 3)));

  return 0;
}

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

int rotate(int pos, char c, int n) {
  int s = (c == 'L') ? -1 : 1;
  return (pos + s * n + 100) % 100;
}

int solve(const char *fname) {
  FILE *fp = fopen(fname, "r");
  abort(!fp, "Unable to open input file!");

  char line[BUFSIZ], c;
  int n;
  int pos = 50;
  int password = 0;
  while (fgets(line, BUFSIZ, fp)) {
    sscanf(line, "%c%d\n", &c, &n);
    pos = rotate(pos, c, n);
    password += (pos == 0);
  }

  fclose(fp);

  return password;
}

int main(int argc, char *argv[]) {
  char fname[PATH_MAX + 4];
  strncpy(fname, argv[0], PATH_MAX);

  int mode = 0;
  if (argc == 2) mode = atoi(argv[1]);

  char *ext = ".in";
  if (mode) ext = ".ex";

  printf("%d\n", solve(strncat(fname, ext, 3)));

  return 0;
}

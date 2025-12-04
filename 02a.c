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

typedef unsigned long long ulong;
typedef unsigned uint;

ulong powi(ulong b, uint e) {
  ulong r = 1;
  while (e) r *= b, e--;
  return r;
}

ulong rsum(const char *fid, const char *sid) {
  ulong f = atoll(fid), s = atoll(sid);

  ulong sum = 0;
  for (uint l = MIN(2, strlen(fid)); l <= strlen(sid); l++) {
    uint h = (l + 1) / 2;
    ulong d = powi(10, h) + 1;
    ulong mr = MAX(powi(10, h - 1), f / d + ((f % d) != 0));
    ulong Mr = MIN(powi(10, h) - 1, s / d);
    if (mr <= Mr) sum += ((Mr + mr) * (Mr - mr + 1) * d) / 2;
    l++;
  }

  return sum;
}

ulong solve(const char *fname) {
  FILE *fp = fopen(fname, "r");
  abort(!fp, "Unable to open input file!");

  char line[BUFSIZ];
  fgets(line, BUFSIZ, fp);
  fclose(fp);

  const char *delims = "-,\n";
  char *fid = strtok(line, delims), *sid = 0;
  ulong sum = 0;
  while (fid) {
    sid = strtok(NULL, delims);
    sum += rsum(fid, sid);
    fid = strtok(NULL, delims);
  }

  return sum;
}

int main(int argc, char *argv[]) {
  char fname[PATH_MAX + 4];
  strncpy(fname, argv[0], PATH_MAX);

  int ex = (argc == 2) ? atoi(argv[1]) : 0;
  char *ext = ex ? ".ex" : ".in";
  printf("%llu\n", solve(strncat(fname, ext, 3)));

  return 0;
}

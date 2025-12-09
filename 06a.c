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

typedef struct {
  u64 max, n;
  void *ptr;
} Array_t, *Array;

void arr_init_(Array arr, u64 max, u64 us) {
  arr->max = max, arr->n = 0, arr->ptr = calloc(max, us);
}
#define arr_init(T, arr, max) arr_init_(arr, max, sizeof(T))

void arr_cat_(Array arr, const void *ptr, u64 n, u64 us) {
  if (arr->max < (arr->n + n)) arr->max += arr->max / 2 + 1;
  if (arr->max < (arr->n + n)) arr->max = arr->n + n;
  arr->ptr = realloc(arr->ptr, arr->max * us);
  memcpy((char *)arr->ptr + arr->n * us, ptr, n * us);
  arr->n += n;
}
#define arr_cat(T, arr, ptr, n) arr_cat_(arr, ptr, n, sizeof(T))

void arr_free(Array arr) { free(arr->ptr), arr->n = 0, arr->max = 0; }

typedef struct {
  u64 v;
} Number_t, *Number;

u64 solve(const char *fname) {
  FILE *fp = fopen(fname, "r");
  abort(!fp, "Unable to open input file!");

  Array_t A;
  arr_init(Number_t, &A, 1000);

  char line[8 * BUFSIZ], *delims = " \n";
  u64 L = 0;
  Number_t N;
  while (fgets(line, 8 * BUFSIZ, fp)) {
    const char *a = strtok(line, delims);
    while (a) {
      N.v = (a[0] == '+') ? 0 : (a[0] == '*' ? 1 : atoll(a));
      arr_cat(Number_t, &A, &N, 1), a = strtok(0, delims);
    }
    L++;
  }

  u64 P = A.n / L;
  Number ptr = A.ptr;
  u64 sum = 0;
  for (u32 p = 0; p < P; p++) {
    if (ptr[p + (L - 1) * P].v == 0) { /* + */
      for (u32 l = 0; l < (L - 1); l++) sum += ptr[p + l * P].v;
    } else { /* x */
      u64 prod = 1;
      for (u32 l = 0; l < (L - 1); l++) prod *= ptr[p + l * P].v;
      sum += prod;
    }
  }

  fclose(fp);
  arr_free(&A);

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

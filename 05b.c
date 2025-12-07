#include <assert.h>
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
  arr->max = max, arr->n = 0;
  arr->ptr = calloc(max, us);
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
  u64 a, b;
} Range_t, *Range;

int cmp(const void *a_, const void *b_) {
  const Range a = (const Range)a_;
  const Range b = (const Range)b_;
  if (a->a < b->a) return -1;
  if (a->a == b->a) return 0;
  return 1;
}

u64 solve(const char *fname) {
  FILE *fp = fopen(fname, "r");
  abort(!fp, "Unable to open input file!");

  Array_t A;
  arr_init(Range_t, &A, 100);

  char line[BUFSIZ];
  const char *delims = "-\n";
  Range_t r;
  while (strlen(fgets(line, BUFSIZ, fp)) > 1) {
    r.a = atoll(strtok(line, delims)), r.b = atoll(strtok(0, delims));
    assert(r.a <= r.b);
    arr_cat(Range_t, &A, &r, 1);
  }

  qsort(A.ptr, A.n, sizeof(Range_t), cmp);

  Range p = (Range)A.ptr;
  u32 i = 1;
  while (i < A.n) {
    // skip if we can't merge the ranges.
    if (p[i - 1].b < p[i].a) {
      i++;
      continue;
    }

    // merge otherwise.
    p[i - 1].b = MAX(p[i - 1].b, p[i].b);
    for (u32 j = i; j < A.n - 1; j++) p[j] = p[j + 1];
    A.n--;
  }

  u64 sum = 0;
  p = (Range)A.ptr;
  for (u32 i = 0; i < A.n; i++) sum += (p[i].b - p[i].a + 1);

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

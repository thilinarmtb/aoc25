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

u64 absi(i64 a) { return (a < 0) ? -a : a; }

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
  i64 x, y;
} Tile_t, *Tile;

u64 solve(const char *fname) {
  FILE *fp = fopen(fname, "r");
  abort(!fp, "Unable to open input file!");

  Array_t tiles;
  arr_init(Tile_t, &tiles, 1000);

  char line[BUFSIZ];
  Tile_t t;
  while (fgets(line, BUFSIZ, fp)) {
    sscanf(line, "%lld,%lld,\n", &t.x, &t.y);
    arr_cat(Tile_t, &tiles, &t, 1);
  }
  fclose(fp);

  u64 A = 0;
  Tile tp = (Tile)tiles.ptr;
  for (u32 i = 0; i < tiles.n; i++) {
    for (u32 j = i + 1; j < tiles.n; j++)
      if ((absi(tp[i].x - tp[j].x + 1) * absi(tp[i].y - tp[j].y + 1)) > A)
        A = absi(tp[i].x - tp[j].x + 1) * absi(tp[i].y - tp[j].y + 1);
  }

  arr_free(&tiles);

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

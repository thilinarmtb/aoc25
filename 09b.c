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

int cmp(const void *a, const void *b) {
  Tile ta = (Tile)a, tb = (Tile)b;
  return (ta->y < tb->y
              ? -1
              : (ta->y > tb->y
                     ? 1
                     : (ta->x < tb->x ? -1 : (ta->x > tb->x ? 1 : 0))));
}

int inside_bbox(Tile p0, Tile p1, Array bbox) {
  i64 miny = MIN(p0->y, p1->y);
  i64 maxy = MAX(p0->y, p1->y);
  i64 minx = MIN(p0->x, p1->x);
  i64 maxx = MAX(p0->x, p1->x);

  Tile bp = (Tile)bbox->ptr;
  u32 s = 0;
  while (s < bbox->n && bp[s].y < miny) s += 2;
  assert(s < bbox->n && bp[s].y == miny);
  for (u32 i = s; i < bbox->n && bp[i].y <= maxy; i += 2)
    if (bp[i].x > minx || bp[i + 1].x < maxx) return 0;
  return 1;
}

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

  /* add first tile to end to close the loop */
  arr_cat(Tile_t, &tiles, &((Tile)tiles.ptr)[0], 1);

  Array_t ranges;
  arr_init(Tile_t, &ranges, 1000);

  Tile tp = (Tile)tiles.ptr;
  for (u32 i = 0; i < tiles.n - 1; i++) {
    i64 minx = MIN(tp[i].x, tp[i + 1].x);
    i64 maxx = MAX(tp[i].x, tp[i + 1].x);
    i64 miny = MIN(tp[i].y, tp[i + 1].y);
    i64 maxy = MAX(tp[i].y, tp[i + 1].y);
    for (i64 x = minx; x <= maxx; x++) {
      for (i64 y = miny; y <= maxy; y++) {
        t.x = x, t.y = y;
        arr_cat(Tile_t, &ranges, &t, 1);
      }
    }
  }

  /* sort tiles first by y then by x */
  qsort(ranges.ptr, ranges.n, sizeof(Tile_t), cmp);

  Array_t bbox;
  arr_init(Tile_t, &bbox, 1000);

  Tile rp = (Tile)ranges.ptr;
  u32 s = 0;
  while (s < ranges.n - 1) {
    u32 e = s + 1;
    while (e < ranges.n && rp[e].y == rp[s].y) e++;
    assert((e - 1) > s);
    arr_cat(Tile_t, &bbox, &rp[s], 1);
    arr_cat(Tile_t, &bbox, &rp[e - 1], 1);
    s = e;
  }

#define AREA(a, b) ((absi((a).x - (b).x) + 1) * (absi((a).y - (b).y) + 1))
  u64 A = 0;
  for (u32 i = 0; i < tiles.n; i++) {
    for (u32 j = i + 1; j < tiles.n; j++) {
      if (inside_bbox(tp + i, tp + j, &bbox) && AREA(tp[i], tp[j]) > A)
        A = AREA(tp[i], tp[j]);
    }
  }

  arr_free(&tiles), arr_free(&ranges), arr_free(&bbox);

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

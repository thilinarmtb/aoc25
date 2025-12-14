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
  i64 x, y, z;
} Box_t, *Box;

typedef struct {
  i64 i, j, d;
} Edge_t, *Edge;

int cmp_edges(const void *a, const void *b) {
  Edge ea = (Edge)a, eb = (Edge)b;
  return (ea->d < eb->d ? -1 : (ea->d > eb->d ? 1 : 0));
}

int cmp_u64(const void *a, const void *b) {
  u64 ua = *(u64 *)a, ub = *(u64 *)b;
  return (ua < ub) ? -1 : ((ua > ub) ? 1 : 0);
}

i64 xn(i64 x, u32 n) {
  i64 r = 1;
  for (u32 i = 0; i < n; i++) r *= x;
  return r;
}

u64 solve(const char *fname, u32 max_conns) {
  FILE *fp = fopen(fname, "r");
  abort(!fp, "Unable to open input file!");

  Array_t boxes;
  arr_init(Box_t, &boxes, 1000);

  char line[BUFSIZ];
  Box_t b;
  while (fgets(line, BUFSIZ, fp)) {
    sscanf(line, "%lld,%lld,%lld\n", &b.x, &b.y, &b.z);
    arr_cat(Box_t, &boxes, &b, 1);
  }
  fclose(fp);

  Array_t edges;
  arr_init(Edge_t, &edges, 1000);
  Box bp = (Box)boxes.ptr;
  for (u64 i = 0; i < boxes.n; i++) {
    for (u64 j = i + 1; j < boxes.n; j++) {
      i64 d = xn(bp[i].x - bp[j].x, 2) + xn(bp[i].y - bp[j].y, 2) +
              xn(bp[i].z - bp[j].z, 2);
      Edge_t e = {.i = i, .j = j, .d = d};
      arr_cat(Edge_t, &edges, &e, 1);
    }
  }

  qsort(edges.ptr, edges.n, sizeof(Edge_t), cmp_edges);

  Edge ep = (Edge)edges.ptr;
  u32 *circuit = calloc(boxes.n, sizeof(u32));
  u32 ncircuits = 0, done = 0;
  for (u64 n = 0; (n < max_conns) && !done; n++) {
    Edge_t e = ep[n];
    u32 ci = circuit[e.i], cj = circuit[e.j];

    if (ci == 0 && cj == 0) {
      ncircuits++;
      circuit[e.i] = ncircuits;
      circuit[e.j] = ncircuits;
    } else if (ci != 0 && cj == 0) {
      circuit[e.j] = ci;
    } else if (ci == 0 && cj != 0) {
      circuit[e.i] = cj;
    } else if (ci != cj) {
      u32 cmin = MIN(ci, cj);
      u32 cmax = MAX(ci, cj);
      for (u64 k = 0; k < boxes.n; k++)
        if (circuit[k] == cmax) circuit[k] = cmin;
    } else if (ci == cj) {
    }

    done = 1;
    for (u64 k = 0; k < boxes.n; k++) {
      if (circuit[k] == 0) {
        done = 0;
        break;
      }
    }
  }

  u64 *sizes = calloc(ncircuits + 1, sizeof(u64));
  for (u64 i = 0; i < boxes.n; i++) sizes[circuit[i]]++;

  /* exclude size 0 */
  qsort(sizes + 1, ncircuits, sizeof(u64), cmp_u64);

  printf("Circuit sizes:\n");
  for (u32 i = 0; i <= ncircuits; i++) printf("Size %u: %llu\n", i, sizes[i]);

  u64 A = sizes[ncircuits] * sizes[ncircuits - 1] * sizes[ncircuits - 2];

  arr_free(&boxes), arr_free(&edges);
  free(circuit), free(sizes);

  return A;
}

int main(int argc, char *argv[]) {
  char fname[PATH_MAX + 4];
  strncpy(fname, argv[0], PATH_MAX);

  i32 ex = (argc == 2) ? atoi(argv[1]) : 0;
  char *ext = ex ? ".ex" : ".in";
  u32 max_conns = ex ? 10 : 1000;
  printf("%llu\n", solve(strncat(fname, ext, 3), max_conns));

  return 0;
}

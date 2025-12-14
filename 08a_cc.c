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

typedef struct {
  i64 u, v, i;
} Node_t, *Node;

int cmp_distance(const void *a, const void *b) {
<<<<<<< HEAD
  Edge ea = (Edge)a, eb = (Edge)b;
  return (ea->d < eb->d ? -1 : (ea->d > eb->d ? 1 : 0));
=======
  Edge da = (Edge)a, db = (Edge)b;
  return da->d - db->d;
>>>>>>> fd343b3 (Refactor 08a)
}

int cmp_nodes(const void *a, const void *b) {
  Node na = (Node)a, nb = (Node)b;
  return na->u - nb->u;
}

int cmp_node_indices(const void *a, const void *b) {
  Node ua = (Node)a, ub = (Node)b;
  return ua->i - ub->i;
}

int cmp_u64_reverse(const void *a, const void *b) {
  u64 ua = *(u64 *)a, ub = *(u64 *)b;
  return (ua < ub) ? 1 : ((ua > ub) ? -1 : 0);
}

i64 xn(i64 x, u32 n) {
  i64 r = 1;
  for (u32 i = 0; i < n; i++) r *= x;
  return r;
}

u64 solve(const char *fname, u32 npairs) {
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

  Box pb = (Box)boxes.ptr;
  for (u64 i = 0; i < boxes.n; i++) {
    for (u64 j = i + 1; j < boxes.n; j++) {
      i64 d = xn(pb[i].x - pb[j].x, 2) + xn(pb[i].y - pb[j].y, 2) +
              xn(pb[i].z - pb[j].z, 2);
      Edge_t e = {.i = i, .j = j, .d = d};
      arr_cat(Edge_t, &edges, &e, 1);
    }
  }
  assert(edges.n == (boxes.n * (boxes.n - 1)) / 2);

  /* first sort by distance */
  qsort(edges.ptr, edges.n, sizeof(Edge_t), cmp_distance);

  /* renumber the nodes to be contiguous from 0 */
  Array_t nodes;
  arr_init(Node_t, &nodes, 2 * npairs);

  /* Add nodes to an array and sort them */
  Edge pe = (Edge)edges.ptr;
  for (u64 i = 0; i < npairs; i++) {
    Node_t ni = {.u = pe[i].i, .i = i};
    arr_cat(Node_t, &nodes, &ni, 1);
    Node_t nj = {.u = pe[i].j, .i = i};
    arr_cat(Node_t, &nodes, &nj, 1);
  }

  qsort(nodes.ptr, nodes.n, sizeof(Node_t), cmp_nodes);

  /* renumber the node values */
  i64 N = 0;
  u64 s = 0;
  Node pn = (Node)nodes.ptr;
  while (s < nodes.n) {
    i64 u = pn[s].u;
    while (s < nodes.n && pn[s].u == u) pn[s].u = N, s++;
    N++;
  }

  /* sort the nodes back to the original order */
  qsort(nodes.ptr, nodes.n, sizeof(Node_t), cmp_node_indices);

  /* reconstruct the edges with the new node numbers  and use it to construct
   * the adjacency matrix */
  u64 *adj = calloc(N * N, sizeof(u64));
  pn = (Node)nodes.ptr;
  for (u64 i = 0; i < npairs; i++) {
    u64 u = pn[2 * i].u;
    u64 v = pn[2 * i + 1].u;
    adj[u * N + v] = adj[v * N + u] = 1;
  }

  /* Now find the connected components by the most inefficient way possible --
   * by adjacency matrix times a seed vector multiplication until convergence */
  u32 seed = 1;
  u32 *found = calloc(N, sizeof(u32));
  u32 *x = calloc(N, sizeof(u32));
  u32 *y = calloc(N, sizeof(u32));

  for (u64 k = 0; k < N; k++) {
    if (found[k] > 0) continue;

    found[k] = seed;
    for (u64 i = 0; i < N; i++) x[i] = 0;
    x[k] = 1;

    u64 c0 = 0, c1 = 1;
    while (c0 < c1) {
      for (u64 i = 0; i < N; i++) {
        y[i] = 0;
        for (u64 j = 0; j < N; j++) y[i] += adj[i * N + j] * x[j];
      }

      for (u64 i = 0; i < N; i++) {
        found[i] = (found[i] > 0) ? found[i] : ((y[i] > 0) ? seed : 0);
        x[i] = (found[i] == seed);
      }

      c0 = c1, c1 = 0;
      for (u64 i = 0; i < N; i++) c1 += (found[i] > 0);
    }

    seed++;
  }

  u64 *sizes = calloc(seed, sizeof(u64));
  for (u32 s = 1; s < seed; s++) {
    u64 c = 0;
    for (u64 i = 0; i < N; i++)
      if (found[i] == s) c++;
    sizes[s - 1] = c;
  }
  qsort(sizes, seed - 1, sizeof(u64), cmp_u64_reverse);

  u64 A = sizes[0] * sizes[1] * sizes[2];

  arr_free(&boxes), arr_free(&edges), arr_free(&nodes);
  free(adj), free(found), free(x), free(y), free(sizes);

  return A;
}

int main(int argc, char *argv[]) {
  char fname[PATH_MAX + 4];
  strncpy(fname, argv[0], PATH_MAX);

  i32 ex = (argc == 2) ? atoi(argv[1]) : 0;
  char *ext = ex ? ".ex" : ".in";
  u32 npairs = ex ? 10 : 1000;
  printf("%llu\n", solve(strncat(fname, ext, 3), npairs));

  return 0;
}

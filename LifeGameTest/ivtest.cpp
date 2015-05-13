#include "stdafx.h"

#include "..\LifeGame\ivlib.h"

using namespace std;

void generic_ui8vaddu(int n, const uint8_t *lhs, const uint8_t *rhs,
                      uint8_t *dst) {
  for (int i = 0; i < n; ++i) {
    dst[i] = lhs[i] + rhs[i];
  }
}

void generic_ui8vcmpcu(int n, const uint8_t *v, uint8_t *r, int a, ivOp op) {
  switch (op) {
  case ivEq:
    for (int i = 0; i < n; ++i) {
      if (v[i] == a)
        r[i] = 0xFF;
      else
        r[i] = 0x00;
    }
    break;
  }
}

void generic_ui8vandu(int n, const uint8_t *a, const uint8_t *b, uint8_t *x) {
  for (int i = 0; i < n; ++i) {
    x[i] = a[i] & b[i];
  }
}

void generic_ui8voru(int n, const uint8_t *a, const uint8_t *b, uint8_t *x) {
  for (int i = 0; i < n; ++i) {
    x[i] = a[i] | b[i];
  }
}

void generic_ui8vandcu(int n, const uint8_t *a, uint8_t *x, uint8_t c) {
  for (int i = 0; i < n; ++i) {
    x[i] = a[i] & c;
  }
}

TEST(ivlib, ivadd_aligned_memory) {
  const int N = 128;
  uint8_t lhs[N], rhs[N];
  fill(lhs, lhs + N, 1);
  fill(rhs, rhs + N, 2);
  uint8_t sum[N];

  ui8vadd(N, lhs, rhs, sum);

  uint8_t expect[N];
  generic_ui8vaddu(N, lhs, rhs, expect);

  ASSERT_EQ(0, memcmp(sum, expect, N));
}

TEST(ivlib, ui8vaddu) {
  const int N = 128;
  uint8_t lhs[N], rhs[N];
  fill(lhs, lhs + N, 1);
  fill(rhs, rhs + N, 2);
  uint8_t actual[N] = { 33 };

  ui8vaddu(N - 3, lhs + 3, rhs + 3, actual + 1);

  uint8_t expect[N] = { 33 };
  generic_ui8vaddu(N - 3, lhs + 3, rhs + 3, expect + 1);

  ASSERT_EQ(0, memcmp(actual, expect, N));
}

TEST(ivlib, ui8vcmpcu) {
  const int N = 143;
  uint8_t v[N], actual[N];
  fill(v, v + 33, 2);
  fill(v + 33, v + 143, 3);

  ui8vcmpcu(N, v, actual, 2, ivEq);

  uint8_t expect[N];
  generic_ui8vcmpcu(N, v, expect, 2, ivEq);

  ASSERT_EQ(0, memcmp(actual, expect, N));
}

TEST(ivlib, ui8vandu) {
  const int N = 143;
  uint8_t a[N], b[N], actual[N];
  fill(a, a + N, 2);
  fill(b, b + N, 2);

  ui8vandu(N, a, b, actual);

  uint8_t expect[N];
  generic_ui8vandu(N, a, b, expect);

  ASSERT_EQ(0, memcmp(actual, expect, N));
}

TEST(ivlib, ui8voru) {
  const int N = 143;
  uint8_t a[N], b[N], actual[N];
  fill(a, a + N, 2);
  fill(b, b + N, 2);

  ui8voru(N, a, b, actual);

  uint8_t expect[N];
  generic_ui8voru(N, a, b, expect);

  ASSERT_EQ(0, memcmp(actual, expect, N));
}

TEST(ivlib, ui8vandcu) {
  const int N = 243;
  uint8_t a[N], actual[N];
  fill(a, a + 100, 0xFF);
  fill(a + 100, a + 100 + 100, 0);
  uint8_t c = 1;

  ui8vandcu(N, a, actual, c);

  uint8_t expect[N];
  generic_ui8vandcu(N, a, expect, c);

  ASSERT_EQ(0, memcmp(actual, expect, N));
}

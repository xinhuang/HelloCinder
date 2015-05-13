#pragma once

#include <stdint.h>

//#ifdef __cplusplus

void ui8vadd(int n, const uint8_t *lhs, int lhs_stride, const uint8_t *rhs,
              int rhs_stride, uint8_t *dst, int dst_stride);
void ui8vaddu(int n, const uint8_t *lhs, int lhs_stride, const uint8_t *rhs,
              int rhs_stride, uint8_t *dst, int dst_stride);

enum ivOp {
	ivEq = 0, //_MM_CMP_INT_EQ
};

void ui8vcmpcu(int n, const uint8_t* a, uint8_t* x, uint8_t c, ivOp op);

void ui8vandu(int n, const uint8_t* a, const uint8_t* b, uint8_t* x);
void ui8voru(int n, const uint8_t* a, const uint8_t* b, uint8_t* x);

//#endif // __cplusplus

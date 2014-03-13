#pragma once

#include <cstdint>
#include <cassert>

void *aligned_malloc(int size, int alignment);
void aligned_free(void *ptr);

#if defined USE_IPP
#define _IPP_SEQUENTIAL_STATIC
#include <ipp.h>

#else

enum IppStatus {
  ippStsNoErr,
};

enum IppCmpOp {
  ippCmpLess,
  ippCmpLessEq,
  ippCmpEq,
  ippCmpGreaterEq,
  ippCmpGreater
};

struct IppiSize {
  int width;
  int height;
};

IppStatus ippInit();

inline uint8_t *ippsMalloc_8u(int size) {
  assert(size >= sizeof(intptr_t));
  return static_cast<uint8_t*>(aligned_malloc(size, 32));
}

inline void ippsFree(void *p) { return aligned_free(p); }

IppStatus ippiAndC_8u_C1R(const uint8_t *pSrc, int srcStep, const uint8_t value,
                          uint8_t *pDst, int dstStep, IppiSize roiSize);

IppStatus ippiAnd_8u_C1R(const uint8_t *pSrc1, int src1Step,
                         const uint8_t *pSrc2, int src2Step, uint8_t *pDst,
                         int dstStep, IppiSize roiSize);

IppStatus ippiOr_8u_C1R(const uint8_t *pSrc1, int src1Step,
                        const uint8_t *pSrc2, int src2Step, uint8_t *pDst,
                        int dstStep, IppiSize roiSize);

IppStatus ippiSet_8u_C1R(uint8_t value, uint8_t *pDst, int dstStep,
                         IppiSize roiSize);

IppStatus ippiAdd_8u_C1RSfs(const uint8_t *pSrc1, int src1Step,
                            const uint8_t *pSrc2, int src2Step, uint8_t *pDst,
                            int dstStep, IppiSize roiSize, int scaleFactor);

IppStatus ippiCopy_8u_C1R(const uint8_t *pSrc, int srcStep, uint8_t *pDst,
                          int dstStep, IppiSize roiSize);

IppStatus ippiCompareC_8u_C1R(const uint8_t *pSrc, int srcStep, uint8_t value,
                              uint8_t *pDst, int dstStep, IppiSize roiSize,
                              IppCmpOp ippCmpOp);

#endif // USE_IPP

#ifndef __LOCBIN_FUNC__
#define __LOCBIN_FUNC__

void LocBinarize(unsigned char *pDst, const unsigned char *pSrc, 
                 int nDstPitch, int nSrcPitch, int nWidth, int nHeight,
                 int nThreshold1, int nThreshold2, int nRadius,
                 int nDiscardThreshold, int nBaseThreshold1,
                 int nBaseThreshold2, int nWeight, bool upper);

#endif
#ifndef __MAGOUILLE_H__
#define __MAGOUILLE_H__

#include "BaseFilter.h"

class Magouille : public GenericVideoFilter {
protected:

	int nThreshold;
	int nWidth, nHeight;

	void (*filter)(BYTE *pDst, const BYTE *pSrc, int nDstPitch, int nSrcPitch, int nThreshold);

public:
	Magouille(PClip _child, int nMode, int nThreshold, bool mmx, bool isse, bool sse2, IScriptEnvironment* env);
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
};

#endif
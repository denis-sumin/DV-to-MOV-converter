#ifndef __MV_DEGRAIN3__
#define __MV_DEGRAIN3__

#include "MVInterface.h"
#include "CopyCode.h"
#include "yuy2planes.h"
#include "overlap.h"

typedef void (Denoise3Function)(BYTE *pDst, int nDstPitch, const BYTE *pSrc, int nSrcPitch,
						const BYTE *pRefB, int BPitch, const BYTE *pRefF, int FPitch,
						const BYTE *pRefB2, int B2Pitch, const BYTE *pRefF2, int F2Pitch,
						const BYTE *pRefB3, int B3Pitch, const BYTE *pRefF3, int F3Pitch,
						int WSrc, int WRefB, int WRefF, int WRefB2, int WRefF2, int WRefB3, int WRefF3);

/*! \brief Filter that denoise the picture
 */
class MVDegrain3 : public GenericVideoFilter, public MVFilter {
private:

   MVClip mvClipB;
   MVClip mvClipF;
   MVClip mvClipB2;
   MVClip mvClipF2;
   MVClip mvClipB3;
   MVClip mvClipF3;
   int thSAD;
   int thSADC;
   int YUVplanes;
   int nLimit;
   int nLimitC;
   PClip super;
   bool isse;
   bool planar;

    int nSuperModeYUV;

	YUY2Planes * DstPlanes;
	YUY2Planes * SrcPlanes;

	short *winOver;
	short *winOverUV;

	OverlapWindows *OverWins;
	OverlapWindows *OverWinsUV;

	OverlapsFunction *OVERSLUMA;
	OverlapsFunction *OVERSCHROMA;
	Denoise3Function *DEGRAINLUMA;
	Denoise3Function *DEGRAINCHROMA;

	unsigned char *tmpBlock;
	unsigned short * DstShort;
	int dstShortPitch;

    MVGroupOfFrames *pRefBGOF, *pRefFGOF;
    MVGroupOfFrames *pRefB2GOF, *pRefF2GOF;
    MVGroupOfFrames *pRefB3GOF, *pRefF3GOF;

public:
	MVDegrain3(PClip _child, PClip _super, PClip _mvbw, PClip _mvfw, PClip _mvbw2,  PClip _mvfw2, PClip _mvbw3,  PClip _mvfw3,
	int _thSAD, int _thSADC, int _YUVplanes, int _nLimit, int _nLimitC,
		int nSCD1, int nSCD2, bool isse, bool _planar, IScriptEnvironment* env);
	~MVDegrain3();
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
};

template<int blockWidth, int blockHeight>
void Degrain3_C(BYTE *pDst, int nDstPitch, const BYTE *pSrc, int nSrcPitch,
						const BYTE *pRefB, int BPitch, const BYTE *pRefF, int FPitch,
						const BYTE *pRefB2, int B2Pitch, const BYTE *pRefF2, int F2Pitch,
						const BYTE *pRefB3, int B3Pitch, const BYTE *pRefF3, int F3Pitch,
						int WSrc, int WRefB, int WRefF, int WRefB2, int WRefF2, int WRefB3, int WRefF3)
{
	for (int h=0; h<blockHeight; h++)
	{
		for (int x=0; x<blockWidth; x++)
		{
			 pDst[x] = (pRefF[x]*WRefF + pSrc[x]*WSrc + pRefB[x]*WRefB + pRefF2[x]*WRefF2 + pRefB2[x]*WRefB2 + pRefF3[x]*WRefF3 + pRefB3[x]*WRefB3 + 128)>>8;
		}
		pDst += nDstPitch;
		pSrc += nSrcPitch;
		pRefB += BPitch;
		pRefF += FPitch;
		pRefB2 += B2Pitch;
		pRefF2 += F2Pitch;
		pRefB3 += B3Pitch;
		pRefF3 += F3Pitch;
	}
}

#include <mmintrin.h>
template<int blockWidth, int blockHeight>
void Degrain3_mmx(BYTE *pDst, int nDstPitch, const BYTE *pSrc, int nSrcPitch,
						const BYTE *pRefB, int BPitch, const BYTE *pRefF, int FPitch,
						const BYTE *pRefB2, int B2Pitch, const BYTE *pRefF2, int F2Pitch,
						const BYTE *pRefB3, int B3Pitch, const BYTE *pRefF3, int F3Pitch,
						int WSrc, int WRefB, int WRefF, int WRefB2, int WRefF2, int WRefB3, int WRefF3)
{
	__m64 z = _mm_setzero_si64();
	__m64 o = _mm_set1_pi16(128);
	__m64 ws = _mm_set1_pi16(WSrc);
	__m64 wb1 = _mm_set1_pi16(WRefB);
	__m64 wf1 = _mm_set1_pi16(WRefF);
	__m64 wb2 = _mm_set1_pi16(WRefB2);
	__m64 wf2 = _mm_set1_pi16(WRefF2);
	__m64 wb3 = _mm_set1_pi16(WRefB3);
	__m64 wf3 = _mm_set1_pi16(WRefF3);

	for (int h=0; h<blockHeight; h++)
	{
		for (int x=0; x<blockWidth; x+=4)
		{
			 *(int*)(pDst + x) = _m_to_int(_m_packuswb(_m_psrlwi(
				 _m_paddw(_m_pmullw(_m_punpcklbw(*(__m64*)(pSrc   + x), z), ws),
				 _m_paddw(_m_pmullw(_m_punpcklbw(*(__m64*)(pRefB  + x), z), wb1),
				 _m_paddw(_m_pmullw(_m_punpcklbw(*(__m64*)(pRefF  + x), z), wf1),
				 _m_paddw(_m_pmullw(_m_punpcklbw(*(__m64*)(pRefB2 + x), z), wb2),
				 _m_paddw(_m_pmullw(_m_punpcklbw(*(__m64*)(pRefF2 + x), z), wf2),
				 _m_paddw(_m_pmullw(_m_punpcklbw(*(__m64*)(pRefB3 + x), z), wb3),
				 _m_paddw(_m_pmullw(_m_punpcklbw(*(__m64*)(pRefF3 + x), z), wf3),
				 o))))))), 8), z));
//			 pDst[x] = (pRefF[x]*WRefF + pSrc[x]*WSrc + pRefB[x]*WRefB + pRefF2[x]*WRefF2 + pRefB2[x]*WRefB2 + pRefF3[x]*WRefF3 + pRefB3[x]*WRefB3 + 128)>>8;
		}
		pDst += nDstPitch;
		pSrc += nSrcPitch;
		pRefB += BPitch;
		pRefF += FPitch;
		pRefB2 += B2Pitch;
		pRefF2 += F2Pitch;
		pRefB3 += B3Pitch;
		pRefF3 += F3Pitch;
	}
	_m_empty();
}

#include <emmintrin.h>
template<int blockWidth, int blockHeight>
void Degrain3_sse2(BYTE *pDst, int nDstPitch, const BYTE *pSrc, int nSrcPitch,
						const BYTE *pRefB, int BPitch, const BYTE *pRefF, int FPitch,
						const BYTE *pRefB2, int B2Pitch, const BYTE *pRefF2, int F2Pitch,
						const BYTE *pRefB3, int B3Pitch, const BYTE *pRefF3, int F3Pitch,
						int WSrc, int WRefB, int WRefF, int WRefB2, int WRefF2, int WRefB3, int WRefF3)
{
	__m128i z = _mm_setzero_si128();
	__m128i o = _mm_set1_epi16(128);
	__m128i ws = _mm_set1_epi16(WSrc);
	__m128i wb1 = _mm_set1_epi16(WRefB);
	__m128i wf1 = _mm_set1_epi16(WRefF);
	__m128i wb2 = _mm_set1_epi16(WRefB2);
	__m128i wf2 = _mm_set1_epi16(WRefF2);
	__m128i wb3 = _mm_set1_epi16(WRefB3);
	__m128i wf3 = _mm_set1_epi16(WRefF3);

	for (int h=0; h<blockHeight; h++)
	{
		for (int x=0; x<blockWidth; x+=8)
		{
			 _mm_storel_epi64((__m128i*)(pDst + x), _mm_packus_epi16(_mm_srli_epi16(
				 _mm_add_epi16(_mm_mullo_epi16(_mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)(pSrc   + x)), z), ws),
				 _mm_add_epi16(_mm_mullo_epi16(_mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)(pRefB  + x)), z), wb1),
				 _mm_add_epi16(_mm_mullo_epi16(_mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)(pRefF  + x)), z), wf1),
				 _mm_add_epi16(_mm_mullo_epi16(_mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)(pRefB2 + x)), z), wb2),
				 _mm_add_epi16(_mm_mullo_epi16(_mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)(pRefF2 + x)), z), wf2),
				 _mm_add_epi16(_mm_mullo_epi16(_mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)(pRefB3 + x)), z), wb3),
				 _mm_add_epi16(_mm_mullo_epi16(_mm_unpacklo_epi8(_mm_loadl_epi64((__m128i*)(pRefF3 + x)), z), wf3),
				 o))))))), 8), z));
//			 pDst[x] = (pRefF[x]*WRefF + pSrc[x]*WSrc + pRefB[x]*WRefB + pRefF2[x]*WRefF2 + pRefB2[x]*WRefB2 + pRefF3[x]*WRefF3 + pRefB3[x]*WRefB3 + 128)>>8;
		}
		pDst += nDstPitch;
		pSrc += nSrcPitch;
		pRefB += BPitch;
		pRefF += FPitch;
		pRefB2 += B2Pitch;
		pRefF2 += F2Pitch;
		pRefB3 += B3Pitch;
		pRefF3 += F3Pitch;
	}
}

#endif

#include "magouille.h"

#define MAG_OPERATION(__step, __op__, __th) \
{ \
	for ( int j = (__step); j < 24 - (__step); j++ ) \
	{ \
		for ( int i = (__step); i < 24 - (__step); i++ ) \
		{ \
			const BYTE *sin = &in[i + j * in_pitch]; \
			BYTE min = sin[0]; \
			if ( sin[1] __op__ min ) min = sin[1]; \
			if ( sin[-1] __op__ min ) min = sin[-1]; \
			if ( sin[in_pitch] __op__ min ) min = sin[in_pitch]; \
			if ( sin[-in_pitch] __op__ min ) min = sin[-in_pitch]; \
			out[i + j * out_pitch] = min; \
		} \
	} \
}


void FilterC(BYTE *pDst, const BYTE *pSrc, int nDstPitch, int nSrcPitch, int nThreshold)
{
	BYTE buffer[2][24*24];
	const BYTE *in;
	BYTE *out;
	int in_pitch, out_pitch;

	/* erosion */
	in = pSrc - 4 - 4 * nSrcPitch, out = buffer[0];
	in_pitch = nSrcPitch, out_pitch = 24;
	
	MAG_OPERATION(1, <, -nThreshold);

	/* dilatation */
	in = buffer[0], out = buffer[1];
	in_pitch = 24;

	MAG_OPERATION(2, >, nThreshold);

	/* dilatation */
	in = buffer[1], out = buffer[0];

	MAG_OPERATION(3, >, nThreshold);

	/* erosion */
	in = buffer[0], out = pDst - 4 - 4 * nDstPitch;
	out_pitch = nDstPitch;

	MAG_OPERATION(4, <, -nThreshold);
}

void FilteriSSE(BYTE *pDst, const BYTE *pSrc, int nDstPitch, int nSrcPitch, int nThreshold)
{
	BYTE buffer[2][32*32];
	const BYTE *pin;
	BYTE *pout;
	int in_pitch, out_pitch;

#define DO_OP(__step, __op, __hop) \
	pout += __step * out_pitch; \
	pin += (__step - 1) * in_pitch; \
	for ( int j = __step; j < 24 - __step; j++ ) \
	{ \
	 	for ( int i = __hop; i < 24 - __hop; i += 8 ) \
		{ \
			const BYTE *p = pin + i; \
			BYTE *pp = pout + i; \
			__asm mov esi, [p] \
			__asm mov ecx, [in_pitch] \
			__asm mov edi, [pp] \
			__asm movq mm2, [esi + ecx - 1] \
			__asm movq mm3, [esi + ecx + 1] \
			__asm movq mm4, [esi + 2 * ecx] \
			__asm __op mm2, mm3 \
			__asm movq mm1, [esi + ecx] \
			__asm movq mm0, [esi] \
			__asm __op mm4, mm1 \
			__asm __op mm0, mm2 \
			__asm __op mm0, mm4 \
			__asm movq [edi], mm0 \
		} \
		pin += in_pitch; \
		pout += out_pitch; \
	}

	/* erosion */
	pin = pSrc - 4 - 4 * nSrcPitch, pout = buffer[0] + 4 + 4 * 32;
	in_pitch = nSrcPitch, out_pitch = 32;
	
	DO_OP(1, pminub, 0);

	/* dilatation */
	pin = buffer[0] + 4 + 4 * 32, pout = buffer[1] + 4 + 4 * 32;
	in_pitch = 32;

	DO_OP(2, pmaxub, 0);

	/* dilatation */
	pin = buffer[1] + 4 + 4 * 32, pout = buffer[0] + 4 + 4 * 32;

	DO_OP(3, pmaxub, 0);

	/* erosion */
	pin = buffer[0] + 4 + 4 * 32, pout = pDst - 4 - 4 * nDstPitch;
	out_pitch = nDstPitch;

	DO_OP(4, pminub, 4);
}

extern "C" void __cdecl magouille_isse(BYTE *pDst, const BYTE *pSrc, int nDstPitch,
									   int nSrcPitch, int nThreshold);
extern "C" void __cdecl magouille_sse2(BYTE *pDst, const BYTE *pSrc, int nDstPitch,
									   int nSrcPitch, int nThreshold);

Magouille::Magouille(PClip _child, int nMode, int nThreshold, bool mmx, bool isse, bool sse2, IScriptEnvironment* env) :
GenericVideoFilter(_child)
{
	if ( !vi.IsYV12() )
		env->ThrowError("Magouille : colorspace must be YV12");

	nWidth = vi.width;
	nHeight = vi.height;

	this->nThreshold = nThreshold;

	if ( sse2 )
		filter = magouille_sse2;
	else if ( isse )
		filter = magouille_isse;
	else 
		filter = FilterC;
}

PVideoFrame __stdcall Magouille::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame src = child->GetFrame(n, env);
	PVideoFrame dst = env->NewVideoFrame(vi);

	const BYTE *pSrc;
	BYTE *pDst;
	int nSrcPitch, nDstPitch, nWidth, nHeight;

	const int plane[3] = { PLANAR_Y, PLANAR_U, PLANAR_V };
	const int ratio[3] = { 0, 1, 1};

	for ( int i = 0; i < 3; i++ )
	{
		pSrc = src->GetReadPtr(plane[i]);
		pDst = dst->GetWritePtr(plane[i]);
		nSrcPitch = src->GetPitch(plane[i]);
		nDstPitch = dst->GetPitch(plane[i]);
		nWidth = ( this->nWidth >> ratio[i] ) & ~15;
		nHeight = ( this->nHeight >> ratio[i] ) & ~15;
		env->BitBlt(pDst, nDstPitch, pSrc, nSrcPitch, (this->nWidth >> ratio[i]), (this->nHeight >> ratio[i]));
		for ( int y = 16; y < nHeight - 16; y += 16 )
			for ( int x = 16; x < nWidth - 16; x += 16 )
				filter(pDst + x + y * nDstPitch, pSrc + x + y * nSrcPitch, nDstPitch, nSrcPitch, nThreshold);
	}

	__asm emms

	return dst;
}


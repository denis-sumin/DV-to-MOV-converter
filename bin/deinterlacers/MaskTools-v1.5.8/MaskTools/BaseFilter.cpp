#include "BaseFilter.h"
#include "GenericTools.h"
#include <string.h>

BaseFilter::BaseFilter(PClip _child, int _X, int _Y, int _w, int _h, int y,
					   int u, int v, bool _mmx, bool _isse, 
					   IScriptEnvironment *env, const char *name) : GenericVideoFilter(_child)
{
   try {
	Y = y; U = u; V = v;
	nOffX = _X; nOffY = _Y;
	nWidth = _w; nHeight = _h;
   mmx = _mmx; isse = _isse;

	// Check for frame coordinates
	if ( nOffX < 0 ) nOffX = 0;
	if (( nWidth < 0 ) || ( nOffX  + nWidth > vi.width )) nWidth = vi.width - nOffX;

	if ( nOffY < 0 ) nOffY = 0;
	if (( nHeight < 0 ) || ( nOffY  + nHeight > vi.height )) nHeight = vi.height - nOffY;

	// Adaptation to YV12 colorspace : all offset & width & height must be even
	nOffX &= 0xFFFFFFFE;
	nOffY &= 0xFFFFFFFE;
	nWidth &= 0xFFFFFFFE;
	nHeight &= 0xFFFFFFFE;

	// Check on Y, U and V
	if ( Y < -255 ) Y = -255;
	if ( U < -255 ) U = -255;
	if ( V < -255 ) V = -255;
	if ( Y > 3 ) Y = 3;
	if ( U > 3 ) U = 3;
	if ( V > 3 ) V = 3;

   strcpy(filterName, name);
   }
   catch (...) {
      throw;
   }
}

bool BaseFilter::CheckMMXOptimizations(IScriptEnvironment *env, int nModWidth, int nModHeight)
{
   return ((env->GetCPUFlags() && CPUF_MMX) && !(nWidth & nModWidth) && !(nHeight & nModHeight) && mmx);
}

bool BaseFilter::CheckiSSEOptimizations(IScriptEnvironment *env, int nModWidth, int nModHeight)
{
   return ((env->GetCPUFlags() && CPUF_INTEGER_SSE) && !(nWidth & nModWidth) && !(nHeight & nModHeight) && isse);
}

int BaseFilter::Saturate(int a, int b, int c)
{
   return (a < b) ? b : ((a > c) ? c : a);
}

void BaseFilter::CheckFrameSize(const VideoInfo &vi1, const VideoInfo &vi2, IScriptEnvironment *env)
{
   try {
   if (( vi1.width != vi2.width ) || ( vi1.height != vi2.height ))
      env->ThrowError("%s : Clip's size must be the same", filterName);
   }
   catch (...) {
      throw;
   }
}

void BaseFilter::CheckColorSpace(const VideoInfo &vip, IScriptEnvironment *env)
{
   try {
   if ( !vip.IsYV12() )
      env->ThrowError("%s : Input clip format must be YV12", filterName);
   }
   catch (...) {
      throw;
   }
}

BaseFilter::~BaseFilter()
{
}

void BaseFilter::ProcessFrame(PVideoFrame &dst, PVideoFrame src1, PVideoFrame src2, PVideoFrame src3, bool inPlace,
                              IScriptEnvironment *env)
{
   BYTE *pDst;
   const BYTE *pSrc1, *pSrc2, *pSrc3;
   int nDstPitch, nSrc1Pitch, nSrc2Pitch, nSrc3Pitch;

   env->MakeWritable(&dst);

   // y plane
   nDstPitch = dst->GetPitch(PLANAR_Y);
   pDst = dst->GetWritePtr(PLANAR_Y) + nOffX + nOffY * nDstPitch;
   if ( src1 )
   {
      nSrc1Pitch = src1->GetPitch(PLANAR_Y);
      pSrc1 = src1->GetReadPtr(PLANAR_Y) + nOffX + nOffY * nSrc1Pitch;
   }
   if ( src2 )
   {
      nSrc2Pitch = src2->GetPitch(PLANAR_Y);
      pSrc2 = src2->GetReadPtr(PLANAR_Y) + nOffX + nOffY * nSrc2Pitch;
   }
   if ( src3 )
   {
      nSrc3Pitch = src3->GetPitch(PLANAR_Y);
      pSrc3 = src3->GetReadPtr(PLANAR_Y) + nOffX + nOffY * nSrc3Pitch;
   }

   switch(Y)
   {
   case OPERATION : ProcessY(pDst, nDstPitch, pSrc1, nSrc1Pitch, pSrc2, nSrc2Pitch, pSrc3, nSrc3Pitch); break;
   case COPY_SOURCE : if (!inPlace) env->BitBlt(pDst, nDstPitch, pSrc1, nSrc1Pitch, nWidth, nHeight); break;
   case NO_OPERATION : break;
   default : MemZoneSet(pDst, -Y, nWidth, nHeight, 0, 0, nDstPitch);
   }

   // u plane
   nDstPitch = dst->GetPitch(PLANAR_U);
   pDst = dst->GetWritePtr(PLANAR_U) + (nOffX / 2) + (nOffY / 2) * nDstPitch;
   if ( src1 )
   {
      nSrc1Pitch = src1->GetPitch(PLANAR_U);
      pSrc1 = src1->GetReadPtr(PLANAR_U) + (nOffX / 2) + (nOffY / 2) * nSrc1Pitch;
   }
   if ( src2 )
   {
      nSrc2Pitch = src2->GetPitch(PLANAR_U);
      pSrc2 = src2->GetReadPtr(PLANAR_U) + (nOffX / 2) + (nOffY / 2) * nSrc2Pitch;
   }
   if ( src3 )
   {
      nSrc3Pitch = src3->GetPitch(PLANAR_U);
      pSrc3 = src3->GetReadPtr(PLANAR_U) + (nOffX / 2) + (nOffY / 2) * nSrc3Pitch;
   }

   switch(U)
   {
   case OPERATION : ProcessU(pDst, nDstPitch, pSrc1, nSrc1Pitch, pSrc2, nSrc2Pitch, pSrc3, nSrc3Pitch); break;
   case COPY_SOURCE : if (!inPlace) env->BitBlt(pDst, nDstPitch, pSrc1, nSrc1Pitch, nWidth / 2, nHeight / 2); break;
   case NO_OPERATION : break;
   default : MemZoneSet(pDst, -U, nWidth / 2, nHeight / 2, 0, 0, nDstPitch);
   }

   // v plane
   nDstPitch = dst->GetPitch(PLANAR_V);
   pDst = dst->GetWritePtr(PLANAR_V) + (nOffX / 2) + (nOffY / 2) * nDstPitch;
   if ( src1 )
   {
      nSrc1Pitch = src1->GetPitch(PLANAR_V);
      pSrc1 = src1->GetReadPtr(PLANAR_V) + (nOffX / 2) + (nOffY / 2) * nSrc1Pitch;
   }
   if ( src2 )
   {
      nSrc2Pitch = src2->GetPitch(PLANAR_V);
      pSrc2 = src2->GetReadPtr(PLANAR_V) + (nOffX / 2) + (nOffY / 2) * nSrc2Pitch;
   }
   if ( src3 )
   {
      nSrc3Pitch = src3->GetPitch(PLANAR_V);
      pSrc3 = src3->GetReadPtr(PLANAR_V) + (nOffX / 2) + (nOffY / 2) * nSrc3Pitch;
   }

   switch(V)
   {
   case OPERATION : ProcessV(pDst, nDstPitch, pSrc1, nSrc1Pitch, pSrc2, nSrc2Pitch, pSrc3, nSrc3Pitch); break;
   case COPY_SOURCE : if (!inPlace) env->BitBlt(pDst, nDstPitch, pSrc1, nSrc1Pitch, nWidth / 2, nHeight / 2); break;
   case NO_OPERATION : break;
   default : MemZoneSet(pDst, -V, nWidth / 2, nHeight / 2, 0, 0, nDstPitch);
   }
}


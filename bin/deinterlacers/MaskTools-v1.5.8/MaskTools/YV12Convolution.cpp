// General Convolution : allow to make a convolution in YV12 colorspace
// General Convolution (C) 2004 Manao (manao@melix.net)

// See legal notice in Interface.cpp and Readme.txt for more information

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA, or visit
// http://www.gnu.org/copyleft/gpl.html .

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "YV12Convolution.h"
#include "GC_Func.h"
#include "GenericTools.h"

bool YV12Convolution::AnalyseString(const char *string, double **vectf, int **vecti, int *halflength, IScriptEnvironment* env, bool *isFloat)
{
	int separatorCount = 0;
	int length = strlen(string);
	char separator = ' ';
	bool answer = false;
	int i, count;

	for ( i = 0; i < length; i++ )
		separatorCount += (string[i] == separator) ? 1 : 0;

	const char *sp = string;
	char *array;

	(*isFloat) |= (!(NULL == strchr(string,'.')));

	(*halflength) = (separatorCount / 2);
	if (separatorCount % 2) env->ThrowError("YV12Convolution: Vector's length have to be odd");
	(*vectf) = new double[(*halflength)*2+1];
	(*vecti) = new int[(*halflength)*2+1];

	for ( count = 0; count < separatorCount; count++ )
	{
		int end = strcspn(sp,&separator);
		array = new char[end+1];
		for ( int i = 0; i < end; i++ )
			array[i] = sp[i];
		array[end] = 0;
		sp = sp + end + strspn(sp+end,&separator);

		(*vectf)[count] = atof(array);
		(*vecti)[count] = atof(array);
		answer |= ( (*vectf)[count] < 0 );
		delete[] array;
	}

	int end = strlen(sp);
	array = new char[end+1];
	for ( i = 0; i < end; i++ )
		array[i] = sp[i];
	array[end] = 0;

	(*vectf)[separatorCount] = atof(array);
	(*vecti)[separatorCount] = atof(array);
	answer |= ( (*vectf)[separatorCount] < 0 );
	delete[] array;
	
	return answer;
}

YV12Convolution::YV12Convolution(PClip _child, const char * horizontal, const char *vertical,
					             int ttl, bool automatic, bool saturate,int _X, int _Y, int _w, int _h,
                                 int y, int u, int v, bool _mmx, bool _isse, IScriptEnvironment* env) :
BaseFilter(_child, _X, _Y, _w, _h, y, u, v, _mmx, _isse, env, "YV12LUTxy")
{
    CheckColorSpace(vi, env);

	totali = ttl;
	totalf = ttl;

	isFloat = false;

	AnalyseString(horizontal,&horvectf,&horvecti,&horres,env,&isFloat);
	AnalyseString(vertical,&vervectf,&vervecti,&verres,env,&isFloat);

	if (isFloat)
	{
		double other_total = 0;

		if (automatic) {
			other_total = 0;
			for (int x = 0; x < horres * 2 + 1; x++ )
				for (int y = 0; y < verres * 2 + 1; y++ )
					other_total += horvectf[x] * vervectf[y];
		}

		if ( saturate ) GCf = GC_MTFLTSAT_C;
		else GCf = GC_MTFLTABS_C;
		
		if ( automatic ) totalf = other_total;
		if ( fabs(totalf) < 0.001 ) totalf = 1.0;
	}
	else {
		int other_total = 0;

		if (automatic) {
			other_total = 0;
			for (int x = 0; x < horres * 2 + 1; x++ )
				for (int y = 0; y < verres * 2 + 1; y++ )
					other_total += horvecti[x] * vervecti[y];
		}

		if ( saturate ) GC = GC_MTINTSAT_C;
		else GC = GC_MTINTABS_C;

		if ( automatic ) totali = other_total;

		if ( totali == 0 ) totali = 1;
	}
}

YV12Convolution::~YV12Convolution()
{
	delete[] horvecti;
	delete[] vervecti;
	delete[] horvectf;
	delete[] vervectf;
}

void YV12Convolution::ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                               const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   if ( isFloat )
      GCf(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth, nHeight, horvectf, vervectf, horres, verres, totalf);
   else
      GC(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth, nHeight, horvecti, vervecti, horres, verres, totali);
}

void YV12Convolution::ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                               const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   if ( isFloat )
      GCf(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth / 2, nHeight / 2, horvectf, vervectf, horres, verres, totalf);
   else
      GC(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth / 2, nHeight / 2, horvecti, vervecti, horres, verres, totali);
}

void YV12Convolution::ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                               const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
   if ( isFloat )
      GCf(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth / 2, nHeight / 2, horvectf, vervectf, horres, verres, totalf);
   else
      GC(pDst, pSrc1, nDstPitch, nSrc1Pitch, nWidth / 2, nHeight / 2, horvecti, vervecti, horres, verres, totali);
}

PVideoFrame __stdcall YV12Convolution::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame	dst = env->NewVideoFrame(vi);
	PVideoFrame	src = child->GetFrame(n, env);	
   ProcessFrame(dst, src, NULL, NULL, false, env);
	return dst;
}

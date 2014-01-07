// EdgeMask - builds the edge map of a video plane based on selectable
// differential operator
// EdgeMask (C) 2003 Kurosu (kurosu@inforezo.org)

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
#include "GenericTools.h"
#include "DEdgeMask2.h"

int DEdgeMask2::ConvertCharToInt(const char c)
{
	switch(c)
	{
		case '1' : return 1;
		case '2' : return 2;
		case '3' : return 3;
		case '4' : return 4;
		case '5' : return 5;
		case '6' : return 6;
		case '7' : return 7;
		case '8' : return 8;
		case '9' : return 9;
		default : return 0;
	}
}

int DEdgeMask2::ConvertStringToInt(const char *string, int end)
{
	int answer = 0;
	int decade = 1;
	for ( int pointer = end-1; pointer >= 0; pointer--)
	{
		if (( string[pointer] == '-') && (pointer == 0))
			answer = -answer;
		else 
			answer += ConvertCharToInt(string[pointer]) * decade;
		decade *= 10;
	}
	return answer;
}

bool DEdgeMask2::AnalyseString(const char *string, int *vect, IScriptEnvironment* env)
{
	int length = strlen(string);
	char separator[12] = {'0','1','2','3','4','5','6','7','8','9','-',0};

   int pos;
	int count = 0;
	int prevpos = 0;
	bool answer = false;
	while ( (pos = strspn(string + prevpos,separator)) > 0)
	{
		prevpos = prevpos + pos + strcspn(string+prevpos+pos,separator);
		count++;
	}
	
	if (count != 9) env->ThrowError("DEdgeMask: matrix must have 9 values");
	
	prevpos = 0;
	count = 0;
	while ( (pos = strspn(string+prevpos,separator)) > 0)
	{
		vect[count] = ConvertStringToInt(string + prevpos, pos);
		answer |= ( vect[count] < 0 );
		prevpos = prevpos + pos + strcspn(string+prevpos+pos,separator);
		count++;
	}

	vect[count] = ConvertStringToInt(string + prevpos, length - prevpos);
	answer |= ( vect[count] < 0 );

	return answer;
}

DEdgeMask2::DEdgeMask2(PClip _child, PClip clip2, PClip clip3, const char * s, 
			              bool setDivisor, int div, int _X, int _Y, int _w, int _h,
                       int y, int u, int v, bool _mmx, bool _isse, IScriptEnvironment* env) :
BaseFilter(_child, _X, _Y, _w, _h, y, u, v, _mmx, _isse, env, "EdgeMask"), child2(clip2), child3(clip3)
{
   const VideoInfo& vi2 = child2->GetVideoInfo();
	const VideoInfo& vi3 = child3->GetVideoInfo();

   CheckFrameSize(vi, vi2, env);
   CheckFrameSize(vi, vi3, env);
   CheckColorSpace(vi, env);
   CheckColorSpace(vi2, env);
   CheckColorSpace(vi3, env);

   AnalyseString(s,array,env);

   BM_Fun = &DEdgeMask2::BuildMask;

	if ( setDivisor )
	{
		divisor = div;
	}
	else {
		divisor = 0;
		for ( int i = 0; i < 9; i++ )
		{
			divisor += (array[i] > 0) ? array[i] : 0;
		}
	}
}

void DEdgeMask2::BuildMask(unsigned char *dstp, const unsigned char *srcp,
                          const int dst_pitch, const int src_pitch,
                          const unsigned char *th_low, int low_pitch,
                          const unsigned char *th_high, int high_pitch,
                          int row_size, const int height)
{
	const unsigned char *su = srcp + 1;
	const unsigned char *s = srcp + src_pitch + 1;
	const unsigned char *sd = srcp + 2 * src_pitch + 1;
	unsigned char *d = dstp + dst_pitch;
	int srcmod = src_pitch + 2 - row_size;
	int sum;

	for ( int j = 1; j < height - 1; j++ )
	{
		for ( int i = 1; i < row_size - 1; i++ )
		{
			sum = MyAbs(array[0] * su[-1] + array[1] * su[0] + array[2] * su[1] +
						   array[3] * s[-1]  + array[4] * s[0]  + array[5] * s[1]  +
						   array[6] * sd[-1] + array[7] * sd[0] + array[8] * sd[1]) / divisor;

         d[i] = ( sum <= th_high[i] ) ? 0 : ( sum > th_low[i] ) ? 255 : sum;
			s++; su++; sd++;
		}
		d[0] = d[1];
		d[row_size-1] = d[row_size-2];
		d += dst_pitch;
		s += srcmod;
		su += srcmod;
		sd += srcmod;
      th_high += high_pitch;
      th_low += low_pitch;
	}
	
	for ( int x = 0; x < row_size; x++ )
	{
		dstp[x] = dstp[x+dst_pitch];
		d[x] = d[x-dst_pitch];
	}

}

void DEdgeMask2::ProcessY(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                         const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
    (this->*BM_Fun)(pDst, pSrc1, nDstPitch, nSrc1Pitch, pSrc2, nSrc2Pitch, pSrc3, nSrc3Pitch, nWidth, nHeight);
}

void DEdgeMask2::ProcessU(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                         const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
    (this->*BM_Fun)(pDst, pSrc1, nDstPitch, nSrc1Pitch, pSrc2, nSrc2Pitch, pSrc3, nSrc3Pitch, nWidth / 2, nHeight / 2);
}

void DEdgeMask2::ProcessV(BYTE *pDst, int nDstPitch, const BYTE *pSrc1, int nSrc1Pitch,
                         const BYTE *pSrc2, int nSrc2Pitch, const BYTE *pSrc3, int nSrc3Pitch)
{
    (this->*BM_Fun)(pDst, pSrc1, nDstPitch, nSrc1Pitch, pSrc2, nSrc2Pitch, pSrc3, nSrc3Pitch, nWidth / 2, nHeight / 2);
}

PVideoFrame __stdcall DEdgeMask2::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame	dst = env->NewVideoFrame(vi);
   ProcessFrame(dst, child->GetFrame(n, env), child2->GetFrame(n, env), child3->GetFrame(n, env), false, env);
	return dst;
}

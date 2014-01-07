// Motion Detection : create a mask from a basic motion detection
// Motion Detection (C) 2004 Manao (manao@melix.net)

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

#include "MD_Func.h"
#include "GenericTools.h"

bool SD_C (const unsigned char *srcp, const unsigned char *srcp_prev,
		   const int src_pitch, const int row_size, const int height, 
		   const unsigned char thressd)
{
	const unsigned char *s = srcp;
	const unsigned char *sp = srcp_prev;
	int srcmod = src_pitch - row_size;
	int src_prevmod = src_pitch - row_size;
	int x, y;

	int sum = 0;
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < row_size; x++)
		{
			sum += MyAbs((*sp) - (*s));
			sp++;
			s++;
		}
		sp += src_prevmod;
		s += srcmod;
	}
	return (( sum / (height * row_size)) > thressd);
}

bool SD_ISSE (const unsigned char *srcp, const unsigned char *srcp_prev,
			     const int src_pitch, const int row_size, const int height, 
			     const unsigned char thressd)
{
	int modulo = src_pitch - row_size + src_pitch;
	unsigned int sum;

	__asm {
		mov			esi, srcp
		mov			edi, srcp_prev
		mov			ebx, [height]
		shr			ebx, 1
		mov			ecx, src_pitch
		pxor		mm7, mm7
		pxor		mm6, mm6

		align		16

loopy :

		mov			eax, row_size
		shr			eax, 3

loopx :

		movq		mm0, [esi]
		 movq		 mm1, [edi]
	    movq		mm2, [esi+ecx]
		 movq		 mm3, [edi+ecx]

		psadbw		mm0, mm1
		 psadbw		 mm2, mm3

		paddd		mm7, mm0
		 paddd		 mm6, mm2

		add			esi, 8
		 add		 edi, 8

	    dec			eax
		jnz			loopx

		add			esi, [modulo]
		 add		 edi, [modulo]

	    dec			ebx
		jnz			loopy

		paddd		mm7, mm6
		movd		[sum], mm7

		emms
	}

	

	return (( sum / (height * row_size)) > thressd);


}

void MD_C(unsigned char *derp, const unsigned char *srcp, const unsigned char *srcp_prev,
		  const int der_pitch, const int src_pitch, const int src_prev_pitch, 
		  const int row_size, const int height, 
		  const unsigned char thresinf, const unsigned char thressup)
{
	unsigned int sum;
	int lastline, midline, newline;
	const unsigned char *sup, *sp, *sdp, *su, *s, *sd;
	int x, y;

	int srcmod = src_pitch - row_size + 2;
	int src_prevmod = src_prev_pitch - row_size + 2;
	int dermod = der_pitch - row_size + 1;

	sp = srcp_prev + 1;
	sdp = srcp_prev + src_prev_pitch + 1;
	sup = srcp_prev + 1;
	s = srcp + 1;
	sd = srcp + src_pitch + 1;
	su = srcp + 1;

	lastline = sd[-1] + s[-1] - ( sdp[-1] + sp[-1] );
	midline = sd[0] + s[0] - ( sdp[0] + sp[0] );

	sum = MyAbs(midline + lastline) / 4;
	*derp = ( sum < thresinf ) ? 0 : ( sum > thressup ) ? 255 : sum;
	derp++;

	for (x = 1; x < row_size - 1; x++)
	{
		s++; sd++; sp++; sdp++;
		newline = *sd + *s - ( *sdp + *sp );

		sum = MyAbs(midline+lastline+newline) / 6;
		*derp = ( sum < thresinf ) ? 0 : ( sum > thressup ) ? 255 : sum;

		lastline = midline;
		midline = newline;
		derp++;
	}

	sum = MyAbs(newline+midline);
	*derp = ( sum < thresinf ) ? 0 : ( sum > thressup ) ? 255 : sum;

	s += srcmod;
	sd += srcmod;
	sp += src_prevmod;
	sdp += src_prevmod;

	derp += dermod;

	for (y = 1; y < height-1; y++) 
	{

		lastline = su[-1] + s[-1] + sd[-1] -
				( sup[-1] + sp[-1] + sdp[-1] );
		midline = su[0] + s[0] + sd[0] -
				( sup[0] + sp[0] + sdp[0] );

		sum = MyAbs(lastline + midline) / 6;

		*derp = ( sum < thresinf ) ? 0 : ( sum > thressup ) ? 255 : sum;
		derp++;
		
		for (x = 1; x < row_size-1; x++)
		{
			s++; su++; sp++; sd++; sup++; sdp++;
			newline =  *su + *s + *sd - 
					( *sup + *sp + *sdp ) ;
			sum = MyAbs((int)lastline + (int)midline + (int)newline) / 9;
			lastline = midline;
			midline = newline;
			*derp = ( sum < thresinf ) ? 0 : ( sum > thressup ) ? 255 : sum;
			derp++;
		}
		sum = MyAbs(newline + midline) / 6;
		*derp = ( sum < thresinf ) ? 0 : ( sum > thressup ) ? 255 : sum;

		s += srcmod;
		su += srcmod;
		sd += srcmod;
		sp += src_prevmod;
		sup += src_prevmod;
		sdp += src_prevmod;
		derp += dermod;
	}

	lastline = su[-1] + s[-1] - ( sup[-1] + sp[-1] );
	midline = su[0] + s[0] - ( sup[0] + sp[0] );

	sum = MyAbs(midline+lastline) / 4;
	*derp = ( sum < thresinf ) ? 0 : ( sum > thressup ) ? 255 : sum;
	derp++;

	for (x = 1; x < row_size - 1; x++)
	{
		s++; su++; sp++; sup++;
		newline = *su + *s - ( *sup + *sp );

		sum = MyAbs(midline+lastline+newline) / 6;
		*derp = ( sum < thresinf ) ? 0 : ( sum > thressup ) ? 255 : sum;

		lastline = midline;
		midline = newline;
		derp++;
	}

	sum = MyAbs(newline+midline);
	*derp = ( sum < thresinf ) ? 0 : ( sum > thressup ) ? 255 : sum;

}

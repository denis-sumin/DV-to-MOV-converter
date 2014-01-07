// Comb Mask : create a mask from a combing detection
// Comb Mask (C) 2004 Manao (manao@melix.net)

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

#include "CM_Func.h"
#include "GenericTools.h"

void CM_C(const unsigned char *srcp, unsigned char *derp, const int src_pitch,
		  	const int der_pitch, const int row_size, const int height, 
			const unsigned char thresinf, const unsigned char thressup)
{
	const unsigned char *s = srcp + src_pitch;
	const unsigned char *su = srcp;
	const unsigned char *sd = srcp + 2 * src_pitch;
	unsigned char *d = derp;
	const int smod = src_pitch - row_size;
	const int dmod = der_pitch - row_size;
	int x, y;

	int prod;

	for (x = 0; x < row_size - 0; x++ )
	{
		*d = 0;
		d++;
	}

	d += dmod;

	for (y = 1; y < height - 1; y++ )
	{
		for (x = 0; x < row_size - 0; x++ )
		{
			prod = (((*(su) - (*s)) ) * ( (*(sd) - (*s)) ));

			if ( prod < thresinf ) *d = 0;
			else if ( prod > thressup ) *d = 255;
			else *d = (prod >> 8);
			s++;
			su++;
			sd++;
			d++;
		}
		d += dmod;
		s += smod;
		su += smod;
		sd += smod;
	}

	for (x = 0; x < row_size - 0; x++ )
	{
		*d = 0;
		d++;
	}

}

void CM_MMX(const unsigned char *srcp, unsigned char *derp, const int src_pitch,
		  	const int der_pitch, const int row_size, const int height, 
			const unsigned char thresinf, const unsigned char thressup)
{
	const unsigned char *s = srcp;
	unsigned char *d = derp;
	const int smod = src_pitch - row_size;
	const int dmod = der_pitch - row_size;

	int sizediv8 = row_size >> 3;
	int x;

	for (x = 0; x < row_size - 0; x++ )
	{
		*d = 0;
		d++;
	}

	d += dmod;

	__asm {
		mov			eax, s
		mov			ebx, d
		mov			edx, [height]
		dec			edx
		dec			edx
		mov			ecx, [src_pitch]
		mov			esi, [src_pitch]
		shl			esi, 1

		align 16

yloop :

		mov			edi, [sizediv8]

xloop : 

		movq		mm0, [eax+ecx]
		 movq		 mm1, [eax]
		movq		mm2, [eax+esi]
		 movq		 mm3, mm1
		movq		mm4, mm0
		 movq		 mm5, mm2

		psubusb		mm1, mm0
		 psubusb	 mm2, mm0
		 
		psubusb		mm0, mm3
		 psubusb	 mm4, mm5

		por			mm0, mm1
		 por		 mm2, mm4

		pavgb		mm0, mm2

		movq		[ebx], mm0

		add			ebx, 8
		add			eax, 8
		dec			edi
		jnz			xloop

		add			ebx, [dmod]
		add			eax, [smod]
		dec			edx
		jnz			yloop

		mov			[d], ebx

		emms
	}


	for (x = 0; x < row_size - 0; x++ )
	{
		*d = 0;
		d++;
	}

}

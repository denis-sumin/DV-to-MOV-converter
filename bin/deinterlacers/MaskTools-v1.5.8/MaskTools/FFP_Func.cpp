// FastFitPlane - fits a plane into another one of different dimensions
// using own resizers
// FastFitPlane (C) 2003 Kurosu (kurosu@inforezo.org)

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

/*
 *	Resizers assume the dimensions are the one of the input plane
 *	They also assume the working area has the dimensions of the bigger plane
 */

/* Will most probably cause problem on the last pixel(s) */
void FastReduceBy2_MMX(unsigned char *dstp, const unsigned char *srcp, unsigned char *workp,
					   const int dst_pitch, const int src_pitch, const int work_pitch,
					   const int width, const int height)
{
	static const __int64 Two = 0x0002000200020002i64;
	static const __int64 QOdd = 0xFF00FF00FF00FF00i64;
	static const __int64 QEven = 0x00FF00FF00FF00FFi64;

	__asm {
		/* UNROLL ME !!! */
		mov			eax,[workp]
		mov			ebx,[srcp]
		mov			ecx,ebx
		mov			esi,[height]
		add			ecx,[src_pitch]
		mov			edi,0
		mov			edx,ecx
		shr			esi,1
		add			edx,[src_pitch]
		movq		mm7,[Two]
		shl			[src_pitch],1
		pxor		mm6,mm6
		align		16
loopvert:
		movd		mm0,[ebx+edi]
		movd		mm1,[ecx+edi]
		punpcklbw	mm0,mm6
		movd		mm2,[edx+edi]

		punpcklbw	mm1,mm6
		paddusw		mm0,mm7
		punpcklbw	mm2,mm6
		psllw		mm1,1
		paddusw		mm0,mm2
		paddusw		mm0,mm1
		psrlw		mm0,2
		
		packuswb	mm0,mm5
		movd		[eax+edi],mm0

		add			edi,4

		cmp			edi,[width]
		jl			loopvert

		mov			edi,0

		add			eax,[work_pitch]
		add			ecx,[src_pitch]
		add			ebx,[src_pitch]
		add			edx,[src_pitch]

		dec			esi
		jnz			loopvert

		/* Here starts the horizontal resizer, if you want to use this code */
		/* UNROLL ME !!! */
		mov			eax,[workp]
		mov			ecx,[width]
		mov			ebx,[dstp]
		mov			edx,0
		mov			esi,[height]
		shr			ecx,1
		mov			edi,0
		shr			esi,1
		movq		mm6,[QOdd]
		movq		mm5,[QEven]
		align		16
loophoriz:
		movq		mm0,[eax+edi]
		movq		mm1,[eax+edi+1]
		pand		mm0,mm6
		movq		mm2,[eax+edi+2]
		
		pand		mm1,mm5
		psrlw		mm0,8
		pand		mm2,mm6
		psllw		mm1,1
		paddusb		mm0,mm7
		psrlw		mm2,8

		paddusw		mm2,mm1
		paddusw		mm2,mm0

		psrlw		mm2,2
		packuswb	mm2,mm7
		movd		[ebx+edx],mm2

		add			edi,8
		add			edx,4

		cmp			edx,ecx
		jl			loophoriz

		mov			edi,0
		mov			edx,0

		add			eax,[work_pitch]
		add			ebx,[dst_pitch]

		dec			esi
		jnz			loophoriz

		emms
	}
}

void FastReduceBy2_iSSE(unsigned char *dstp, const unsigned char *srcp, unsigned char *workp,
					   const int dst_pitch, const int src_pitch, const int work_pitch,
					   const int width, const int height)
{
	static const __int64 Two = 0x0002000200020002i64;
	static const __int64 QOdd = 0xFF00FF00FF00FF00i64;
	static const __int64 QEven = 0x00FF00FF00FF00FFi64;

	__asm {
		// UNROLL ME !!!
		mov			eax,[workp]
		mov			ebx,[srcp]
		mov			ecx,ebx
//		prefetch	[ebx+64]
		mov			esi,[height]
		add			ecx,[src_pitch]
		mov			edi,0
		mov			edx,ecx
		shr			esi,1
		//prefetch	[ecx+64]
		add			edx,[src_pitch]
		shl			[src_pitch],1
loopvy:
		//prefetch	[edx+64]
		align		16
loopvert:
		movq		mm0,[ebx+edi]
		movq		mm1,[ecx+edi]
		pavgb		mm0,[edx+edi]

		pavgb		mm0,mm1
		movntq		[eax+edi],mm0

		add			edi,8

		cmp			edi,[width]
		jl			loopvert

		mov			edi,0

		add			edx,[src_pitch]
		add			eax,[work_pitch]
		add			ecx,[src_pitch]
		add			ebx,[src_pitch]

		dec			esi
		jnz			loopvy

		/* Here starts the horizontal resizer, if you want to use this code */
		/* UNROLL ME !!! */
		mov			eax,[workp]
		mov			ecx,[width]
		mov			ebx,[dstp]
		mov			edx,0
		mov			esi,[height]
		shr			ecx,1
		mov			edi,0
		shr			esi,1
		movq		mm6,[QOdd]
		movq		mm5,[QEven]
		align		16
loophy:
		//prefetch	[eax+64]
		align		16
loophoriz:
		movq		mm0,[eax+edi]
		movq		mm1,[eax+edi+1]
		pand		mm0,mm6
		movq		mm2,[eax+edi+2]
		
		pand		mm1,mm5
		psrlw		mm0,8
		pand		mm2,mm6
		psllw		mm1,1
		paddusb		mm0,mm7
		psrlw		mm2,8

		paddusw		mm2,mm1
		paddusw		mm2,mm0

		psrlw		mm2,2
		packuswb	mm2,mm7
		movd		[ebx+edx],mm2

		add			edi,8
		add			edx,4

		cmp			edx,ecx
		jl			loophoriz

		mov			edi,0
		mov			edx,0

		add			eax,[work_pitch]
		add			ebx,[dst_pitch]

		dec			esi
		jnz			loophy

		emms
	}
}

void FastReduceBy2_C(unsigned char *dstp, const unsigned char *srcp, unsigned char *workp,
					 const int dst_pitch, const int src_pitch, const int work_pitch,
					 const int width, const int height)
{
	const unsigned char *srcpp = srcp;
	const unsigned char *srcpn = srcp + (src_pitch<<1);
	unsigned char		*workp_store = workp;

	srcp +=  src_pitch;
	int x2, x, y;

	/* Vertical reduce by 2*/
	for (y=0; y<(height>>1); y++)
	{
		/* Could be unrolled, but it's here for compatibility purpose... */
		for (x=0; x<width; x++)
			workp[x] = (srcpn[x] + (srcp[x]<<1) + srcpn[x] + 2)>>2;
		workp += work_pitch;
		srcpp += src_pitch<<1;
		srcp += src_pitch<<1;
		srcpn += src_pitch<<1;
	}

	/* Horizontal reduce by 2*/
	workp = workp_store;
	for (y=0; y<(height>>1); y++)
	{
		x2 = 0;
		for (x=0; x<(width>>1); x++)
		{
			dstp[x] = (workp[x2] + (workp[x2+1]<<1) + workp[x2+2] + 2)>>2;
			x2 += 2;
		}
		workp += work_pitch;
		dstp += dst_pitch;
	}
}

void FastIncreaseBy2_MMX(unsigned char *dstp, const unsigned char *srcp, unsigned char *workp,
					   const int dst_pitch, const int src_pitch, const int work_pitch,
					   const int width, const int height)
{
}

void FastIncreaseBy2_C(unsigned char *dstp, const unsigned char *srcp, unsigned char *workp,
					 const int dst_pitch, const int src_pitch, const int work_pitch,
					 const int width, const int height)
{
	int x2, x, y;
	unsigned char *workp_store = workp;

	/* Horizontal increase by 2*/
	for (y=0; y<height; y++)
	{
		x2 = 0;
		//Restore first column
		workp[x2] = (srcp[0] + (srcp[0]<<1) - srcp[1] + 1)>>1;
		for (x=0; x<width; x++)
		{
			//will not smooth image, rather sharpen it
			//to smooth, probably use (srcp[x-1]+4*srcp[x]+srcp[x+1]+3)/6
			workp[x2+1] = srcp[x];
			workp[x2+2] = (srcp[x] + srcp[x+1] + 1)>>2;
			x2 += 2;
		}
		//Restore last pixel
		workp[x2+1] = (srcp[width-1] + (srcp[width-1]<<1) - srcp[width-2] + 1)>>1;
		workp += work_pitch;
		srcp += src_pitch;
	}

	unsigned char *workpn, *dstpn;
	workp = workp_store;
	workpn = workp + work_pitch;

	/* Vertical increase by 2*/
		//Restore first row
	for (x=0; x<(width<<1); x++)
		dstp[x] = (workp[x] + (workp[x]<<1) - workpn[x] + 1)>>1;
	dstp += dst_pitch;
	dstpn = dstp + dst_pitch;

		//Loop
	for (y=0; y<height; y++)
	{
		for (x=0; x<(width<<1); x++)
		{
			dstp[x] = workp[x];
			dstpn[x] = (workp[x] + workpn[x] + 1)>>1;
		}
		dstp += dst_pitch<<1;
		workp += work_pitch;
		workpn += work_pitch;
	}
		//Restore last line
	for (x=0; x<(width<<1); x++)
		dstp[x] = (workpn[x] + (workpn[x]<<1) - workp[x] + 1)>>1;

}

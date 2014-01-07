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

#include <math.h>
#include <memory.h>
#include "GenericTools.h"

void Line_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			  const int src_pitch, const int rowsize, const int height,
			  const unsigned char th1, const unsigned char th2)
{
	//Threshold for test on words - needs unpacking prior to test
	static const __int64 thres1 =	((__int64)th1<<48) + ((__int64)th1<<32) +
									((__int64)th1<<16) + ((__int64)th1);
	static const __int64 thres2 =	((__int64)th2<<48) + ((__int64)th2<<32) +
									((__int64)th2<<16) + ((__int64)th2);
	static const __int64 Keep0088 = 0x00FF00FF00FF00FFi64;

	const int last = rowsize - 1;
    int width = rowsize - 2;
	memset(dstp,0x00,rowsize);
	__asm
	{
			mov			eax,[srcp]			//eax = srcpp
			mov			edx,[dstp]			//edx = dstpp
			mov			ebx,eax
			add			edx,[dst_pitch]		//edx = dstp
			mov			ecx,ebx				//ecx = srcp
			add			ebx,[src_pitch]		//ebx = srcp
			mov			esi,[height]		//y = height
			add			ecx,[src_pitch]		//ecx = srcpn
			mov			edi,0				//x = 0
			sub			esi,2				//y = height - 2
			movq		mm7,thres2			//mm7 = thres2
			movq		mm6,thres1			//mm6 = thres1
			pxor		mm5,mm5

loophere:
			movq		mm0,[ebx+edi]		//mm0 = srcp[x]
			inc			edi					//edi = x + 1
			movq		mm1,[eax+edi]		//mm1 = srcpp[x+1]
			movq		mm2,[ebx+edi]		//mm2 = srcp[x+1]
			movq		mm3,[ecx+edi]		//mm3 = srcpn[x+1]
			inc			edi					//edi = x + 2
			psubusb		mm1,mm2				//mm1 = srcpp[x+1] - srcp[x+1]
			psubusb		mm3,mm2				//mm2 = srcpn[x+1] - srcp[x+1]
			movq		mm4,[ebx+edi]		//mm4 = srcp[x+2]
			psubusb		mm0,mm2				//mm0 = srcp[x] - srcp[x+1]
			psubusb		mm4,mm2				//mm4 = srcp[x+2] - srcp[x+1]
			paddusb		mm1,mm3				//mm1 = srcpn[x+1] + srcpp[x+1] - 2*srcp[x+1]
			movq		mm2,mm4				//mm2 = srcp[x+2] - srcp[x+1]
			paddusb		mm4,mm0				//mm4 = srcp[x] + srcp[x+2] - 2*srcp[x+1]

			dec			edi
			//Threshold 4 lower bytes
			movq		mm3,mm4				//mm3 = srcp[x] + srcp[x+2] - 2*srcp[x+1]
			punpcklbw	mm3,mm5
			movq		mm2,mm3
			movq		mm1,mm3
			pcmpgtw		mm2,mm6
			pcmpgtw		mm1,mm7
			pand		mm3,mm2
			pand		mm1,Keep0088
			paddusb		mm3,mm1
			
			//Threshold 4 higher bytes
			punpckhbw	mm4,mm5
			movq		mm2,mm4
			movq		mm1,mm4
			pcmpgtw		mm2,mm6
			pcmpgtw		mm1,mm7
			pand		mm4,mm2
			pand		mm1,Keep0088
			paddusb		mm4,mm1
		
			//Gather results and put them back
			packuswb	mm3,mm4		
			movq		[edx+edi],mm3		//dstp[x+1] = edge value
			add			edi,7				//edit = x + 8
		
			cmp			edi,[width]		//if (x < rowsize - 1)
			jl			loophere				//	goto loophere
			
			//Correct 1st and last pixels' value
			/*
			mov			edi,last
			mov			[edx],0
			mov			[edx+edi],0
			*/
			mov			edi,0				//x = 0
			mov			eax,ebx				//srcpp = srcp
			mov			ebx,ecx				//srcp = srcpn
			add			ecx,[src_pitch]		//srcpn += src_pitch
			add			edx,[dst_pitch]		//dstp += dst_pitch
	
			dec			esi					//y--
			jnz			loophere				//if (y == 0) goto loophere

			//Copies value of edx (=dstp) to srcp to avoid one multiply :)
			mov			[dstp],edx
			emms							//end mmx, restore floating point
	}
	//memset(dstp,0x00,rowsize+1);
}

void Line_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			const int src_pitch, const int row_size, const int height,
			const unsigned char th1, const unsigned char th2)
{
	const unsigned char *srcpn, *srcpp;
	int x, y;
	int max, temp1, temp2;
	
	srcpp = srcp;
	srcp += src_pitch;
	srcpn = srcp + src_pitch;

	memset(dstp,0x00,row_size);
	dstp += dst_pitch;
	

	for (y = 1; y < height-1; y++) 
	{
		dstp[0] = 0;
		for (x = 1; x < row_size-1; x++)
		{
			max = srcp[x-1] - srcp[x];
			if (max > 4)
			{
				temp1 = srcp[x+1] - srcp[x];
				if (temp1 > 4) max += temp1;
				else max = 0;
			}
			else max = 0;
			
			temp1 = srcpp[x] - srcp[x];
			if (temp1 > 4)
			{
				temp2 = srcpn[x] - srcp[x];
				if (temp2 > 4) temp1 += temp2;
				else temp1 = 0;
			}
			else temp1 = 0;
			
			max += temp1;
			if (max>th1) dstp[x] = max;
			else dstp[x] = 0;
			if (max>th2) dstp[x] = 255;
		}
		dstp[row_size-1] = 0;
		dstp += dst_pitch;
		srcpp = srcp;
		srcp = srcpn;
		srcpn += src_pitch;
	}
	memset(dstp,0x00,row_size);
}

void Roberts_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				 const int src_pitch, const int rowsize, const int height,
				 const unsigned char th1, const unsigned char th2)
{
	//Threshold for test on words - needs unpacking prior to test
	static const __int64 thres1 =	((__int64)th1<<48) + ((__int64)th1<<32) +
									((__int64)th1<<16) + ((__int64)th1);
	static const __int64 thres2 =	((__int64)th2<<48) + ((__int64)th2<<32) +
									((__int64)th2<<16) + ((__int64)th2);
	static const __int64 Keep34 = 0x7F7F7F7F7F7F7F7Fi64;
	static const __int64 Keep0088 = 0x00FF00FF00FF00FFi64;

	const int last = rowsize - 1;
    int width = rowsize - 1;
	memset(dstp,0x00,rowsize);
	__asm
	{
			mov			eax,[srcp]
			mov			edx,[dstp]
			mov			ebx,eax
			mov			esi,[height]
			add			ebx,[src_pitch]
			mov			edi,0
			sub			esi,1
			movq		mm7,thres2
			movq		mm6,thres1
			movq		mm5,Keep34
			pxor		mm4,mm4
loophere:
			movq		mm0,[eax+edi]
			movq		mm1,[ebx+edi]
			movq		mm2,[eax+edi+1]
			psrlq		mm1,1
			psrlq		mm2,1
			pand		mm1,mm5
			pand		mm2,mm5
			movq		mm3,mm0
			paddusb		mm1,mm2
			
			psubusb		mm0,mm1
			psubusb		mm1,mm3
			por			mm0,mm1
			
			//Threshold 4 lower bytes
			movq		mm1,mm0				//mm3 = srcp[x] + srcp[x+2] - 2*srcp[x+1]
			punpcklbw	mm1,mm4
			movq		mm2,mm1
			movq		mm3,mm1
			pcmpgtw		mm2,mm6
			pcmpgtw		mm3,mm7
			pand		mm1,mm2
			pand		mm3,Keep0088
			paddusb		mm1,mm3
			
			//Threshold 4 higher bytes
			punpckhbw	mm0,mm4
			movq		mm2,mm0
			movq		mm3,mm0
			pcmpgtw		mm2,mm6
			pcmpgtw		mm3,mm7
			pand		mm0,mm2
			pand		mm3,Keep0088
			paddusb		mm0,mm3
			
			//Gather results and put them back
			packuswb	mm1,mm0		
			movq		[edx+edi],mm1
			
			add			edi,8
			cmp			edi,[width]
			jl			loophere
			
			//Correct last pixel' value
			mov			edi,last
			mov			[edx+edi],0

			mov			edi,0
			mov			eax,ebx
			add			ebx,[src_pitch]
			add			edx,[dst_pitch]
			
			dec			esi
			jnz			loophere
			
			//Copies value of edx (=dstp) to srcp to avoid one multiply :)
			mov			[dstp],edx
			emms
	}
	memset(dstp,0x00,rowsize+1);
}

void Roberts_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			   const int src_pitch, const int rowsize, const int height,
			   const unsigned char th1, const unsigned char th2)
{
	const unsigned char *srcpn;
	unsigned char temp;
	
	srcpn = srcp + src_pitch;
	
	for (int y = 0; y < height-1; y++) 
	{
		for (int x = 0; x < rowsize-1; x++)
		{
			temp = abs( ((int)srcp[x]<<1) - srcp[x+1] - srcpn[x] )>>1;
			dstp[x] = ( temp <= th1 ) ? 0 : ( temp > th2 ) ? 255 : temp;
		}
		dstp[rowsize-1] = dstp[rowsize-2];
		dstp += dst_pitch;
		srcp = srcpn;
		srcpn += src_pitch;
	}
	for (int x = 0; x < rowsize; x++)
		dstp[x] = dstp[x-dst_pitch];
}

//void Sobel_iSSE(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
//			   const int src_pitch, int rowsize, const int height,
//			   const unsigned char th1, const unsigned char th2)
//{
//	static const __int64 thres1 =	((__int64)th1<<48) + ((__int64)th1<<32) +
//		((__int64)th1<<16) + ((__int64)th1);
//	static const __int64 thres2 =	((__int64)th2<<48) + ((__int64)th2<<32) +
//		((__int64)th2<<16) + ((__int64)th2);
//	static const __int64 Keep34 = 0x7F7F7F7F7F7F7F7Fi64;
//	static const __int64 Keep0088 = 0x00FF00FF00FF00FFi64;
//
//	const int last = rowsize - 1;
//	memset(dstp,0x00,rowsize);
//	rowsize -= 2;
//	__asm
//	{
//		mov			eax,[srcp]
//		mov			edx,[dstp]
//		add			eax,[src_pitch]
//		mov			ebx,eax
//		add			edx,[dst_pitch]
//		add			ebx,[src_pitch]
//		mov			esi,[height]
//		mov			ecx,ebx		
//		mov			edi,0
//		add			ecx,[src_pitch]
//		sub			esi,2
//		movq		mm7,thres2
//		movq		mm6,thres1
//		movq		mm5,Keep34
//		pxor		mm4,mm4
//		prefetchnta	[eax+64]
//loophy:
//		prefetchnta	[ecx+64]
//loophere:
//		movq		mm2,[ebx+edi]
//		movq		mm1,[eax+edi]
//		pavgusb		mm2,[ebx+edi+1]
//		pavgusb		mm1,[ecx+edi]
//		
//		movq		mm0,mm2
//		psubusb		mm2,mm1
//		psubusb		mm1,mm0
//		por			mm1,mm2
//
//			//Threshold 4 lower bytes
//		movq		mm0,mm1				//mm3 = srcp[x] + srcp[x+2] - 2*srcp[x+1]
//		punpcklbw	mm1,mm4
//		movq		mm2,mm1
//		movq		mm3,mm1
//		pcmpgtw		mm2,mm6
//		pcmpgtw		mm3,mm7
//		pand		mm1,mm2
//		pand		mm3,Keep0088
//		paddusb		mm1,mm3
//		
//		//Threshold 4 higher bytes
//		punpckhbw	mm0,mm4
//		movq		mm2,mm0
//		movq		mm3,mm0
//		pcmpgtw		mm2,mm6
//		pcmpgtw		mm3,mm7
//		pand		mm0,mm2
//		pand		mm3,Keep0088
//		paddusb		mm0,mm3
//		
//		//Gather results and put them back
//		packuswb	mm1,mm0		
//		movntq		[edx+edi],mm1
//		
//		add			edi,7
//		cmp			edi,[rowsize]
//		jl			loophere
//		
//		//Correct 1st and last pixels' value
//		mov			edi,last
//		mov			byte ptr[edx],0
//		mov			byte ptr[edx+edi],0
//		mov			eax,ebx
//		mov			edi,0
//		mov			ebx,ecx
//		add			ecx,[src_pitch]
//		add			edx,[dst_pitch]
//		
//		dec			esi
//		jnz			loophy
//		
//		//Copies value of edx (=dstp) to srcp to avoid one multiply :)
//		mov			[dstp],edx
//		emms
//	}
//	memset(dstp,0x00,rowsize+2);
//}
//
void Sobel_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			   const int src_pitch, const int rowsize, const int height,
			   const unsigned char th1, const unsigned char th2)
{
	static const __int64 thres1 =	((__int64)th1<<48) + ((__int64)th1<<32) +
		((__int64)th1<<16) + ((__int64)th1);
	static const __int64 thres2 =	((__int64)th2<<48) + ((__int64)th2<<32) +
		((__int64)th2<<16) + ((__int64)th2);
	static const __int64 Keep34 = 0x7F7F7F7F7F7F7F7Fi64;
	static const __int64 Keep0088 = 0x00FF00FF00FF00FFi64;

	const int last = rowsize - 1;
    int width = rowsize - 2;
	memset(dstp,0x00,rowsize);
	__asm
	{
			mov			eax,[srcp]
			mov			edx,[dstp]
			add			eax,[src_pitch]
			mov			ebx,eax
			add			edx,[dst_pitch]
			add			ebx,[src_pitch]
			mov			esi,[height]
			mov			ecx,ebx		
			mov			edi,0
			add			ecx,[src_pitch]
			sub			esi,2
			movq		mm7,thres2
			movq		mm6,thres1
			movq		mm5,Keep34
			pxor		mm4,mm4
loophere:
			movq		mm0,[ebx+edi]
			inc			edi
			movq		mm1,[eax+edi]
			movq		mm2,[ecx+edi]
			inc			edi
			movq		mm3,[ebx+edi]
			
			psrlq		mm0,1
			psrlq		mm1,1
			pand		mm0,mm5
			psrlq		mm2,1
			pand		mm1,mm5
			psrlq		mm3,1
			pand		mm2,mm5
			pand		mm3,mm5
			
			paddusb		mm1,mm0
			paddusb		mm2,mm3
			movq		mm0,mm1
			psubusb		mm1,mm2
			psubusb		mm2,mm0
			por			mm1,mm2

			dec			edi
			
			//Threshold 4 lower bytes
			movq		mm0,mm1				//mm3 = srcp[x] + srcp[x+2] - 2*srcp[x+1]
			punpcklbw	mm1,mm4
			movq		mm2,mm1
			movq		mm3,mm1
			pcmpgtw		mm2,mm6
			pcmpgtw		mm3,mm7
			pand		mm1,mm2
			pand		mm3,Keep0088
			paddusb		mm1,mm3
			
			//Threshold 4 higher bytes
			punpckhbw	mm0,mm4
			movq		mm2,mm0
			movq		mm3,mm0
			pcmpgtw		mm2,mm6
			pcmpgtw		mm3,mm7
			pand		mm0,mm2
			pand		mm3,Keep0088
			paddusb		mm0,mm3
			
			//Gather results and put them back
			packuswb	mm1,mm0		
			movq		[edx+edi],mm1
			
			add			edi,7
			cmp			edi,[width]
			jl			loophere
			
			//Correct 1st and last pixels' value
			mov			edi,last
			mov			[edx],0
			mov			[edx+edi],0

			mov			eax,ebx
			mov			edi,0
			mov			ebx,ecx
			add			ecx,[src_pitch]
			add			edx,[dst_pitch]
			
			dec			esi
			jnz			loophere
			
			//Copies value of edx (=dstp) to srcp to avoid one multiply :)
			mov			[dstp],edx
			emms
	}
	memset(dstp,0x00,rowsize+2);
}

void Sobel_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			 const int src_pitch, const int rowsize, const int height,
			 const unsigned char th1, const unsigned char th2)
{
	const unsigned char *srcpn, *srcpp;
	
	srcpp = srcp;
	srcp += src_pitch;
	srcpn = srcp + src_pitch;
	dstp += dst_pitch;
	unsigned char  temp;
	
	for (int y = 1; y < height-1; y++) 
	{
		
		for (int x = 1; x < rowsize-1; x++)
		{
			temp = abs( (int)srcp[x+1] + srcpn[x] - srcp[x-1] - srcpp[x] )>>1;
			dstp[x] = ( temp <= th1 ) ? 0 : ( temp > th2 ) ? 255 : temp;
		}
		dstp[rowsize-1] = dstp[rowsize-2];
		dstp[0] = dstp[1];
		dstp += dst_pitch;
		srcpp = srcp;
		srcp = srcpn;
		srcpn += src_pitch;
	}
	unsigned char * dstp1 = dstp - dst_pitch * (height-1);
	for ( int i = 0; i < rowsize; i++)
	{
		dstp1[i] = dstp1[i+dst_pitch];
		dstp[i] = dstp[i-dst_pitch];
	}
}

void Special_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			const int src_pitch, const int rowsize, const int height,
			const unsigned char th1, const unsigned char th2)
{
	static const __int64 thres1 =	((__int64)th1<<48) + ((__int64)th1<<32) +
									((__int64)th1<<16) + ((__int64)th1);
	static const __int64 thres2 =	((__int64)th2<<48) + ((__int64)th2<<32) +
									((__int64)th2<<16) + ((__int64)th2);
	static const __int64 Keep24 =	0x3F3F3F3F3F3F3F3Fi64;
	static const __int64 Keep0088 = 0x00FF00FF00FF00FFi64;

	memset(dstp,0x00,rowsize);
    int width = rowsize - 2;
	const int last = rowsize-1;
	__asm
	{
			mov			eax,[srcp]
			mov			edx,[dstp]
			mov			ebx,eax
			add			edx,[dst_pitch]
			add			ebx,[src_pitch]
			mov			esi,[height]
			mov			ecx,ebx
			mov			edi,0
			add			ecx,[src_pitch]
			sub			esi,2
			movq		mm7,thres2
			movq		mm6,thres1
			movq		mm5,Keep24
loophere:
			movq		mm1,[eax+edi]
			movq		mm2,[ecx+edi]
			inc			edi
			movq		mm0,[ebx+edi]
			inc			edi
			movq		mm3,[eax+edi]
			movq		mm4,[ecx+edi]
			
			psrlq		mm1,2
			psrlq		mm2,2
			psrlq		mm3,2
			psrlq		mm4,2
			pand		mm1,mm5
			pand		mm2,mm5
			pand		mm3,mm5
			pand		mm4,mm5
			paddusb		mm1,mm2
			paddusb		mm3,mm4
			movq		mm2,mm0
			paddusb		mm3,mm1
			dec			edi
			
			psubusb		mm0,mm3
			psubusb		mm3,mm2
			por			mm0,mm3
			pxor		mm4,mm4
			
			//Threshold 4 lower bytes
			movq		mm1,mm0				//mm3 = srcp[x] + srcp[x+2] - 2*srcp[x+1]
			punpcklbw	mm1,mm4
			movq		mm2,mm1
			movq		mm3,mm1
			pcmpgtw		mm2,mm6
			pcmpgtw		mm3,mm7
			pand		mm1,mm2
			pand		mm3,Keep0088
			paddusb		mm1,mm3
			
			//Threshold 4 higher bytes
			punpckhbw	mm0,mm4
			movq		mm2,mm0
			movq		mm3,mm0
			pcmpgtw		mm2,mm6
			pcmpgtw		mm3,mm7
			pand		mm0,mm2
			pand		mm3,Keep0088
			paddusb		mm0,mm3
			
			//Gather results and put them back
			packuswb	mm1,mm0		
			movq		[edx+edi],mm1
			
			add			edi,7
			cmp			edi,[width]
			jl			loophere
			
			//Correct 1st and last pixels' value
			mov			edi,last
			mov			[edx],0
			mov			[edx+edi],0

			mov			eax,ebx
			mov			edi,0
			mov			ebx,ecx
			add			edx,[dst_pitch]
			add			ecx,[src_pitch]
			
			dec			esi
			jnz			loophere

			//Copies value of edx (=dstp) to srcp to avoid one multiply :)
			mov			[dstp],edx
			emms
	}
    memset(dstp,0x00,rowsize+1);
}

void Special_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				 const int src_pitch, const int rowsize, const int height,
				 const unsigned char th1, const unsigned char th2)
{
	const unsigned char *srcpn, *srcpp;
	int temp;
	
	srcpp = srcp;
	srcp += src_pitch;
	dstp += dst_pitch;
	srcpn = srcp + src_pitch;
	
	for (int y = 1; y < height-1; y++) 
	{
		
		for (int x = 1; x < rowsize-1; x++)
		{
			temp = abs( ((int)srcp[x]<<2) - srcpp[x-1] - srcpn[x+1] - srcpn[x-1] - srcpp[x+1])>>2;
			dstp[x] = ( temp <= th1 ) ? 0 : ( temp > th2 ) ? 255 : temp;
		}
		dstp[0] = dstp[1];
		dstp[rowsize-1] = dstp[rowsize-2];
		dstp += dst_pitch;
		srcpp = srcp;
		srcp = srcpn;
		srcpn += src_pitch;
	}
	unsigned char * dstp1 = dstp - dst_pitch * (height-1);
	for ( int i = 0; i < rowsize; i++)
	{
		dstp1[i] = dstp1[i+dst_pitch];
		dstp[i] = dstp[i-dst_pitch];
	}

}

//Cartoon is just a Roberts modification to only accept darker pixels, ie negative edges
void Cartoon_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				 const int src_pitch, const int rowsize, const int height,
				 const unsigned char th1, const unsigned char th2)
{
	//Threshold for test on words - needs unpacking prior to test
	static const __int64 thres1 =	((__int64)th1<<48) + ((__int64)th1<<32) +
									((__int64)th1<<16) + ((__int64)th1);
	static const __int64 thres2 =	((__int64)th2<<48) + ((__int64)th2<<32) +
									((__int64)th2<<16) + ((__int64)th2);
	static const __int64 Keep34 = 0x7F7F7F7F7F7F7F7Fi64;
	static const __int64 Keep0088 = 0x00FF00FF00FF00FFi64;

	memset(dstp,0x00,rowsize);
    int width = rowsize - 1;
	const int last = rowsize - 1;
	__asm
	{
			mov			eax,[srcp]
			mov			edx,[dstp]
			//add			eax,src_pitch
			mov			ebx,eax
			add			edx,dst_pitch
			add			ebx,src_pitch
			mov			esi,[height]
			mov			edi,0
			sub			esi,1
			movq		mm7,thres2
			movq		mm6,thres1
			movq		mm5,Keep34
			pxor		mm4,mm4
loophere:
			movq		mm0,[eax+edi]
			movq		mm1,[ebx+edi]
			inc			edi
			movq		mm2,[eax+edi]
			psrlq		mm1,1
			psrlq		mm2,1
			pand		mm1,mm5
			pand		mm2,mm5
			dec			edi
			paddusb		mm1,mm2
			
			psubusb		mm1,mm0
			psllq		mm1,1
			
			//Threshold 4 lower bytes
			movq		mm0,mm1
			punpcklbw	mm1,mm4
			movq		mm2,mm1
			movq		mm3,mm1
			pcmpgtw		mm2,mm6
			pcmpgtw		mm3,mm7
			pand		mm1,mm2
			pand		mm3,Keep0088
			paddusb		mm1,mm3
			
			//Threshold 4 higher bytes
			punpckhbw	mm0,mm4
			movq		mm2,mm0
			movq		mm3,mm0
			pcmpgtw		mm2,mm6
			pcmpgtw		mm3,mm7
			pand		mm0,mm2
			pand		mm3,Keep0088
			paddusb		mm0,mm3
			
			//Gather results and put them back
			packuswb	mm1,mm0		
			movq		[edx+edi],mm1
			
			add			edi,8
			cmp			edi,[width]
			jl			loophere
			
			//Correct last pixel' value
			mov			edi,last
			mov			[edx+edi],0

			mov			edi,0
			mov			eax,ebx
			add			ebx,[src_pitch]
			add			edx,[dst_pitch]
			
			dec			esi
			jnz			loophere
			
			//Copies value of edx (=dstp) to srcp to avoid one multiply :)
			mov			[dstp],edx
			emms
	}
	memset(dstp,0x00,rowsize);
}

void Cartoon_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			   const int src_pitch, const int row_size, const int height,
			   const unsigned char th1, const unsigned char th2)
{
	const unsigned char *srcpn;
	int temp;
	
	srcp += src_pitch;
	srcpn = srcp + src_pitch;
	
	for (int y = 0; y < height-1; y++) 
	{
		for (int x = 0; x < row_size-1; x++)
		{
			temp = ((int)srcp[x]<<1) - srcp[x+1] - srcpn[x];
			dstp[x] = ( temp <= th1 ) ? 0 : ( temp > th2 ) ? 255 : temp;
		}
		dstp[row_size-1] = dstp[row_size-2];
		dstp += dst_pitch;
		srcp = srcpn;
		srcpn += src_pitch;
	}
	for (int x = 0; x < row_size; x++)
	dstp[x] = dstp[x-dst_pitch];
}

void Laplace_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			   const int src_pitch, const int row_size, const int height,
			   const unsigned char th1, const unsigned char th2)
{
	const unsigned char *su = srcp + 1;
	const unsigned char *s = srcp + src_pitch + 1;
	const unsigned char *sd = srcp + 2 * src_pitch + 1;
	unsigned char *d = dstp + dst_pitch;

	int lastline;
	int midline;
	int newline;
	int dermod = dst_pitch;
	int srcmod = src_pitch + 2 - row_size;
	int sum;

	for ( int y = 1; y < height - 1; y++) 
	{
		lastline = - ( su[-1] + sd[-1] + s[-1] );
		midline = - ( su[0] + sd[0] + s[0] );

		for ( int x = 1; x < row_size - 1; x++)
		{
			s++; su++; sd++; 
			newline = - ( *su + *sd + *s );

			sum = MyAbs((int)lastline + (int)midline + (int)newline + 9 * s[-1]) / 8;
			lastline = midline;
			midline = newline;

			d[x] = ( sum <= th1 ) ? 0 : ( sum > th2 ) ? 255 : sum;

		}

		d[0] = d[1];
		d[row_size-1] = d[row_size-2];

		s += srcmod;
		su += srcmod;
		sd += srcmod;
		d += dermod;
	}

	for ( int x = 0; x < row_size; x++ )
	{
		dstp[x] = dstp[x+dst_pitch];
		d[x] = d[x-dst_pitch];
	}

}
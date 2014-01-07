// YV12Layer - port of AviSynth's Layer functions to YV12
// YV12Layer (C) 2003 Kurosu (kurosu@inforezo.org)
// Layer (C) Poptones

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
static const unsigned char LookUpTable[256*3] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
	 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
	 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
	 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
	 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
	 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
	 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
	128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
	144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
	160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
	176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
	192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
	208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
	224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};
static const unsigned char *LookUp = LookUpTable + 256;
*/

void LAdd_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			  const int src_pitch, const int row_size, const int height,
			  unsigned char level)
{
	short int temp = 0;
	unsigned char s1, s2;
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<row_size; x++)
		{
			s1 = dstp[x];
			s2 = srcp[x];
			temp = (level * (s2-s1)) & 0x0000FFFF;
			dstp[x] = s1 + (temp>>8);
		}
		dstp += dst_pitch;
		srcp += src_pitch;
	}
}

void LAdd_NoCr_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			  const int src_pitch, const int row_size, const int height,
			  unsigned char level)
{
	short int temp = 0;
	unsigned char s1;
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<row_size; x++)
		{
			s1 = dstp[x];
			temp = (level * (128-s1)) & 0x0000FFFF;
			dstp[x] = s1 + (temp>>8);
		}
		dstp += dst_pitch;
		srcp += src_pitch;
	}
}

void LAdd_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				const int src_pitch, const int row_size, const int height,
				unsigned char level)
{
	static const __int64 Level = ((__int64)level<<48) + ((__int64)level<<32) +
								 ((__int64)level<<16) + level;
	__asm {
			mov			eax,[dstp]
			mov			ecx,[srcp]
			mov			ebx,eax
			mov			edx,ecx
			add			ebx,[dst_pitch]
			add			edx,[src_pitch]
			mov			esi,[height]
			shl			[src_pitch],1
			shl			[dst_pitch],1
			mov			edi,0
			shr			esi,1
			movq		mm7,[Level]
			pxor		mm6,mm6
loopx:
			movd		mm0,[eax+edi]
			movd		mm2,[ebx+edi]
			punpcklbw	mm0,mm6
			movd		mm1,[ecx+edi]
			punpcklbw	mm1,mm6
			movd		mm3,[edx+edi]

			psubsw		mm1,mm0
			pmullw		mm1,mm7
			punpcklbw	mm2,mm6
			punpcklbw	mm3,mm6
			psrlw		mm1,8
			psubsw		mm3,mm2
			paddb		mm1,mm0
			pmullw		mm3,mm7

			packuswb	mm1,mm6
			movd		[eax+edi],mm1
			psrlw		mm3,8
			paddb		mm3,mm2
			packuswb	mm3,mm6
			movd		[ebx+edi],mm3

			add			edi,4
			cmp			edi,[row_size]
			jl			loopx
			
			mov			edi,0
			add			eax,dst_pitch
			add			ecx,src_pitch
			add			ebx,dst_pitch
			add			edx,src_pitch
			dec			esi
			
			jnz			loopx
			
			emms
	}
}

void LAdd_NoCr_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				const int src_pitch, const int row_size, const int height,
				unsigned char level)
{
	static const __int64 Level = ((__int64)level<<48) + ((__int64)level<<32) +
								 ((__int64)level<<16) + level;
	static const __int64 Chroma = 0x0080008000800080i64;
	__asm {
			mov			eax,[dstp]
			mov			ebx,eax
			mov			esi,[height]
			add			ebx,[dst_pitch]
			mov			edi,0
			shl			[dst_pitch],1
			movq		mm7,[Level]
			shr			esi,1
			pxor		mm6,mm6
			movq		mm5,[Chroma]
loopx:
			movq		mm0,[eax+edi]
			movq		mm4,mm5
			punpcklbw	mm0,mm6
			movd		mm1,[ebx+edi]
			psubsw		mm4,mm0
			movq		mm3,mm5

			pmullw		mm4,mm7
			punpcklbw	mm1,mm6
			psubsw		mm3,mm1
			psrlw		mm4,8
			pmullw		mm3,mm7
			paddb		mm4,mm0

			packuswb	mm4,mm6
			psrlw		mm3,8

			movd		[eax+edi],mm4
			paddb		mm3,mm2
			packuswb	mm3,mm6
			movd		[ebx+edi],mm3

			add			edi,4
			cmp			edi,[row_size]
			jl			loopx
			
			mov			edi,0
			add			eax,dst_pitch
			add			ecx,src_pitch
			add			ebx,dst_pitch
			add			edx,src_pitch
			dec			esi
			
			jnz			loopx
			
			emms
	}
}

void LSub_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			  const int src_pitch, const int row_size, const int height,
			  unsigned char level)
{
	short int temp = 0;
	unsigned char s1, s2;
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<row_size; x++)
		{
			s1 = dstp[x];
			s2 = 255 - srcp[x];
			temp = (((short int)s2 - s1) * level) & 0x0000FFFF;
			dstp[x] = s1 + (temp>>8);
		}
		dstp += dst_pitch;
		srcp += src_pitch;
	}
}

void LSub_NoCr_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			  const int src_pitch, const int row_size, const int height,
			  unsigned char level)
{
	short int temp = 0;
	unsigned char s1;
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<row_size; x++)
		{
			s1 = dstp[x];
			temp = (((short int)128 - s1) * level) & 0x0000FFFF;
			dstp[x] = s1 + (temp>>8);
		}
		dstp += dst_pitch;
		srcp += src_pitch;
	}
}

void LSub_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				const int src_pitch, const int row_size, const int height,
				unsigned char level)
{
	static const __int64 Level = ((__int64)level<<48) + ((__int64)level<<32) +
								 ((__int64)level<<16) + level;
	static const __int64 Invert = 0x00FF00FF00FF00FFi64;
	__asm {
			mov			eax,[dstp]
			mov			ecx,[srcp]
			mov			esi,[height]
			mov			edi,0
			movq		mm7,[Level]
			pxor		mm6,mm6
			movq		mm5,[Invert]
loopx:
			movd		mm1,[ecx+edi]
			movd		mm0,[eax+edi]
			
			punpcklbw	mm1,mm6
			movq		mm4,mm5
			punpcklbw	mm0,mm6
			psubb		mm4,mm1
			psubsw		mm4,mm0
			pmullw		mm4,mm7
			psrlw		mm4,8
			paddb		mm4,mm0
			
			packuswb	mm4,mm6
			movd		[eax+edi],mm4
			
			add			edi,4
			cmp			edi,[row_size]
			jl			loopx
			
			mov			edi,0
			add			eax,dst_pitch
			add			ecx,src_pitch
			dec			esi
			
			jnz			loopx
			
			emms
	}
}

void LSub_NoCr_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				const int src_pitch, const int row_size, const int height,
				unsigned char level)
{
	static const __int64 Level = ((__int64)level<<48) + ((__int64)level<<32) +
								 ((__int64)level<<16) + (__int64)level;
	static const __int64 Invert = 0x0080008000800080i64;
	__asm {
			mov			eax,[dstp]
			mov			ecx,[srcp]
			mov			esi,[height]
			mov			edi,0
			movq		mm7,[Level]
			pxor		mm6,mm6
			movq		mm5,[Invert]
loopx:
			movd		mm1,[ecx+edi]
			punpcklbw	mm1,mm6
			movq		mm4,mm5
			
			psubsw		mm4,mm1
			pmullw		mm4,mm7
			psrlw		mm4,8
			paddb		mm4,mm1
			
			packuswb	mm4,mm6
			movd		[eax+edi],mm4
			
			add			edi,4
			cmp			edi,[row_size]
			jl			loopx
			
			mov			edi,0
			add			eax,dst_pitch
			add			ecx,src_pitch
			dec			esi
			
			jnz			loopx
			
			emms
	}
}

void LFast_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			   const int src_pitch, const int row_size, const int height,
			   unsigned char level)
{
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<row_size; x+=2)
		{
			dstp[x] = 1 + ( (dstp[x]+srcp[x])>>1 );
			dstp[x+1] = 1 + ( (dstp[x+1]+srcp[x+1])>>1 );
		}
		dstp += dst_pitch;
		srcp += src_pitch;
	}
}

void LFast_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				 const int src_pitch, const int row_size, const int height,
				 unsigned char level)
{
	static const __int64 Mask = 0x7F7F7F7F7F7Fi64;
	static const __int64 One = 0x0101010101010101i64;
	__asm {
			mov			eax,[dstp]
			mov			ecx,[dstp]
			mov			esi,[height]
			mov			edi,0
			movq		mm7,[Mask]
			movq		mm6,[One]
			movq		mm5,[Mask]
			movq		mm4,[One]
loopx:
			movq		mm0,[eax+edi]
			movq		mm1,[ecx+edi]
			psrlq		mm0,1
			psrlq		mm1,1
			pand		mm0,mm7
			pand		mm1,mm5
			paddb		mm0,mm1
			movq		[eax+edi],mm0

			add			edi,8
			cmp			edi,[row_size]
			jl			loopx
			
			mov			edi,0
			add			eax,dst_pitch
			add			ecx,src_pitch
			add			ebx,dst_pitch
			add			edx,src_pitch
			dec			esi
			
			jnz			loopx
			
			emms
	}
}

void LMul_C_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			  const int src_pitch, const int row_size, const int height,
			  unsigned char level)
{
	unsigned char s1, s2;
	int temp = 0;
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<row_size; x++)
		{
			s1 = dstp[x];
			s2 = srcp[x];
			temp = level * ((short int)s2-s1);
			temp &= 0x0000FFFF;
			temp >>= 8;
			dstp[x] = s1 + temp;
		}
		dstp += dst_pitch;
		srcp += src_pitch;
	}
}

void LMul_C_NoCr_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			  const int src_pitch, const int row_size, const int height,
			  unsigned char level)
{
	unsigned char s1, s2;
	int temp = 0;
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<row_size; x++)
		{
			s1 = dstp[x];
			s2 = srcp[x];
			temp = level * ((short int)128-s1);
			temp >>= 8;
			dstp[x] = s1 + ( (temp & 0x0000FFFF)>>8 );
		}
		dstp += dst_pitch;
		srcp += src_pitch;
	}
}

/* Could be optimized so as to use all 8 registers, and compute 4 pixels
   per iteration - would also avoid stall due to pmullw */
void LMul_C_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				const int src_pitch, const int row_size, const int height,
				unsigned char level)
{
	static const __int64 Level = ((__int64)level<<48) + ((__int64)level<<32) +
								 ((__int64)level<<16) + level;
#if 1
	__asm {
			mov			eax,[dstp]
			mov			ecx,[srcp]
			mov			esi,[height]
			mov			edi,0
			movq		mm7,[Level]
			pxor		mm6,mm6
loopx:
			movd		mm0,[eax+edi]
			movd		mm1,[ecx+edi]
			
			punpcklbw	mm0,mm6
			punpcklbw	mm1,mm6
			
			psubsw		mm1,mm0
			pmullw		mm1,mm7
			psrlw		mm1,8
			paddb		mm1,mm0
			packuswb	mm1,mm6
			
			movd		[eax+edi],mm1
			
			add			edi,4
			cmp			edi,[row_size]
			jl			loopx
			
			mov			edi,0
			add			eax,dst_pitch
			add			ecx,src_pitch
			dec			esi
			
			jnz			loopx
			
			emms
	}

#else //I suck at real optimizing = same speed as above
	__asm {
			mov			eax,[dstp]
			mov			ebx,eax
			mov			esi,[height]
			mov			edi,0
			movq		mm7,[Level]
			pxor		mm6,mm6
loopx:
			movd		mm0,[eax+edi]
			movd		mm1,[ecx+edi]
			
			punpcklbw	mm0,mm6
			add			edi,4

			punpcklbw	mm1,mm6
			movd		mm2,[ebx+edi]
			psubsw		mm1,mm0
			movd		mm3,[edx+edi]
			punpcklbw	mm2,mm6

			pmullw		mm1,mm7
			punpcklbw	mm3,mm6
			sub			edi,4
			psubsw		mm3,mm2
			psrlw		mm1,8
			pmullw		mm3,mm7
			paddb		mm1,mm0
			psrlw		mm3,8
			paddb		mm3,mm2
			packuswb	mm3,mm1
			movd		[eax+edi],mm3
			
			add			edi,4
			cmp			edi,[row_size]
			jl			loopx
			
			mov			edi,0
			add			eax,dst_pitch
			add			ecx,src_pitch
			dec			esi
			
			jnz			loopx
			
			emms
	}
#endif
}

void LMul_C_NoCr_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				const int src_pitch, const int row_size, const int height,
				unsigned char level)
{
	static const __int64 Level = ( ((__int64)level<<47) + ((__int64)level<<31) +
								   ((__int64)level<<15) + ((__int64)level>>1) ) & 0x7F7F7F7F7F7F7F7Fi64;
	static const __int64 Chroma = 0x0080008000800080i64;
	__asm {
			mov			eax,[dstp]
			mov			ecx,[srcp]
			mov			esi,[height]
			mov			edi,0
			movq		mm7,[Level]
			pxor		mm6,mm6
			movq		mm5,[Chroma]
loopx:
			movd		mm0,[eax+edi]
			movq		mm4,mm5
			
			punpcklbw	mm0,mm6

			psubsw		mm4,mm0
			pmullw		mm4,mm7
			psrlw		mm4,8
			paddb		mm4,mm0
			packuswb	mm4,mm6
			
			movd		[eax+edi],mm4
			
			add			edi,4
			cmp			edi,[row_size]
			jl			loopx
			
			mov			edi,0
			add			eax,dst_pitch
			add			ecx,src_pitch
			dec			esi
			
			jnz			loopx
			
			emms
	}
}
	
void LMul_Y_C(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
			  const int src_pitch, const int row_size, const int height,
			  unsigned char level)
{
	short int temp = 0;
	unsigned char s1, s2;
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<row_size; x++)
		{
			s1 = dstp[x];
			s2 = srcp[x];
			temp = ((s1*s2)>>8) - s1;
			temp = (level * temp) & 0x0000FFFF;
			dstp[x] = s1 + (temp>>8);
		}
		dstp += dst_pitch;
		srcp += src_pitch;
	}
}

/* Could be optimized so as to use all 8 registers, and compute 4 pixels
   per iteration - would also avoid stall due to pmullw */
void LMul_Y_MMX(unsigned char *dstp, const unsigned char *srcp, const int dst_pitch,
				const int src_pitch, const int row_size, const int height,
				unsigned char level)
{
	static const __int64 Level = ((__int64)level<<48) + ((__int64)level<<32) +
								 ((__int64)level<<16) + level;
	__asm {
			mov			eax,[dstp]
			mov			ecx,[srcp]
			mov			esi,[height]
			mov			edi,0
			movq		mm7,[Level]
			pxor		mm6,mm6
loopx:
			movd		mm0,[eax+edi]
			movd		mm1,[ecx+edi]
			
			punpcklbw	mm0,mm6
			punpcklbw	mm1,mm6
			movq		mm2,mm1
			
			pmullw		mm1,mm0
			psrlw		mm1,8
			psubsw		mm1,mm2
			pmullw		mm1,mm7
			psrlw		mm1,8
			paddb		mm1,mm2
			packuswb	mm1,mm6
			
			movd		[eax+edi],mm1
			
			add			edi,4
			cmp			edi,[row_size]
			jl			loopx
			
			mov			edi,0
			add			eax,dst_pitch
			add			ecx,src_pitch
			dec			esi
			
			jnz			loopx
			
			emms
	}
}


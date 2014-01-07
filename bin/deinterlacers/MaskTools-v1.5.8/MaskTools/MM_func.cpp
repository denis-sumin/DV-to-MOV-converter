// MaskedMerge - Weighed merge of video planes depending on masks
// MaskedMerge (C) 2003 Kurosu (kurosu@inforezo.org)

// Similar functionnalities (but not directly linked to it) can be found
// in MergeLuma and MergeChroma from Avisynth (www.avisynth.org) by
// Sh0dan (Sh0dan@avisynth.org)

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

/* dst will at worst (when mask=0) have an influence of 1 */
void MMerge_MMX(unsigned char *pDst, const unsigned char *pSrc1,
                const unsigned char *pSrc2, const unsigned char *pMask,
                int dstPitch, int src1Pitch, int src2Pitch, int maskPitch,
                int width, int height)
{
	__declspec(align(8)) static const __int64 Addi = 0x0080008000800080i64;

	__asm {
        emms

		mov			eax,[pSrc1]
		mov			ebx,[pSrc2]
		mov			ecx,[pMask]
		mov			edx,[pDst]
		mov			edi,0
		mov			esi,[height]
		pxor		mm6, mm6
		align 16
loopx:
		movq		mm0, [eax+edi]
		 movq		 mm1, [ebx+edi]
		movq		mm2, [ecx+edi]
		 movq		 mm3, [eax+edi]
		movq		mm4, [ebx+edi]
		 movq		 mm5, [ecx+edi]

		punpcklbw	mm0, mm6
		 punpcklbw	 mm1, mm6
		punpcklbw	mm2, mm6
		 punpckhbw	 mm3, mm6
		punpckhbw	mm4, mm6
		 punpckhbw	 mm5, mm6

		psubw		mm1, mm0
		 psubw		 mm4, mm3

		pmullw		mm1, mm2
		 pmullw		 mm4, mm5

		psllw		mm0, 8
		 psllw		 mm3, 8

		pxor		mm0, Addi
		 pxor		 mm3, Addi

		paddw		mm0, mm1
		 paddw		 mm3, mm4

		psrlw		mm0, 8
		 psrlw		 mm3, 8

		packuswb	mm0, mm3

		movq		[edx+edi],mm0

		add			edi,8
        cmp			edi, [width]
		jl			loopx

		mov			edi,0
		add			eax,[src1Pitch]
		add			ebx,[src2Pitch]
		add			ecx,[maskPitch]
        add			edx,[dstPitch]
		dec			esi

		jnz			loopx

		emms
	}
}

void MMerge_C(unsigned char *pDst, const unsigned char *pSrc1,
              const unsigned char *pSrc2, const unsigned char *pMask,
              int dstPitch, int src1Pitch, int src2Pitch, int maskPitch,
              int width, int height)
{
	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x+=2)
		{
			pDst[x] = ( (256-pMask[x]) * pSrc1[x] + pMask[x] * pSrc2[x] + 128) >> 8;
			pDst[x+1] = ( (256-pMask[x + 1]) * pSrc1[x + 1] + pMask[x + 1] * pSrc2[x + 1] + 128) >> 8;
		}
		pDst += dstPitch;
		pSrc1 += src1Pitch;
        pSrc2 += src2Pitch;
		pMask += maskPitch;
	}
}
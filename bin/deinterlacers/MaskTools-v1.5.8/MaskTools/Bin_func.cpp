// Binarize - Binarize (outputs either 0 or 255) an video plane
// Binarize (C) 2003 Kurosu (kurosu@inforezo.org)

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

void BinUpper_MMX(unsigned char *pDst, const unsigned char *pSrc,
                  int dstPitch, int srcPitch, int width, int height, int threshold)
{
	unsigned int thres = threshold;
	thres |= (thres << 8);
	thres |= (thres << 16);
    int srcPitchx2 = srcPitch * 2;
    int dstPitchx2 = dstPitch * 2;
	__asm {
        emms 

        mov			ebx, [pSrc]
		mov			eax, [pDst]
		mov			edx, [srcPitch]
        mov         ecx, [dstPitch]
        
        add         edx, ebx
        add         ecx, eax

        mov			esi, [height]
        shr         esi, 1

        xor         edi, edi
		movd		mm7, [thres]
		movd		mm6, [thres]
		punpcklbw	mm7, mm6
		movq		mm6, mm7
		pxor		mm4, mm4
		pxor		mm5, mm5

		align 16

xloop : 

		movq		mm0, [ebx + edi]
         movq        mm1, [edx + edi]
		psubusb		mm0, mm6
         psubusb	 mm1, mm7
		pcmpeqb		mm0, mm4
         pcmpeqb	 mm1, mm5
		movq		[eax + edi], mm0
         movq		 [ecx + edi], mm1

	    add			edi, 8
        cmp			edi, [width]
		jl			xloop

        xor         edi, edi

		add			ebx, [srcPitchx2]
         add         eax, [dstPitchx2]
        add         edx, [srcPitchx2]
         add         ecx, [dstPitchx2]
        
		dec			esi
		jnz			xloop

		emms
	}
}

void BinUpper_C(unsigned char *pDst, const unsigned char *pSrc,
                int dstPitch, int srcPitch, int width, int height, int threshold)
{
	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x++)
            pDst[x] = (pSrc[x] > threshold) ? 0 : 255;
        pSrc += srcPitch;
        pDst += dstPitch;
	}
}

void BinLower_MMX(unsigned char *pDst, const unsigned char *pSrc,
                  int dstPitch, int srcPitch, int width, int height, int threshold)
{
	unsigned int thres = threshold;
	thres |= (thres << 8);
	thres |= (thres << 16);
    unsigned int complement = 0xffffffff;
    int srcPitchx2 = srcPitch * 2;
    int dstPitchx2 = dstPitch * 2;
	__asm {
        emms 

        mov			ebx, [pSrc]
		mov			eax, [pDst]
		mov			edx, [srcPitch]
        mov         ecx, [dstPitch]
        add         edx, ebx
        add         ecx, eax

        mov			esi, [height]
        shr         esi, 1

        xor         edi, edi
		movd		mm7, [thres]
		movd		mm6, [thres]
		punpcklbw	mm7, mm6
		movd		mm5, [complement]
		movd		mm4, [complement]
		punpcklbw	mm5, mm4
		movq		mm6, mm7
		movq		mm4, mm5
		pxor		mm3, mm3
		pxor		mm2, mm2

		align 16

xloop : 

		movq		mm0, [ebx + edi]
         movq        mm1, [edx + edi]
		psubusb		mm0, mm6
         psubusb	 mm1, mm7
		pcmpeqb		mm0, mm2
         pcmpeqb	 mm1, mm3
        pxor		mm0, mm4
		 pxor		 mm1, mm5
		movq		[eax + edi], mm0
         movq		 [ecx + edi], mm1

	    add			edi, 8
        cmp			edi, [width]
		jl			xloop

        xor         edi, edi

		add			ebx, [srcPitchx2]
         add         eax, [dstPitchx2]
        add         edx, [srcPitchx2]
         add         ecx, [dstPitchx2]
        
		dec			esi
		jnz			xloop

		emms
	}
}

void BinLower_C(unsigned char *pDst, const unsigned char *pSrc,
                int dstPitch, int srcPitch, int width, int height, int threshold)
{
	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x++)
            pDst[x] = (pSrc[x] <= threshold) ? 0 : 255;
        pSrc += srcPitch;
        pDst += dstPitch;
	}

}
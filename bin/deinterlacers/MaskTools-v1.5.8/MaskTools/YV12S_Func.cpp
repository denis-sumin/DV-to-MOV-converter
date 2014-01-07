// YV12Subtract - expansion of AviSynth's Subtract
// YV12Subtract (C) 2003 Kurosu (kurosu@inforezo.org)
// Subtract (C) AviSynth's team

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

#include <math.h>
#include "YV12Substract.h"
//Yeah I know, 2nd version doesn't work if sub result is <127 or >127
void Substract_MMX(unsigned char *pDst, const unsigned char *pSrc1,
                   const unsigned char *pSrc2, int dstPitch, int src1Pitch,
                   int src2Pitch, int width, int height, unsigned char tol,
                   unsigned char *LUT)
{
	static const __int64 Addi2 = 0x8080808080808080i64;
	__asm {
        emms

        mov         eax, [pDst]
        mov         ebx, [pSrc1]
        mov         ecx, [pSrc2]

		mov			esi, [height]
		pxor		mm6, mm6
		pxor		mm5, mm5
		movq		mm7, [Addi2]
		mov			edi, 0
loopx:
		movq		mm2, [ebx + edi]
		movq		mm0, [ecx + edi]

		psubb		mm2, mm0
		paddb		mm2, mm7
		movq		[eax + edi], mm2

		add			edi, 8
		cmp			edi, [width]
		jl			loopx

		add			eax, [src2Pitch]
        add         ebx, [src1Pitch]
		add			ecx, [dstPitch]
		xor			edi, edi
		dec			esi

		jnz			loopx

		emms
	}
}

void Substract_C2(unsigned char *pDst, const unsigned char *pSrc1,
                  const unsigned char *pSrc2, int dstPitch, int src1Pitch,
                  int src2Pitch, int width, int height, unsigned char tol,
                  unsigned char *LUT)
{
	unsigned char *pDstn = pDst + dstPitch;
	const unsigned char *pSrc1n = pSrc1 + src1Pitch;
    const unsigned char *pSrc2n = pSrc2 + src2Pitch;

	for (int y = 0; y < height; y += 2)
	{
		for (int x = 0; x < width; x += 2)
		{
			pDst[x] = (pSrc1[x] - pSrc2[x]) + 128;
			pDst[x + 1] = (pSrc1[x + 1] - pSrc2[x+1]) + 128;
			pDstn[x] = (pSrc1n[x] - pSrc2n[x]) + 128;
			pDstn[x + 1] = (pSrc1n[x + 1] - pSrc2n[x + 1]) + 128;
		}
		pDst += dstPitch << 1;
        pDstn += dstPitch << 1;
        pSrc1 += src1Pitch << 1;
        pSrc1n += src1Pitch << 1;
        pSrc2 += src2Pitch << 1;
        pSrc2n += src2Pitch << 1;
	}
}

void Substract_C1(unsigned char *pDst, const unsigned char *pSrc1,
                  const unsigned char *pSrc2, int dstPitch, int src1Pitch,
                  int src2Pitch, int width, int height, unsigned char tol,
                  unsigned char *LUT)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
			pDst[x] = (pSrc1[x] - pSrc2[x]) + 128;
		pDst += dstPitch;
        pSrc1 += src1Pitch;
        pSrc2 += src2Pitch;
	}
}

void SubstractW_C2(unsigned char *pDst, const unsigned char *pSrc1,
                   const unsigned char *pSrc2, int dstPitch, int src1Pitch,
                   int src2Pitch, int width, int height, unsigned char tol,
                   unsigned char *LUT)
{
	unsigned char *pDstn = pDst + dstPitch;
	const unsigned char *pSrc1n = pSrc1 + src1Pitch;
    const unsigned char *pSrc2n = pSrc2 + src2Pitch;

	for (int y = 0; y < height; y += 2)
	{
		for (int x = 0; x < width; x += 2)
		{
			pDst[x] = LUT[255 + pSrc1[x] - pSrc2[x]];
			pDst[x + 1] = LUT[255 + pSrc1[x + 1] - pSrc2[x+1]];
			pDstn[x] = LUT[255 + pSrc1n[x] - pSrc2n[x]];
			pDstn[x + 1] = LUT[255 + pSrc1n[x + 1] - pSrc2n[x + 1]];
		}
		pDst += dstPitch << 1;
        pDstn += dstPitch << 1;
        pSrc1 += src1Pitch << 1;
        pSrc1n += src1Pitch << 1;
        pSrc2 += src2Pitch << 1;
        pSrc2n += src2Pitch << 1;
	}
}

void SubstractW_MMX(unsigned char *pDst, const unsigned char *pSrc1,
                    const unsigned char *pSrc2, int dstPitch, int src1Pitch,
                    int src2Pitch, int width, int height, unsigned char tol,
                    unsigned char *LUT)
{
	unsigned char *pDstn = pDst + dstPitch;
	const unsigned char *pSrc1n = pSrc1 + src1Pitch;
    const unsigned char *pSrc2n = pSrc2 + src2Pitch;

	for (int y = 0; y < height; y += 2)
	{
		for (int x = 0; x < width; x += 2)
		{
			pDst[x] = LUT[255 + pSrc1[x] - pSrc2[x]];
			pDst[x + 1] = LUT[255 + pSrc1[x + 1] - pSrc2[x+1]];
			pDstn[x] = LUT[255 + pSrc1n[x] - pSrc2n[x]];
			pDstn[x + 1] = LUT[255 + pSrc1n[x + 1] - pSrc2n[x + 1]];
		}
		pDst += dstPitch << 1;
        pDstn += dstPitch << 1;
        pSrc1 += src1Pitch << 1;
        pSrc1n += src1Pitch << 1;
        pSrc2 += src2Pitch << 1;
        pSrc2n += src2Pitch << 1;
	}
}

void SubstractW_C1(unsigned char *pDst, const unsigned char *pSrc1,
                   const unsigned char *pSrc2, int dstPitch, int src1Pitch,
                   int src2Pitch, int width, int height, unsigned char tol,
                   unsigned char *LUT)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
			pDst[x] = LUT[255 + pSrc1[x] - pSrc2[x]];
		pDst += dstPitch;
        pSrc1 += src1Pitch;
        pSrc2 += src2Pitch;
	}
}


void Diff_C(unsigned char *pDst, const unsigned char *pSrc1,
            const unsigned char *pSrc2, int dstPitch, int src1Pitch,
            int src2Pitch, int width, int height, unsigned char tol,
            unsigned char *LUT)
{
	short int temp;
	for (int y = 0; y < height; y ++)
	{
		for (int x = 0; x < width; x ++)
		{
            temp = abs(pSrc1[x] - pSrc2[x]) - tol;
            pDst[x] = (temp < 0) ? 0 : (unsigned char)temp;
        }
		pDst += dstPitch;
        pSrc1 += src1Pitch;
        pSrc2 += src2Pitch;
	}
}

void Diff_MMX(unsigned char *pDst, const unsigned char *pSrc1,
              const unsigned char *pSrc2, int dstPitch, int src1Pitch,
              int src2Pitch, int width, int height, unsigned char tol,
              unsigned char *LUT)
{
	static const __int64 Tolerance = ((__int64)tol << 56) + ((__int64)tol << 48) + ((__int64)tol << 40) +
									((__int64)tol << 32) + ((__int64)tol << 24) + ((__int64)tol << 16) +
									((__int64)tol << 8) + ((__int64)tol);
	__asm {
        emms 

        mov         eax, [pDst]
        mov         ebx, [pSrc1]
        mov         ecx, [pSrc2]

		mov			esi, [height]
		xor			edi, edi
		movq		mm7, [Tolerance]
loopx:
		movq		mm0, [ebx + edi]
		movq		mm2, [ecx + edi]
		movq		mm4, mm0
		psubusb		mm0, mm2
		psubusb		mm2, mm4
		por			mm0, mm2
		psubusb		mm0, mm7
		movq		[eax + edi], mm0

		add			edi, 8
		cmp			edi, [width]
		jl			loopx

		xor			edi, edi
		add			eax, dstPitch
		add			ebx, src1Pitch
		add			ecx, src2Pitch
		dec			esi

		jnz			loopx

		emms
	}
}
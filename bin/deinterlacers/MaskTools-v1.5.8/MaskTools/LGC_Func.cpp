// Logic - very basic mask logic manipulations
// Logic (C) 2003 Kurosu (kurosu@inforezo.org)

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

#include <memory.h>

void And_C4(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	const unsigned char *pSrc1n = pSrc1 + src1Pitch;
	const unsigned char *pSrc2n = pSrc2 + src2Pitch;
    unsigned char *pDstn = pDst + dstPitch;
	for (int y = 0; y < height; y += 2)
	{
		for (int x = 0; x < width; x += 4)
		{
			*(unsigned int *)(pDst + x) = *(unsigned int *)(pSrc1 + x) & *(unsigned int *)(pSrc2 + x);
			*(unsigned int *)(pDstn + x) = *(unsigned int *)(pSrc1n + x) & *(unsigned int *)(pSrc2n + x);
		}
		pSrc2 += src2Pitch << 1;
        pSrc1 += src1Pitch << 1;
        pSrc2n += src2Pitch << 1;
        pSrc1n += src1Pitch << 1;
        pDst += dstPitch << 1;
        pDstn += dstPitch << 1;
	}
}

void And_C1(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	for (int y = 0; y < height; y ++)
	{
		for (int x = 0; x < width; x ++)
			pDst[x] = pSrc1[x] & pSrc2[x];
        pDst += dstPitch;
		pSrc1 += src1Pitch;
		pSrc2 += src2Pitch;
	}
}


void And_MMX(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	__asm {
		mov			eax, [pDst]
        mov         ebx, [pSrc1]
		mov			ecx, [pSrc2]
		mov			esi, [height]
		xor			edi, edi
loopx:
		movq		mm0, [ebx + edi]
        pand        mm0, [ecx + edi]
		movq		[eax + edi], mm0

		add			edi,8
		cmp			edi,[width]
		jl			loopx

		xor			edi, edi
		add			eax, [dstPitch]
		add			ebx, [src1Pitch]
		add			ecx, [src2Pitch]
		dec			esi

		jnz			loopx

		emms
	}
}

void Or_C4(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	const unsigned char *pSrc1n = pSrc1 + src1Pitch;
	const unsigned char *pSrc2n = pSrc2 + src2Pitch;
    unsigned char *pDstn = pDst + dstPitch;
	for (int y = 0; y < height; y += 2)
	{
		for (int x = 0; x < width; x += 4)
		{
			*(unsigned int *)(pDst + x) = *(unsigned int *)(pSrc1 + x) | *(unsigned int *)(pSrc2 + x);
			*(unsigned int *)(pDstn + x) = *(unsigned int *)(pSrc1n + x) | *(unsigned int *)(pSrc2n + x);
		}
		pSrc2 += src2Pitch << 1;
        pSrc1 += src1Pitch << 1;
        pSrc2n += src2Pitch << 1;
        pSrc1n += src1Pitch << 1;
        pDst += dstPitch << 1;
        pDstn += dstPitch << 1;
	}
}

void Or_C1(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	for (int y = 0; y < height; y ++)
	{
		for (int x = 0; x < width; x ++)
			pDst[x] = pSrc1[x] | pSrc2[x];
        pDst += dstPitch;
		pSrc1 += src1Pitch;
		pSrc2 += src2Pitch;
	}
}

void Or_MMX(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	__asm {
		mov			eax, [pDst]
        mov         ebx, [pSrc1]
		mov			ecx, [pSrc2]
		mov			esi, [height]
		xor			edi, edi
loopx:
		movq		mm0, [ebx + edi]
        por         mm0, [ecx + edi]
		movq		[eax + edi], mm0

		add			edi,8
		cmp			edi,[width]
		jl			loopx

		xor			edi, edi
		add			eax, [dstPitch]
		add			ebx, [src1Pitch]
		add			ecx, [src2Pitch]
		dec			esi

		jnz			loopx

		emms
	}
}

void Xor_C4(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	const unsigned char *pSrc1n = pSrc1 + src1Pitch;
	const unsigned char *pSrc2n = pSrc2 + src2Pitch;
    unsigned char *pDstn = pDst + dstPitch;
	for (int y = 0; y < height; y += 2)
	{
		for (int x = 0; x < width; x += 4)
		{
			*(unsigned int *)(pDst + x) = *(unsigned int *)(pSrc1 + x) ^ *(unsigned int *)(pSrc2 + x);
			*(unsigned int *)(pDstn + x) = *(unsigned int *)(pSrc1n + x) ^ *(unsigned int *)(pSrc2n + x);
		}
		pSrc2 += src2Pitch << 1;
        pSrc1 += src1Pitch << 1;
        pSrc2n += src2Pitch << 1;
        pSrc1n += src1Pitch << 1;
        pDst += dstPitch << 1;
        pDstn += dstPitch << 1;
	}
}

void Xor_C1(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	for (int y = 0; y < height; y ++)
	{
		for (int x = 0; x < width; x ++)
			pDst[x] = pSrc1[x] ^ pSrc2[x];
        pDst += dstPitch;
		pSrc1 += src1Pitch;
		pSrc2 += src2Pitch;
	}
}

void Xor_MMX(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	__asm {
		mov			eax, [pDst]
        mov         ebx, [pSrc1]
		mov			ecx, [pSrc2]
		mov			esi, [height]
		xor			edi, edi
loopx:
		movq		mm0, [ebx + edi]
        pxor         mm0, [ecx + edi]
		movq		[eax + edi], mm0

		add			edi,8
		cmp			edi,[width]
		jl			loopx

		xor			edi, edi
		add			eax, [dstPitch]
		add			ebx, [src1Pitch]
		add			ecx, [src2Pitch]
		dec			esi

		jnz			loopx

		emms
	}
}

void Andn_C4(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	const unsigned char *pSrc1n = pSrc1 + src1Pitch;
	const unsigned char *pSrc2n = pSrc2 + src2Pitch;
    unsigned char *pDstn = pDst + dstPitch;
	for (int y = 0; y < height; y += 2)
	{
		for (int x = 0; x < width; x += 4)
		{
			*(unsigned int *)(pDst + x) = *(unsigned int *)(pSrc1 + x) & !*(unsigned int *)(pSrc2 + x);
			*(unsigned int *)(pDstn + x) = *(unsigned int *)(pSrc1n + x) & !*(unsigned int *)(pSrc2n + x);
		}
		pSrc2 += src2Pitch << 1;
        pSrc1 += src1Pitch << 1;
        pSrc2n += src2Pitch << 1;
        pSrc1n += src1Pitch << 1;
        pDst += dstPitch << 1;
        pDstn += dstPitch << 1;
	}
}

void Andn_C1(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	for (int y = 0; y < height; y ++)
	{
		for (int x = 0; x < width; x ++)
			pDst[x] = pSrc1[x] & !pSrc2[x];
        pDst += dstPitch;
		pSrc1 += src1Pitch;
		pSrc2 += src2Pitch;
	}
}

void Andn_MMX(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	__asm {
		mov			eax, [pDst]
        mov         ebx, [pSrc1]
		mov			ecx, [pSrc2]
		mov			esi, [height]
		xor			edi, edi
loopx:
		movq		mm0, [ebx + edi]
        pandn       mm0, [ecx + edi]
		movq		[eax + edi], mm0

		add			edi,8
		cmp			edi,[width]
		jl			loopx

		xor			edi, edi
		add			eax, [dstPitch]
		add			ebx, [src1Pitch]
		add			ecx, [src2Pitch]
		dec			esi

		jnz			loopx

		emms
	}
}

void Min_C(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	for (int y = 0; y < height; y ++)
	{
		for (int x = 0; x < width; x ++)
            pDst[x] = (pSrc1[x] < pSrc2[x]) ? pSrc1[x] : pSrc2[x];
        pDst += dstPitch;
		pSrc1 += src1Pitch;
		pSrc2 += src2Pitch;
	}
}

void Max_C(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	for (int y = 0; y < height; y ++)
	{
		for (int x = 0; x < width; x ++)
            pDst[x] = (pSrc1[x] > pSrc2[x]) ? pSrc1[x] : pSrc2[x];
        pDst += dstPitch;
		pSrc1 += src1Pitch;
		pSrc2 += src2Pitch;
	}
}


void Min_iSSE(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	__asm {
		mov			eax, [pDst]
        mov         ebx, [pSrc1]
		mov			ecx, [pSrc2]
		mov			esi, [height]
		xor			edi, edi
loopx:
		movq		mm0, [ebx + edi]
        pminub      mm0, [ecx + edi]
		movq		[eax + edi], mm0

		add			edi,8
		cmp			edi,[width]
		jl			loopx

		xor			edi, edi
		add			eax, [dstPitch]
		add			ebx, [src1Pitch]
		add			ecx, [src2Pitch]
		dec			esi

		jnz			loopx

		emms
	}
}

void Min_MMX(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	__asm {
		mov			eax, [pDst]
        mov         ebx, [pSrc1]
		mov			ecx, [pSrc2]
		mov			esi, [height]
		xor			edi, edi
loopx:
		movq		mm0, [ecx + edi]
        movq		mm1, [ecx + edi]
        pcmpgtb     mm0, [ebx + edi]
        movq        mm2, mm0
        pand        mm0, [ebx + edi]
        pandn       mm1, mm2
        paddb       mm0, mm1
		movq		[eax + edi], mm0

		add			edi,8
		cmp			edi,[width]
		jl			loopx

		xor			edi, edi
		add			eax, [dstPitch]
		add			ebx, [src1Pitch]
		add			ecx, [src2Pitch]
		dec			esi

		jnz			loopx

		emms
	}
}

void Max_iSSE(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	__asm {
		mov			eax, [pDst]
        mov         ebx, [pSrc1]
		mov			ecx, [pSrc2]
		mov			esi, [height]
		xor			edi, edi
loopx:
		movq		mm0, [ebx + edi]
        pmaxub      mm0, [ecx + edi]
		movq		[eax + edi], mm0

		add			edi,8
		cmp			edi,[width]
		jl			loopx

		xor			edi, edi
		add			eax, [dstPitch]
		add			ebx, [src1Pitch]
		add			ecx, [src2Pitch]
		dec			esi

		jnz			loopx

		emms
	}
}

void Max_MMX(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height)
{
	__asm {
		mov			eax, [pDst]
        mov         ebx, [pSrc1]
		mov			ecx, [pSrc2]
		mov			esi, [height]
		xor			edi, edi
loopx:
		movq		mm0, [ecx + edi]
        movq		mm1, [ecx + edi]
        pcmpgtb     mm0, [ebx + edi]
        movq        mm2, mm0
        pandn       mm0, [ebx + edi]
        pand        mm1, mm2
        paddb       mm0, mm1
		movq		[eax + edi], mm0

		add			edi,8
		cmp			edi,[width]
		jl			loopx

		xor			edi, edi
		add			eax, [dstPitch]
		add			ebx, [src1Pitch]
		add			ecx, [src2Pitch]
		dec			esi

		jnz			loopx

		emms
	}
}




// Invert - returns the complement to 255
// Invert (C) 2003 Kurosu (kurosu@inforezo.org)

// Similar functionnalities (but not directly linked to it) can be found
// for instance in Invert from VirtualDub (www.virtualdub.org) by
// Avery Lee (phaeron@virtualdub.org)

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

void Inv_C(unsigned char *srcp, const int src_pitch,
			const int row_size, const int height)
{
	unsigned int temp1, temp2;
	unsigned char *srcpn = srcp + src_pitch;

	for (int y=0; y<height; y+=2)
	{
		for (int x=0; x<row_size; x+=4)
		{
			memcpy(&temp1,srcp+x,4);
			memcpy(&temp2,srcpn+x,4);
			temp1 = (unsigned int)0xFFFFFFFF - (unsigned int)temp1;
			temp2 = (unsigned int)0xFFFFFFFF - (unsigned int)temp2;
			memcpy(srcp+x,&temp1,4);
			memcpy(srcpn+x,&temp2,4);
		}
		srcp += src_pitch<<1;
		srcpn += src_pitch<<1;
	}
/* slower ...*/
/*
	_asm {
		mov			eax,[srcp]
		mov			edx,[row_size]
		mov			esi,[height]
		add			eax,src_pitch
		mov			edi,0

loopx:
		mov			ebx,[eax+edi  ]
		mov			ecx,[eax+edi+4]
		xor			ebx,-1
		xor			ecx,-1
		mov			[eax+edi  ],ebx
		mov			[eax+edi+4],ecx

		add			edi,8
		cmp			edi,edx
		jl			loopx

		mov			edi,0
		add			eax,src_pitch
		dec			esi
		jnz			loopx
	}
*/
}

void Inv_MMX(unsigned char *srcp, const int src_pitch,
			const int row_size, const int height)
{
	__declspec(align(8)) static const __int64 Invert = 0xFFFFFFFFFFFFFFFFi64;
	__asm {
		mov			eax,[srcp]
		mov			ebx,[srcp]
		mov			esi,[height]
		add			ebx,src_pitch
		mov			edi,0
		shr			esi,1
		shl			src_pitch,1
		movq		mm7,[Invert]
loopx:
		movq		mm6,mm7
		movq		mm0,[eax+edi]
		movq		mm5,mm7
		movq		mm1,[ebx+edi]
		psubusb		mm6,mm0
		psubusb		mm5,mm1

		movq		[eax+edi],mm6
		movq		[ebx+edi],mm5

		add			edi,8
		cmp			edi,[row_size]
		jl			loopx

		mov			edi,0
		add			eax,src_pitch
		add			ebx,src_pitch
		dec			esi

		jnz			loopx

		emms
	}
}

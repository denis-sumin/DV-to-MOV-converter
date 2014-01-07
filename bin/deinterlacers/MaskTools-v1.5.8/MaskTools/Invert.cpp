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

#include "Invert.h"
#include "Inv_func.h"
#include "GenericTools.h"

Invert::Invert(PClip _child, int _X, int _Y, int _w, int _h,
			   int y, int u, int v, IScriptEnvironment* env) :
GenericVideoFilter(_child), Y(y), V(v), U(u), w(_w), h(_h),
offX(_X & 0xFFFFFFFE), offY(_Y & 0xFFFFFFFE)
{
	if (!vi.IsYV12())
		env->ThrowError("Invert: Needs YV12 data");
	
	if (offX<0 || offX>vi.width)
		env->ThrowError("Invert: horizontal offset %i bigger than source width %i",
						offX, vi.width);
	if (offY<0 || offY>vi.height)
		env->ThrowError("Invert: vertical offset %i bigger than source height %i",
						offY, vi.height);

	if (w<0) w = vi.width;
	if (w>vi.width-offX) w = vi.width-offX;
	if (h<0) h = vi.height;
	if (h>vi.height-offY) h = vi.height-offY;
	w &= 0xFFFFFFFE;
	h &= 0xFFFFFFFE;

	if (env->GetCPUFlags() & CPUF_MMX)
	{ 
		if (!(w%16) && !(h%4))
		{ InvY = Inv_MMX; InvUV = Inv_MMX; }
		else if (!(w%8))
		{ InvY = Inv_MMX; InvUV = Inv_C; }
		else 
		{ InvY = Inv_C; InvUV = Inv_C; }
	}
	else 
	{ InvY = Inv_C; InvUV = Inv_C; }

	if (!Verify(Y)) env->ThrowError("Invert: Unvalid operation mode on Y");
	if (!Verify(U)) env->ThrowError("Invert: Unvalid operation mode on U");
	if (!Verify(V)) env->ThrowError("Invert: Unvalid operation mode on V");
}

PVideoFrame __stdcall Invert::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame			src = child->GetFrame(n, env);
	env->MakeWritable(&src);
	unsigned char		*srcp_y = src->GetWritePtr(PLANAR_Y);
	unsigned char		*srcp_u = src->GetWritePtr(PLANAR_U);
	unsigned char		*srcp_v = src->GetWritePtr(PLANAR_V);
	const int			src_pitch_y = src->GetPitch(PLANAR_Y);
	const int			src_pitch_uv = src->GetPitch(PLANAR_U);
	
	const int			row_size = src->GetRowSize(PLANAR_Y);
	const int			height = src->GetHeight(PLANAR_Y);
	
	switch (Y)
	{
	case OPERATION:
		InvY(srcp_y + offX + offY * src_pitch_y, src_pitch_y, w, h);
		break;
	case COPY_SOURCE:
	case NO_OPERATION:
		break;
	default:
		MemZoneSet(srcp_y, -Y, w, h, offX, offY, src_pitch_y);
		break;
	}
	
	switch (U)
	{
	case OPERATION:
		InvUV(srcp_u + offX / 2 + offY * src_pitch_uv / 2, src_pitch_uv, w>>1, h>>1);
		break;
	case COPY_SOURCE:
	case NO_OPERATION:
		break;
	default:
		MemZoneSet(srcp_u, -U, w>>1, h>>1, offX>>1, offY>>1, src_pitch_uv);
		break;
	}
	
	switch (V)
	{
	case OPERATION:
		InvUV(srcp_v + offX / 2 + offY * src_pitch_uv / 2, src_pitch_uv, w>>1, h>>1);
		break;
	case COPY_SOURCE:
	case NO_OPERATION:
		break;
	default:
		MemZoneSet(srcp_v, -V, w>>1, h>>1, offX>>1, offY>>1, src_pitch_uv);
		break;
	}
	
	return src;
}
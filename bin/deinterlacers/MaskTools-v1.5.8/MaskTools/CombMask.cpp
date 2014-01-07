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

#include "CombMask.h"
#include "CM_func.h"
#include "GenericTools.h"

CombMask::CombMask(PClip _child, unsigned int thY1, unsigned int thY2,
				   int y, int u, int v, bool usemmx, IScriptEnvironment* env) :
GenericVideoFilter(_child), Yth1(thY1), Yth2(thY2), Y(y), U(u), V(v)
{
	if (!Verify(Y)) env->ThrowError("CombMask: Unvalid operation mode on Y");
	if (!Verify(U)) env->ThrowError("CombMask: Unvalid operation mode on U");
	if (!Verify(V)) env->ThrowError("CombMask: Unvalid operation mode on V");

	if (!vi.IsYV12())
		env->ThrowError("CombMask: Needs YV12 data");
	
	if ( thY1 > 255)
		env->ThrowError("CombMask: first luma threshold not in the range [0..255]");
	if ( thY2 > 255)
		env->ThrowError("CombMask: second luma threshold not in the range [0..255]");
	
	if (thY1 > thY2)
		env->ThrowError("CombMask: the first luma threshold should not be superior to the second one");

	CM = CM_C;

}

CombMask::~CombMask()
{
}

PVideoFrame __stdcall CombMask::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame			der = env->NewVideoFrame(vi);
	unsigned char		*derp_y = der->GetWritePtr(PLANAR_Y);
	unsigned char		*derp_u = der->GetWritePtr(PLANAR_U);
	unsigned char		*derp_v = der->GetWritePtr(PLANAR_V);
	const int			der_pitch_y = der->GetPitch(PLANAR_Y);
	const int			der_pitch_uv = der->GetPitch(PLANAR_U);

	PVideoFrame			src = child->GetFrame(n, env);

	const int			row_size = src->GetRowSize(PLANAR_Y);
	const int			height = src->GetHeight(PLANAR_Y);

	const unsigned char *srcp_y = src->GetReadPtr(PLANAR_Y);
	const unsigned char *srcp_u = src->GetReadPtr(PLANAR_U);
	const unsigned char *srcp_v = src->GetReadPtr(PLANAR_V);
	const int			src_pitch_y = src->GetPitch(PLANAR_Y);
	const int			src_pitch_uv = src->GetPitch(PLANAR_U);

	switch (Y)
	{
	case OPERATION:
		CM(srcp_y, derp_y, src_pitch_y, der_pitch_y, row_size,
			height,	Yth1, Yth2);
		break;
	case COPY_SOURCE:
		env->BitBlt(derp_y,der_pitch_y,srcp_y,src_pitch_y,row_size,height);
		break;
	case NO_OPERATION:
		break;
	default:
		MemZoneSet(derp_y, -Y, row_size, height, 0, 0, der_pitch_y);
		break;
	}
			
	switch (U)
	{
	case OPERATION:
	case COPY_SOURCE:
		env->BitBlt(derp_u,	der_pitch_uv, srcp_u, src_pitch_uv,
					(row_size>>1), (height>>1));
	case NO_OPERATION:
		break;
	default:
		MemZoneSet(derp_u, -U, row_size / 2, height / 2, 0, 0, der_pitch_uv);
		break;
	}

	switch (V)
	{
	case OPERATION:
	case COPY_SOURCE:
		env->BitBlt(derp_v,	der_pitch_uv, srcp_v, src_pitch_uv,
					(row_size>>1),(height>>1));
		break;
	case NO_OPERATION:
		break;
	default:
		MemZoneSet(derp_v, -V, row_size / 2, height / 2, 0, 0, der_pitch_uv);
		break;
	}

	return der;
}

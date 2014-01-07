//MaskOverlay - outputs a mask suitable for background replacement.

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
#include "Overlay.h"

Overlay::Overlay(PClip _child, PClip cl2, int _tY, int _tC, int _s, IScriptEnvironment* env) : 
GenericVideoFilter(_child), clip2(cl2), tY(_tY), tC(_tC), strictness(_s)
{
	const VideoInfo& vi2 = clip2->GetVideoInfo();
	
	if (!vi.IsYV12() || !vi2.IsYV12())
		env->ThrowError("OverlayMask: needs YV12");
	
	if (vi2.width != vi.width || vi.height != vi2.height)
		env->ThrowError("OverlayMask: Size of 2nd clip different from 1st");
	
	if (_tY<0 || tY>255)
		env->ThrowError("OverlayMask: Unvalid luma threshold");
	if (_tC<0 || tC>255)
		env->ThrowError("OverlayMask: Unvalid chroma threshold");
	
	if (_s<0) env->ThrowError("OverlayMask: not strict enough :p");
	if (_s>3) env->ThrowError("OverlayMask: too strict :p");
}

PVideoFrame __stdcall Overlay::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame			src1 = child->GetFrame(n, env);
	env->MakeWritable(&src1);
	unsigned char		*src1p_y = src1->GetWritePtr(PLANAR_Y);
	unsigned char		*src1p_u = src1->GetWritePtr(PLANAR_U);
	unsigned char		*src1p_v = src1->GetWritePtr(PLANAR_V);
	const int			src1_pitch_y = src1->GetPitch(PLANAR_Y);
	unsigned char		*src1pn_y = src1p_y + src1_pitch_y;
	const int			src1_pitch_uv = src1->GetPitch(PLANAR_U);
	
	PVideoFrame			src2 = clip2->GetFrame(n, env);
	const unsigned char	*src2p_y = src2->GetReadPtr(PLANAR_Y);
	const unsigned char	*src2p_u = src2->GetReadPtr(PLANAR_U);
	const unsigned char	*src2p_v = src2->GetReadPtr(PLANAR_V);
	const int			src2_pitch_y = src2->GetPitch(PLANAR_Y);
	const unsigned char	*src2pn_y = src2p_y + src2_pitch_y;
	const int			src2_pitch_uv = src2->GetPitch(PLANAR_U);
	
	const int			row_size = src1->GetRowSize(PLANAR_U);
	const int			height = src1->GetHeight(PLANAR_U);
	
	int					x, y, x2;
	unsigned char		t1, t2, t3, t4;
	
	for (y=0; y<height; y++)
	{
		x2 = 0;
		for (x=0; x<row_size; x++)
		{
			if ( (abs(src1p_u[x]-src2p_u[x]) < tC) &&
				(abs(src1p_v[x]-src2p_v[x]) < tC)
				)
			{
				t1 = ( abs(src1p_y[x2]-src2p_y[x2]) < tY );
				t2 = ( abs(src1p_y[x2+1]-src2p_y[x2+1]) < tY );
				t3 = ( abs(src1pn_y[x2]-src2pn_y[x2]) < tY );
				t4 = ( abs(src1pn_y[x2+1]-src2pn_y[x2+1]) < tY );
				
				if (t1 + t2 + t3 + t4 > strictness)
				{
					src1p_u[x] = 255;
					src1p_v[x] = 255;
					
					if (t1) src1p_y[x2] = 255;
					else src1p_y[x2] = 0;
					if (t2) src1p_y[x2+1] = 255;
					else src1p_y[x2+1] = 0;
					if (t3) src1pn_y[x2] = 255;
					else src1pn_y[x2] = 0;
					if (t4) src1pn_y[x2+1] = 255;
					else src1pn_y[x2+1] = 0;
				}
				else
				{
					src1p_u[x] = 0;
					src1p_v[x] = 0;
					
					src1p_y[x2] = 0;
					src1p_y[x2+1] = 0;
					src1pn_y[x2] = 0;
					src1pn_y[x2+1] = 0;
				}
			}
			else
			{
				src1p_u[x] = 0;
				src1p_v[x] = 0;
				src1p_y[x2] = 0;
				src1p_y[x2+1] = 0;
				src1pn_y[x2] = 0;
				src1pn_y[x2+1] = 0;
			}
			x2 += 2;
		}
		src1p_u += src1_pitch_uv;
		src1p_v += src1_pitch_uv;
		src2p_u += src2_pitch_uv;
		src2p_v += src2_pitch_uv;
		
		src1p_y += src1_pitch_y<<1;
		src1pn_y += src1_pitch_y<<1;
		src2p_y += src2_pitch_y<<1;
		src2pn_y += src2_pitch_y<<1;
	}
	return src1;
}
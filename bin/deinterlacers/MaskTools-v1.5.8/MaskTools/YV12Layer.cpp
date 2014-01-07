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

#include "GenericTools.h"
#include "YV12Layer.h"
#include "YV12L_Func.h"

YV12Layer::YV12Layer(PClip _child1, PClip _child2, const char *string, int _lev,
					 bool Chroma, int _x, int _y, int _w, int _h,
					 int _Y, int _U, int _V, IScriptEnvironment* env ) :
child2(_child2), level(_lev), Y(_Y), U(_U), V(_V),
GenericVideoFilter(_child1)
{
	const VideoInfo& vi1 = _child1->GetVideoInfo();
	const VideoInfo& vi2 = _child2->GetVideoInfo();
    
    /*if (vi1.pixel_type != vi2.pixel_type)
      env->ThrowError("YV12Layer: image formats don't match");*/

	if (!vi1.IsYV12() || !vi2.IsYV12()) 
		env->ThrowError("YV12Layer: only YV12 formats");

	if (!Verify(Y)) env->ThrowError("YV12Layer: Unvalid operation mode on Y");
	if (!Verify(U)) env->ThrowError("YV12Layer: Unvalid operation mode on U");
	if (!Verify(V)) env->ThrowError("YV12Layer: Unvalid operation mode on V");

	_x &= 0xFFFFFFFE; //YV12 must be aligned on even pixels
	_y &= 0xFFFFFFFE; //YV12 must be aligned on even pixels

	xdest=(_x < 0)? 0: _x;
	ydest=(_y < 0)? 0: _y;

	xsrc=(_x < 0)? (0-_x): 0;
	ysrc=(_y < 0)? (0-_y): 0;

	if (_w<0)
		xcount = (vi1.width < (_x + vi2.width))? (vi1.width-xdest) : (vi2.width - xsrc);
	else
	{
		if (xdest + _w> vi1.width) _w = vi1.width - xdest;
		if (xsrc + _w> vi2.width) _w = vi2.width - xsrc;
		xcount = _w;
	}
	if (_h<0)
		ycount = (vi1.height <  (_y + vi2.height))? (vi1.height-ydest) : (vi2.height - ysrc);
	else
	{
		if (ydest + _h> vi1.height) _h = vi1.height - ydest;
		if (ysrc + _h> vi2.height) _h = vi2.height - ysrc;
		ycount = _h;
	}

	if (!lstrcmpi(string,"Sub"))
	{
		if ((env->GetCPUFlags() & CPUF_MMX) && !(xcount%16))
		{
			Process_Y = LSub_MMX;
			if (Chroma) Process_C = LSub_MMX;
			else Process_C = LSub_NoCr_MMX;
		}
		else
		{
			Process_Y = LSub_C;
			if (Chroma) Process_C = LSub_C;
			else Process_C = LSub_NoCr_C;
		}
	}
	else if (!lstrcmpi(string,"Add"))
	{
		if ((env->GetCPUFlags() & CPUF_MMX) && !(xcount%16))
		{
			Process_Y = LAdd_MMX;
			if (Chroma) Process_C = LAdd_MMX;
			else Process_C = LAdd_NoCr_MMX;
		}
		else
		{
			Process_Y = LAdd_C;
			if (Chroma) Process_C = LAdd_C;
			else Process_C = LAdd_NoCr_C;
		}
	}
	else if (!lstrcmpi(string,"Fast"))
	{
		if (!Chroma) env->ThrowError("YV12Layer: Fast doesn't support that chroma mode, use \"Add\" instead");
		if ((env->GetCPUFlags() & CPUF_MMX) && !(xcount%16))
		{
			Process_Y = LFast_MMX;
			Process_C = LFast_MMX;
		}
		else
		{
			Process_Y = LFast_C;
			Process_C = LFast_C;
		}
	}
	else if (!lstrcmpi(string,"Mul"))
	{
		if ((env->GetCPUFlags() & CPUF_MMX) && !(xcount%16))
		{
			Process_Y = LMul_Y_MMX;
			if (Chroma) Process_C = LMul_C_MMX;
			else Process_C = LMul_C_NoCr_MMX;
		}
		else
		{
			Process_Y = LMul_Y_C;
			if (Chroma) Process_C = LMul_C_C;
			else Process_C = LMul_C_NoCr_C;
		}
	}
	/*	The following modes aren't available in YV12 because they are too complex
		Note - the original YUY2 functions aren't exactly behaving as supposed
		either
     */
	/*
	else if (!lstrcmpi(string,"Lighten"))
	{
		if (!Chroma) env->ThrowError("Layer: monochrome lighten illegal op");
		if ((env->GetCPUFlags() & CPUF_MMX) && !(xcount%16))
		{
			Process_Y = LLighten_Y_MMX;
			Process_C = LLighten_C_MMX;
		}
		else
		{
			Process_Y = LLighten_Y_C;
			Process_C = LLighten_C_C;
		}
	}
	else if (!lstrcmpi(string,"Darken"))
	{
		if (!Chroma) env->ThrowError("Layer: monochrome lighten illegal op");
		if ((env->GetCPUFlags() & CPUF_MMX) && !(xcount%16))
		{
			Process_Y = LDarken_Y_MMX;
			Process_C = LDarken_C_MMX;
		}
		else
		{
			Process_Y = LDarken_Y_C;
			Process_C = LDarken_C_C;
		}
	}
	*/
	else env->ThrowError("Arithmetic: unvalid operator");
}

PVideoFrame __stdcall YV12Layer::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame			src1 = child->GetFrame(n, env);

	if (xcount<=0 || ycount<=0) return src1;
	env->MakeWritable(&src1);

	unsigned char		*src1p_y = src1->GetWritePtr(PLANAR_Y);
	unsigned char		*src1p_u = src1->GetWritePtr(PLANAR_U);
	unsigned char		*src1p_v = src1->GetWritePtr(PLANAR_V);
	const int			src1_pitch_y = src1->GetPitch(PLANAR_Y);
	const int			src1_pitch_uv = src1->GetPitch(PLANAR_U);
	
	PVideoFrame			src2 = child2->GetFrame(n, env);
	const unsigned char	*src2p_y = src2->GetReadPtr(PLANAR_Y);
	const unsigned char	*src2p_u = src2->GetReadPtr(PLANAR_U);
	const unsigned char	*src2p_v = src2->GetReadPtr(PLANAR_V);
	const int			src2_pitch_y = src2->GetPitch(PLANAR_Y);
	const int			src2_pitch_uv = src2->GetPitch(PLANAR_U);
	
	const int			row_size = src1->GetRowSize(PLANAR_Y);
	const int			height = src1->GetHeight(PLANAR_Y);
	
	switch (Y)
	{
	case OPERATION:
		Process_Y(src1p_y + xdest + ydest*src1_pitch_y, src2p_y + xsrc + ysrc*src2_pitch_y,
				src1_pitch_y, src2_pitch_y, xcount, ycount, level);
		break;
	case COPY_SOURCE:
		env->BitBlt(src1p_y + xdest + ydest*src1_pitch_y, src1_pitch_y,
					src2p_y + xsrc + ysrc*src2_pitch_y, src2_pitch_y, xcount, ycount);
	case NO_OPERATION:
		break;
	default:
		MemZoneSet(src1p_y, -Y, xcount, ycount, xdest, ydest, src1_pitch_y);
		break;
	}
	
	switch (U)
	{
	case OPERATION:
		Process_C(src1p_u + (xdest>>1) + (ydest>>1)*src1_pitch_uv,
				src2p_u + (xsrc>>1) + (ysrc>>1)*src2_pitch_uv,
				src1_pitch_uv, src2_pitch_uv, xcount>>1, ycount>>1, level);
		break;
	case COPY_SOURCE:
		env->BitBlt(src1p_u + (xdest>>1) + (ydest>>1)*src1_pitch_uv, src1_pitch_uv,
					src2p_u + (xsrc>>1) + (ysrc>>1)*src2_pitch_uv, src2_pitch_uv,
					(xcount>>1), (ycount>>1));
	case NO_OPERATION:
		break;
	default:
		MemZoneSet(src1p_u, -U, xcount>>1, ycount>>1, xdest>>1, ydest>>1, src1_pitch_uv);
		break;
	}
	
	switch (V)
	{
	case OPERATION:
		Process_C(src1p_v + (xdest>>1) + (ydest>>1)*src1_pitch_uv,
				src2p_v + (xsrc>>1) + (ysrc>>1)*src2_pitch_uv,
				src1_pitch_uv, src2_pitch_uv, xcount>>1, ycount>>1, level);
		break;
	case COPY_SOURCE:
		env->BitBlt(src1p_v + (xdest>>1) + (ydest>>1)*src1_pitch_uv, src1_pitch_uv,
					src2p_v + (xsrc>>1) + (ysrc>>1)*src2_pitch_uv, src2_pitch_uv,
					(xcount>>1), (ycount>>1));
	case NO_OPERATION:
		break;
	default:
		MemZoneSet(src1p_v, -V, xcount>>1, ycount>>1, xdest>>1, ydest>>1, src1_pitch_uv);
		break;
	}
	
	return src1;
}
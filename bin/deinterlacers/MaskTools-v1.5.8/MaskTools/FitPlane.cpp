// FitPlane - fits a plane into another one of different dimensions
// using AviSynth's resizers
// FitPlane (C) 2003 Kurosu (kurosu@inforezo.org)
// All resizers used (C) AviSynth's team

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

#include <string.h>

#include "FitPlane.h"

FitPlane::FitPlane(PClip _child, int _mode, const char *res, IScriptEnvironment* env) :
GenericVideoFilter(_child), mode(_mode)
{
	const char* ResizerString;

	if (!vi.IsYV12())
		env->ThrowError("FitPlane: First clip must be in YV12");
	
	if ( (_mode!=FP_Y2U) && (_mode!=FP_Y2V) && (_mode!=FP_Y2U+FP_Y2V) &&
		 (_mode!=FP_U2Y) && (_mode!=FP_V2Y)
	   )
		env->ThrowError("FitPlane: Unvalid plane operation");

	// Exchange bilinear downsizer with reduce if possible
    if ( (mode<4) &&
		 (lstrcmpi(res, "bilinear") == 0) &&
		 !((vi.width&15)+(vi.height&15))
	   )
      res = "reduce";

    // Exchange reduce with bilinear if reduce is not possible
    if ( (mode<4) &&
		 (lstrcmpi(res, "reduce") == 0) &&
         ((vi.width&15) + (vi.height&15))
	   )
        res = "bilinear";
 
	// Verify resizer
	if (lstrcmpi(res, "bilinear") == 0) 
      ResizerString="BilinearResize";
    else if (lstrcmpi(res, "reduce") == 0)
      ResizerString="ReduceBy2";
    else if (lstrcmpi(res, "bicubic") == 0)
      ResizerString="BicubicResize";
    else if (lstrcmpi(res, "lanczos") == 0)
      ResizerString="LanczosResize";
    else if (lstrcmpi(res, "point") == 0)
      ResizerString="PointResize";
    else 
      env->ThrowError("FitPlane: Resizer must be either \"Point\", \"Bilinear\", \"Bicubic\" or \"Lanczos\" (%s is invalid)",res);

	if (mode<4)
	{
		if (lstrcmpi(ResizerString, "ReduceBy2") == 0)
		{
			AVSValue args[1] = { child };
			Resized = env->Invoke(ResizerString,AVSValue(args,1)).AsClip();
		}
		else
		{
			AVSValue args[3] = { child,vi. width/2, vi.height/2 }; 
			Resized = env->Invoke(ResizerString,AVSValue(args,3)).AsClip();
		}
	}
	else
	{
		AVSValue args[3] = { child,vi. width*2, vi.height*2 }; 
		Resized = env->Invoke(ResizerString,AVSValue(args,3)).AsClip();
	}
}

PVideoFrame __stdcall FitPlane::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame			src = child->GetFrame(n, env);
	env->MakeWritable(&src);
	unsigned char		*srcp_y = src->GetWritePtr(PLANAR_Y);
	unsigned char		*srcp_u = src->GetWritePtr(PLANAR_U);
	unsigned char		*srcp_v = src->GetWritePtr(PLANAR_V);
	const int			src_pitch = src->GetPitch(PLANAR_Y);
	const int			src_pitch_uv = src->GetPitch(PLANAR_U);

	const int			width = src->GetRowSize(PLANAR_Y);
	const int			height = src->GetHeight(PLANAR_Y);

	PVideoFrame			rszd = Resized->GetFrame(n, env);
	const unsigned char	*rszdp_y = rszd->GetReadPtr(PLANAR_Y);
	const unsigned char	*rszdp_u = rszd->GetReadPtr(PLANAR_U);
	const unsigned char	*rszdp_v = rszd->GetReadPtr(PLANAR_V);
	const int			rszd_pitch = rszd->GetPitch(PLANAR_Y);
	const int			rszd_pitch_uv = rszd->GetPitch(PLANAR_U);

	if (mode & FP_Y2U)
		env->BitBlt(srcp_u, src_pitch_uv, rszdp_y, rszd_pitch, width>>1, height>>1);
	if (mode & FP_Y2V)
		env->BitBlt(srcp_v, src_pitch_uv, rszdp_y, rszd_pitch, width>>1, height>>1);
	if (mode == FP_U2Y)
		env->BitBlt(srcp_y, src_pitch, rszdp_u, rszd_pitch_uv, width, height);
	if (mode == FP_V2Y)
		env->BitBlt(srcp_y, src_pitch, rszdp_v, rszd_pitch_uv, width, height);

	return src;
}
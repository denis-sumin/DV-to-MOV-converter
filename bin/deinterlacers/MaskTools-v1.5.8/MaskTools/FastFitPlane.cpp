// FastFitPlane - fits a plane into another one of different dimensions
// using own resizers
// FastFitPlane (C) 2003 Kurosu (kurosu@inforezo.org)

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


#include "FastFitPlane.h"
#include "FFP_func.h"

FastFitPlane::FastFitPlane(PClip _child, int _mode, IScriptEnvironment* env) :
GenericVideoFilter(_child), mode(_mode)
{
	if (!vi.IsYV12())
		env->ThrowError("FastFitPlane: needs YV12");
	if (vi.width%4)
		env->ThrowError("FastFitPlane: needs at least MOD4 resolution");

	//SetCacheHints(0,0);
	
	if ((mode & FFP_Y2U) || (mode & FFP_Y2V))
	{
		if (env->GetCPUFlags() & CPUF_INTEGER_SSE)
			ResizePlane = FastReduceBy2_iSSE;
		else if (env->GetCPUFlags() & CPUF_MMX)
			ResizePlane = FastReduceBy2_MMX;
		else ResizePlane = FastReduceBy2_C;
	}
	else if ((mode == FFP_U2Y) || (mode == FFP_V2Y))
	{
		if ((env->GetCPUFlags() & CPUF_MMX) && !(vi.width%8))
			ResizePlane = FastIncreaseBy2_C;
		else ResizePlane = FastIncreaseBy2_C;
	}
	else env->ThrowError("FastFitPlane: unexpected fitting mode %i",_mode);
}

PVideoFrame __stdcall FastFitPlane::GetFrame(int n, IScriptEnvironment* env)
{
	PVideoFrame			src = child->GetFrame(n, env);
	env->MakeWritable(&src);
	work = env->NewVideoFrame(vi);

	if (mode == FFP_V2U)
	{
		env->BitBlt(src->GetWritePtr(PLANAR_V), src->GetPitch(PLANAR_U),
					src->GetWritePtr(PLANAR_V), src->GetPitch(PLANAR_U),
					src->GetRowSize(PLANAR_U), src->GetRowSize(PLANAR_U));
		return src;
	}
	if (mode == FFP_V2U)
	{
		env->BitBlt(src->GetWritePtr(PLANAR_U), src->GetPitch(PLANAR_V),
					src->GetWritePtr(PLANAR_U), src->GetPitch(PLANAR_V),
					src->GetRowSize(PLANAR_U), src->GetRowSize(PLANAR_U));
		return src;
	}

	if (mode&FFP_Y2U || mode&FFP_Y2V)
	{
		const int			width = src->GetRowSize(PLANAR_Y);
		const int			height = src->GetHeight(PLANAR_Y);

		if (mode & FFP_Y2U)
		{
			unsigned char		*srcp_u = src->GetWritePtr(PLANAR_U);
			const int			src_pitch_uv = src->GetPitch(PLANAR_U);
			
			
			ResizePlane(srcp_u, src->GetReadPtr(PLANAR_Y), work->GetWritePtr(PLANAR_Y),
						src_pitch_uv, src->GetPitch(PLANAR_Y), work->GetPitch(PLANAR_Y),
						width, height);
			
			if (mode & FFP_Y2V)
				env->BitBlt(src->GetWritePtr(PLANAR_V), src_pitch_uv,
							srcp_u, src_pitch_uv, width>>1, height>>1);
		}
		else
		{
			ResizePlane(src->GetWritePtr(PLANAR_V), src->GetReadPtr(PLANAR_Y),
						work->GetWritePtr(PLANAR_Y), src->GetPitch(PLANAR_U),
						src->GetPitch(PLANAR_Y), work->GetPitch(PLANAR_Y),
						width, height);
		}
		return src;
	}
	else
	{
		const int			width = src->GetRowSize(PLANAR_U);
		const int			height = src->GetHeight(PLANAR_U);
		const int			src_pitch_uv = src->GetPitch(PLANAR_U);

		if (mode == FFP_U2Y)
			ResizePlane(src->GetWritePtr(PLANAR_Y), src->GetReadPtr(PLANAR_U),
						work->GetWritePtr(PLANAR_Y), src->GetPitch(PLANAR_Y),
						src_pitch_uv, work->GetPitch(PLANAR_Y), width, height);
		else
			ResizePlane(src->GetWritePtr(PLANAR_Y), src->GetReadPtr(PLANAR_V),
						work->GetWritePtr(PLANAR_Y), src->GetPitch(PLANAR_Y),
						src_pitch_uv, work->GetPitch(PLANAR_Y), width, height);
		return src;
	}
	
	return src;
}


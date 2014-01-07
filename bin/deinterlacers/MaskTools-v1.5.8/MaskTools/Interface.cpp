// MaskTools - various mask management tools for Avisynth
// MaskTools (C) 2003 Kurosu (kurosu@inforezo.org)

// MaskedMerge, Invert, EdgeMask, Binarize, Expand, Logic, YV12Layer,
// YV12Subtract, MaskOverlay, FitPlane and FastFitPlane are 
// (C) 2003 Kurosu (kurosu@inforezo.org)

// Masktools: http://kurosu.inforezo.org/avs/MaskTools/index.html
// Avisynth: www.avisynth.org

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

#define WIN32_LEAN_AND_MEAN

#include "AviSynth.h"

#include "EdgeMask.h"
#include "DEdgeMask.h"
#include "DEdgeMask2.h"
#include "MaskedMerge.h"
#include "MotionMask.h"
#include "CombMask.h"
#include "YV12Convolution.h"
#include "HysteresyMask.h"

#include "Binarize.h"
#include "LocalBinarize.h"
#include "LUT.h"
#include "RGBLUT.h"
#include "YUY2LUT.h"
#include "LUTxy.h"

#include "FitPlane.h"
#include "FastFitPlane.h"
#include "Expand.h"

#include "Invert.h"
#include "Logic.h"

#include "YV12Layer.h"
#include "YV12Substract.h"

#include "Overlay.h"

AVSValue __cdecl Create_EdgeMask(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int		thY1 = 0, thY2 = 20;
	int		thC1 = 0, thC2 = 20;
	char	detector[] = "sobel";
	int 	Y = 3, U = 1, V = 1;
   int     offx = 0, offy = 0, w = -1, h = -1;
   bool    mmx = false, isse = true;

	return new EdgeMask(
			args[0].AsClip(),
			args[1].AsInt(thY1),
			args[2].AsInt(thY2),
			args[3].AsInt(thC1),
			args[4].AsInt(thC2),
			args[5].AsString(detector),
            args[6].AsInt(offx),
            args[7].AsInt(offy),
            args[8].AsInt(w),
            args[9].AsInt(h),
			args[10].AsInt(Y),
			args[11].AsInt(U),
			args[12].AsInt(V),
			args[13].AsBool(mmx),
            args[14].AsBool(isse),
			env);
}

AVSValue __cdecl Create_DEdgeMask(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int		thY1 = 0, thY2 = 20;
	int		thC1 = 0, thC2 = 20;
	char	detector[] = "-1 -1 -1 -1 8 -1 -1 -1 -1";
	int 	Y = 3, U = 1, V = 1;
    int     offx = 0, offy = 0, w = -1, h = -1;
    bool    mmx = false, isse = true;
	bool	setDivisor = args[7].AsBool(true) && args[6].Defined();
	int		div = args[6].AsInt(8);
	double	fdiv = args[6].AsFloat(8);
	bool	isf = args[6].IsFloat() && !args[6].IsInt();

	return new DEdgeMask(
			args[0].AsClip(),
			args[1].AsInt(thY1),
			args[2].AsInt(thY2),
			args[3].AsInt(thC1),
			args[4].AsInt(thC2),
			args[5].AsString(detector),
			setDivisor,
			div,
			fdiv,
			isf,
            args[8].AsBool(false), 
            args[9].AsInt(offx),
            args[10].AsInt(offy),
            args[11].AsInt(w),
            args[12].AsInt(h),
			args[13].AsInt(Y),
			args[14].AsInt(U),
			args[15].AsInt(V),
			args[16].AsBool(mmx),
            args[17].AsBool(isse),
			env);
}

AVSValue __cdecl Create_DEdgeMask2(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	char	   detector[] = "-1 -1 -1 -1 8 -1 -1 -1 -1";
	int 	   Y = 3, U = 1, V = 1;
   int      offx = 0, offy = 0, w = -1, h = -1;
   bool     mmx = false, isse = true;
	bool	   setDivisor = args[5].AsBool(true) && args[4].Defined();
	int		div = args[4].AsInt(8);

	return new DEdgeMask2(
			args[0].AsClip(),
         args[1].AsClip(),
         args[2].AsClip(),
			args[3].AsString(detector),
			setDivisor,
			div,
         args[6].AsInt(offx),
         args[7].AsInt(offy),
         args[8].AsInt(w),
         args[9].AsInt(h),
			args[10].AsInt(Y),
			args[11].AsInt(U),
			args[12].AsInt(V),
			args[13].AsBool(mmx),
         args[14].AsBool(isse),
			env);
}


AVSValue __cdecl Create_MotionMask(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int		thY1 = 20, thY2 = 20;
	int		thC1 = 10, thC2 = 10;
    int     thSCD = 10;
	int 	Y = 3, U = 1, V = 1;
    int     offx = 0, offy = 0, w = -1, h = -1;
    bool    mmx = false, isse = true;

    return new MotionMask(
			args[0].AsClip(),
			args[1].AsInt(thY1),
			args[2].AsInt(thY2),
			args[3].AsInt(thC1),
			args[4].AsInt(thC2),
			args[5].AsInt(thSCD),
            args[6].AsInt(offx),
            args[7].AsInt(offy),
            args[8].AsInt(w),
            args[9].AsInt(h),
			args[10].AsInt(Y),
			args[11].AsInt(U),
			args[12].AsInt(V),
			args[13].AsBool(mmx),
            args[14].AsBool(isse),
			env);
}

AVSValue __cdecl Create_CombMask(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int		thY1 = 30, thY2 = 30;
	int 	Y = 3, U = 1, V = 1;
	bool	usemmx = true;

	return new CombMask(
			args[0].AsClip(),
			args[1].AsInt(thY1),
			args[2].AsInt(thY2),
			args[3].AsInt(Y),
			args[4].AsInt(U),
			args[5].AsInt(V),
			args[6].AsBool(usemmx),
			env);
}

AVSValue __cdecl Create_YV12Convolution(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int 	Y = 3, U = 3, V = 3;
    int     offx = 0, offy = 0, w = -1, h = -1;
	bool	automatic = !args[3].Defined();
	int		total = args[3].AsInt(8);
	bool	mmx = true, isse = true;
	bool	saturate = true;
	char	horizontal[6] = { '1', ' ', '1', ' ', '1', 0 };
	char	vertical[6] = { '1', ' ', '1', ' ', '1', 0 };

	return new YV12Convolution(
			args[0].AsClip(),
			args[1].AsString(horizontal),
			args[2].AsString(vertical),
			args[3].AsInt(total),
			automatic,
			args[4].AsBool(saturate),
            args[5].AsInt(offx),
            args[6].AsInt(offy),
            args[7].AsInt(w),
            args[8].AsInt(h),
			args[9].AsInt(Y),
			args[10].AsInt(U),
			args[11].AsInt(V),
			args[12].AsBool(mmx),
            args[13].AsBool(isse), 
			env);
}


AVSValue __cdecl Create_Binarize(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int		threshold = 20;
	bool	upper = true, mmx = true, isse = true;
	int		offx = 0, offy = 0;
	int 	Y = 3, U = 1, V = 1;
	int		w = -1, h = -1;

	return new Binarize(
			args[0].AsClip(),
			args[1].AsInt(threshold),
			args[2].AsBool(upper),
			args[3].AsInt(offx),
			args[4].AsInt(offy),
			args[5].AsInt(w),
			args[6].AsInt(h),
			args[7].AsInt(Y),
			args[8].AsInt(U),
			args[9].AsInt(V),
			args[10].AsBool(mmx),
            args[11].AsBool(isse),
			env);
}

AVSValue __cdecl Create_LocalBinarize(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int		th1 = 0, th2 = 255, rad = 3;
    int     th3 = 5;
    int     bth1 = 0, bth2 = 255;
    float   weight = 1.0;
    bool    upper = false;
	bool	mmx = true, isse = true;
	int		offx = 0, offy = 0;
	int 	Y = 3, U = 1, V = 1;
	int		w = -1, h = -1;

	return new LocalBinarize(
			args[0].AsClip(),
			args[1].AsInt(th1),
            args[2].AsInt(th2),
			args[3].AsInt(rad),
            args[4].AsInt(th3),
            args[5].AsInt(bth1),
            args[6].AsInt(bth2),
            args[7].AsFloat(weight),
            args[8].AsBool(upper),
			args[9].AsInt(offx),
			args[10].AsInt(offy),
			args[11].AsInt(w),
			args[12].AsInt(h),
			args[13].AsInt(Y),
			args[14].AsInt(U),
			args[15].AsInt(V),
			args[16].AsBool(mmx),
            args[17].AsBool(isse),
			env);
}

AVSValue __cdecl Create_LUT(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int		Y = 3, U = 1, V = 1;
	int		offx = 0, offy = 0;
	int		w = -1, h = -1;
	char	Yexpr[] = "x";
	char	Uexpr[] = "x";
	char	Vexpr[] = "x";
    bool    mmx = true, isse = true;

	return new LUT(
			args[0].AsClip(),
			args[1].AsString(Yexpr),
			args[2].AsString(Uexpr),
			args[3].AsString(Vexpr),
			args[4].AsInt(offx),
			args[5].AsInt(offy),
			args[6].AsInt(w),
			args[7].AsInt(h),
			args[8].AsInt(Y),
			args[9].AsInt(U),
			args[10].AsInt(V),
            args[11].AsBool(mmx),
            args[12].AsBool(isse),
			env);
}

AVSValue __cdecl Create_LUTxy(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int		Y = 3, U = 1, V = 1;
	int		offx = 0, offy = 0;
	int		w = -1, h = -1;
	char	Yexpr[] = "x";
	char	Uexpr[] = "x";
	char	Vexpr[] = "x";
    bool    mmx = true, isse = true;

	return new LUTxy(
			args[0].AsClip(),
            args[1].AsClip(),
			args[2].AsString(Yexpr),
			args[3].AsString(Uexpr),
			args[4].AsString(Vexpr),
			args[5].AsInt(offx),
			args[6].AsInt(offy),
			args[7].AsInt(w),
			args[8].AsInt(h),
			args[9].AsInt(Y),
			args[10].AsInt(U),
			args[11].AsInt(V),
            args[12].AsBool(mmx),
            args[13].AsBool(isse),
			env);
}

AVSValue __cdecl Create_RGBLUT(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int		R = 3, G = 3, B = 3;
	int		offsetX = 0, offsetY = 0;
	int		w=-1, h=-1;
	char	Rexpr[] = "x";
	char	Gexpr[] = "x";
	char	Bexpr[] = "x";
	char	AMPfile[] = "color.amp";

	return new RGBLUT(
			args[0].AsClip(),
			args[1].AsString(Rexpr),
			args[2].AsString(Gexpr),
			args[3].AsString(Bexpr),
			args[4].AsInt(offsetX),
			args[5].AsInt(offsetY),
			args[6].AsInt(w),
			args[7].AsInt(h),
			args[8].AsInt(R),
			args[9].AsInt(G),
			args[10].AsInt(B),
			args[11].AsString(AMPfile),
			env);
}

AVSValue __cdecl Create_YUY2LUT(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int		Y = 3, U = 3, V = 3;
	int		offsetX = 0, offsetY = 0;
	int		w=-1, h=-1;
	char	Yexpr[] = "x";
	char	Uexpr[] = "x";
	char	Vexpr[] = "x";

	return new YUY2LUT(
			args[0].AsClip(),
			args[1].AsString(Yexpr),
			args[2].AsString(Uexpr),
			args[3].AsString(Vexpr),
			args[4].AsInt(offsetX),
			args[5].AsInt(offsetY),
			args[6].AsInt(w),
			args[7].AsInt(h),
			args[8].AsInt(Y),
			args[9].AsInt(U),
			args[10].AsInt(V),
			env);
}


AVSValue __cdecl Create_MaskedMerge(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int 	Y = 3, U = 1, V = 1;
	int		offx = 0, offy = 0;
	int		w = -1, h =- 1;
	bool	mmx = true, isse = true;

	return new MaskedMerge(
			args[0].AsClip(),
			args[1].AsClip(),
			args[2].AsClip(),
			args[3].AsInt(offx),
			args[4].AsInt(offy),
			args[5].AsInt(w),
			args[6].AsInt(h),
			args[7].AsInt(Y),
			args[8].AsInt(U),
			args[9].AsInt(V),
			args[10].AsBool(mmx),
            args[11].AsBool(isse),
			env);
}

AVSValue __cdecl Create_Inv(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int 	Y = 3, U = 1, V = 1;
	int		offsetX = 0, offsetY = 0;
	int		w = -1, h = -1;

	return new Invert(
			args[0].AsClip(),
			args[1].AsInt(offsetX),
			args[2].AsInt(offsetY),
			args[3].AsInt(w),
			args[4].AsInt(h),
			args[5].AsInt(Y),
			args[6].AsInt(U),
			args[7].AsInt(V),
			env);
}

AVSValue __cdecl Create_Expand(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int 	Y = 3, U = 1, V = 1;
	int		offx = 0, offy = 0;
	int		w = -1, h = -1;
    bool    mmx = true, isse = true;

	return new Expand(
			args[0].AsClip(),
			MODE_EXPAND,
			args[1].AsInt(offx),
			args[2].AsInt(offy),
			args[3].AsInt(w),
			args[4].AsInt(h),
			args[5].AsInt(Y),
			args[6].AsInt(U),
			args[7].AsInt(V),
            args[8].AsBool(mmx),
            args[9].AsBool(isse),
			env);
}

AVSValue __cdecl Create_Inpand(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int 	Y = 3, U = 1, V = 1;
	int		offx = 0, offy = 0;
	int		w = -1, h = -1;
    bool    mmx = true, isse = true;

	return new Expand(
			args[0].AsClip(),
			MODE_INPAND,
			args[1].AsInt(offx),
			args[2].AsInt(offy),
			args[3].AsInt(w),
			args[4].AsInt(h),
			args[5].AsInt(Y),
			args[6].AsInt(U),
			args[7].AsInt(V),
            args[8].AsBool(mmx),
            args[9].AsBool(isse),
			env);
}

AVSValue __cdecl Create_Inflate(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int 	Y = 3, U = 1, V = 1;
	int		offx = 0, offy = 0;
	int		w = -1, h = -1;
    bool    mmx = true, isse = true;

	return new Expand(
			args[0].AsClip(),
			MODE_INFLATE,
			args[1].AsInt(offx),
			args[2].AsInt(offy),
			args[3].AsInt(w),
			args[4].AsInt(h),
			args[5].AsInt(Y),
			args[6].AsInt(U),
			args[7].AsInt(V),
            args[8].AsBool(mmx),
            args[9].AsBool(isse),
			env);
}

AVSValue __cdecl Create_Deflate(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int 	Y = 3, U = 1, V = 1;
	int		offx = 0, offy = 0;
	int		w = -1, h = -1;
    bool    mmx = true, isse = true;

	return new Expand(
			args[0].AsClip(),
			MODE_DEFLATE,
			args[1].AsInt(offx),
			args[2].AsInt(offy),
			args[3].AsInt(w),
			args[4].AsInt(h),
			args[5].AsInt(Y),
			args[6].AsInt(U),
			args[7].AsInt(V),
            args[8].AsBool(mmx),
            args[9].AsBool(isse),
			env);
}

AVSValue __cdecl Create_Logic(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int 	Y = 3, U = 1, V = 1;
	char    mode[] = "and";
	int		offx = 0, offy = 0;
	int		w = -1, h = -1;
    bool    mmx = true, isse = true;


	return new Logic(
			args[0].AsClip(),
			args[1].AsClip(),
			args[2].AsString(mode),
			args[3].AsInt(offx),
			args[4].AsInt(offy),
			args[5].AsInt(w),
			args[6].AsInt(h),
			args[7].AsInt(Y),
			args[8].AsInt(U),
			args[9].AsInt(V),
            args[10].AsBool(mmx),
            args[11].AsBool(isse),
			env);
}

AVSValue __cdecl Create_YV12Layer(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	char    operation[4] = {'n', 'o', 'n', 'e'};
	int		level = 255;
	int		offsetx = 0, offsety = 0;
	int		thY = 0, thU = 0;
	int		Y=3, U=1, V=1;
	int		w=-1, h=-1;
	bool	use_chroma = true;

	return new YV12Layer(
			args[0].AsClip(),
			args[1].AsClip(),
			args[2].AsString(operation),
			args[3].AsInt(level),
			args[4].AsBool(use_chroma),
			args[5].AsInt(offsetx),
			args[6].AsInt(offsety),
			args[7].AsInt(w),
			args[8].AsInt(h),
			args[9].AsInt(Y),
			args[10].AsInt(U),
			args[11].AsInt(V),
			env);
}

AVSValue __cdecl Create_YV12Substract(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int 	Y = 3, U = 2, V = 2;
	int		offx = 0, offy = 0;
	int		w = -1, h = -1;
    bool    mmx = true, isse = true;
	int		tolerance = -1;
	bool    wideRange = false;

	return new YV12Substract(
			args[0].AsClip(),
			args[1].AsClip(),
			args[2].AsFloat(tolerance),
            args[3].AsBool(wideRange),
			args[4].AsInt(offx),
			args[5].AsInt(offy),
			args[6].AsInt(w),
			args[7].AsInt(h),
			args[8].AsInt(Y),
			args[9].AsInt(U),
			args[10].AsInt(V),
			args[11].AsBool(mmx),
            args[12].AsBool(isse),
			env);
}

AVSValue __cdecl Create_HysteresyMask(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int 	Y = 3, U = 1, V = 1;
	int		offx = 0, offy = 0;
	int		w = -1, h = -1;
    bool    mmx = true, isse = true;
	
	return new HysteresyMask(
			args[0].AsClip(),
			args[1].AsClip(),
            args[2].AsInt(offx),
            args[3].AsInt(offy),
            args[4].AsInt(w),
            args[5].AsInt(h),
			args[6].AsInt(Y),
			args[7].AsInt(U),
			args[8].AsInt(V),
            args[9].AsBool(mmx),
            args[10].AsBool(isse),
			env);
}
	
AVSValue __cdecl Create_Overlay(AVSValue args, void* user_data, IScriptEnvironment* env) 
{
	int		thY=10, thC=10, strictness=3;

	return new Overlay(
			args[0].AsClip(),
			args[1].AsClip(),
			args[2].AsInt(thY),
			args[3].AsInt(thC),
			args[4].AsInt(strictness),
			env);
}

AVSValue __cdecl Create_Y2U(AVSValue args, void* user_data, IScriptEnvironment* env)
{
	return new FitPlane(
			args[0].AsClip(),
			FP_Y2U,
			args[1].AsString("reduce"),
			env);
}

AVSValue __cdecl Create_Y2V(AVSValue args, void* user_data, IScriptEnvironment* env)
{
	return new FitPlane(
			args[0].AsClip(),
			FP_Y2V,
			args[1].AsString("reduce"),
			env);
}

AVSValue __cdecl Create_Y2UV(AVSValue args, void* user_data, IScriptEnvironment* env)
{
	return new FitPlane(
			args[0].AsClip(),
			FP_Y2V + FP_Y2U,
			args[1].AsString("reduce"),
			env);
}

AVSValue __cdecl Create_U2Y(AVSValue args, void* user_data, IScriptEnvironment* env)
{
	return new FitPlane(
			args[0].AsClip(),
			FP_U2Y,
			args[1].AsString("bilinear"),
			env);
}

AVSValue __cdecl Create_V2Y(AVSValue args, void* user_data, IScriptEnvironment* env)
{
	return new FitPlane(
			args[0].AsClip(),
			FP_V2Y,
			args[1].AsString("bilinear"),
			env);
}

AVSValue __cdecl Create_FY2U(AVSValue args, void* user_data, IScriptEnvironment* env)
{
	return new FastFitPlane(
			args[0].AsClip(),
			FFP_Y2U,
			env);
}

AVSValue __cdecl Create_FY2V(AVSValue args, void* user_data, IScriptEnvironment* env)
{
	return new FastFitPlane(
			args[0].AsClip(),
			FFP_Y2V,
			env);
}

AVSValue __cdecl Create_FY2UV(AVSValue args, void* user_data, IScriptEnvironment* env)
{
	return new FastFitPlane(
			args[0].AsClip(),
			FFP_Y2V | FFP_Y2U,
			env);
}

AVSValue __cdecl Create_FU2Y(AVSValue args, void* user_data, IScriptEnvironment* env)
{
	return new FastFitPlane(
			args[0].AsClip(),
			FFP_U2Y,
			env);
}

AVSValue __cdecl Create_FV2Y(AVSValue args, void* user_data, IScriptEnvironment* env)
{
	return new FastFitPlane(
			args[0].AsClip(),
			FFP_V2Y,
			env);
}

AVSValue __cdecl Create_FV2U(AVSValue args, void* user_data, IScriptEnvironment* env)
{
	return new FastFitPlane(
			args[0].AsClip(),
			FFP_V2Y,
			env);
}

AVSValue __cdecl Create_FU2V(AVSValue args, void* user_data, IScriptEnvironment* env)
{
	return new FastFitPlane(
			args[0].AsClip(),
			FFP_U2V,
			env);
}

extern "C" __declspec(dllexport) const char* __stdcall
AvisynthPluginInit2(IScriptEnvironment* env) {

    // PIV hyperthreading bug prevention
    try {

	//Thresholding etc
	env->AddFunction("EdgeMask", "c[thY1]i[thY2]i[thC1]i[thC2]i[type]s[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mmx]b[isse]b",
					Create_EdgeMask, 0);
	env->AddFunction("DEdgeMask", "c[thY1]i[thY2]i[thC1]i[thC2]i[matrix]s[divisor]f[setdivisor]b[vmode]b[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mmx]b[isse]b",
					Create_DEdgeMask, 0);
	env->AddFunction("DEdgeMask2", "ccc[matrix]s[divisor]f[setdivisor]b[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mmx]b[isse]b",
					Create_DEdgeMask, 0);
	env->AddFunction("MotionMask", "c[thY1]i[thY2]i[thC1]i[thC2]i[thSD]i[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mmx]b[isse]b",
					Create_MotionMask, 0);
	env->AddFunction("CombMask", "c[thY1]i[thY2]i[Y]i[U]i[V]i[usemmx]b",
					Create_CombMask, 0);

	env->AddFunction("MaskedMerge", "ccc[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mmx]b[isse]b",
					Create_MaskedMerge, 0);

    //Mask processors
	env->AddFunction("Logic", "cc[mode]s[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mmx]b[isse]b", Create_Logic, 0);
	env->AddFunction("YV12Subtract", "cc[tol]f[widerange]b[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mmx]b[isse]b",
					Create_YV12Substract, 0);
	env->AddFunction("HysteresyMask", "cc[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mxx]b[isse]b",
					Create_HysteresyMask, 0);



    env->AddFunction("YV12Convolution", "c[horizontal]s[vertical]s[total]i[saturate]b[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mmx]b[isse]b",
					Create_YV12Convolution, 0);

	env->AddFunction("Binarize", "c[threshold]i[upper]b[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mmx]b[isse]b",
					Create_Binarize, 0);
	env->AddFunction("LocalBinarize", "c[th1]i[th2]i[radius]i[th3]i[bth1]i[bth2]i[weight]f[upper]b[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mmx]b[isse]b",
					Create_LocalBinarize, 0);

	//Level modifiers
	env->AddFunction("YV12LUT", "c[Yexpr]s[Uexpr]s[Vexpr]s[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mxx]b[isse]b",
					Create_LUT, 0);
	env->AddFunction("YUY2LUT", "c[Yexpr]s[Uexpr]s[Vexpr]s[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i",
					Create_YUY2LUT, 0);
	env->AddFunction("YV12LUTxy", "cc[Yexpr]s[Uexpr]s[Vexpr]s[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mxx]b[isse]b",
					Create_LUTxy, 0);

	env->AddFunction("RGBLUT", "c[Rexpr]s[Gexpr]s[Bexpr]s[offX]i[offY]i[w]i[h]i[R]i[G]i[B]i[AMPFile]s",
					Create_RGBLUT, 0);
	
	//Area modifiers
//	env->AddFunction("Invert", "c[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i", Create_Inv, 0);
	env->AddFunction("Inpand", "c[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mmx]b[isse]b", Create_Inpand, 0);
	env->AddFunction("Expand", "c[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mmx]b[isse]b", Create_Expand, 0);
	env->AddFunction("Inflate", "c[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mmx]b[isse]b", Create_Inflate, 0);
	env->AddFunction("Deflate", "c[offX]i[offY]i[w]i[h]i[Y]i[U]i[V]i[mmx]b[isse]b", Create_Deflate, 0);

	//Mask adapters - most probably correct
	env->AddFunction("FitY2U", "c[resizer]s", Create_Y2U, 0);
	env->AddFunction("FitY2V", "c[resizer]s", Create_Y2V, 0);
	env->AddFunction("FitY2UV", "c[resizer]s", Create_Y2UV, 0);
	env->AddFunction("FitU2Y", "c[resizer]s", Create_U2Y, 0);
	env->AddFunction("FitV2Y", "c[resizer]s", Create_V2Y, 0);
	//Mask adapters - fast mode
	env->AddFunction("FastFitY2U", "c", Create_FY2U, 0);
	env->AddFunction("FastFitY2V", "c", Create_FY2V, 0);
	env->AddFunction("FastFitY2UV", "c", Create_FY2UV, 0);
	env->AddFunction("FastFitU2Y", "c", Create_FU2Y, 0);
	env->AddFunction("FastFitV2Y", "c", Create_FV2Y, 0);
	env->AddFunction("FastFitU2V", "c", Create_FU2V, 0);
	env->AddFunction("FastFitV2U", "c", Create_FV2U, 0);

    }
    catch (...) {
        throw;
    }
	return("MaskTools: a set of tools to work with masks");
}

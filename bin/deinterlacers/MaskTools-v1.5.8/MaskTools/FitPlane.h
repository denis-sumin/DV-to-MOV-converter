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

#ifndef __FIT__
#define __FIT__

#include "avisynth.h"

#define		FP_Y2U		1
#define		FP_Y2V		2
#define		FP_U2Y		4
#define		FP_V2Y		8

class FitPlane : public GenericVideoFilter {
private:
	PClip			Resized;
	unsigned char	mode;
public:
	FitPlane(PClip _child, int _mode, const char *resizer_, IScriptEnvironment* env);
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
};

#endif
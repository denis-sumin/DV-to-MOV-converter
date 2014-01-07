// General Convolution : allow to make a convolution in YV12 colorspace
// General Convolution (C) 2004 Manao (manao@melix.net)

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

#ifndef __GC_FUNC__
#define __GC_FUNC__

#include <math.h>

#define MTFLT double
#define MTINT int

inline unsigned char FitIntoUC(double x)
{
	if (x < 0.5) return 0;
	else if (x > 254.5) return 255;
	else return x + 0.5;
}

inline unsigned char FitIntoUCABS(double x)
{
	if (fabs(x) > 254.5) return 255;
	else return fabs(x) + 0.5;
}

inline unsigned char FitIntoUC(int x)
{
	return ( x < 0 ) ? 0 : ( x > 255 ) ? 255 : x;
}

inline unsigned char FitIntoUCABS(int x)
{
	return ( abs(x) > 255 ) ? 255 : abs(x);
}

#define MAKE_GC_H(TYPE,ABSFUN) \
void GC_ ## TYPE ## ABSFUN ## _C (unsigned char *derp, const unsigned char *srcp, const int der_pitch, \
				                  const int src_pitch, const int row_size, \
				                  const int height, const TYPE *horvect, const TYPE *vervect, \
				                  const int horres, const int verres, const TYPE total);

MAKE_GC_H(MTINT, NORM)
MAKE_GC_H(MTINT, ABS)
MAKE_GC_H(MTINT, SAT)
MAKE_GC_H(MTFLT, NORM)
MAKE_GC_H(MTFLT, ABS)
MAKE_GC_H(MTFLT, SAT)
/*
void GCf_C (const unsigned char *srcp, unsigned char *derp, const int src_pitch,
				const int der_pitch, const int row_size, 
				const int height, const int *horvect, const int *vervect,
				const double *horvectf, const double *vervectf,
				const int horres, const int verres, const int total, const double totalf);
void GC_C_ABS (const unsigned char *srcp, unsigned char *derp, const int src_pitch,
				const int der_pitch, const int row_size, 
				const int height, const int *horvect, const int *vervect,
				const double *horvectf, const double *vervectf,
				const int horres, const int verres, const int total, const double totalf);
void GCf_C_ABS (const unsigned char *srcp, unsigned char *derp, const int src_pitch,
				const int der_pitch, const int row_size, 
				const int height, const int *horvect, const int *vervect,
				const double *horvectf, const double *vervectf,
				const int horres, const int verres, const int total, const double totalf);
void GC_C_SAT (const unsigned char *srcp, unsigned char *derp, const int src_pitch,
				const int der_pitch, const int row_size, 
				const int height, const int *horvect, const int *vervect,
				const double *horvectf, const double *vervectf,
				const int horres, const int verres, const int total, const double totalf);
void GCf_C_SAT (const unsigned char *srcp, unsigned char *derp, const int src_pitch,
				const int der_pitch, const int row_size, 
				const int height, const int *horvect, const int *vervect,
				const double *horvectf, const double *vervectf,
				const int horres, const int verres, const int total, const double totalf);
*/
#endif
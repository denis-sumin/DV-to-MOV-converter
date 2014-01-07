// YV12Subtract - expansion of AviSynth's Subtract
// YV12Subtract (C) 2003 Kurosu (kurosu@inforezo.org)
// Subtract (C) AviSynth's team

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

#ifndef __YV12S_FUNC__
#define __YV12S_FUNC__

void Substract_C2(unsigned char *pDst, const unsigned char *pSrc1,
                  const unsigned char *pSrc2, int dstPitch, int src1Pitch,
                  int src2Pitch, int width, int height, unsigned char tol,
                  unsigned char *LUT);
void Substract_C1(unsigned char *pDst, const unsigned char *pSrc1,
                  const unsigned char *pSrc2, int dstPitch, int src1Pitch,
                  int src2Pitch, int width, int height, unsigned char tol,
                  unsigned char *LUT);
void Substract_MMX(unsigned char *pDst, const unsigned char *pSrc1,
                   const unsigned char *pSrc2, int dstPitch, int src1Pitch,
                   int src2Pitch, int width, int height, unsigned char tol,
                   unsigned char *LUT);
void Diff_C(unsigned char *pDst, const unsigned char *pSrc1,
            const unsigned char *pSrc2, int dstPitch, int src1Pitch,
            int src2Pitch, int width, int height, unsigned char tol,
            unsigned char *LUT);
void Diff_MMX(unsigned char *pDst, const unsigned char *pSrc1,
              const unsigned char *pSrc2, int dstPitch, int src1Pitch,
              int src2Pitch, int width, int height, unsigned char tol,
              unsigned char *LUT);
void SubstractW_C2(unsigned char *pDst, const unsigned char *pSrc1,
                   const unsigned char *pSrc2, int dstPitch, int src1Pitch,
                   int src2Pitch, int width, int height, unsigned char tol,
                   unsigned char *LUT);
void SubstractW_C1(unsigned char *pDst, const unsigned char *pSrc1,
                   const unsigned char *pSrc2, int dstPitch, int src1Pitch,
                   int src2Pitch, int width, int height, unsigned char tol,
                   unsigned char *LUT);
void SubstractW_MMX(unsigned char *pDst, const unsigned char *pSrc1,
                    const unsigned char *pSrc2, int dstPitch, int src1Pitch,
                    int src2Pitch, int width, int height, unsigned char tol,
                    unsigned char *LUT);

#endif
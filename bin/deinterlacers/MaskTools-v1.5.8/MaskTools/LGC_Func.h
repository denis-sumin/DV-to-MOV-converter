// Logic - very basic mask logic manipulations
// Logic (C) 2003 Kurosu (kurosu@inforezo.org)

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

#ifndef __LGC_FUNC__
#define __LGC_FUNC__

void And_C4(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void And_C1(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void And_MMX(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
             int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void Or_C4(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
           int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void Or_C1(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
           int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void Or_MMX(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void Xor_C4(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void Xor_C1(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
            int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void Xor_MMX(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
             int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void Andn_C4(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
             int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void Andn_C1(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
             int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void Andn_MMX(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
             int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void Min_MMX(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
             int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void Min_C(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
             int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void Min_iSSE(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
              int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void Max_MMX(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
             int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void Max_C(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
           int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
void Max_iSSE(unsigned char *pDst, const unsigned char *pSrc1, const unsigned char *pSrc2,
             int dstPitch, int src1Pitch, int src2Pitch, int width, int height);
#endif __LGC_FUNC__


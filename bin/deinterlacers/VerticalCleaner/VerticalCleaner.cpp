#define LOGO		"VerticalCleaner 0.1\n"
// An Avisynth plugin for 
//
// By Rainer Wittmann <gorw@gmx.de>
//
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
// To get a copy of the GNU General Public License write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA, or visit
// http://www.gnu.org/copyleft/gpl.html .

//#define	SSE2_TEST		// ISSE2 version that can be used side by side with the SSE version
//#define	DEBUG_NAME		// for debugging
#define	ISSE	1		// P3, Athlon 32
//#define	ISSE	2		// P4, Athlon 64, Sempron 3100
//#define NOFP			// old Athlons	
//#define	ISSE	3		// Prescott P4	
//#define	LOADEARLY		// avoid the REGEXEC macro inefficiency, not yet usable
//#define	CVERSION		// for debugging only
#define	ALIGNPITCH			// to cope with Avisynth's alignment bug
//#define	MAGNIFY			// for MagnifyBy2 filter
//#define	AMPLIFY_LIMITS

#define	MAX_MODE		2
#define	DEFAULT_MODE	2

#define VC_EXTRALEAN 
#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <xmmintrin.h>

#include "avisynth.h"
#include "planar.h"

#ifdef	SSE2_TEST
#ifndef	ISSE
#define	ISSE	2
#endif
#ifndef	DEBUG_NAME
#define	DEBUG_NAME
#endif
#endif

#if		ISSE > 1			
#define	CPUFLAGS		CPUF_SSE2
#else
#ifdef	NOFPISSE
#define	CPUFLAGS		CPUF_INTEGER_SSE
#else
#define	CPUFLAGS		CPUF_SSE
#endif
#endif

#if		ISSE > 1
#define	SSE_INCREMENT	16
#define	SSE_SHIFT		4
#define	SSE_MOVE		movdqu
#if		ISSE > 2
#define	SSE3_MOVE		lddqu
#else
#define	SSE3_MOVE		movdqu
#endif
#define	SSE_RMOVE		movdqa
#define	SSE_HMOVE		movq
#define	SSE0			xmm0
#define	SSE1			xmm1
#define	SSE2			xmm2
#define	SSE3			xmm3
#define	SSE4			xmm4
#define	SSE5			xmm5
#define	SSE6			xmm6
#define	SSE7			xmm7
#define	SHIFTLEFT(reg, amount)	\
__asm	pslldq			reg,	amount
#define	SHIFTRIGHT(reg, amount)	\
__asm	psrldq			reg,	amount
#define	SSE_EMMS	
#define	REGEXEC(instruction, dest, src, reg)	\
__asm	SSE3_MOVE		reg,	src				\
__asm	instruction		dest,	reg
#else	// ISSE
#define	SSE_INCREMENT	8
#define	SSE_SHIFT		3
#define	SSE_MOVE		movq
#define	SSE3_MOVE		movq
#define	SSE_RMOVE		movq
#define	SSE_HMOVE		movd
#define	SSE0			mm0
#define	SSE1			mm1
#define	SSE2			mm2
#define	SSE3			mm3
#define	SSE4			mm4
#define	SSE5			mm5
#define	SSE6			mm6
#define	SSE7			mm7
#define	SHIFTLEFT(reg, amount)	\
__asm	psllq			reg,	8*(amount)
#define	SHIFTRIGHT(reg, amount)	\
__asm	psrlq			reg,	8*(amount)
#define	SSE_EMMS		__asm	emms
#define	REGEXEC(instruction, dest, src, reg)	\
__asm	instruction		dest,	src
#endif	// ISSE

#ifdef	LOADEARLY
#undef	LOADEARLY
#define	LOADEARLY(reg, mem)			\
__asm	SSE_MOVE		reg,	mem
#define	REGEXECLE(instruction, dest, src, reg)	\
__asm	instruction		dest,	reg
#else
#define	LOADEARLY(reg, mem)
#define	REGEXECLE(instruction, dest, src, reg)	\
__asm	instruction		dest,	src
#endif

#if	defined(ALIGNPITCH) && (ISSE > 1)
#define	PFACTOR			2
#else
#define	PFACTOR			1
#endif

static	void	debug_printf(const char *format, ...)
{
	char	buffer[200];
	va_list	args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	OutputDebugString(buffer);	
}

static	IScriptEnvironment	*AVSenvironment;

#ifndef	ISSE

void	Median(BYTE *dest, int dpitch, const BYTE *src, int spitch, int width, int height)
{
	memcpy(dest, src, width); 
	dest += dpitch;
	height -= 2;
	dpitch -= width;
	int incspitch = spitch - width;
	do
	{
		int i = width;
		do
		{
			unsigned m, n;
			if( (m = src[0]) < (n = src[2 * spitch]) )
			{
				if( m < src[spitch] )
				{
					m = src[spitch];
					if( m > n ) m = n;
				}
			}
			else
			{
				if( m > src[spitch] )
				{
					m = src[spitch];
					if( m < n ) m = n;
				}
			}
			*dest++ = (BYTE) m;
			src++;
		} while( --i );
		src += incspitch;
		dest += dpitch;
	} while( --height );
	memcpy(dest, src + spitch, width); 
}

void	RelaxedMedian(BYTE *dest, int dpitch, const BYTE *src, int spitch, int width, int height)
{
	memcpy(dest, src, width);
	dest += dpitch;
	memcpy(dest, src + spitch, width);
	dest += dpitch;
	height -= 4;
	int incdpitch = dpitch - width;
	int incspitch = spitch - width;
	int	spitch3 = 3 * spitch;
	do
	{
		int i = width;
		do
		{
			int m, n;
			if( (m = src[spitch]) < (n = src[spitch3]) )
			{
				int d1;
				if( (d1 = m - src[0]) > 0 )
				{
					int d2;
					if( (d2 = n - src[4 * spitch]) > 0 )
					{
						d2 += n;
						d1 += m;
						if( d1 > n )
						{
							if( d1 > d2 ) d1 = d2;
							n = d1;
						}
					}
				}
				else
				{
					int d2;
					if( (d2 = n - src[4 * spitch]) < 0 )
					{
						d2 += n;
						d1 += m;
						if( d2 < m )
						{
							if( d2 < d1 ) d2 = d1;
							m = d2;
						}
					}
				}
				if( m < src[2 * spitch] )
				{
					m = src[2 * spitch];
					if( m > n ) m = n;
				}
			}
			else
			{
				int d1;
				if( (d1 = m - src[0]) > 0 )
				{
					int d2;
					if( (d2 = n - src[4 * spitch]) > 0 )
					{
						d2 += n;
						d1 += m;
						if( d2 > m )
						{
							if( d1 < d2 ) d2 = d1;
							m = d2;
						}
					}
				}
				else
				{
					int d2;
					if( (d2 = n - src[4 * spitch]) < 0 )
					{
						d2 += n;
						d1 += m;
						if( d1 < n )
						{
							if( d2 > d1 ) d1 = d2;
							n = d1;
						}
					}
				}
				if( m > src[2 * spitch] )
				{
					m = src[2 * spitch];
					if( m < n ) m = n;
				}
			}
			*dest++ = (BYTE) m;
			src++;
		} while( --i );
		src += incspitch;
		dest += incdpitch;
	} while( --height );
	memcpy(dest, src += 2*spitch, width); 
	memcpy(dest + dpitch, src + spitch, width); 
}

void	CopyFrame(BYTE *dest, int dpitch, const BYTE *src, int spitch, int width, int height)
{
	AVSenvironment->BitBlt(dest, dpitch, src, spitch, width, height);
}

void	do_nothing(BYTE *dest, int dpitch, const BYTE *src, int spitch, int width, int height)
{
}

void	(*VerticalCleanerFuncs[])(BYTE *dest, int dpitch, const BYTE *src, int spitch, int width, int height)
			= { CopyFrame, Median, RelaxedMedian };

class	VerticalCleaner	 : public GenericVideoFilter, public PlanarAccess
{
	void	(*CleanerFunc[3])(BYTE *dest, int dpitch, const BYTE *src, int spitch, int width, int height);
	
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env)
	{
		PVideoFrame df = env->NewVideoFrame(vi, 32);
		PVideoFrame sf = child->GetFrame(n, env);
		
		int	i = planes;
		do
		{
			CleanerFunc[i](GetWritePtr(df, i), GetPitch(df, i), GetReadPtr(sf, i), GetPitch(sf, i), width[i], height[i]);
		} while( --i >= 0 );
		return df;
	}

public:
	VerticalCleaner(PClip clip, int *mode, bool planar) : GenericVideoFilter(clip), PlanarAccess(vi, planar)
	{	
		int i = planes;
		do
		{
			int	m;
			if( (unsigned) (m = mode[i]) > MAX_MODE ) CleanerFunc[i] = do_nothing;
			else CleanerFunc[i] = VerticalCleanerFuncs[mode[i]];
			if( height[i] <= 4 ) AVSenvironment->ThrowError("VerticalCleaner: height too small");
		} while( --i >= 0 );
	}
};

#else	// ISSE

#define	a_median(reg1, reg2, reg3, addr1, addr2, addr3)	\
	__asm	SSE_RMOVE		reg1,		[addr2]				\
	__asm	SSE_RMOVE		reg2,		[addr3]				\
	__asm	SSE_RMOVE		reg3,		reg1				\
	__asm	pminub			reg1,		reg2				\
	__asm	pmaxub			reg2,		reg3				\
	__asm	pmaxub			reg1,		[addr1]				\
	__asm	pminub			reg1,		reg2

#define	a_relaxed_median(reg1, reg2, reg3, reg4, reg5, reg6, addr1, addr2, addr3, addr4, addr5)	\
	__asm	SSE_RMOVE		reg1,		[addr2]				\
	__asm	SSE_RMOVE		reg2,		[addr4]				\
	__asm	SSE_RMOVE		reg3,		reg1				\
	__asm	SSE_RMOVE		reg5,		[addr1]				\
	__asm	SSE_RMOVE		reg4,		reg2				\
	__asm	SSE_RMOVE		reg6,		[addr5]				\
	__asm	psubusb			reg3,		reg5				\
	__asm	psubusb			reg4,		reg6				\
	__asm	paddusb			reg3,		reg1				\
	__asm	paddusb			reg4,		reg2				\
	__asm	pminub			reg3,		reg4				\
	__asm	pmaxub			reg3,		reg1				\
	__asm	pmaxub			reg3,		reg2				\
	__asm	SSE_RMOVE		reg4,		reg1				\
	__asm	psubusb			reg5,		reg1				\
	__asm	pminub			reg4,		reg2				\
	__asm	psubusb			reg6,		reg2				\
	__asm	psubusb			reg1,		reg5				\
	__asm	psubusb			reg2,		reg6				\
	__asm	pmaxub			reg1,		reg2				\
	__asm	pminub			reg4,		reg1				\
	__asm	pminub			reg3,		[addr3]				\
	__asm	pmaxub			reg3,		reg4


#if	ISSE == 1
#define	ua_median(reg1, reg2, reg3, reg4, addr1, addr2, addr3)	a_median(reg1, reg2, reg3, addr1, addr2, addr3)
#define	ua_relaxed_median a_relaxed_median
#else
#define	ua_median(reg1, reg2, reg3, reg4, addr1, addr2, addr3)		\
	__asm	SSE3_MOVE		reg1,		[addr2]				\
	__asm	SSE3_MOVE		reg2,		[addr3]				\
	__asm	SSE_RMOVE		reg3,		reg1				\
	__asm	SSE3_MOVE		reg4,		[addr1]				\
	__asm	pminub			reg1,		reg2				\
	__asm	pmaxub			reg2,		reg3				\
	__asm	pmaxub			reg1,		reg4				\
	__asm	pminub			reg1,		reg2

#define	ua_relaxed_median(reg1, reg2, reg3, reg4, reg5, reg6, addr1, addr2, addr3, addr4, addr5)	\
	__asm	SSE3_MOVE		reg1,		[addr2]				\
	__asm	SSE3_MOVE		reg2,		[addr4]				\
	__asm	SSE_RMOVE		reg3,		reg1				\
	__asm	SSE3_MOVE		reg5,		[addr1]				\
	__asm	SSE_RMOVE		reg4,		reg2				\
	__asm	SSE3_MOVE		reg6,		[addr5]				\
	__asm	psubusb			reg3,		reg5				\
	__asm	psubusb			reg4,		reg6				\
	__asm	paddusb			reg3,		reg1				\
	__asm	paddusb			reg4,		reg2				\
	__asm	pminub			reg3,		reg4				\
	__asm	pmaxub			reg3,		reg1				\
	__asm	pmaxub			reg3,		reg2				\
	__asm	SSE_RMOVE		reg4,		reg1				\
	__asm	psubusb			reg5,		reg1				\
	__asm	pminub			reg4,		reg2				\
	__asm	psubusb			reg6,		reg2				\
	__asm	psubusb			reg1,		reg5				\
	__asm	psubusb			reg2,		reg6				\
	__asm	pmaxub			reg1,		reg2				\
	__asm	SSE3_MOVE		reg5,		[addr3]				\
	__asm	pminub			reg4,		reg1				\
	__asm	pminub			reg3,		reg5				\
	__asm	pmaxub			reg3,		reg4
#endif

void	aligned_median(BYTE *dest, int dpitch, const BYTE *src, int spitch, int hblocks, int remainder, int width, int height)
{
	memcpy(dest, src, width); 
	height -= 2;
	dest += dpitch;
	__asm	mov				eax,			width
	__asm	mov				ebx,			spitch
	__asm	sub				eax,			SSE_INCREMENT
	__asm	mov				edi,			dest
	__asm	sub				dpitch,			eax
	__asm	mov				esi,			src
	__asm	sub				spitch,			eax
	__asm	lea				edx,			[2*ebx]
	__asm	mov				eax,			remainder
	__asm	mov				ecx,			hblocks
	__asm	align			16
	__asm	loop_label:
			a_median(SSE0, SSE1, SSE2, esi + ebx, esi, esi + edx)
	__asm	add				esi,			SSE_INCREMENT
	__asm	SSE_RMOVE		[edi],			SSE0
	__asm	add				edi,			SSE_INCREMENT
	__asm	loop			loop_label
	__asm	add				esi,			eax
	__asm	add				edi,			eax
			ua_median(SSE0, SSE1, SSE2, SSE3, esi + ebx, esi, esi + edx)
	__asm	add				esi,			spitch
	__asm	SSE_MOVE		[edi],			SSE0
	__asm	add				edi,			dpitch
	__asm	dec				height
	__asm	mov				ecx,			hblocks
	__asm	jnz				loop_label
	__asm	add				esi,			ebx
	__asm	mov				dest,			edi
	__asm	mov				src,			esi
	memcpy(dest, src, width); 
}

void	aligned_relaxed_median(BYTE *dest, int dpitch, const BYTE *src, int spitch, int hblocks, int remainder, int width, int height)
{
	memcpy(dest, src, width);
	memcpy(dest += dpitch, src + spitch, width);
	height -= 4;
	dest += dpitch;
	__asm	mov				eax,			width
	__asm	mov				ebx,			spitch
	__asm	sub				eax,			SSE_INCREMENT
	__asm	mov				edi,			dest
	__asm	sub				dpitch,			eax
	__asm	mov				esi,			src
	__asm	sub				spitch,			eax
	__asm	lea				edx,			[ebx + 2*ebx]
	__asm	mov				eax,			remainder
	__asm	mov				ecx,			hblocks
	__asm	align			16
	__asm	loop_label:
			a_relaxed_median(SSE0, SSE1, SSE2, SSE3, SSE4, SSE5, esi, esi + ebx, esi + 2*ebx, esi + edx, esi + 4*ebx)
	__asm	add				esi,			SSE_INCREMENT
	__asm	SSE_RMOVE		[edi],			SSE2
	__asm	add				edi,			SSE_INCREMENT
	__asm	loop			loop_label
	__asm	add				esi,			eax
	__asm	add				edi,			eax
			ua_relaxed_median(SSE0, SSE1, SSE2, SSE3, SSE4, SSE5, esi, esi + ebx, esi + 2*ebx, esi + edx, esi + 4*ebx)
	__asm	add				esi,			spitch
	__asm	SSE_MOVE		[edi],			SSE2
	__asm	add				edi,			dpitch
	__asm	dec				height
	__asm	mov				ecx,			hblocks
	__asm	jnz				loop_label
	__asm	lea				esi,			[esi + 2*ebx]
	__asm	mov				dest,			edi
	__asm	mov				src,			esi
	__asm	mov				spitch,			ebx
	memcpy(dest, src, width); 
	memcpy(dest + dpitch + width - SSE_INCREMENT, src + spitch, width);
}



#if	ISSE > 1
void	unaligned_median(BYTE *dest, int dpitch, const BYTE *src, int spitch, int hblocks, int remainder, int width, int height)
{
	memcpy(dest, src, width); 
	height -= 2;
	dest += dpitch;
	__asm	mov				eax,			width
	__asm	mov				ebx,			spitch
	__asm	sub				eax,			SSE_INCREMENT
	__asm	mov				edi,			dest
	__asm	sub				dpitch,			eax
	__asm	mov				esi,			src
	__asm	sub				spitch,			eax
	__asm	lea				edx,			[2*ebx]
	__asm	mov				eax,			remainder
	__asm	mov				ecx,			hblocks
	__asm	align			16
	__asm	loop_label:
			ua_median(SSE0, SSE1, SSE2, SSE4, esi + ebx, esi, esi + edx)
	__asm	add				esi,			SSE_INCREMENT
	__asm	SSE_MOVE		[edi],			SSE0
	__asm	add				edi,			SSE_INCREMENT
	__asm	loop			loop_label
	__asm	add				esi,			eax
	__asm	add				edi,			eax
			ua_median(SSE0, SSE1, SSE2, SSE3, esi + ebx, esi, esi + edx)
	__asm	add				esi,			spitch
	__asm	SSE_MOVE		[edi],			SSE0
	__asm	add				edi,			dpitch
	__asm	dec				height
	__asm	mov				ecx,			hblocks
	__asm	jnz				loop_label
	__asm	add				esi,			ebx
	__asm	mov				dest,			edi
	__asm	mov				src,			esi
	memcpy(dest, src, width); 
}

void	Median(BYTE *dest, int dpitch, const BYTE *src, int spitch, int hblocks, int remainder, int width, int height)
{
	(((unsigned) dest) & 15) + (((unsigned) src) & 15) + (((unsigned) dpitch) & 15) + (((unsigned) spitch) & 15) == 0 ?
			  aligned_median(dest, dpitch, src, spitch, hblocks, remainder, width, height)
			: unaligned_median(dest, dpitch, src, spitch, hblocks, remainder, width, height);
}

void	unaligned_relaxed_median(BYTE *dest, int dpitch, const BYTE *src, int spitch, int hblocks, int remainder, int width, int height)
{
	memcpy(dest, src, width);
	memcpy(dest += dpitch, src + spitch, width);
	height -= 4;
	dest += dpitch;
	__asm	mov				eax,			width
	__asm	mov				ebx,			spitch
	__asm	sub				eax,			SSE_INCREMENT
	__asm	mov				edi,			dest
	__asm	sub				dpitch,			eax
	__asm	mov				esi,			src
	__asm	sub				spitch,			eax
	__asm	lea				edx,			[ebx + 2*ebx]
	__asm	mov				eax,			remainder
	__asm	mov				ecx,			hblocks
	__asm	align			16
	__asm	loop_label:
			ua_relaxed_median(SSE0, SSE1, SSE2, SSE3, SSE4, SSE5, esi, esi + ebx, esi + 2*ebx, esi + edx, esi + 4*ebx)
	__asm	add				esi,			SSE_INCREMENT
	__asm	SSE_MOVE		[edi],			SSE2
	__asm	add				edi,			SSE_INCREMENT
	__asm	loop			loop_label
	__asm	add				esi,			eax
	__asm	add				edi,			eax
			ua_relaxed_median(SSE0, SSE1, SSE2, SSE3, SSE4, SSE5, esi, esi + ebx, esi + 2*ebx, esi + edx, esi + 4*ebx)
	__asm	add				esi,			spitch
	__asm	SSE_MOVE		[edi],			SSE2
	__asm	add				edi,			dpitch
	__asm	dec				height
	__asm	mov				ecx,			hblocks
	__asm	jnz				loop_label
	__asm	lea				esi,			[esi + 2*ebx]
	__asm	mov				dest,			edi
	__asm	mov				src,			esi
	__asm	mov				spitch,			ebx
	memcpy(dest, src, width); 
	memcpy(dest + dpitch + width - SSE_INCREMENT, src + spitch, width);
}

void	RelaxedMedian(BYTE *dest, int dpitch, const BYTE *src, int spitch, int hblocks, int remainder, int width, int height)
{
	(((unsigned) dest) & 15) + (((unsigned) src) & 15) + (((unsigned) dpitch) & 15) + (((unsigned) spitch) & 15) == 0 ?
			  aligned_relaxed_median(dest, dpitch, src, spitch, hblocks, remainder, width, height)
			: unaligned_relaxed_median(dest, dpitch, src, spitch, hblocks, remainder, width, height);
}
#else
#define	Median			aligned_median
#define	RelaxedMedian	aligned_relaxed_median
#endif

void	CopyFrame(BYTE *dest, int dpitch, const BYTE *src, int spitch, int hblocks, int remainder, int width, int height)
{
	AVSenvironment->BitBlt(dest, dpitch, src, spitch, width, height);
}

void	do_nothing(BYTE *dest, int dpitch, const BYTE *src, int spitch, int hblocks, int remainder, int width, int height)
{
}


void	(*VerticalCleanerFuncs[])(BYTE *dest, int dpitch, const BYTE *src, int spitch, int hblocks, int remainder, int width, int height)
			= { CopyFrame, Median, RelaxedMedian };

class	VerticalCleaner	 : public GenericVideoFilter, public PlanarAccess
{
	void	(*CleanerFunc[3])(BYTE *dest, int dpitch, const BYTE *src, int spitch, int hblocks, int remainder, int width, int height);
	int		hblocks[3];
	int		remainder[3];
	
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env)
	{
		PVideoFrame df = env->NewVideoFrame(vi, 32);
		PVideoFrame sf = child->GetFrame(n, env);
		
		int	i = planes;
		do
		{
			CleanerFunc[i](GetWritePtr(df, i), GetPitch(df, i), GetReadPtr(sf, i), GetPitch(sf, i), hblocks[i], remainder[i], width[i], height[i]);
		} while( --i >= 0 );
		SSE_EMMS
		return df;
	}

public:
	VerticalCleaner(PClip clip, int *mode, bool planar) : GenericVideoFilter(clip), PlanarAccess(vi, planar)
	{	
		int i = planes;
		do
		{
			int	m;
			if( (unsigned) (m = mode[i]) > MAX_MODE ) CleanerFunc[i] = do_nothing;
			else CleanerFunc[i] = VerticalCleanerFuncs[mode[i]];
			hblocks[i] = (width[i] - 1) / SSE_INCREMENT;
			remainder[i] = width[i] - (hblocks[i] + 1) * SSE_INCREMENT;
			if( height[i] <= 4 ) AVSenvironment->ThrowError("VerticalCleaner: height too small");
		} while( --i >= 0 );
	}
};

#endif // ISSE

AVSValue __cdecl CreateVerticalCleaner(AVSValue args, void* user_data, IScriptEnvironment* env)
{
	enum ARGS { CLIP, M, MY, MU, MV, PLANAR};

	int dmode = args[M].AsInt(DEFAULT_MODE);
	int	mode[3];

	mode[0] = args[MY].AsInt(dmode);
	mode[1] = args[MU].AsInt(dmode);
	mode[2] = args[MV].AsInt(dmode);

	return	new VerticalCleaner(args[CLIP].AsClip(), mode, args[PLANAR].AsBool(false));
}

extern "C" __declspec(dllexport) const char* __stdcall AvisynthPluginInit2(IScriptEnvironment* env)
{
	AVSenvironment = env;
#ifdef	DEBUG_NAME
	env->AddFunction("DVerticalCleaner", "c[mode]i[modeY]i[modeU]i[modeV]i[planar]b", CreateVerticalCleaner, 0);	
#else
	env->AddFunction("VerticalCleaner", "c[mode]i[modeY]i[modeU]i[modeV]i[planar]b", CreateVerticalCleaner, 0);							
#endif
	
	if( (CPUFLAGS & env->GetCPUFlags()) != CPUFLAGS ) 
#if	ISSE > 1
		env->ThrowError("HQBob needs an SSE2 capable cpu!\n");
#else
		env->ThrowError("HQBob needs needs an SSE capable cpu!\n");
#endif
//	debug_printf(LOGO);
	return NULL;
}

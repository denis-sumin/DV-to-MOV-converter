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

#include "GC_Func.h"
#include "GenericTools.h"

#define FITNORM ((sum + offset) / total)

#define FITSAT FitIntoUC((sum + offset) / total)

#define FITABS FitIntoUCABS((sum + offset) / total)

#define MAKE_GC(TYPE,ABSFUN) \
void GC_ ## TYPE ## ABSFUN ## _C (unsigned char *derp, const unsigned char *srcp, const int der_pitch, \
 		                          const int src_pitch, const int row_size, \
			                      const int height, const TYPE *horvect, const TYPE *vervect, \
			                      const int horres, const int verres, const TYPE total) \
{ \
	TYPE sum; \
	TYPE *columnp; \
	TYPE savedcolumn; \
	const TYPE *tempp; \
	int dmodulo = der_pitch - row_size; \
	int smodulo = src_pitch - row_size; \
    const unsigned char **s; \
    s = new const unsigned char *[verres * 2 + 1]; \
	TYPE *column; \
    column = new TYPE[horres * 2 + 1]; \
	const unsigned char **sp; \
	TYPE offset = total / 2; \
	int x, y, i, j; \
\
	for ( x = 0; x < 2 * verres + 1; x++ ) \
		s[x] = srcp + x * src_pitch; \
\
	/* First 'verres' lines, missing pixels are filled with the closest of the picture */ \
	/* We do not want these lines to be compute inside the main loop, because of */ \
	/* speed considerations */ \
\
	for ( y = 0; y < verres; y++ ) \
	{ \
\
		/* These columns are outside the picture, horizontally */ \
		/* We begin by the first one */ \
\
		(*column) = 0; \
		for ( j = 0; j < 2 * verres + 1; j++ ) \
			(*column) += vervect[j] * \
				((j < verres - y) ? *srcp : *(s[j - verres + y])); \
\
		/* Then all the others are the same than the first one */ \
\
		columnp = column + 1; \
		for ( x = 1; x < horres; x++) \
		{ \
			(*columnp) = (*column); \
			columnp++; \
		} \
\
		/* These ones are inside the picture */ \
\
		for ( x = 0; x < horres; x++ ) \
		{ \
			column[x + horres] = 0; \
			for ( j = 0; j < 2 * verres + 1; j++ ) \
				column[x + horres] += vervect[j] * \
					((j < verres - y) ? *(srcp + x) : *(s[j - verres + y] + x)); \
		} \
\
		/* We calculate pixels for almost the entire line */ \
\
		for ( x = horres; x < row_size; x++ ) \
		{ \
			column[2 * horres] = 0; \
			for ( j = 0; j < 2 * verres + 1; j++ ) \
				column[2 * horres] += vervect[j] * \
					((j < verres - y) ? *(srcp + x) : *(s[j - verres + y] + x)); \
\
			sum = 0; \
\
			for ( j = 0; j < 2 * horres; j++ ) \
			{ \
				sum += horvect[j] * column[j]; \
				column[j] = column[j+1]; \
			} \
\
			sum += horvect[2 * horres] * column[2 * horres]; \
\
            *(derp + x - horres) = FIT ## ABSFUN; \
		} \
\
		/* We finished the line */ \
\
		/* First we compute the first column outside the picture */ \
\
		savedcolumn = 0; \
		for ( j = 0; j < 2 * verres + 1; j++ ) \
			savedcolumn += vervect[j] * \
					((j < verres - y) ? *(srcp + row_size - 1) : *(s[j - verres + y] + row_size - 1)); \
\
		/* Then we do the the pixels */ \
\
		for ( x = row_size - horres; x < row_size; x++ ) \
		{ \
			column[2 * horres] = savedcolumn; \
			sum = 0; \
			for ( j = 0; j < 2 * horres; j++ ) \
			{ \
				sum += horvect[j] * column[j]; \
				column[j] = column[j+1]; \
			} \
\
			sum += horvect[2 * horres] * column[2 * horres]; \
\
			*(derp + x) = FIT ## ABSFUN; \
		} \
		derp += der_pitch; \
	} \
\
	/* This is the main loop */ \
\
	for ( y = verres; y < height - verres; y++ ) \
	{ \
\
		/* It begins with columns outside the picture */ \
		/* We compute the first one */ \
\
		(*column) = 0; \
		sp = s; \
		tempp = vervect; \
		for ( j = 0; j < 2 * verres + 1; j++ ) \
		{ \
			(*column) += (**sp) * (*tempp); \
			sp++; \
			tempp++; \
		} \
\
		/* Then all the others are the same that the first one */ \
\
		columnp = column + 1; \
		for ( x = 1; x < horres; x++) \
		{ \
			(*columnp) = (*column); \
			columnp++; \
		} \
\
		/* Then columns inside the picture */ \
\
		for ( x = 0; x < horres; x++ ) \
		{ \
			*columnp = 0; \
			sp = s; \
			tempp = vervect; \
			for ( j = 0; j < 2 * verres + 1; j++ ) \
			{ \
				(*columnp) += (**sp) * (*tempp); \
				(*sp)++; \
				sp++; \
				tempp++; \
			} \
			columnp++; \
		} \
\
		/* We process the line not until the end*/ \
\
		for ( x = 0; x < row_size - horres; x++ ) \
		{ \
			columnp = column + 2 * horres; \
			*columnp = 0; \
			sp = s; \
			tempp = vervect; \
			for ( j = 0; j < 2 * verres + 1; j++ ) { \
                *columnp += (**sp) * (*tempp); \
				(*sp)++; \
				sp++; \
				tempp++; \
			} \
			sum = 0; \
			columnp = column; \
			tempp = horvect; \
			for ( j = 0; j < 2 * horres; j++ ) \
			{ \
				sum += (*columnp) * (*tempp); \
				*columnp = *(columnp+1); \
				tempp++; \
				columnp++; \
			} \
			sum += (*columnp) * (*tempp); \
\
			(*derp) = FIT ## ABSFUN; \
			derp++; \
		} \
\
		/* Then we finish the line, with columns outside the picture */ \
		/* We compute the first one */ \
\
		savedcolumn = 0; \
		tempp = vervect; \
		sp = s; \
		for ( j = 0; j < 2 * verres + 1; j++ ) \
		{ \
			savedcolumn += (*((*sp)-1)) * (*tempp); \
			sp++; \
			tempp++; \
		} \
\
		/* Then we do the remaining pixels */ \
\
		for ( x = 0; x < horres; x++ ) \
		{ \
			*(column + 2 * horres) = savedcolumn; \
\
			sum = 0; \
			columnp = column; \
			tempp = horvect; \
			for ( j = 0; j < 2 * horres; j++ ) \
			{ \
				sum += (*columnp) * (*tempp); \
				*columnp = *(columnp+1); \
				tempp++; \
				columnp++; \
			} \
			sum += (*columnp) * (*tempp); \
\
			(*derp) = FIT ## ABSFUN; \
			derp++; \
		} \
\
		/* Finally, we make the pointers point on the next lines */ \
\
		derp += dmodulo; \
		sp = s; \
		for ( x = 0; x < 2 * verres + 1; x++ ) \
		{ \
			(*sp) += smodulo; \
			sp++; \
		} \
	} \
\
	/* Finally (!), the last 'verres' lines */ \
	/* Note that the s array is at the bottom of the picture, s[2*verres] points */ \
	/* on the first line under the picture. */ \
\
	for ( y = verres - 1; y >= 0; y-- ) \
	{ \
\
		/* These columns are outside the picture, horizontally */ \
		/* We begin by the first one */ \
\
		(*column) = 0; \
		for ( j = 0; j < 2 * verres + 1; j++ ) \
			(*column) += vervect[j] *  \
				((j > verres + y) ? *(s[2 * verres - 1]) : *(s[j + verres - 1 - y])); \
\
		/* Then all the others are the same than the first one */ \
\
		columnp = column + 1; \
		for ( x = 1; x < horres; x++) \
		{ \
			(*columnp) = (*column); \
			columnp++; \
		} \
\
		/* These ones are inside the picture */ \
\
		for ( x = 0; x < horres; x++ ) \
		{ \
			column[x + horres] = 0; \
			for ( j = 0; j < 2 * verres + 1; j++ ) \
				column[x + horres] += vervect[j] *  \
					((j > verres + y) ? *(s[2 * verres - 1] + x) : *(s[j + verres - 1 - y]+ x)); \
		} \
\
		/* We calculate pixels for almost the entire line */ \
\
		for ( x = horres; x < row_size; x++ ) \
		{ \
			column[2 * horres] = 0; \
			for ( j = 0; j < 2 * verres + 1; j++ ) \
				column[2 * horres] += vervect[j] *  \
					((j > verres + y) ? *(s[2 * verres - 1] + x) : *(s[j + verres - 1 - y]+ x)); \
\
			sum = 0; \
\
			for ( j = 0; j < 2 * horres; j++ ) \
			{ \
				sum += horvect[j] * column[j]; \
				column[j] = column[j+1]; \
			} \
\
			sum += horvect[2 * horres] * column[2 * horres]; \
\
			*(derp + x - horres) = FIT ## ABSFUN; \
		} \
\
		/* We finish the line */ \
\
		/* First we compute the first column outside the picture */ \
\
		savedcolumn = 0; \
		for ( j = 0; j < 2 * verres + 1; j++ ) \
			savedcolumn += vervect[j] *  \
				((j > verres + y) ? *(s[2 * verres - 1] + row_size - 1) : *(s[j + verres - 1 - y] + row_size - 1)); \
\
		/* Then we do the the pixels */ \
\
		for ( x = row_size - horres; x < row_size; x++ ) \
		{ \
			column[2 * horres] = savedcolumn; \
			sum = 0; \
			for ( j = 0; j < 2 * horres; j++ ) \
			{ \
				sum += horvect[j] * column[j]; \
				column[j] = column[j+1]; \
			} \
\
			sum += horvect[2 * horres] * column[2 * horres]; \
\
			*(derp + x) = FIT ## ABSFUN; \
		} \
		derp += der_pitch; \
	} \
\
	delete[] s; \
	delete[] column; \
\
\
}

MAKE_GC(MTFLT, NORM)
MAKE_GC(MTFLT, ABS)
MAKE_GC(MTFLT, SAT)

MAKE_GC(MTINT, NORM)
MAKE_GC(MTINT, ABS)
MAKE_GC(MTINT, SAT)



/*
void GC_C (const unsigned char *srcp, unsigned char *derp, const int src_pitch,
			const int der_pitch, const int row_size, 
			const int height, const int *horvect, const int *vervect,
			const double *horvectf, const double *vervectf,
			const int horres, const int verres, const int total, double totalf)
{
	int sum;
	int *columnp;
	int savedcolumn;
	const int *tempp;
	int dmodulo = der_pitch - row_size;
	int smodulo = src_pitch - row_size;
	const unsigned char **s;
	int *column = new int[horres*2+1];
	s = new const unsigned char*[verres*2+1];
	const unsigned char **sp;
	int offset = total / 2;
	int x,y,i,j;

	for ( x = 0; x < 2 * verres + 1; x++ )
		s[x] = srcp + x * src_pitch;

	// First 'verres' lines, missing pixels are filled with the closest of the picture
	// We do not want these lines to be compute inside the main loop, because of
	// speed considerations

	for ( y = 0; y < verres; y++ )
	{

		// These columns are outside the picture, horizontally
		// We begin by the first one

		(*column) = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			(*column) += vervect[j] *
				((j < verres - y) ? *srcp : *(s[j - verres + y]));

		// Then all the others are the same than the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// These ones are inside the picture

		for ( x = 0; x < horres; x++ )
		{
			column[x + horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[x + horres] += vervect[j] * 
					((j < verres - y) ? *(srcp + x) : *(s[j - verres + y] + x));
		}

		// We calculate pixels for almost the entire line

		for ( x = horres; x < row_size; x++ )
		{
			column[2 * horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[2 * horres] += vervect[j] * 
					((j < verres - y) ? *(srcp + x) : *(s[j - verres + y] + x));

			sum = 0;

			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvect[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvect[2 * horres] * column[2 * horres];

			*(derp + x - horres) = (sum + offset) / total;
		}

		// We finish the line

		// First we compute the first column outside the picture

		savedcolumn = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			savedcolumn += vervect[j] * 
					((j < verres - y) ? *(srcp + row_size - 1) : *(s[j - verres + y] + row_size - 1));

		// Then we do the the pixels

		for ( x = row_size - horres; x < row_size; x++ )
		{
			column[2 * horres] = savedcolumn;
			sum = 0;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvect[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvect[2 * horres] * column[2 * horres];

			*(derp + x) = (sum + offset) / total;
		}
		derp += der_pitch;
	}

	// This is the main loop

	for ( y = verres; y < height - verres; y++ )
	{

		// It begins with columns outside the picture
		// We compute the first one

		(*column) = 0;
		sp = s;
		tempp = vervect;
		for ( j = 0; j < 2 * verres + 1; j++ )
		{
			(*column) += (**sp) * (*tempp);
			sp++;
			tempp++;
		}

		// Then all the others are the same that the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// Then columns inside the picture

		for ( x = 0; x < horres; x++ )
		{
			*columnp = 0;
			sp = s;
			tempp = vervect;
			for ( j = 0; j < 2 * verres + 1; j++ )
			{
				(*columnp) += (**sp) * (*tempp);
				(*sp)++;
				sp++;
				tempp++;
			}
			columnp++;
		}

		// We process the line not until the end

		for ( x = 0; x < row_size - horres; x++ )
		{
			columnp = column + 2 * horres;
			*columnp = 0;
			sp = s;
			tempp = vervect;
			for ( j = 0; j < 2 * verres + 1; j++ ) {
                *columnp += (**sp) * (*tempp);
				(*sp)++;
				sp++;
				tempp++;
			}
			sum = 0;
			columnp = column;
			tempp = horvect;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += (*columnp) * (*tempp);
				*columnp = *(columnp+1);
				tempp++;
				columnp++;
			}
			sum += (*columnp) * (*tempp);

			(*derp) = (sum + offset) / total;
			derp++;
		}

		// Then we finish the line, with columns outside the picture
		// We compute the first one

		savedcolumn = 0;
		tempp = vervect;
		sp = s;
		for ( j = 0; j < 2 * verres + 1; j++ )
		{
			savedcolumn += (*((*sp)-1)) * (*tempp);
			sp++;
			tempp++;
		}

		// Then we do the remaining pixels

		for ( x = 0; x < horres; x++ )
		{
			*(column + 2 * horres) = savedcolumn;

			sum = 0;
			columnp = column;
			tempp = horvect;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += (*columnp) * (*tempp);
				*columnp = *(columnp+1);
				tempp++;
				columnp++;
			}
			sum += (*columnp) * (*tempp);

			(*derp) = (sum + offset) / total;
			derp++;
		}

		// Finally, we make the pointers point on the next lines

		derp += dmodulo;
		sp = s;
		for ( x = 0; x < 2 * verres + 1; x++ )
		{
			(*sp) += smodulo;
			sp++;
		}
	}

	// Finally (!), the last 'verres' lines
	// Note that the s array is at the bottom of the picture, s[2*verres] points
	// on the first line under the picture.

	for ( y = verres - 1; y >= 0; y-- )
	{

		// These columns are outside the picture, horizontally
		// We begin by the first one

		(*column) = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			(*column) += vervect[j] * 
				((j > verres + y) ? *(s[2 * verres - 1]) : *(s[j + verres - 1 - y]));

		// Then all the others are the same than the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// These ones are inside the picture

		for ( x = 0; x < horres; x++ )
		{
			column[x + horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[x + horres] += vervect[j] * 
					((j > verres + y) ? *(s[2 * verres - 1] + x) : *(s[j + verres - 1 - y]+ x));
		}

		// We calculate pixels for almost the entire line

		for ( x = horres; x < row_size; x++ )
		{
			column[2 * horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[2 * horres] += vervect[j] * 
					((j > verres + y) ? *(s[2 * verres - 1] + x) : *(s[j + verres - 1 - y]+ x));

			sum = 0;

			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvect[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvect[2 * horres] * column[2 * horres];

			*(derp + x - horres) = (sum + offset) / total;
		}

		// We finish the line

		// First we compute the first column outside the picture

		savedcolumn = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			savedcolumn += vervect[j] * 
				((j > verres + y) ? *(s[2 * verres - 1] + row_size - 1) : *(s[j + verres - 1 - y] + row_size - 1));

		// Then we do the the pixels

		for ( x = row_size - horres; x < row_size; x++ )
		{
			column[2 * horres] = savedcolumn;
			sum = 0;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvect[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvect[2 * horres] * column[2 * horres];

			*(derp + x) = (sum + offset) / total;
		}
		derp += der_pitch;
	}

	delete[] s;
	delete[] column;


}

void GCf_C (const unsigned char *srcp, unsigned char *derp, const int src_pitch,
			const int der_pitch, const int row_size, 
			const int height, const int *horvect, const int *vervect,
			const double *horvectf, const double *vervectf,
			const int horres, const int verres, const int total, double totalf)
{
	double sum;
	double *columnp;
	double savedcolumn;
	const double *tempp;
	int dmodulo = der_pitch - row_size;
	int smodulo = src_pitch - row_size;
	const unsigned char **s;
	double *column = new double[horres*2+1];
	s = new const unsigned char*[verres*2+1];
	const unsigned char **sp;
	double offset = totalf / 2.0;
	int x,y,i,j;

	for ( x = 0; x < 2 * verres + 1; x++ )
		s[x] = srcp + x * src_pitch;

	// First 'verres' lines, missing pixels are filled with the closest of the picture
	// We do not want these lines to be compute inside the main loop, because of
	// speed considerations

	for ( y = 0; y < verres; y++ )
	{

		// These columns are outside the picture, horizontally
		// We begin by the first one

		(*column) = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			(*column) += vervectf[j] *
				((j < verres - y) ? *srcp : *(s[j - verres + y]));

		// Then all the others are the same than the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// These ones are inside the picture

		for ( x = 0; x < horres; x++ )
		{
			column[x + horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[x + horres] += vervectf[j] * 
					((j < verres - y) ? *(srcp + x) : *(s[j - verres + y] + x));
		}

		// We calculate pixels for almost the entire line

		for ( x = horres; x < row_size; x++ )
		{
			column[2 * horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[2 * horres] += vervectf[j] * 
					((j < verres - y) ? *(srcp + x) : *(s[j - verres + y] + x));

			sum = 0;

			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvectf[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvectf[2 * horres] * column[2 * horres];

			*(derp + x - horres) = ((sum + offset)/ totalf);
		}

		// We finish the line

		// First we compute the first column outside the picture

		savedcolumn = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			savedcolumn += vervectf[j] * 
					((j < verres - y) ? *(srcp + row_size - 1) : *(s[j - verres + y] + row_size - 1));

		// Then we do the the pixels

		for ( x = row_size - horres; x < row_size; x++ )
		{
			column[2 * horres] = savedcolumn;
			sum = 0;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvectf[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvectf[2 * horres] * column[2 * horres];

			*(derp + x) = ((sum + offset)/ totalf);
		}
		derp += der_pitch;
	}

	// This is the main loop

	for ( y = verres; y < height - verres; y++ )
	{

		// It begins with columns outside the picture
		// We compute the first one

		(*column) = 0;
		sp = s;
		tempp = vervectf;
		for ( j = 0; j < 2 * verres + 1; j++ )
		{
			(*column) += (**sp) * (*tempp);
			sp++;
			tempp++;
		}

		// Then all the others are the same that the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// Then columns inside the picture

		for ( x = 0; x < horres; x++ )
		{
			*columnp = 0;
			sp = s;
			tempp = vervectf;
			for ( j = 0; j < 2 * verres + 1; j++ )
			{
				(*columnp) += (**sp) * (*tempp);
				(*sp)++;
				sp++;
				tempp++;
			}
			columnp++;
		}

		// We process the line not until the end

		for ( x = 0; x < row_size - horres; x++ )
		{
			columnp = column + 2 * horres;
			*columnp = 0;
			sp = s;
			tempp = vervectf;
			for ( j = 0; j < 2 * verres + 1; j++ ) {
                *columnp += (**sp) * (*tempp);
				(*sp)++;
				sp++;
				tempp++;
			}
			sum = 0;
			columnp = column;
			tempp = horvectf;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += (*columnp) * (*tempp);
				*columnp = *(columnp+1);
				tempp++;
				columnp++;
			}
			sum += (*columnp) * (*tempp);

			(*derp) = ((sum + offset)/ totalf);
			derp++;
		}

		// Then we finish the line, with columns outside the picture
		// We compute the first one

		savedcolumn = 0;
		tempp = vervectf;
		sp = s;
		for ( j = 0; j < 2 * verres + 1; j++ )
		{
			savedcolumn += (*((*sp)-1)) * (*tempp);
			sp++;
			tempp++;
		}

		// Then we do the remaining pixels

		for ( x = 0; x < horres; x++ )
		{
			*(column + 2 * horres) = savedcolumn;

			sum = 0;
			columnp = column;
			tempp = horvectf;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += (*columnp) * (*tempp);
				*columnp = *(columnp+1);
				tempp++;
				columnp++;
			}
			sum += (*columnp) * (*tempp);

			(*derp) = ((sum + offset)/ totalf);
			derp++;
		}

		// Finally, we make the pointers point on the next lines

		derp += dmodulo;
		sp = s;
		for ( x = 0; x < 2 * verres + 1; x++ )
		{
			(*sp) += smodulo;
			sp++;
		}
	}

	// Finally (!), the last 'verres' lines
	// Note that the s array is at the bottom of the picture, s[2*verres] points
	// on the first line under the picture.

	for ( y = verres - 1; y >= 0; y-- )
	{

		// These columns are outside the picture, horizontally
		// We begin by the first one

		(*column) = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			(*column) += vervectf[j] * 
				((j > verres + y) ? *(s[2 * verres - 1]) : *(s[j + verres - 1 - y]));

		// Then all the others are the same than the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// These ones are inside the picture

		for ( x = 0; x < horres; x++ )
		{
			column[x + horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[x + horres] += vervectf[j] * 
					((j > verres + y) ? *(s[2 * verres - 1] + x) : *(s[j + verres - 1 - y]+ x));
		}

		// We calculate pixels for almost the entire line

		for ( x = horres; x < row_size; x++ )
		{
			column[2 * horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[2 * horres] += vervectf[j] * 
					((j > verres + y) ? *(s[2 * verres - 1] + x) : *(s[j + verres - 1 - y]+ x));

			sum = 0;

			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvectf[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvectf[2 * horres] * column[2 * horres];

			*(derp + x - horres) = ((sum + offset)/ totalf);
		}

		// We finish the line

		// First we compute the first column outside the picture

		savedcolumn = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			savedcolumn += vervectf[j] * 
				((j > verres + y) ? *(s[2 * verres - 1] + row_size - 1) : *(s[j + verres - 1 - y] + row_size - 1));

		// Then we do the the pixels

		for ( x = row_size - horres; x < row_size; x++ )
		{
			column[2 * horres] = savedcolumn;
			sum = 0;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvectf[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvectf[2 * horres] * column[2 * horres];

			*(derp + x) = ((sum + offset)/ totalf);
		}
		derp += der_pitch;
	}

	delete[] s;
	delete[] column;


}

void GC_C_ABS (const unsigned char *srcp, unsigned char *derp, const int src_pitch,
			const int der_pitch, const int row_size, 
			const int height, const int *horvect, const int *vervect,
			const double *horvectf, const double *vervectf,
			const int horres, const int verres, const int total, double totalf)
{
	int sum;
	int *columnp;
	int savedcolumn;
	const int *tempp;
	int dmodulo = der_pitch - row_size;
	int smodulo = src_pitch - row_size;
	const unsigned char **s;
	int *column = new int[horres*2+1];
	s = new const unsigned char*[verres*2+1];
	const unsigned char **sp;
	int offset = total / 2;
	int x,y,i,j;

	for ( x = 0; x < 2 * verres + 1; x++ )
		s[x] = srcp + x * src_pitch;

	// First 'verres' lines, missing pixels are filled with the closest of the picture
	// We do not want these lines to be compute inside the main loop, because of
	// speed considerations

	for ( y = 0; y < verres; y++ )
	{

		// These columns are outside the picture, horizontally
		// We begin by the first one

		(*column) = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			(*column) += vervect[j] *
				((j < verres - y) ? *srcp : *(s[j - verres + y]));

		// Then all the others are the same than the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// These ones are inside the picture

		for ( x = 0; x < horres; x++ )
		{
			column[x + horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[x + horres] += vervect[j] * 
					((j < verres - y) ? *(srcp + x) : *(s[j - verres + y] + x));
		}

		// We calculate pixels for almost the entire line

		for ( x = horres; x < row_size; x++ )
		{
			column[2 * horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[2 * horres] += vervect[j] * 
					((j < verres - y) ? *(srcp + x) : *(s[j - verres + y] + x));

			sum = 0;

			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvect[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvect[2 * horres] * column[2 * horres];

			*(derp + x - horres) = FitIntoUCABS((sum + offset) / total);
		}

		// We finish the line

		// First we compute the first column outside the picture

		savedcolumn = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			savedcolumn += vervect[j] * 
					((j < verres - y) ? *(srcp + row_size - 1) : *(s[j - verres + y] + row_size - 1));

		// Then we do the the pixels

		for ( x = row_size - horres; x < row_size; x++ )
		{
			column[2 * horres] = savedcolumn;
			sum = 0;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvect[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvect[2 * horres] * column[2 * horres];

			*(derp + x) = FitIntoUCABS((sum + offset) / total);
		}
		derp += der_pitch;
	}

	// This is the main loop

	for ( y = verres; y < height - verres; y++ )
	{

		// It begins with columns outside the picture
		// We compute the first one

		(*column) = 0;
		sp = s;
		tempp = vervect;
		for ( j = 0; j < 2 * verres + 1; j++ )
		{
			(*column) += (**sp) * (*tempp);
			sp++;
			tempp++;
		}

		// Then all the others are the same that the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// Then columns inside the picture

		for ( x = 0; x < horres; x++ )
		{
			*columnp = 0;
			sp = s;
			tempp = vervect;
			for ( j = 0; j < 2 * verres + 1; j++ )
			{
				(*columnp) += (**sp) * (*tempp);
				(*sp)++;
				sp++;
				tempp++;
			}
			columnp++;
		}

		// We process the line not until the end

		for ( x = 0; x < row_size - horres; x++ )
		{
			columnp = column + 2 * horres;
			*columnp = 0;
			sp = s;
			tempp = vervect;
			for ( j = 0; j < 2 * verres + 1; j++ ) {
                *columnp += (**sp) * (*tempp);
				(*sp)++;
				sp++;
				tempp++;
			}
			sum = 0;
			columnp = column;
			tempp = horvect;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += (*columnp) * (*tempp);
				*columnp = *(columnp+1);
				tempp++;
				columnp++;
			}
			sum += (*columnp) * (*tempp);

			(*derp) = FitIntoUCABS((sum + offset) / total);
			derp++;
		}

		// Then we finish the line, with columns outside the picture
		// We compute the first one

		savedcolumn = 0;
		tempp = vervect;
		sp = s;
		for ( j = 0; j < 2 * verres + 1; j++ )
		{
			savedcolumn += (*((*sp)-1)) * (*tempp);
			sp++;
			tempp++;
		}

		// Then we do the remaining pixels

		for ( x = 0; x < horres; x++ )
		{
			*(column + 2 * horres) = savedcolumn;

			sum = 0;
			columnp = column;
			tempp = horvect;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += (*columnp) * (*tempp);
				*columnp = *(columnp+1);
				tempp++;
				columnp++;
			}
			sum += (*columnp) * (*tempp);

			(*derp) = FitIntoUCABS((sum + offset) / total);
			derp++;
		}

		// Finally, we make the pointers point on the next lines

		derp += dmodulo;
		sp = s;
		for ( x = 0; x < 2 * verres + 1; x++ )
		{
			(*sp) += smodulo;
			sp++;
		}
	}

	// Finally (!), the last 'verres' lines
	// Note that the s array is at the bottom of the picture, s[2*verres] points
	// on the first line under the picture.

	for ( y = verres - 1; y >= 0; y-- )
	{

		// These columns are outside the picture, horizontally
		// We begin by the first one

		(*column) = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			(*column) += vervect[j] * 
				((j > verres + y) ? *(s[2 * verres - 1]) : *(s[j + verres - 1 - y]));

		// Then all the others are the same than the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// These ones are inside the picture

		for ( x = 0; x < horres; x++ )
		{
			column[x + horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[x + horres] += vervect[j] * 
					((j > verres + y) ? *(s[2 * verres - 1] + x) : *(s[j + verres - 1 - y]+ x));
		}

		// We calculate pixels for almost the entire line

		for ( x = horres; x < row_size; x++ )
		{
			column[2 * horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[2 * horres] += vervect[j] * 
					((j > verres + y) ? *(s[2 * verres - 1] + x) : *(s[j + verres - 1 - y]+ x));

			sum = 0;

			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvect[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvect[2 * horres] * column[2 * horres];

			*(derp + x - horres) = FitIntoUCABS((sum + offset) / total);
		}

		// We finish the line

		// First we compute the first column outside the picture

		savedcolumn = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			savedcolumn += vervect[j] * 
				((j > verres + y) ? *(s[2 * verres - 1] + row_size - 1) : *(s[j + verres - 1 - y] + row_size - 1));

		// Then we do the the pixels

		for ( x = row_size - horres; x < row_size; x++ )
		{
			column[2 * horres] = savedcolumn;
			sum = 0;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvect[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvect[2 * horres] * column[2 * horres];

			*(derp + x) = FitIntoUCABS((sum + offset) / total);
		}
		derp += der_pitch;
	}

	delete[] s;
	delete[] column;


}

void GCf_C_ABS (const unsigned char *srcp, unsigned char *derp, const int src_pitch,
			const int der_pitch, const int row_size, 
			const int height, const int *horvect, const int *vervect,
			const double *horvectf, const double *vervectf,
			const int horres, const int verres, const int total, double totalf)
{
	double sum;
	double *columnp;
	double savedcolumn;
	const double *tempp;
	int dmodulo = der_pitch - row_size;
	int smodulo = src_pitch - row_size;
	const unsigned char **s;
	double *column = new double[horres*2+1];
	s = new const unsigned char*[verres*2+1];
	const unsigned char **sp;
	int x,y,i,j;

	for ( x = 0; x < 2 * verres + 1; x++ )
		s[x] = srcp + x * src_pitch;

	// First 'verres' lines, missing pixels are filled with the closest of the picture
	// We do not want these lines to be compute inside the main loop, because of
	// speed considerations

	for ( y = 0; y < verres; y++ )
	{

		// These columns are outside the picture, horizontally
		// We begin by the first one

		(*column) = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			(*column) += vervectf[j] *
				((j < verres - y) ? *srcp : *(s[j - verres + y]));

		// Then all the others are the same than the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// These ones are inside the picture

		for ( x = 0; x < horres; x++ )
		{
			column[x + horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[x + horres] += vervectf[j] * 
					((j < verres - y) ? *(srcp + x) : *(s[j - verres + y] + x));
		}

		// We calculate pixels for almost the entire line

		for ( x = horres; x < row_size; x++ )
		{
			column[2 * horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[2 * horres] += vervectf[j] * 
					((j < verres - y) ? *(srcp + x) : *(s[j - verres + y] + x));

			sum = 0;

			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvectf[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvectf[2 * horres] * column[2 * horres];

			*(derp + x - horres) = FitIntoUCABS(sum / totalf);
		}

		// We finish the line

		// First we compute the first column outside the picture

		savedcolumn = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			savedcolumn += vervectf[j] * 
					((j < verres - y) ? *(srcp + row_size - 1) : *(s[j - verres + y] + row_size - 1));

		// Then we do the the pixels

		for ( x = row_size - horres; x < row_size; x++ )
		{
			column[2 * horres] = savedcolumn;
			sum = 0;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvectf[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvectf[2 * horres] * column[2 * horres];

			*(derp + x) = FitIntoUCABS(sum / totalf);
		}
		derp += der_pitch;
	}

	// This is the main loop

	for ( y = verres; y < height - verres; y++ )
	{

		// It begins with columns outside the picture
		// We compute the first one

		(*column) = 0;
		sp = s;
		tempp = vervectf;
		for ( j = 0; j < 2 * verres + 1; j++ )
		{
			(*column) += (**sp) * (*tempp);
			sp++;
			tempp++;
		}

		// Then all the others are the same that the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// Then columns inside the picture

		for ( x = 0; x < horres; x++ )
		{
			*columnp = 0;
			sp = s;
			tempp = vervectf;
			for ( j = 0; j < 2 * verres + 1; j++ )
			{
				(*columnp) += (**sp) * (*tempp);
				(*sp)++;
				sp++;
				tempp++;
			}
			columnp++;
		}

		// We process the line not until the end

		for ( x = 0; x < row_size - horres; x++ )
		{
			columnp = column + 2 * horres;
			*columnp = 0;
			sp = s;
			tempp = vervectf;
			for ( j = 0; j < 2 * verres + 1; j++ ) {
                *columnp += (**sp) * (*tempp);
				(*sp)++;
				sp++;
				tempp++;
			}
			sum = 0;
			columnp = column;
			tempp = horvectf;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += (*columnp) * (*tempp);
				*columnp = *(columnp+1);
				tempp++;
				columnp++;
			}
			sum += (*columnp) * (*tempp);

			(*derp) = FitIntoUCABS(sum / totalf);
			derp++;
		}

		// Then we finish the line, with columns outside the picture
		// We compute the first one

		savedcolumn = 0;
		tempp = vervectf;
		sp = s;
		for ( j = 0; j < 2 * verres + 1; j++ )
		{
			savedcolumn += (*((*sp)-1)) * (*tempp);
			sp++;
			tempp++;
		}

		// Then we do the remaining pixels

		for ( x = 0; x < horres; x++ )
		{
			*(column + 2 * horres) = savedcolumn;

			sum = 0;
			columnp = column;
			tempp = horvectf;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += (*columnp) * (*tempp);
				*columnp = *(columnp+1);
				tempp++;
				columnp++;
			}
			sum += (*columnp) * (*tempp);

			(*derp) = FitIntoUCABS(sum / totalf);
			derp++;
		}

		// Finally, we make the pointers point on the next lines

		derp += dmodulo;
		sp = s;
		for ( x = 0; x < 2 * verres + 1; x++ )
		{
			(*sp) += smodulo;
			sp++;
		}
	}

	// Finally (!), the last 'verres' lines
	// Note that the s array is at the bottom of the picture, s[2*verres] points
	// on the first line under the picture.

	for ( y = verres - 1; y >= 0; y-- )
	{

		// These columns are outside the picture, horizontally
		// We begin by the first one

		(*column) = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			(*column) += vervectf[j] * 
				((j > verres + y) ? *(s[2 * verres - 1]) : *(s[j + verres - 1 - y]));

		// Then all the others are the same than the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// These ones are inside the picture

		for ( x = 0; x < horres; x++ )
		{
			column[x + horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[x + horres] += vervectf[j] * 
					((j > verres + y) ? *(s[2 * verres - 1] + x) : *(s[j + verres - 1 - y]+ x));
		}

		// We calculate pixels for almost the entire line

		for ( x = horres; x < row_size; x++ )
		{
			column[2 * horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[2 * horres] += vervectf[j] * 
					((j > verres + y) ? *(s[2 * verres - 1] + x) : *(s[j + verres - 1 - y]+ x));

			sum = 0;

			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvectf[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvectf[2 * horres] * column[2 * horres];

			*(derp + x - horres) = FitIntoUCABS(sum / totalf);
		}

		// We finish the line

		// First we compute the first column outside the picture

		savedcolumn = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			savedcolumn += vervectf[j] * 
				((j > verres + y) ? *(s[2 * verres - 1] + row_size - 1) : *(s[j + verres - 1 - y] + row_size - 1));

		// Then we do the the pixels

		for ( x = row_size - horres; x < row_size; x++ )
		{
			column[2 * horres] = savedcolumn;
			sum = 0;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvectf[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvectf[2 * horres] * column[2 * horres];

			*(derp + x) = FitIntoUCABS(sum / totalf);
		}
		derp += der_pitch;
	}

	delete[] s;
	delete[] column;


}

void GC_C_SAT (const unsigned char *srcp, unsigned char *derp, const int src_pitch,
			const int der_pitch, const int row_size, 
			const int height, const int *horvect, const int *vervect,
			const double *horvectf, const double *vervectf,
			const int horres, const int verres, const int total, double totalf)
{
	int sum;
	int *columnp;
	int savedcolumn;
	const int *tempp;
	int dmodulo = der_pitch - row_size;
	int smodulo = src_pitch - row_size;
	const unsigned char **s;
	int *column = new int[horres*2+1];
	s = new const unsigned char*[verres*2+1];
	const unsigned char **sp;
	int offset = total / 2;
	int x,y,i,j;

	for ( x = 0; x < 2 * verres + 1; x++ )
		s[x] = srcp + x * src_pitch;

	// First 'verres' lines, missing pixels are filled with the closest of the picture
	// We do not want these lines to be compute inside the main loop, because of
	// speed considerations

	for ( y = 0; y < verres; y++ )
	{

		// These columns are outside the picture, horizontally
		// We begin by the first one

		(*column) = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			(*column) += vervect[j] *
				((j < verres - y) ? *srcp : *(s[j - verres + y]));

		// Then all the others are the same than the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// These ones are inside the picture

		for ( x = 0; x < horres; x++ )
		{
			column[x + horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[x + horres] += vervect[j] * 
					((j < verres - y) ? *(srcp + x) : *(s[j - verres + y] + x));
		}

		// We calculate pixels for almost the entire line

		for ( x = horres; x < row_size; x++ )
		{
			column[2 * horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[2 * horres] += vervect[j] * 
					((j < verres - y) ? *(srcp + x) : *(s[j - verres + y] + x));

			sum = 0;

			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvect[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvect[2 * horres] * column[2 * horres];

			*(derp + x - horres) = FitIntoUC((sum + offset) / total);
		}

		// We finish the line

		// First we compute the first column outside the picture

		savedcolumn = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			savedcolumn += vervect[j] * 
					((j < verres - y) ? *(srcp + row_size - 1) : *(s[j - verres + y] + row_size - 1));

		// Then we do the the pixels

		for ( x = row_size - horres; x < row_size; x++ )
		{
			column[2 * horres] = savedcolumn;
			sum = 0;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvect[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvect[2 * horres] * column[2 * horres];

			*(derp + x) = FitIntoUC((sum + offset) / total);
		}
		derp += der_pitch;
	}

	// This is the main loop

	for ( y = verres; y < height - verres; y++ )
	{

		// It begins with columns outside the picture
		// We compute the first one

		(*column) = 0;
		sp = s;
		tempp = vervect;
		for ( j = 0; j < 2 * verres + 1; j++ )
		{
			(*column) += (**sp) * (*tempp);
			sp++;
			tempp++;
		}

		// Then all the others are the same that the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// Then columns inside the picture

		for ( x = 0; x < horres; x++ )
		{
			*columnp = 0;
			sp = s;
			tempp = vervect;
			for ( j = 0; j < 2 * verres + 1; j++ )
			{
				(*columnp) += (**sp) * (*tempp);
				(*sp)++;
				sp++;
				tempp++;
			}
			columnp++;
		}

		// We process the line not until the end

		for ( x = 0; x < row_size - horres; x++ )
		{
			columnp = column + 2 * horres;
			*columnp = 0;
			sp = s;
			tempp = vervect;
			for ( j = 0; j < 2 * verres + 1; j++ ) {
                *columnp += (**sp) * (*tempp);
				(*sp)++;
				sp++;
				tempp++;
			}
			sum = 0;
			columnp = column;
			tempp = horvect;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += (*columnp) * (*tempp);
				*columnp = *(columnp+1);
				tempp++;
				columnp++;
			}
			sum += (*columnp) * (*tempp);

			(*derp) = FitIntoUC((sum + offset) / total);
			derp++;
		}

		// Then we finish the line, with columns outside the picture
		// We compute the first one

		savedcolumn = 0;
		tempp = vervect;
		sp = s;
		for ( j = 0; j < 2 * verres + 1; j++ )
		{
			savedcolumn += (*((*sp)-1)) * (*tempp);
			sp++;
			tempp++;
		}

		// Then we do the remaining pixels

		for ( x = 0; x < horres; x++ )
		{
			*(column + 2 * horres) = savedcolumn;

			sum = 0;
			columnp = column;
			tempp = horvect;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += (*columnp) * (*tempp);
				*columnp = *(columnp+1);
				tempp++;
				columnp++;
			}
			sum += (*columnp) * (*tempp);

			(*derp) = FitIntoUC((sum + offset) / total);
			derp++;
		}

		// Finally, we make the pointers point on the next lines

		derp += dmodulo;
		sp = s;
		for ( x = 0; x < 2 * verres + 1; x++ )
		{
			(*sp) += smodulo;
			sp++;
		}
	}

	// Finally (!), the last 'verres' lines
	// Note that the s array is at the bottom of the picture, s[2*verres] points
	// on the first line under the picture.

	for ( y = verres - 1; y >= 0; y-- )
	{

		// These columns are outside the picture, horizontally
		// We begin by the first one

		(*column) = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			(*column) += vervect[j] * 
				((j > verres + y) ? *(s[2 * verres - 1]) : *(s[j + verres - 1 - y]));

		// Then all the others are the same than the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// These ones are inside the picture

		for ( x = 0; x < horres; x++ )
		{
			column[x + horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[x + horres] += vervect[j] * 
					((j > verres + y) ? *(s[2 * verres - 1] + x) : *(s[j + verres - 1 - y]+ x));
		}

		// We calculate pixels for almost the entire line

		for ( x = horres; x < row_size; x++ )
		{
			column[2 * horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[2 * horres] += vervect[j] * 
					((j > verres + y) ? *(s[2 * verres - 1] + x) : *(s[j + verres - 1 - y]+ x));

			sum = 0;

			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvect[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvect[2 * horres] * column[2 * horres];

			*(derp + x - horres) = FitIntoUC((sum + offset) / total);
		}

		// We finish the line

		// First we compute the first column outside the picture

		savedcolumn = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			savedcolumn += vervect[j] * 
				((j > verres + y) ? *(s[2 * verres - 1] + row_size - 1) : *(s[j + verres - 1 - y] + row_size - 1));

		// Then we do the the pixels

		for ( x = row_size - horres; x < row_size; x++ )
		{
			column[2 * horres] = savedcolumn;
			sum = 0;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvect[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvect[2 * horres] * column[2 * horres];

			*(derp + x) = FitIntoUC((sum + offset) / total);
		}
		derp += der_pitch;
	}

	delete[] s;
	delete[] column;


}

void GCf_C_SAT (const unsigned char *srcp, unsigned char *derp, const int src_pitch,
			const int der_pitch, const int row_size, 
			const int height, const int *horvect, const int *vervect,
			const double *horvectf, const double *vervectf,
			const int horres, const int verres, const int total, double totalf)
{
	double sum;
	double *columnp;
	double savedcolumn;
	const double *tempp;
	int dmodulo = der_pitch - row_size;
	int smodulo = src_pitch - row_size;
	const unsigned char **s;
	double *column = new double[horres*2+1];
	s = new const unsigned char*[verres*2+1];
	const unsigned char **sp;
	int x,y,i,j;

	for ( x = 0; x < 2 * verres + 1; x++ )
		s[x] = srcp + x * src_pitch;

	// First 'verres' lines, missing pixels are filled with the closest of the picture
	// We do not want these lines to be compute inside the main loop, because of
	// speed considerations

	for ( y = 0; y < verres; y++ )
	{

		// These columns are outside the picture, horizontally
		// We begin by the first one

		(*column) = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			(*column) += vervectf[j] *
				((j < verres - y) ? *srcp : *(s[j - verres + y]));

		// Then all the others are the same than the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// These ones are inside the picture

		for ( x = 0; x < horres; x++ )
		{
			column[x + horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[x + horres] += vervectf[j] * 
					((j < verres - y) ? *(srcp + x) : *(s[j - verres + y] + x));
		}

		// We calculate pixels for almost the entire line

		for ( x = horres; x < row_size; x++ )
		{
			column[2 * horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[2 * horres] += vervectf[j] * 
					((j < verres - y) ? *(srcp + x) : *(s[j - verres + y] + x));

			sum = 0;

			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvectf[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvectf[2 * horres] * column[2 * horres];

			*(derp + x - horres) = FitIntoUC(sum / totalf);
		}

		// We finish the line

		// First we compute the first column outside the picture

		savedcolumn = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			savedcolumn += vervectf[j] * 
					((j < verres - y) ? *(srcp + row_size - 1) : *(s[j - verres + y] + row_size - 1));

		// Then we do the the pixels

		for ( x = row_size - horres; x < row_size; x++ )
		{
			column[2 * horres] = savedcolumn;
			sum = 0;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvectf[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvectf[2 * horres] * column[2 * horres];

			*(derp + x) = FitIntoUC(sum / totalf);
		}
		derp += der_pitch;
	}

	// This is the main loop

	for ( y = verres; y < height - verres; y++ )
	{

		// It begins with columns outside the picture
		// We compute the first one

		(*column) = 0;
		sp = s;
		tempp = vervectf;
		for ( j = 0; j < 2 * verres + 1; j++ )
		{
			(*column) += (**sp) * (*tempp);
			sp++;
			tempp++;
		}

		// Then all the others are the same that the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// Then columns inside the picture

		for ( x = 0; x < horres; x++ )
		{
			*columnp = 0;
			sp = s;
			tempp = vervectf;
			for ( j = 0; j < 2 * verres + 1; j++ )
			{
				(*columnp) += (**sp) * (*tempp);
				(*sp)++;
				sp++;
				tempp++;
			}
			columnp++;
		}

		// We process the line not until the end

		for ( x = 0; x < row_size - horres; x++ )
		{
			columnp = column + 2 * horres;
			*columnp = 0;
			sp = s;
			tempp = vervectf;
			for ( j = 0; j < 2 * verres + 1; j++ ) {
                *columnp += (**sp) * (*tempp);
				(*sp)++;
				sp++;
				tempp++;
			}
			sum = 0;
			columnp = column;
			tempp = horvectf;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += (*columnp) * (*tempp);
				*columnp = *(columnp+1);
				tempp++;
				columnp++;
			}
			sum += (*columnp) * (*tempp);

			(*derp) = FitIntoUC(sum / totalf);
			derp++;
		}

		// Then we finish the line, with columns outside the picture
		// We compute the first one

		savedcolumn = 0;
		tempp = vervectf;
		sp = s;
		for ( j = 0; j < 2 * verres + 1; j++ )
		{
			savedcolumn += (*((*sp)-1)) * (*tempp);
			sp++;
			tempp++;
		}

		// Then we do the remaining pixels

		for ( x = 0; x < horres; x++ )
		{
			*(column + 2 * horres) = savedcolumn;

			sum = 0;
			columnp = column;
			tempp = horvectf;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += (*columnp) * (*tempp);
				*columnp = *(columnp+1);
				tempp++;
				columnp++;
			}
			sum += (*columnp) * (*tempp);

			(*derp) = FitIntoUC(sum / totalf);
			derp++;
		}

		// Finally, we make the pointers point on the next lines

		derp += dmodulo;
		sp = s;
		for ( x = 0; x < 2 * verres + 1; x++ )
		{
			(*sp) += smodulo;
			sp++;
		}
	}

	// Finally (!), the last 'verres' lines
	// Note that the s array is at the bottom of the picture, s[2*verres] points
	// on the first line under the picture.

	for ( y = verres - 1; y >= 0; y-- )
	{

		// These columns are outside the picture, horizontally
		// We begin by the first one

		(*column) = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			(*column) += vervectf[j] * 
				((j > verres + y) ? *(s[2 * verres - 1]) : *(s[j + verres - 1 - y]));

		// Then all the others are the same than the first one

		columnp = column + 1;
		for ( x = 1; x < horres; x++)
		{
			(*columnp) = (*column);
			columnp++;
		}

		// These ones are inside the picture

		for ( x = 0; x < horres; x++ )
		{
			column[x + horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[x + horres] += vervectf[j] * 
					((j > verres + y) ? *(s[2 * verres - 1] + x) : *(s[j + verres - 1 - y]+ x));
		}

		// We calculate pixels for almost the entire line

		for ( x = horres; x < row_size; x++ )
		{
			column[2 * horres] = 0;
			for ( j = 0; j < 2 * verres + 1; j++ )
				column[2 * horres] += vervectf[j] * 
					((j > verres + y) ? *(s[2 * verres - 1] + x) : *(s[j + verres - 1 - y]+ x));

			sum = 0;

			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvectf[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvectf[2 * horres] * column[2 * horres];

			*(derp + x - horres) = FitIntoUC(sum / totalf);
		}

		// We finish the line

		// First we compute the first column outside the picture

		savedcolumn = 0;
		for ( j = 0; j < 2 * verres + 1; j++ )
			savedcolumn += vervectf[j] * 
				((j > verres + y) ? *(s[2 * verres - 1] + row_size - 1) : *(s[j + verres - 1 - y] + row_size - 1));

		// Then we do the the pixels

		for ( x = row_size - horres; x < row_size; x++ )
		{
			column[2 * horres] = savedcolumn;
			sum = 0;
			for ( j = 0; j < 2 * horres; j++ )
			{
				sum += horvectf[j] * column[j];
				column[j] = column[j+1];
			}

			sum += horvectf[2 * horres] * column[2 * horres];

			*(derp + x) = FitIntoUC(sum / totalf);
		}
		derp += der_pitch;
	}

	delete[] s;
	delete[] column;


}*/
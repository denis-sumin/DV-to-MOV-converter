// Expand - set of tools for region growth of at best one pixel
// Expand (C) 2003 Kurosu (kurosu@inforezo.org)

// This work can also be referred to by the word AreaModifier

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

void Expand_C(unsigned char *dstpinit, const unsigned char *srcpinit,
			const int dst_pitch, const int src_pitch,
			const int row_size, const int height)
{
	const unsigned char *srcpn, *srcpp;
	int x, y;
	int max, temp;
	unsigned char *dstp = dstpinit;
	const unsigned char *srcp = srcpinit;

	srcpp = srcp;
	srcp += src_pitch;
	srcpn = srcp + src_pitch;

	//Top left corner
	max = srcpp[0];
	temp = srcpp[1];
	if (temp > max) max = temp;
	temp = srcp[0];
	if (temp > max) max = temp;
	temp = srcp[1];
	if (temp > max) max = temp;
	dstp[0] = max;

	//Topline
	for (x = 1; x < row_size-1; x++)
	{
		max = srcpp[x-1];
		temp = srcpp[x];
		if (temp > max) max = temp;
		temp = srcpp[x+1];
		if (temp > max) max = temp;

		temp = srcp[x-1];
		if (temp > max) max = temp;
		temp = srcp[x];
		if (temp > max) max = temp;
		temp = srcp[x+1];
		if (temp > max) max = temp;

		dstp[x] = max;
	}		

	//Top right corner
	max = srcpp[row_size-2];
	temp = srcpp[row_size-1];
	if (temp > max) max = temp;
	temp = srcp[row_size-2];
	if (temp > max) max = temp;
	temp = srcp[row_size-1];
	if (temp > max) max = temp;
	dstp[row_size-1] = max;

	dstp += dst_pitch;
	for (y = 1; y < height-1; y++) 
	{
		//Line's 1st pixel
		max = srcpp[0];
		temp = srcpp[1];
		if (temp > max) max = temp;

		temp = srcp[0];
		if (temp > max) max = temp;
		temp = srcp[1];
		if (temp > max) max = temp;

		temp = srcpn[0];
		if (temp > max) max = temp;
		temp = srcpn[1];
		if (temp > max) max = temp;
		dstp[0] = max;

		//Line's pixels
		for (x = 1; x < row_size-1; x++)
		{
			max = srcpp[x-1];
			temp = srcpp[x];
			if (temp > max) max = temp;
			temp = srcpp[x+1];
			if (temp > max) max = temp;

			temp = srcp[x-1];
			if (temp > max) max = temp;
			temp = srcp[x];
			if (temp > max) max = temp;
			temp = srcp[x+1];
			if (temp > max) max = temp;

			temp = srcpn[x-1];
			if (temp > max) max = temp;
			temp = srcpn[x];
			if (temp > max) max = temp;
			temp = srcpn[x+1];
			if (temp > max) max = temp;

			dstp[x] = max;
		}

		//Last line's pixel
		max = srcpp[row_size-2];
		temp = srcpp[row_size-1];
		if (temp > max) max = temp;

		temp = srcp[row_size-2];
		if (temp > max) max = temp;
		temp = srcp[row_size-1];
		if (temp > max) max = temp;

		temp = srcpn[row_size-2];
		if (temp > max) max = temp;
		temp = srcpn[row_size-1];
		if (temp > max) max = temp;
		dstp[row_size-1] = max;

		dstp += dst_pitch;
		srcpp = srcp;
		srcp = srcpn;
		srcpn += src_pitch;
	}

	//Bottom left corner
	max = srcpp[0];
	temp = srcpp[1];
	if (temp > max) max = temp;
	temp = srcp[0];
	if (temp > max) max = temp;
	temp = srcp[1];
	if (temp > max) max = temp;
	dstp[0] = max;

	//Bottom line
	for (x = 1; x < row_size-1; x++)
	{
		max = srcpp[x-1];
		temp = srcpp[x];
		if (temp > max) max = temp;
		temp = srcpp[x+1];
		if (temp > max) max = temp;

		temp = srcp[x-1];
		if (temp > max) max = temp;
		temp = srcp[x];
		if (temp > max) max = temp;
		temp = srcp[x+1];
		if (temp > max) max = temp;

		dstp[x] = max;
	}		

	//Bottom right corner
	max = srcpp[row_size-2];
	temp = srcpp[row_size-1];
	if (temp > max) max = temp;
	temp = srcp[row_size-2];
	if (temp > max) max = temp;
	temp = srcp[row_size-1];
	if (temp > max) max = temp;
	dstp[row_size-1] = max;
}

void Inpand_C(unsigned char *dstpinit, const unsigned char *srcpinit,
			const int dst_pitch, const int src_pitch,
			const int row_size, const int height)
{
	const unsigned char *srcpn, *srcpp;
	int x, y;
	unsigned char min, temp;
	unsigned char *dstp = dstpinit;
	const unsigned char *srcp = srcpinit;

	srcpp = srcp;
	srcp += src_pitch;
	srcpn = srcp + src_pitch;

	//Top left corner
	min = srcpp[0];
	temp = srcpp[1];
	if (temp < min) min = temp;
	temp = srcp[0];
	if (temp < min) min = temp;
	temp = srcp[1];
	if (temp < min) min = temp;
	dstp[0] = min;

	//Topline
	for (x = 1; x < row_size-1; x++)
	{
		min = srcpp[x-1];
		temp = srcpp[x];
		if (temp < min) min = temp;
		temp = srcpp[x+1];
		if (temp < min) min = temp;

		temp = srcp[x-1];
		if (temp < min) min = temp;
		temp = srcp[x];
		if (temp < min) min = temp;
		temp = srcp[x+1];
		if (temp < min) min = temp;

		dstp[x] = min;
	}		

	//Top right corner
	min = srcpp[row_size-2];
	temp = srcpp[row_size-1];
	if (temp < min) min = temp;
	temp = srcp[row_size-2];
	if (temp < min) min = temp;
	temp = srcp[row_size-1];
	if (temp < min) min = temp;
	dstp[row_size-1] = min;

	dstp += dst_pitch;
	for (y = 1; y < height-1; y++) 
	{
		//Line's 1st pixel
		min = srcpp[0];
		temp = srcpp[1];
		if (temp < min) min = temp;

		temp = srcp[0];
		if (temp < min) min = temp;
		temp = srcp[1];
		if (temp < min) min = temp;

		temp = srcpn[0];
		if (temp < min) min = temp;
		temp = srcpn[1];
		if (temp < min) min = temp;
		dstp[0] = min;

		//Line's pixels
		for (x = 1; x < row_size-1; x++)
		{
			min = srcpp[x-1];
			temp = srcpp[x];
			if (temp < min) min = temp;
			temp = srcpp[x+1];
			if (temp < min) min = temp;

			temp = srcp[x-1];
			if (temp < min) min = temp;
			temp = srcp[x];
			if (temp < min) min = temp;
			temp = srcp[x+1];
			if (temp < min) min = temp;

			temp = srcpn[x-1];
			if (temp < min) min = temp;
			temp = srcpn[x];
			if (temp < min) min = temp;
			temp = srcpn[x+1];
			if (temp < min) min = temp;

			dstp[x] = min;
		}

		//Last line's pixel
		min = srcpp[row_size-2];
		temp = srcpp[row_size-1];
		if (temp < min) min = temp;

		temp = srcp[row_size-2];
		if (temp < min) min = temp;
		temp = srcp[row_size-1];
		if (temp < min) min = temp;

		temp = srcpn[row_size-2];
		if (temp < min) min = temp;
		temp = srcpn[row_size-1];
		if (temp < min) min = temp;
		dstp[row_size-1] = min;

		dstp += dst_pitch;
		srcpp = srcp;
		srcp = srcpn;
		srcpn += src_pitch;
	}

	//Bottom left corner
	min = srcpp[0];
	temp = srcpp[1];
	if (temp < min) min = temp;
	temp = srcp[0];
	if (temp < min) min = temp;
	temp = srcp[1];
	if (temp < min) min = temp;
	dstp[0] = min;

	//Bottom line
	for (x = 1; x < row_size-1; x++)
	{
		min = srcpp[x-1];
		temp = srcpp[x];
		if (temp < min) min = temp;
		temp = srcpp[x+1];
		if (temp < min) min = temp;

		temp = srcp[x-1];
		if (temp < min) min = temp;
		temp = srcp[x];
		if (temp < min) min = temp;
		temp = srcp[x+1];
		if (temp < min) min = temp;

		dstp[x] = min;
	}		

	//Bottom right corner
	min = srcpp[row_size-2];
	temp = srcpp[row_size-1];
	if (temp < min) min = temp;
	temp = srcp[row_size-2];
	if (temp < min) min = temp;
	temp = srcp[row_size-1];
	if (temp < min) min = temp;
	dstp[row_size-1] = min;
}

void Inflate_C(unsigned char *dstpinit, const unsigned char *srcpinit,
			const int dst_pitch, const int src_pitch,
			const int row_size, const int height)
{
	const unsigned char *srcpn, *srcpp;
	int x, y;
	unsigned char temp;
	unsigned char *dstp = dstpinit;
	const unsigned char *srcp = srcpinit;

	srcpp = srcp;
	srcp += src_pitch;
	srcpn = srcp + src_pitch;

	temp = (srcpp[1] + srcp[0] + srcp[1]) / 3;
	if (temp > srcpp[0]) dstp[0] = temp;
	else dstp[0] = srcpp[0];

	for ( x = 1; x < row_size-1; x++)
	{
		temp = (srcpp[x-1] + srcpp[x+1] + srcp[x-1] + srcp[x+1] + srcp[x]) / 5;
		if (temp > srcpp[x]) dstp[x] = temp;
		else dstp[x] = srcpp[x];
	}

	temp = (srcpp[row_size-2] + srcp[row_size-2] + srcp[row_size-1]) / 3;
	if (temp > srcpp[row_size-1]) dstp[row_size-1] = temp;
	else dstp[row_size-1] = srcpp[row_size-1];

	dstp += dst_pitch;

	for (y = 1; y < height-1; y++) 
	{
		temp = (srcpp[0] + srcpp[1] + srcp[0] + srcpn[0] + srcpn[1])/5;
		if (temp>srcp[0]) dstp[0] = temp;
		else dstp[0] = srcp[0];

		for (x = 1; x < row_size-1; x++)
		{
			temp = (srcpp[x-1] + srcpp[x] + srcpp[x+1] +
					srcp[x-1] +				srcp[x+1] +
					srcpn[x-1] + srcpn[x] + srcpn[x+1])>>3;
			if (temp>srcp[x]) dstp[x] = temp;
			else dstp[x] = srcp[x];
		}

		temp = (srcpp[row_size-2] + srcpp[row_size-1] + srcp[row_size-2] +
				srcpn[row_size-2] + srcpn[row_size-1])/5;
		if (temp>srcp[row_size-1]) dstp[row_size-1] = temp;
		else dstp[row_size-1] = srcp[row_size-1];

		dstp += dst_pitch;
		srcpp = srcp;
		srcp = srcpn;
		srcpn += src_pitch;
	}

	temp = (srcpp[1] + srcp[0] + srcp[1]) / 3;
	if (temp > srcpp[0]) dstp[0] = temp;
	else dstp[0] = srcpp[0];

	for ( x = 1; x < row_size-1; x++)
	{
		temp = (srcpp[x-1] + srcpp[x+1] + srcp[x-1] + srcp[x+1] + srcp[x]) / 5;
		if (temp > srcpp[x]) dstp[x] = temp;
		else dstp[x] = srcpp[x];
	}

	temp = (srcpp[row_size-2] + srcp[row_size-2] + srcp[row_size-1]) / 3;
	if (temp > srcpp[row_size-1]) dstp[row_size-1] = temp;
	else dstp[row_size-1] = srcpp[row_size-1];

}

void Deflate_C(unsigned char *dstpinit, const unsigned char *srcpinit,
			const int dst_pitch, const int src_pitch,
			const int row_size, const int height)
{
	const unsigned char *srcpn, *srcpp;
	int x, y;
	unsigned char temp;
	unsigned char *dstp = dstpinit;
	const unsigned char *srcp = srcpinit;

	srcpp = srcp;
	srcp += src_pitch;
	srcpn = srcp + src_pitch;

	temp = (srcpp[1] + srcp[0] + srcp[1]) / 3;
	if (temp < srcpp[0]) dstp[0] = temp;
	else dstp[0] = srcpp[0];

	for ( x = 1; x < row_size-1; x++)
	{
		temp = (srcpp[x-1] + srcpp[x+1] + srcp[x-1] + srcp[x+1] + srcp[x]) / 5;
		if (temp < srcpp[x]) dstp[x] = temp;
		else dstp[x] = srcpp[x];
	}

	temp = (srcpp[row_size-2] + srcp[row_size-2] + srcp[row_size-1]) / 3;
	if (temp < srcpp[row_size-1]) dstp[row_size-1] = temp;
	else dstp[row_size-1] = srcpp[row_size-1];

	dstp += dst_pitch;

	for (y = 1; y < height-1; y++) 
	{
		temp = (srcpp[0] + srcpp[1] + srcp[0] + srcpn[0] + srcpn[1])/5;
		if (temp<srcp[0]) dstp[0] = temp;
		else dstp[0] = srcp[0];

		for (x = 1; x < row_size-1; x++)
		{
			temp = (srcpp[x-1] + srcpp[x] + srcpp[x+1] +
					srcp[x-1] +				srcp[x+1] +
					srcpn[x-1] + srcpn[x] + srcpn[x+1])>>3;
			if (temp<srcp[x]) dstp[x] = temp;
			else dstp[x] = srcp[x];
		}

		temp = (srcpp[row_size-2] + srcpp[row_size-1] + srcp[row_size-2] +
				srcpn[row_size-2] + srcpn[row_size-1])/5;
		if (temp<srcp[row_size-1]) dstp[row_size-1] = temp;
		else dstp[row_size-1] = srcp[row_size-1];

		dstp += dst_pitch;
		srcpp = srcp;
		srcp = srcpn;
		srcpn += src_pitch;
	}

	temp = (srcpp[1] + srcp[0] + srcp[1]) / 3;
	if (temp < srcpp[0]) dstp[0] = temp;
	else dstp[0] = srcpp[0];

	for ( x = 1; x < row_size-1; x++)
	{
		temp = (srcpp[x-1] + srcpp[x+1] + srcp[x-1] + srcp[x+1] + srcp[x]) / 5;
		if (temp < srcpp[x]) dstp[x] = temp;
		else dstp[x] = srcpp[x];
	}

	temp = (srcpp[row_size-2] + srcp[row_size-2] + srcp[row_size-1]) / 3;
	if (temp < srcpp[row_size-1]) dstp[row_size-1] = temp;
	else dstp[row_size-1] = srcpp[row_size-1];

}
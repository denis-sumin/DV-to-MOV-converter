#include <math.h>
#include "locbin_func.h"

void LocBinarize(unsigned char *pDst, const unsigned char *pSrc, 
                 int nDstPitch, int nSrcPitch, int nWidth, int nHeight,
                 int nThreshold1, int nThreshold2, int nRadius,
                 int nDiscardThreshold, int nBaseThreshold1,
                 int nBaseThreshold2, int nWeight, bool upper)
{
    int sum, sum2, count;
    int mean, var, std;
    int th1, th2;
    int min, max;
    const unsigned char *s;
    for ( int j = 0; j < nHeight; j++ )
    {
        for ( int i = 0; i < nWidth; i++ )
        {
            min = 255;
            max = 0;
            s = pSrc + i + j * nSrcPitch;
            for ( int y = -nRadius ; y <= nRadius; y++ )
            {
                if (( y + j >= 0 ) && ( y + j < nHeight ))
                {
                    for ( int x = -nRadius; x <= nRadius; x++ )
                    {
                        if (( x + i >= 0 ) && ( x + i < nWidth ))
                        {
                            min = ( s[x + y * nSrcPitch] < min ) ? s[x + y * nSrcPitch] : min;
                            max = ( s[x + y * nSrcPitch] > max ) ? s[x + y * nSrcPitch] : max;
                        }
                    }
                }
            }
            th1 = (256 * min + (max - min) * nThreshold1) * nWeight + 256 * nBaseThreshold1 * (256 - nWeight);
            th2 = (256 * min + (max - min) * nThreshold2) * nWeight + 256 * nBaseThreshold2 * (256 - nWeight);
            if ( th1 < 0 ) th1 = 0;
            if ( th2 < 0 ) th2 = 0;

            if (( max - min ) < nDiscardThreshold) th2 = th1;
            //th1 = 0;

            if (th2 == th1)
                pDst[i + j * nDstPitch] = 128;
            else
                if (upper)
                    pDst[i + j * nDstPitch] = 
                        ((int)*s * 65536 < th1 ) ? 255 :
                        (( (int)*s * 65536 > th2 ) ? 0 :
                        ((th2 - *s * 65536) * 255 / (th2 - th1)));

                else
                    pDst[i + j * nDstPitch] = 
                        ((int)*s * 65536 < th1 ) ? 0 :
                        (( (int)*s * 65536 > th2 ) ? 255 :
                        ((*s * 65536 - th1) * 255 / (th2 - th1)));
        }
    }
}
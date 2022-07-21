//
// Created by 神强 on 2022/7/18.
//

#include <cstring>
#include "FFTHelper.h"
#include <typeinfo>
#include <cmath>


float hanningWindow(short in, size_t i, size_t s)
{
    return in*0.5f*(1.0f-cos(2.0f*M_PI*(float)(i)/(float)(s-1.0f)));
}
void FFTHelper::fft(const short *data, int32_t length, short *outData) {
    kiss_fftr_cfg mycfg=kiss_fftr_alloc(length,0,nullptr,nullptr);
//    kiss_fft_cpx in[length];
    kiss_fft_cpx out[length];

//    for (int i = 0; i < length; ++i) {
//        in[i].r = hanningWindow(data[i], i, length);
//        in[i].i = 0;
//    }

    memset(out,0, sizeof(short) * length);

    kiss_fftr(mycfg, data, out);

    for (int i = 0; i < length; ++i) {
        outData[i] = 10 * log10(out[i].r * out[i].r + out[i].i * out[i].i);
    }


    free(mycfg);


}

void FFTHelper::fft(const short *data, int32_t length, Spectrum *outData) {
    kiss_fftr_cfg mycfg=kiss_fftr_alloc(length,0,nullptr,nullptr);
    short in[length];
    kiss_fft_cpx out[length];

    for (int i = 0; i < length; ++i) {
        in[i] = hanningWindow(data[i], i, length);
    }

    memset(out,0, sizeof(short) * length);

    kiss_fftr(mycfg, data, out);

    for (int i = 0; i < length; ++i) {
        outData[i].amplitude = 10 * log10(out[i].r * out[i].r + out[i].i * out[i].i);
    }


    free(mycfg);
}


//
// Created by 神强 on 2022/7/18.
//

#include <cstring>
#include "FFTHelper.h"
#include <typeinfo>
#include "math.h"


void FFTHelper::fft(const short *data, int32_t length, short *outData) {
    kiss_fft_cfg mycfg=kiss_fft_alloc(length,0,nullptr,nullptr);
    kiss_fft_cpx in[length];
    kiss_fft_cpx out[length];

    for (int i = 0; i < length; ++i) {
        in[i].r = data[i];
        in[i].i = 0;
    }

    memset(out,0, sizeof(short) * length);

    for (int i = 0; i < length; ++i) {
        outData[i] = 10 * log10(out[i].r * out[i].r + out[i].i * out[i].i);
    }

    kiss_fft(mycfg, in, out);

    free(mycfg);


}

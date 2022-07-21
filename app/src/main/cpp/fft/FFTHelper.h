//
// Created by 神强 on 2022/7/18.
//

#ifndef RECORDKIT_FFTHELPER_H
#define RECORDKIT_FFTHELPER_H


#define FIXED_POINT
#  define kiss_fft_scalar spx_int16_t

#include <cstdint>
#include "kiss_fftr.h"
#include "../Log.h"

struct Spectrum {
    int16_t amplitude;
    int32_t frequency;
};

class FFTHelper {
public:
    static void fft(const int16_t *data, int32_t length, Spectrum *outData);
};


#endif //RECORDKIT_FFTHELPER_H

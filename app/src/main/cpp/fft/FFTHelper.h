//
// Created by 神强 on 2022/7/18.
//

#ifndef RECORDKIT_FFTHELPER_H
#define RECORDKIT_FFTHELPER_H


#define FIXED_POINT
#  define kiss_fft_scalar spx_int16_t

#include <cstdint>
#include "kiss_fft.h"
#include "../Log.h"



class FFTHelper {
public:
    static void fft(const short *data, int32_t length, short *outData);
};


#endif //RECORDKIT_FFTHELPER_H

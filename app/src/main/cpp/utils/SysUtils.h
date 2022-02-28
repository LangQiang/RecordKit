//
// Created by 神强 on 2022/2/27.
//

#ifndef RECORDKIT_SYSUTILS_H
#define RECORDKIT_SYSUTILS_H

#include <unistd.h>
#include "sys/syscall.h"
#include "../global/GlobalConfig.h"

class SysUtils {

public:
    static int64_t getCurrentThreadId();
    static bool isMainThread();
};


#endif //RECORDKIT_SYSUTILS_H

//
// Created by 神强 on 2022/2/27.
//

#include "SysUtils.h"
#include "../Log.h"
int64_t SysUtils::getCurrentThreadId() {
    return syscall(SYS_gettid);
}

bool SysUtils::isMainThread() {
    LOGD("main:%ld  current:%ld", mainThreadId, SysUtils::getCurrentThreadId());
    return mainThreadId == SysUtils::getCurrentThreadId();
}

//
// Created by 神强 on 2022/3/18.
//

#include "Test.h"
#include "BlockingQueue.h"
#include "pthread.h"
#include "unistd.h"

static BlockingQueue<int16_t> *blockingQueue = new BlockingQueue<int16_t>(); //NOLINT


void *pop(void *arg) {
    int32_t time = 1000;
    while (time-- > 0) {
        blockingQueue->pop();
    }

    return arg;
}

void Test::test_BlockingQueue() {
    pthread_t pId;

    pthread_create(&pId, nullptr, pop, nullptr);

    for (int i = 0; i < 1000; ++i) {
        blockingQueue->push(i);
        usleep(1);
    }
}

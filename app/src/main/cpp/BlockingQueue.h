//
// Created by 神强 on 2022/3/2.
//

#ifndef RECORDKIT_BLOCKINGQUEUE_H
#define RECORDKIT_BLOCKINGQUEUE_H

#include <mutex>
#include <queue>
#include "Log.h"


template <typename T>
class BlockingQueue {
    std::mutex _mutex;
    std::condition_variable _condition;
    std::queue<T> _queue;

public:
    bool isEmpty() {
        return _queue.empty();
    }

    void push(T data) {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _queue.push(data);
            LOGE("push%d", data);
        }
        _condition.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(_mutex);
        for(;;) {
            if (isEmpty()) {
                LOGE("wait");
                _condition.wait(lock);
            } else {
                break;
            }
        }
        T ret = _queue.front();
        _queue.pop();
        LOGE("pop%d", ret);
        return ret;
    }

};

#endif //RECORDKIT_BLOCKINGQUEUE_H

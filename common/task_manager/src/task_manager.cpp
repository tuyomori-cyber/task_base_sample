// Engine/core/task_manager/src/task_manager.cpp

#include "task_manager.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

struct Task {
    std::thread              worker;
    task_entry_fn            entry;
    void*                    user_data;

    std::mutex               mtx;
    std::condition_variable  cv;
    std::queue<void*>        queue;

    std::atomic<bool>        started{false};
    std::atomic<bool>        cancel_flag{false};
    std::atomic<task_state_t> state{TASK_READY};
};

static Task* to_task(task_handle h) {
    return static_cast<Task*>(h);
}

static void task_thread_main(Task* t, task_handle h) {
    if (t->entry) {
        t->entry(h, t->user_data);
    }
    t->state.store(TASK_FINISHED, std::memory_order_release);
}

/*===============================
    C API 実装
================================*/

extern "C" {

task_handle task_create(task_entry_fn entry, void* user_data) {
    Task* t = new Task();
    t->entry     = entry;
    t->user_data = user_data;
    t->state.store(TASK_READY, std::memory_order_release);
    return static_cast<task_handle>(t);
}

void task_start(task_handle h) {
    Task* t = to_task(h);
    if (!t || t->started.exchange(true)) {
        return;
    }
    t->worker = std::thread(task_thread_main, t, h);
}

void task_push(task_handle h, void* msg) {
    Task* t = to_task(h);
    if (!t) return;

    // cancel 済みなら無視
    if (t->cancel_flag.load(std::memory_order_acquire)) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(t->mtx);
        t->queue.push(msg);
    }
    t->cv.notify_one();
}

void* task_pop(task_handle h) {
    Task* t = to_task(h);
    if (!t) return nullptr;

    std::unique_lock<std::mutex> lock(t->mtx);
    t->cv.wait(lock, [&] {
        return !t->queue.empty() || t->cancel_flag.load(std::memory_order_acquire);
    });

    if (t->cancel_flag.load(std::memory_order_acquire) && t->queue.empty()) {
        return nullptr; // cancel 要求で終了
    }

    void* msg = t->queue.front();
    t->queue.pop();
    return msg;
}

void task_cancel(task_handle h) {
    Task* t = to_task(h);
    if (!t) return;

    bool expected = false;
    if (!t->cancel_flag.compare_exchange_strong(expected, true, std::memory_order_acq_rel)) {
        return; // 既に cancel 済み
    }

    t->state.store(TASK_CANCELING, std::memory_order_release);

    {
        std::lock_guard<std::mutex> lock(t->mtx);
        // queue はそのまま。pop 側は cancel_flag を見て抜ける
    }
    t->cv.notify_all();
}

void task_join(task_handle h) {
    Task* t = to_task(h);
    if (!t) return;

    if (t->worker.joinable()) {
        t->worker.join();
    }

    delete t;
}

task_state_t task_get_state(task_handle h) {
    Task* t = to_task(h);
    if (!t) return TASK_FINISHED;
    return t->state.load(std::memory_order_acquire);
}

void task_set_state(task_handle h, task_state_t state) {
    Task* t = to_task(h);
    if (!t) return;
    t->state.store(state, std::memory_order_release);
}

} // extern "C"

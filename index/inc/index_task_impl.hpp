#pragma once
#include "task_manager.h"
#include "index_payload.hpp"

// IntentPayload の前方宣言（必要）
struct IntentPayload;

class index_task_impl {
public:
    // TaskBase から呼ばれる 4 つのメソッド
    void on_task_start(task_handle h, void* user_data);
    void on_task_handle(task_handle h, IntentPayload* msg);
    void free_payload(IntentPayload* msg);
    void on_task_end(task_handle h);
};

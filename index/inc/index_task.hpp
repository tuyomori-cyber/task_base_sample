#pragma once

#include "task_manager.h"

// index のイベントループ（TaskManager のタスクエントリ）
#ifdef __cplusplus
extern "C" {
#endif

// TaskManager から呼ばれるエントリポイント
void index_task_entry(task_handle h, void* user_data);

#ifdef __cplusplus
}
#endif

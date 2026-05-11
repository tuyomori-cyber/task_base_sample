#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

// タスクハンドル（内部構造は C++ 側に隠蔽）
typedef void* task_handle;

// タスクのエントリポイント（イベントループ）
typedef void (*task_entry_fn)(task_handle h, void* user_data);

// タスクの状態（タスク自身の状態）
typedef enum {
    TASK_READY,
    TASK_WORKING,
    TASK_CANCELING,
    TASK_FINISHED
} task_state_t;

/*===============================
    TaskManager API（呼び出し部）
================================*/

// タスク生成（スレッドはまだ開始しない）
task_handle task_create(task_entry_fn entry, void* user_data);

// タスク開始（内部で std::thread を起動）
void task_start(task_handle h);

// イベントをタスクに送る（push）
void task_push(task_handle h, void* msg);

// タスク側がイベントを受け取る（pop）
// cancel 時は NULL を返す（イベントループ終了）
void* task_pop(task_handle h);

// タスクにキャンセル要求を送る
void task_cancel(task_handle h);

// タスク終了を待つ
void task_join(task_handle h);

// タスクの状態を取得
task_state_t task_get_state(task_handle h);

// タスクの状態を設定（タスク側が呼ぶ）
void task_set_state(task_handle h, task_state_t state);

#ifdef __cplusplus
}
#endif

#endif // TASK_MANAGER_H

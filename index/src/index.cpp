#include "index.hpp"
#include "index_task.hpp"
#include <cstring>   // _strdup 用

// index のタスクハンドル（C API 側でも参照される）
task_handle g_fm_task = nullptr;

// ------------------------------------------------------------
// シングルトン instance()
// ------------------------------------------------------------
index& index::instance() {
    static index inst;
    return inst;
}

// ------------------------------------------------------------
// init / shutdown
// ------------------------------------------------------------
void index::init() {
    // index 自身のタスクを起動
    fm_task = task_create(index_task_entry, nullptr);
    g_fm_task = fm_task;

    // 下位タスクも起動（必要なら）
    //csv_task   = task_create(csv_task_entry, nullptr);
    //search_task = task_create(search_task_entry, nullptr);
    //index_task  = task_create(index_task_entry, nullptr);


    fm_state = 1; // READY
}

void index::shutdown() {
    task_cancel(fm_task);
    task_join(fm_task);


    fm_task = nullptr;

    fm_state = 0;
}

// ------------------------------------------------------------
// handle() — IntentPayload を intent ごとに振り分け
// ------------------------------------------------------------
void index::handle(const IntentPayload& msg) {

    switch (msg.intent) {

    case INTENT_INIT:
        on_intent_init();
        break;

    case INTENT_CANCEL:
        on_intent_cancel((PayloadCancel*)msg.payload);
        break;

    case INTENT_BUILD:
        on_intent_build(((PayloadBuild*)msg.payload)->path);
        break;

    default:
        break;
    }
}
void index::on_intent_init() {
    // TODO: implement
}

void index::on_intent_cancel(const PayloadCancel* p) {
    // TODO
}

void index::on_intent_build(const char* path) {
    // TODO: implement with path
}

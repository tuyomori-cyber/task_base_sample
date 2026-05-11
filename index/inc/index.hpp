#pragma once

#include "index_payload.hpp"
#include "task_manager.h"

// index 本体（シングルトン）
class index {
public:
    static index& instance();

    // Engine lifecycle
    void init();        // index のタスク起動
    void shutdown();    // index と下位タスク停止

    // App → Engine の入口（index_task_entry から呼ばれる）
    void handle(const IntentPayload& msg);

private:
    index() = default;
    ~index() = default;

    index(const index&) = delete;
    index& operator=(const index&) = delete;

    // --- index 自身のタスク ---
    task_handle fm_task = nullptr;


    // --- 状態管理 ---
    int fm_state = 0;

    // --- intent ごとの処理（構造体モデル） ---
    void on_intent_init();
    void on_intent_cancel(const PayloadCancel* p);
    void on_intent_build(const char* path);


};

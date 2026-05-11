// index_api_proc.cpp
#include "index.hpp"
#include "index_api.h"     // ← API の構造体定義はここにある
#include "task_manager.h"
#include <cstring>               // _strdup 用

// index のタスクハンドル（index_init() で設定される）
extern task_handle g_fm_task;

// IntentPayload の deep copy（TaskManager キュー用）
static IntentPayload* clone_intent_payload(const IntentPayload* src)
{
    if (!src) return nullptr;

    IntentPayload* dst = new IntentPayload;
    dst->intent = src->intent;

    switch (src->intent) {

        case INTENT_CANCEL: {
            auto* p = (PayloadCancel*)src->payload;
            auto* q = new PayloadCancel(*p);
            dst->payload = q;
            break;
        }
        case INTENT_BUILD: {
            auto* p = (PayloadBuild*)src->payload;
            auto* q = new PayloadBuild;
            q->path = p->path ? _strdup(p->path) : nullptr;
            dst->payload = q;
            break;
        }

        default:
            dst->payload = nullptr;
            break;
    }

    return dst;
}

extern "C" void index_init_cpp(void) {
    index::instance().init();
}

extern "C" void index_handle_message_cpp(const IntentPayload* msg)
{
    if (!msg) return;

    IntentPayload* cloned = clone_intent_payload(msg);
    task_push(g_fm_task, cloned);
}

extern "C" void index_shutdown_cpp(void) {
    index::instance().shutdown();
}

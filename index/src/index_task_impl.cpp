#include "index_task_impl.hpp"
#include "index.hpp"
#include "index_payload.hpp"
#include <cstdlib>   // free()
#include <cstring>   // strdup()

// ------------------------------------------------------------
// ① on_task_start : タスク開始時に呼ばれる
// ------------------------------------------------------------
void index_task_impl::on_task_start(task_handle h, void* user_data)
{
    // ★ 下位タスクへのイベント返却コールバック登録はここで行う
    // register_event_callback(index_event_callback);
    // （下位タスクが整ったら実装する）
    
    // index_task は INIT を自分に push する
    IntentPayload* init_msg = new IntentPayload;
    init_msg->intent = INTENT_INIT;
    init_msg->payload = nullptr;

    task_push(h, init_msg);
}

// ------------------------------------------------------------
// ② on_handle : IntentPayload を受け取って処理する
// ------------------------------------------------------------
void index_task_impl::on_task_handle(task_handle h, IntentPayload* msg)
{
    // index のロジックへ丸投げ
    index::instance().handle(*msg);
}

// ------------------------------------------------------------
// ③ free_payload : IntentPayload と payload の解放
// ------------------------------------------------------------
void index_task_impl::free_payload(IntentPayload* msg)
{
    if (!msg) return;

    switch (msg->intent) {

        case INTENT_CANCEL: {
            auto* p = (PayloadCancel*)msg->payload;
            delete p;
            break;
        }
        case INTENT_BUILD: {
            auto* p = (PayloadBuild*)msg->payload;
            if (p) {
                if (p->path) free((void*)p->path);
                delete p;
            }
            break;
        }

        case INTENT_INIT:
        default:
            // payload なし
            break;
    }

    delete msg;
}

// ------------------------------------------------------------
// ④ on_cleanup : タスク終了時に呼ばれる
// ------------------------------------------------------------
void index_task_impl::on_task_end(task_handle h)
{
    // index_task は特に後処理なし
}

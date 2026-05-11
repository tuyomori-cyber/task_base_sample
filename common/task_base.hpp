// task_base.hpp
#pragma once
#include "task_manager.h"

template <typename Impl>
class TaskBase {
public:
    static void entry(task_handle h, void* user_data)
    {
        Impl impl;

        task_set_state(h, TASK_READY);
        impl.on_task_start(h, user_data);

        while (true) {
            IntentPayload* msg = (IntentPayload*)task_pop(h);
            if (!msg) break;  // cancel → NULL

            task_set_state(h, TASK_WORKING);

            impl.on_task_handle(h, msg);   // intent 分岐は Impl 側
            impl.free_payload(msg);   // free も Impl 側

            task_set_state(h, TASK_READY);
        }

        impl.on_task_end(h);
        task_set_state(h, TASK_FINISHED);
    }
};

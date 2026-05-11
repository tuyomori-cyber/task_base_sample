#include "task_base.hpp"
#include "index_task_impl.hpp"

extern "C" void index_task_entry(task_handle h, void* user_data)
{
    TaskBase<index_task_impl>::entry(h, user_data);
}

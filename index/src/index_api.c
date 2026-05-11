#include "index_api.h"

// C++ 側の実体（実装は .cpp）
#ifdef __cplusplus
extern void index_init_cpp(void);
extern void index_handle_message_cpp(const char* json);
extern void index_shutdown_cpp(void);
#endif

void index_init(void) {
    index_init_cpp();
}

void index_handle_message(const IntentPayload* msg) {
    index_handle_message_cpp(msg);
}

void index_shutdown(void) {
    index_shutdown_cpp();
}

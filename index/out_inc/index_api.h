#ifndef index_API_H
#define index_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// ------------------------------------------------------------
// Intent 種類（UI → ENGINE の命令）
// ------------------------------------------------------------
typedef enum Intent {
    INTENT_INIT = 0,
    INTENT_CANCEL,
    INTENT_BUILD
} Intent;

// ------------------------------------------------------------
// intent ごとの payload 構造体
// ------------------------------------------------------------
typedef struct PayloadBuild {
    const char* path;
} PayloadBuild;

typedef struct PayloadCancel {
    const char dummy;
} PayloadCancel;



// ------------------------------------------------------------
// IntentPayload（TaskManager のキューに積む最小単位）
// ------------------------------------------------------------
typedef struct IntentPayload {
    Intent intent;
    void* payload;
} IntentPayload;

// ------------------------------------------------------------
// C API（UI → Engine の入口）
// ------------------------------------------------------------
void index_init(void);
void index_handle_message(const IntentPayload* msg);
void index_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif // index_API_H


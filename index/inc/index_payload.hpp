#pragma once
#include <cstdint>
#include "index_api.h"

inline IntentPayload* make_intent_payload(Intent intent, void* payload)
{
    IntentPayload* p = new IntentPayload;
    p->intent = intent;
    p->payload = payload;
    return p;
}

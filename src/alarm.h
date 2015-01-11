#pragma once
#include <pebble.h>
extern WakeupId s_wakeup_id;
void wakeup_handler(WakeupId id, int32_t reason);
#pragma once
#include <pebble.h>
#define PERSIST_KEY_WAKEUP_ID 42
extern WakeupId s_wakeup_id;
void wakeup_handler(WakeupId id, int32_t reason);
void check_wakeup();
void select_click_handler(ClickRecognizerRef recognizer, void *context);
void click_config_provider(void *context);
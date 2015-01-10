#pragma once
#include <pebble.h>
#include <recieve.h>
extern WakeupId s_wakeup_id;
void wakeup_handler(WakeupId id, int32_t reason);
void check_wakeup();
void select_click_handler(ClickRecognizerRef recognizer, void *context);
void click_config_provider(void *context);
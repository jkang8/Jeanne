#pragma once
#include <pebble.h>
void tick_handler_medication(struct tm *tick_time, TimeUnits units_changed);
void update_time();

extern TextLayer* s_time_layer;
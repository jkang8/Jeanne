#pragma once

void tick_handler(struct tm*, TimeUnits);
void tick_handler_time(struct tm*, TimeUnits);
void update_time();

extern TextLayer* s_time_layer;
#include <pebble.h>
#include <windowAlarm.h>

// external globals

// static globals
Window *s_window_alarm;

void window_alarm_load(Window *window) {
}

void window_alarm_unload(Window *window) {
}

void window_alarm_init(Window *window) {
  // Create alarm Window
  s_window_alarm = window_create();
  window_set_window_handlers(s_window_alarm, (WindowHandlers) {
    .load = window_alarm_load,
    .unload = window_alarm_unload
  });
  window_set_click_config_provider(s_window_alarm, click_config_provider);
  APP_LOG(APP_LOG_LEVEL_INFO, "pushing Alarm window!");
  window_stack_push(s_window_alarm, true);
}

void window_alarm_deinit(Window *window) {
	window_destroy(s_window_alarm);
}

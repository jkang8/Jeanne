#include <pebble.h>
#include <alarm.h>
#include <windowAlarm.h>

void wakeup_handler(WakeupId id, int32_t reason) {
  APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: wakeup_handler()");
  window_stack_remove(g_window_alarm, true);
  window_stack_push(g_window_alarm, true);
  APP_LOG(APP_LOG_LEVEL_INFO, "push: alarm window");
}

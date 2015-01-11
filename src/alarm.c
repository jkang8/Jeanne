#include <pebble.h>
#include <alarm.h>
#include <windowAlarm.h>

void wakeup_handler(WakeupId id, int32_t reason) {
  APP_LOG(APP_LOG_LEVEL_INFO, "wakeup_handler()");
  APP_LOG(APP_LOG_LEVEL_INFO, "pushing Alarm window!");
  window_stack_remove(g_window_alarm);
  window_stack_push(g_window_alarm, true);
}

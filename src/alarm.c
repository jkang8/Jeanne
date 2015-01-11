#include <pebble.h>
#include <alarm.h>

#define WAKEUP_REASON 0
#define PERSIST_KEY_WAKEUP_ID 42

WakeupId s_wakeup_id;

void wakeup_handler(WakeupId id, int32_t reason) {
  APP_LOG(APP_LOG_LEVEL_INFO, "wakeup_handler()");
  APP_LOG(APP_LOG_LEVEL_INFO, "pushing Alarm window!");
  window_stack_push(s_window_alarm, true);
}

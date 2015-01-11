#include <pebble.h>
#include <alarm.h>
#include <callbacks.h>
#include <TickHandler.h>
#include <recieve.h> 
#include <windowHome.h>
#include <windowTimer.h>
 
static void tap_handler(AccelAxisType axis, int32_t direction) {
  switch (axis) {
  case ACCEL_AXIS_X:
    if (direction != 0) text_layer_set_text(g_main_layer, "Help request sent.");
    break;
  case ACCEL_AXIS_Y:
    if (direction != 0) text_layer_set_text(g_main_layer, "Help request sent.");
    break;
  case ACCEL_AXIS_Z:
    if (direction != 0) text_layer_set_text(g_main_layer, "Help request sent.");
    break;
  }
}
static void init(void) {
  window_home_init();
  window_alarm_init();

  // Subscribe to Wakeup API
  wakeup_service_subscribe(wakeup_handler);

  // Was this a wakeup launch?
  if (launch_reason() == APP_LAUNCH_WAKEUP) {
    // The app was started by a wakeup
    WakeupId id = 0;
    int32_t reason = 0;

    // Get details and handle the wakeup
    wakeup_get_launch_event(&id, &reason);
    wakeup_handler(id, reason);
  }
  
  // Register with TickTimerService to poll the server
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler_medication);//DO NOT ADD ANOTHER SUBSCRIBE LEAVE AS IS
  accel_tap_service_subscribe(tap_handler);
   //
   // Set up message passing to server
   //
  
   // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit(void) {
	window_home_deinit();
	window_alarm_deinit();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

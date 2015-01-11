#include <pebble.h>
#include <alarm.h>
#include <callbacks.h>
#include <TickHandler.h>
#include <recieve.h> 
  
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
  
Window *s_main_window;
TextLayer *s_output_layer;


static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_output_layer = text_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  text_layer_set_text_alignment(s_output_layer, GTextAlignmentCenter);
  int amount = persist_read_int(PERSIST_KEY_AMOUNT);
  char buffer[20];
  snprintf(buffer,20,"%d",amount);
  text_layer_set_text(s_output_layer, amount);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_layer);
}

static void init(void) {
  // Create main Window
  s_main_window = window_create();
  window_set_click_config_provider(s_main_window, click_config_provider);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);

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
  } else {
    // Check whether a wakeup will occur soon
    check_wakeup();
  }
  
  // Register with TickTimerService to poll the server
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
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
  // Destroy main Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

#include <pebble.h>

#define WAKEUP_REASON 0
#define PERSIST_KEY_WAKEUP_ID 42
#define PERSIST_KEY_TIME 1
#define KEY_DRUG 0

  
static Window *s_main_window;
static TextLayer *s_output_layer;

static WakeupId s_wakeup_id;

static void wakeup_handler(WakeupId id, int32_t reason) {
  // The app has woken!
  text_layer_set_text(s_output_layer, "Wakey wakey!");

  // Delete the ID
  persist_delete(PERSIST_KEY_WAKEUP_ID);
}

static void check_wakeup() {
//   // Get the ID
//   s_wakeup_id = persist_read_int(PERSIST_KEY_WAKEUP_ID);

//   if (s_wakeup_id > 0) {
//     // There is a wakeup scheduled soon
//     time_t timestamp = 0;
//     wakeup_query(s_wakeup_id, &timestamp);
//     int seconds_remaining = timestamp - time(NULL);

//     // Show how many seconds to go
//     static char s_buffer[64];
//     snprintf(s_buffer, sizeof(s_buffer), "The event is already scheduled for %d seconds from now!", seconds_remaining);
//     text_layer_set_text(s_output_layer, s_buffer);
//   }
}


void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read first item
  Tuple *t = dict_read_first(iterator);
           
  wakeup_cancel_all();
          time_t future_time = time(NULL) + 5;
         WakeupId x_wakeup_id = wakeup_schedule(future_time,1,true);
         persist_write_int(PERSIST_KEY_WAKEUP_ID, x_wakeup_id);
         APP_LOG(APP_LOG_LEVEL_DEBUG, "made Wakup");
  
  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
      case 0:
        persist_write_int(PERSIST_KEY_TIME, t->value->int32);
        // make_wakeup(persist_read_int(PERSIST_KEY_TIME));

        break;
      case 1:
//         persist_write_string(PERSIST_KEY_DRUG,t->value->cstring);
        break;
      case 2:
//         persist_write_int(PERSIST_KEY_AMOUNT,t->value->int32);
        break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
}


static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
//   //Check the event is not already scheduled
//   if (!wakeup_query(s_wakeup_id, NULL)) {
//     // Current time + 30 seconds
//     time_t future_time = time(NULL) + 30;

//     // Schedule wakeup event and keep the WakeupId
//     s_wakeup_id = wakeup_schedule(future_time, WAKEUP_REASON, true);
//     persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);

//     // Prepare for waking up later
//     text_layer_set_text(s_output_layer, "This app will now wake up in 30 seconds.\n\nClose me!");
//   } else {
//     // Check existing wakeup
//     check_wakeup();
//   }
}



static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_output_layer = text_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  text_layer_set_text_alignment(s_output_layer, GTextAlignmentCenter);
  text_layer_set_text(s_output_layer, "Press SELECT to schedule a Wakeup.");
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_layer);
}

static void init(void) {
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    app_message_register_inbox_received(inbox_received_callback);
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




// void make_wakeup(int time){

//   // Current time + 30 seconds
//   time_t future_time = time(NULL) + 30;

//   // Schedule wakeup event and keep the WakeupId
//   s_wakeup_id = wakeup_schedule(future_time, WAKEUP_REASON, true);
//   persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);

//   // Prepare for waking up later
//   text_layer_set_text(s_output_layer, "This app will now wake up in 30 seconds.\n\nClose me!");

//    wakeup_cancel_all();
//    WakeupId x_wakeup_id = wakeup_schedule(time,1,true);
//    persist_write_int(PERSIST_KEY_WAKEUP_ID, x_wakeup_id);

//    APP_LOG(APP_LOG_LEVEL_DEBUG, "third");
// }

// void make_wakeup(int time){
//    wakeup_cancel_all();
//    WakeupId x_wakeup_id = wakeup_schedule(time,1,true);
//    persist_write_int(PERSIST_KEY_WAKEUP_ID, x_wakeup_id);
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "made Wakup");
// }



/**************************
* Callbacks
****************************/



void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}


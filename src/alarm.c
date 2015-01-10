#include <pebble.h>
#include <alarm.h>
#define WAKEUP_REASON 0
#define PERSIST_KEY_WAKEUP_ID 42
#define number_pills 10
#define medication "viagra"
#define day TODAY
WakeupId s_wakeup_id;
extern TextLayer *s_output_layer;

void wakeup_handler(WakeupId id, int32_t reason) {
  // The app has woken!
  text_layer_set_text(s_output_layer, "Wakey wakey!");
  vibes_long_pulse();
  // Delete the ID
  persist_delete(PERSIST_KEY_WAKEUP_ID);
}

void check_wakeup() {
  // Get the ID
  s_wakeup_id = persist_read_int(PERSIST_KEY_TIME);
  
  if (s_wakeup_id > 0) {
    // There is a wakeup scheduled soon
    time_t timestamp = 0;
    wakeup_query(s_wakeup_id, &timestamp);
    float hours_remaining = (timestamp - time(NULL))/3600;

    // Show how many seconds to go
    static char s_buffer[64];
    snprintf(s_buffer, sizeof(s_buffer), "Take %d %s in %f hours!", number_pills, medication, hours_remaining);
    text_layer_set_text(s_output_layer, s_buffer);
  }
}

void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  //Check the event is not already scheduled
  if (!wakeup_query(s_wakeup_id, NULL)) {
    // Current time + 5 seconds
    time_t future_time = clock_to_timestamp(FRIDAY, 23, 18);

    // Schedule wakeup event and keep the WakeupId
    s_wakeup_id = wakeup_schedule(future_time, WAKEUP_REASON, true);
    persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);

    // Prepare for waking up later
    text_layer_set_text(s_output_layer, "This app will now wake up in 5 seconds.\n\nClose me!");
  } else {
    // Check existing wakeup
    check_wakeup();
  }
}

void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

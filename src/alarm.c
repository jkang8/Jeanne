#include <pebble.h>
#include <alarm.h>
#include <recieve.h>
#define WAKEUP_REASON 0
#define PERSIST_KEY_WAKEUP_ID 42
#define number_pills 10
#define medication "viagra"
#define day TODAY
WakeupId s_wakeup_id;
extern TextLayer *s_output_layer;

void wakeup_handler(WakeupId id, int32_t reason) {
  // Set off alarm
  static char s_buffer[64];
  snprintf(s_buffer, sizeof(s_buffer), "Take %d %s as soon as possible.", number_pills, medication);
  text_layer_set_background_color(s_output_layer, GColorWhite);
  text_layer_set_text_color(s_output_layer, GColorBlack);
  text_layer_set_text_alignment(s_output_layer, GTextAlignmentLeft);
  text_layer_set_text(s_output_layer, s_buffer);
  vibes_long_pulse();
  
  // Delete the ID if yes is pressed
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
    //Placeholder alarm set for 5 seconds from now, in the future we should put how no meds are scheduled to be taken,
    //perhaps check for web connectivity or caregiver input?
    time_t future_time = time(NULL) + 5;
    // Schedule an alarm at the time pulled via receive->JS->website
    //time_t future_time = clock_to_timestamp(FRIDAY, 23, 18);

    // Schedule wakeup event and keep the WakeupId
    s_wakeup_id = wakeup_schedule(future_time, WAKEUP_REASON, true);
    persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);

    // Prepare for waking up later
    text_layer_set_text(s_output_layer, "An alarm will go off in 5 seconds.");
  } else {
    // Check existing wakeup
    check_wakeup();
  }
}

void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

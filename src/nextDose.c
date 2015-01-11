#include <pebble.h>


//Check the event is not already scheduled
  if (!wakeup_query(s_wakeup_id, NULL)) {
    time_t future_time = time(NULL) + 5;
	s_wakeup_id = wakeup_schedule(future_time, WAKEUP_REASON, true);
    persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);



void check_wakeup() {
  // Get the ID
  s_wakeup_id = persist_read_int(PERSIST_KEY_TIME);
  
  if (s_wakeup_id > 0) {
    // There is a wakeup scheduled soon
    time_t timestamp = 0;
    wakeup_query(s_wakeup_id, &timestamp);
    float hours_remaining = (timestamp - time(NULL))/3600;

    // Show how many hours to go
    static char s_buffer[64];
    snprintf(s_buffer, sizeof(s_buffer), "Take %d %s in %f hours!", number_pills, medication, hours_remaining);
    text_layer_set_text(s_output_layer, s_buffer);
  }
}

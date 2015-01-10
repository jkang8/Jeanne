#include <pebble.h>
#include <TickHandler.h>

void tick_handler(struct tm *tick_time, TimeUnits units_changed) {

  // Get medication update every 1 minutes
  if(tick_time->tm_min % 5 == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send();
  }
}
#include <pebble.h>
#include <alarm.h>
#include <recieve.h>

#define WAKEUP_REASON 0
#define PERSIST_KEY_WAKEUP_ID 42
<<<<<<< HEAD
//#define number_pills 10
//#define medication "viagra"
//#define day TODAY

=======
#define medication "viagra"
#define day TODAY
>>>>>>> 41af45cf7af3ffb7574b0e3c674218384f424752
WakeupId s_wakeup_id;
extern TextLayer *s_output_layer;
Window *s_ty_window;

void wakeup_handler(WakeupId id, int32_t reason) {
  // Set off alarm
  //aquire variable values
  int number_pills = persist_read_int(PERSIST_KEY_AMOUNT);
  //int time = persist_read_int(PERSIST_KEY_TIME);
  char drug_buffer[64]; //make more robust?
  persist_read_string(PERSIST_KEY_DRUG,drug_buffer,64);
  
  //print to screen
  static char s_buffer[64];
<<<<<<< HEAD
  snprintf(s_buffer, sizeof(s_buffer), "Take %d %s.", number_pills, drug_buffer);
=======
  int number_pills = persist_read_int(PERSIST_KEY_AMOUNT);
  char drugs[64];
  persist_read_string(PERSIST_KEY_DRUG, drugs, 64);
  snprintf(s_buffer, sizeof(s_buffer), "Take %d %s as soon as possible.", number_pills, drugs);
>>>>>>> 41af45cf7af3ffb7574b0e3c674218384f424752
  text_layer_set_background_color(s_output_layer, GColorWhite);
  text_layer_set_text_color(s_output_layer, GColorBlack);
  text_layer_set_text_alignment(s_output_layer, GTextAlignmentLeft);
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text(s_output_layer, s_buffer);
  
  s_output_layer = text_layer_create(GRect(0, 10, 20, 10));
  text_layer_set_text(s_output_layer, "Taken ->");
  
  s_output_layer = text_layer_create(GRect(0, 20, 20, 10));
  text_layer_set_text(s_output_layer, "Snooze ->");
  
  //vibrates the watch
  vibes_long_pulse();
}

void check_wakeup() {
<<<<<<< HEAD
  //useless stuff for other functions
=======
  // Get the ID
  s_wakeup_id = persist_read_int(PERSIST_KEY_TIME);
  
  if (s_wakeup_id > 0) {
    // There is a wakeup scheduled soon
    time_t timestamp = 0;
    wakeup_query(s_wakeup_id, &timestamp);

    // Show how many seconds to go
    static char s_buffer[64];
    snprintf(s_buffer, sizeof(s_buffer), "Take %d %s!", number_pills, medication);
    text_layer_set_text(s_output_layer, s_buffer);
  }
>>>>>>> 41af45cf7af3ffb7574b0e3c674218384f424752
}

void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	// Delete the ID 
    persist_delete(PERSIST_KEY_WAKEUP_ID);
	
	// Open a thank you window
	s_ty_window = window_create();
	//add bitmap smiley face
	s_output_layer = text_layer_create(GRect(0, 10, 20, 10));
	text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text(s_output_layer, "Thank you!");
	//Move back to main window after 5 seconds?
}

void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	//Schedule an alarm to go off in 5 minutes
	time_t future_time = time(NULL) + 5;    //change to 300 in final version
	s_wakeup_id = wakeup_schedule(future_time, WAKEUP_REASON, true);
    persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);
}

//remove this function in final product
void up_click_handler(ClickRecognizerRef recognizer, void *context) {
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
	}
}

void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler); //set an alarm
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler); //taken
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler); //snooze
}

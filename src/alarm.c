#include <pebble.h>
#include <alarm.h>
#include <recieve.h>
#include <windowHome.h>

#define WAKEUP_REASON 0
#define PERSIST_KEY_WAKEUP_ID 42
#define day TODAY

WakeupId s_wakeup_id;
Window *s_ty_window;

void wakeup_handler(WakeupId id, int32_t reason) {
  // Set off alarm
  //aquire variable values;
  //int time = persist_read_int(PERSIST_KEY_TIME);
  char drugs[64]; //make more robust?
  persist_read_string(PERSIST_KEY_DRUG,drugs,64);
  
  //print to screen
  static char s_buffer[64];
  int number_pills = persist_read_int(PERSIST_KEY_AMOUNT);
  snprintf(s_buffer, sizeof(s_buffer), "Take %d %s as soon as possible.", number_pills, drugs);
  text_layer_set_background_color(g_main_layer, GColorWhite);
  text_layer_set_text_color(g_main_layer, GColorBlack);
  text_layer_set_text_alignment(g_main_layer, GTextAlignmentLeft);
  text_layer_set_font(g_main_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text(g_main_layer, s_buffer);
  
  g_main_layer = text_layer_create(GRect(0, 10, 20, 10));
  text_layer_set_text(g_main_layer, "Taken ->");
  
  g_main_layer = text_layer_create(GRect(0, 20, 20, 10));
  text_layer_set_text(g_main_layer, "Snooze ->");
  
  //vibrates the watch
  vibes_long_pulse();
}

void check_wakeup() {
  // Get the ID
  s_wakeup_id = persist_read_int(PERSIST_KEY_TIME);
  
  if (s_wakeup_id > 0) {
    // There is a wakeup scheduled soon
    time_t timestamp = 0;
    wakeup_query(s_wakeup_id, &timestamp);
    int number_pills = persist_read_int(PERSIST_KEY_AMOUNT);
    char drugs[64];
    persist_read_string(PERSIST_KEY_DRUG, drugs, 64);
    // Show how many seconds to go
    static char s_buffer[64];
    snprintf(s_buffer, sizeof(s_buffer), "Take %d %s!", number_pills, drugs);
    text_layer_set_text(g_main_layer, s_buffer);
  }
}

void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	// Delete the ID 
    persist_delete(PERSIST_KEY_WAKEUP_ID);
	
	// Open a thank you window
	s_ty_window = window_create();
	//add bitmap smiley face
	g_main_layer = text_layer_create(GRect(0, 10, 20, 10));
	text_layer_set_font(g_main_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text(g_main_layer, "Thank you!");
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
    text_layer_set_text(g_main_layer, "An alarm will go off in 5 seconds.");
	}
}

void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler); //set an alarm
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler); //taken
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler); //snooze
}

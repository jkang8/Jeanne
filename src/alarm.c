#include <pebble.h>
#include <alarm.h>
<<<<<<< HEAD
#include <recieve.h>

#define WAKEUP_REASON 0
#define PERSIST_KEY_WAKEUP_ID 42
//#define number_pills 10
//#define medication "viagra"
//#define day TODAY

WakeupId s_wakeup_id;
extern TextLayer *s_output_layer;
static TextLayer *s_alarm_layer;
static TextLayer *s_taken_layer;
static TextLayer *s_snooze_layer;
static TextLayer *s_ty_layer;
static Window *s_alarm_window;
ActionBarLayer *action_bar;

static void alarm_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Acquire variable values
  //int time = persist_read_int(PERSIST_KEY_TIME);
  int number_pills = persist_read_int(PERSIST_KEY_AMOUNT);
  char drug_buffer[64]; //make more robust?
  persist_read_string(PERSIST_KEY_DRUG,drug_buffer,64);

  // Initialize action bar
  action_bar = action_bar_layer_create();
  // Associate the action bar with the window:
  action_bar_layer_add_to_window(action_bar, window);
  // Set the click config provider:
  action_bar_layer_set_click_config_provider(action_bar, click_config_provider);
  // Set the icons
//  action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, &my_icon_previous);
//  action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, &my_icon_next);
  
  // Create alarm TextLayer
  static char s_buffer[64];
  snprintf(s_buffer, sizeof(s_buffer), "It is time to take %d %s.", number_pills, drug_buffer);
  s_alarm_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 5, 64));
  text_layer_set_text_alignment(s_alarm_layer, GTextAlignmentCenter);
  text_layer_set_font(s_alarm_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_overflow_mode(s_alarm_layer, GTextOverflowModeWordWrap);
  
  // Alarm layer
  layer_add_child(window_layer, text_layer_get_layer(s_alarm_layer));
  text_layer_set_text(s_alarm_layer, s_buffer);
  
  // Taken (select) layer
  s_taken_layer = text_layer_create(GRect(0, 64, window_bounds.size.w-40, 30));
  text_layer_set_text_alignment(s_taken_layer, GTextAlignmentCenter);
  text_layer_set_font(s_taken_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text(s_taken_layer, "Taken  ->");
  layer_add_child(window_layer, text_layer_get_layer(s_taken_layer));
  
  // Snooze (down) layer
  s_snooze_layer = text_layer_create(GRect(0, 120, window_bounds.size.w-40, 30));
  text_layer_set_text_alignment(s_snooze_layer, GTextAlignmentCenter);
  text_layer_set_font(s_snooze_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text(s_snooze_layer, "Snooze  ->");
  layer_add_child(window_layer, text_layer_get_layer(s_snooze_layer));
  
  //vibrates the watch
  vibes_long_pulse();
}

static void alarm_window_unload(Window *window) {
  // Destroy TextLayers
  text_layer_destroy(s_alarm_layer);
  text_layer_destroy(s_taken_layer);
  text_layer_destroy(s_snooze_layer);
}

static void wakeInit() {
  // Create alarm Window
  s_alarm_window = window_create();
  window_set_window_handlers(s_alarm_window, (WindowHandlers) {
    .load = alarm_window_load,
    .unload = alarm_window_unload
  });
  window_set_click_config_provider(s_alarm_window, click_config_provider);
  APP_LOG(APP_LOG_LEVEL_INFO, "Window ready to push!");
  window_stack_push(s_alarm_window, true);
}

static void wakeDeinit() {
  // Destroy alarm Window
  window_destroy(s_alarm_window);
}

/////////////////////////////////////////////////////////////////////////////////////////////
//esentially wakeup main
void wakeup_handler(WakeupId id, int32_t reason) {
  // Set off alarm
  wakeInit();
  app_event_loop();
  wakeDeinit();
}

void check_wakeup() {
  // No longer applicable
}

void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  // Delete the ID 
    persist_delete(PERSIST_KEY_WAKEUP_ID);
	
	// Kill alarm layers
	action_bar_layer_destroy(ActionBarLayer *action_bar);
	// Add bitmap smiley face
	s_ty_layer = text_layer_create(GRect(0, 100, 144, 86));
	text_layer_set_text_alignment(s_ty_layer, GTextAlignmentCenter);
	text_layer_set_font(s_ty_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text(s_ty_layer, "Thank you!");
	//Wait 3 seconds
	psleep(300); 
	//call deinit?
}

void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	//Schedule an alarm to go off in 5 minutes
	time_t future_time = time(NULL) + 2;    //change to 300 in final version
	s_wakeup_id = wakeup_schedule(future_time, WAKEUP_REASON, true);
    persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);
}

//remove this function in final product
void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Up clicked!");
  //Placeholder alarm set for 2 seconds from now, in the future we should put how no meds are scheduled to be taken,
  //perhaps check for web connectivity or caregiver input?
  time_t future_time = time(NULL) + 2;
  // Schedule an alarm at the time pulled via receive->JS->website
  //time_t future_time = clock_to_timestamp(FRIDAY, 23, 18);

  // Schedule wakeup event and keep the WakeupId
  s_wakeup_id = wakeup_schedule(future_time, WAKEUP_REASON, true);
  persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);

  // Prepare for waking up later
  //text_layer_set_text(s_output_layer, "An alarm will go off in 2 seconds.");
}

void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler); //set an alarm
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler); //taken
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler); //snooze
=======
#include <windowAlarm.h>

void wakeup_handler(WakeupId id, int32_t reason) {
  APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: wakeup_handler()");
  window_stack_remove(g_window_alarm, true);
  window_stack_push(g_window_alarm, true);
  APP_LOG(APP_LOG_LEVEL_INFO, "push: alarm window");
>>>>>>> a0350ff1fe183f6e7494e1950080b479fe41e12d
}

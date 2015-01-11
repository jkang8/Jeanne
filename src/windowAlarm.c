#include <pebble.h>
#include <windowAlarm.h>
#include <windowHome.h>
#include <recieve.h>

#define WAKEUP_REASON 0

// external globals

// static globals
Window *g_window_alarm;

static TextLayer *s_alarm_layer;
static TextLayer *s_taken_layer;
static TextLayer *s_snooze_layer;
static TextLayer *s_ty_layer;
static Window *s_alarm_window;
static ActionBarLayer *s_action_bar;
static WakeupId s_wakeup_id;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: select_click_handler()");
    // Delete the ID 
    //persist_delete(PERSIST_KEY_WAKEUP_ID);
	
	// Kill alarm layers
	action_bar_layer_destroy(s_action_bar);
	
	// Add bitmap smiley face
	s_ty_layer = text_layer_create(GRect(0, 100, 144, 86));
	text_layer_set_text_alignment(s_ty_layer, GTextAlignmentCenter);
	text_layer_set_font(s_ty_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text(s_ty_layer, "Thank you!");
	//Wait 3 seconds
	psleep(300); 
	window_stack_remove(g_window_home,true);
	window_stack_push(g_window_home,true);
	APP_LOG(APP_LOG_LEVEL_INFO, "push: home window");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: down_click_handler()");
	//Schedule an alarm to go off in 5 minutes
	time_t future_time = time(NULL) + 2;    //change to 300 in final version
	s_wakeup_id = wakeup_schedule(future_time, WAKEUP_REASON, true);
    persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);
}

//remove this function in final product
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: up_click_handler()");
  //Placeholder alarm set for 2 seconds from now, in the future we should put how no meds are scheduled to be taken,
  //perhaps check for web connectivity or caregiver input?
  time_t future_time = time(NULL) + 2;
  // Schedule an alarm at the time pulled via receive->JS->website
  //time_t future_time = clock_to_timestamp(FRIDAY, 23, 18);

  // Schedule wakeup event and keep the WakeupId
  s_wakeup_id = wakeup_schedule(future_time, WAKEUP_REASON, true);
  persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: click_config_provider()");
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler); //set an alarm
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler); //taken
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler); //snooze
}

void window_alarm_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: window_alarm_load()");
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Acquire variable values
  //int time = persist_read_int(PERSIST_KEY_TIME);
  int number_pills = persist_read_int(PERSIST_KEY_AMOUNT);
  char drug_buffer[64]; //make more robust?
  persist_read_string(PERSIST_KEY_DRUG,drug_buffer,64);

  // Initialize action bar
  s_action_bar = action_bar_layer_create();
  // Associate the action bar with the window:
  action_bar_layer_add_to_window(s_action_bar, window);
  // Set the click config provider:
  action_bar_layer_set_click_config_provider(s_action_bar, click_config_provider);
  // Set the icons
//  action_bar_layer_set_icon(s_action_bar, BUTTON_ID_SELECT, &my_icon_previous);
//  action_bar_layer_set_icon(s_action_bar, BUTTON_ID_DOWN, &my_icon_next);

  // Create alarm TextLayer
  static char s_buffer[64];
  snprintf(s_buffer, sizeof(s_buffer), "It is time to take %d %s.", number_pills, drug_buffer);
  s_alarm_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 5, 64));
  /*text_layer_set_background_color(s_alarm_layer, GColorBlack);
  APP_LOG(APP_LOG_LEVEL_INFO, "Background colored!");
  text_layer_set_text_color(s_alarm_layer, GColorWhite);
  APP_LOG(APP_LOG_LEVEL_INFO, "Text colored!");*/
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

void window_alarm_unload(Window *window) {
	APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: window_alarm_unload()");
  // Destroy TextLayers
  text_layer_destroy(s_alarm_layer);
  text_layer_destroy(s_taken_layer);
  text_layer_destroy(s_snooze_layer);
}

void window_alarm_init(Window *window) {
	APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: window_alarm_init()");
  // Create alarm Window
  g_window_alarm = window_create();
  window_set_window_handlers(g_window_alarm, (WindowHandlers) {
    .load = window_alarm_load,
    .unload = window_alarm_unload
  });
}

void window_alarm_deinit(Window *window) {
	APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: window_alarm_deinit()");
	window_destroy(g_window_alarm);
}

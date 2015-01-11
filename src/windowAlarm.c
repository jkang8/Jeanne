#include <pebble.h>
#include <windowAlarm.h>
#include <windowHome.h>
#include <recieve.h>

#define WAKEUP_REASON 0

// external globals
Window *g_window_alarm;
Window *g_window_thanks;

// static globals
static TextLayer *s_alarm_layer;
static TextLayer *s_taken_layer;
static TextLayer *s_snooze_layer;
static ActionBarLayer *s_action_bar;
static WakeupId s_wakeup_id;
static GBitmap *s_check_bitmap;
static GBitmap *s_click_bitmap;
static GBitmap *s_snooze_bitmap;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: select_click_handler()");
    // Delete the ID 
    //persist_delete(PERSIST_KEY_WAKEUP_ID);
	 
	window_stack_remove(g_window_thanks,true);
	window_stack_push(g_window_thanks,true);
	APP_LOG(APP_LOG_LEVEL_INFO, "push: thanks window");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: down_click_handler()");
	//Schedule an alarm to go off in 5 minutes
	time_t future_time = time(NULL) + 2;    //change to 300 in final version
	s_wakeup_id = wakeup_schedule(future_time, WAKEUP_REASON, true);
    persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);
}

//remove this function in final product
/*static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: up_click_handler()");
  //Placeholder alarm set for 2 seconds from now, in the future we should put how no meds are scheduled to be taken,
  //perhaps check for web connectivity or caregiver input?
  time_t future_time = time(NULL) + 2;
  // Schedule an alarm at the time pulled via receive->JS->website
  //time_t future_time = clock_to_timestamp(FRIDAY, 23, 18);

  // Schedule wakeup event and keep the WakeupId
  s_wakeup_id = wakeup_schedule(future_time, WAKEUP_REASON, true);
  persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);
}*/

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: click_config_provider()");
  //window_single_click_subscribe(BUTTON_ID_UP, up_click_handler); //set an alarm
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
  APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: before action_bar_layer_set_click_config_provider()");
  action_bar_layer_set_click_config_provider(s_action_bar, click_config_provider);
  APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: after action_bar_layer_set_click_config_provider()");

  // Set the icons
  s_check_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHECK);
  s_click_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CLICK);
  s_snooze_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SNOOZE);
  
  //action_bar_layer_set_icon(s_action_bar, BUTTON_ID_UP,     s_click_bitmap); 
  action_bar_layer_set_icon(s_action_bar, BUTTON_ID_SELECT, s_check_bitmap);
  action_bar_layer_set_icon(s_action_bar, BUTTON_ID_DOWN,   s_snooze_bitmap);

  // Alarm TextLayer
  static char s_buffer[64];
  snprintf(s_buffer, sizeof(s_buffer), "Take %d %s now.", number_pills, drug_buffer);
  s_alarm_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 40, 64));
  text_layer_set_text_alignment(s_alarm_layer, GTextAlignmentCenter);
  text_layer_set_font(s_alarm_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_overflow_mode(s_alarm_layer, GTextOverflowModeWordWrap);
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
  // Destroy Layers
  text_layer_destroy(s_alarm_layer);
  text_layer_destroy(s_taken_layer);
  text_layer_destroy(s_snooze_layer);
  
  gbitmap_destroy(s_check_bitmap);
  gbitmap_destroy(s_click_bitmap);
  gbitmap_destroy(s_snooze_bitmap);
  
  action_bar_layer_destroy(s_action_bar);
}

void window_alarm_init(Window *window) {
	APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: window_alarm_init()");
  // Create alarm Window
  g_window_alarm = window_create();
  window_set_fullscreen(g_window_alarm,true);
  window_set_window_handlers(g_window_alarm, (WindowHandlers) {
    .load = window_alarm_load,
    .unload = window_alarm_unload
  });
}

void window_alarm_deinit(Window *window) {
	APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: window_alarm_deinit()");
	window_destroy(g_window_alarm);
}

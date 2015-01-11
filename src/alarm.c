#include <pebble.h>
#include <alarm.h>
#include <recieve.h>
#include <windowHome.h>

#define WAKEUP_REASON 0
#define PERSIST_KEY_WAKEUP_ID 42
<<<<<<< HEAD
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
=======
#define day TODAY

WakeupId s_wakeup_id;
Window *s_ty_window;

void wakeup_handler(WakeupId id, int32_t reason) {
  // Set off alarm
  //aquire variable values;
  //int time = persist_read_int(PERSIST_KEY_TIME);
  char drugs[64]; //make more robust?
  persist_read_string(PERSIST_KEY_DRUG,drugs,64);
>>>>>>> 65df900d90887b92b70677c62ffd1698dc06ec01
  
  // Create alarm TextLayer
  static char s_buffer[64];
<<<<<<< HEAD
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
=======
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
>>>>>>> 65df900d90887b92b70677c62ffd1698dc06ec01
  
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
<<<<<<< HEAD
  // No longer applicable
=======
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
>>>>>>> 65df900d90887b92b70677c62ffd1698dc06ec01
}

void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  // Delete the ID 
    persist_delete(PERSIST_KEY_WAKEUP_ID);
	
<<<<<<< HEAD
	// Kill alarm layers
	void action_bar_layer_destroy(ActionBarLayer *action_bar);
	// Add bitmap smiley face
	s_ty_layer = text_layer_create(GRect(0, 100, 144, 86));
	text_layer_set_text_alignment(s_ty_layer, GTextAlignmentCenter);
	text_layer_set_font(s_ty_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text(s_ty_layer, "Thank you!");
	//Wait 3 seconds
	psleep(300); 
	//call deinit?
=======
	// Open a thank you window
	s_ty_window = window_create();
	//add bitmap smiley face
	g_main_layer = text_layer_create(GRect(0, 10, 20, 10));
	text_layer_set_font(g_main_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text(g_main_layer, "Thank you!");
	//Move back to main window after 5 seconds?
>>>>>>> 65df900d90887b92b70677c62ffd1698dc06ec01
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

<<<<<<< HEAD
  // Prepare for waking up later
  //text_layer_set_text(s_output_layer, "An alarm will go off in 2 seconds.");
=======
    // Prepare for waking up later
    text_layer_set_text(g_main_layer, "An alarm will go off in 5 seconds.");
	}
>>>>>>> 65df900d90887b92b70677c62ffd1698dc06ec01
}

void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler); //set an alarm
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler); //taken
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler); //snooze
}

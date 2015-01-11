#include <pebble.h>
#include <windowHome.h>
#include <receive.h>
#include <TickHandler.h>
#include <alarm.h>

// external globals
TextLayer *g_main_layer;

// static globals
static Window *s_window_home;

static void tap_handler(AccelAxisType axis, int32_t direction) {
  switch (axis) {
  case ACCEL_AXIS_X:
    if (direction != 0) text_layer_set_text(g_main_layer, "Help request sent.");
    break;
  case ACCEL_AXIS_Y:
    if (direction != 0) text_layer_set_text(g_main_layer, "Help request sent.");
    break;
  case ACCEL_AXIS_Z:
    if (direction != 0) text_layer_set_text(g_main_layer, "Help request sent.");
    break;
  }
}

void window_home_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  g_main_layer = text_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  text_layer_set_text_alignment(g_main_layer, GTextAlignmentCenter);
	text_layer_set_font(g_main_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text(g_main_layer, "Tap for help.");
  
  // Display time on main window
  layer_add_child(window_layer, text_layer_get_layer(g_main_layer)); 

  accel_tap_service_subscribe(tap_handler);  
  
}

void window_home_unload(Window *window) {
  accel_tap_service_unsubscribe(tap_handler);
  // Destroy output TextLayer
  text_layer_destroy(g_main_layer);
  
}

void window_home_init(Window *window) {
  // Create main Window
  s_window_home = window_create();
  window_set_click_config_provider(s_window_home, click_config_provider);
  window_set_fullscreen(s_window_home,true);
  window_set_window_handlers(s_window_home, (WindowHandlers) {
    .load = window_home_load,
    .unload = window_home_unload,
  });
  window_stack_push(s_window_home, true);
}

void window_home_deinit(Window *window) {
  window_destroy(s_window_home);
}

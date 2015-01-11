#include <pebble.h>
#include <windowHome.h>
#include <windowHelp.h>
#include <recieve.h>
#include <TickHandler.h>
#include <alarm.h>

// external globals
Window *g_window_home;

// static globals
static TextLayer *s_home_layer;

static void tap_handler(AccelAxisType axis, int32_t direction) {
  APP_LOG(APP_LOG_LEVEL_INFO, "tap_handler()");
  switch (axis) {
  case ACCEL_AXIS_X:
  case ACCEL_AXIS_Y:
  case ACCEL_AXIS_Z:
    if (direction != 0) {
	  window_stack_remove(g_window_help, true);
      window_stack_push(g_window_help, true);
	  APP_LOG(APP_LOG_LEVEL_INFO, "push: help window");
	}
    break;
  }
}

void window_home_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_home_layer = text_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  text_layer_set_text_alignment(s_home_layer, GTextAlignmentCenter);
  text_layer_set_font(s_home_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text(s_home_layer, "Tap for help.");
  
  // Display time on main window
  layer_add_child(window_layer, text_layer_get_layer(s_home_layer)); 

  accel_tap_service_subscribe(tap_handler);  
}

void window_home_unload(Window *window) {
  accel_tap_service_unsubscribe();
  text_layer_destroy(s_home_layer);
}

void window_home_init(Window *window) {
  // Create main Window
  g_window_home = window_create();
  window_set_fullscreen(g_window_home,true);
  window_set_window_handlers(g_window_home, (WindowHandlers) {
    .load = window_home_load,
    .unload = window_home_unload,
  });
  window_stack_push(g_window_home, true);
  APP_LOG(APP_LOG_LEVEL_INFO, "push: home window");
}

void window_home_deinit(Window *window) {
  window_destroy(g_window_home);
}

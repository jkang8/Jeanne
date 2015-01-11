#include <pebble.h>
#include <windowHelp.h>

// external globals
Window *g_window_help;

// static globals
static TextLayer *s_help_layer;

void window_help_load(Window *window) {
  vibes_long_pulse();
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_help_layer = text_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  text_layer_set_text_alignment(s_help_layer, GTextAlignmentCenter);
  text_layer_set_font(s_help_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text(s_help_layer, "Help is on the way!");
  
  layer_add_child(window_layer, text_layer_get_layer(s_help_layer)); 
  
  // @TODO!  Send a message to the web server and wait for an acknowledgement from caregiver
  //Wait 3 seconds
  psleep(3000); 
  vibes_long_short();
  window_stack_remove(g_window_home, true);
  window_stack_push(g_window_home, true);
  APP_LOG(APP_LOG_LEVEL_INFO, "push: help window");
}

void window_help_unload(Window *window) {
  text_layer_destroy(s_help_layer);
}

void window_help_init(Window *window) {
  // Create help Window
  g_window_help = window_create();
  window_set_fullscreen(g_window_help,true);
  window_set_window_handlers(g_window_help, (WindowHandlers) {
    .load = window_help_load,
    .unload = window_help_unload,
  });
}

void window_help_deinit(Window *window) {
	window_destroy(g_window_help);
}

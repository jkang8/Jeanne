#include <pebble.h>
#include <windowHome.h>
#include <recieve.h>
#include <TickHandler.h>
#include <alarm.h>

// external globals
TextLayer *g_main_layer;

// static globals
static Window *s_window_home;
static TextLayer *s_time_layer;

void window_home_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  g_main_layer = text_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  text_layer_set_text_alignment(g_main_layer, GTextAlignmentCenter);
  int amount = persist_read_int(PERSIST_KEY_AMOUNT);
  char buffer[20];
  snprintf(buffer,20,"%d",amount);
  text_layer_set_text(g_main_layer, buffer);
  
  // Display time on main window
  layer_add_child(window_layer, text_layer_get_layer(g_main_layer));  
  update_time();
  
  // Add it as a child layer to the Window's root layer
  // CRASHES! s_time_layer is NULL
  //layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

void window_home_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(g_main_layer);
}

void window_home_init(Window *window) {
  // Create main Window
  s_window_home = window_create();
  window_set_click_config_provider(s_window_home, click_config_provider);
  window_set_window_handlers(s_window_home, (WindowHandlers) {
    .load = window_home_load,
    .unload = window_home_unload,
  });
  window_stack_push(s_window_home, true);
}

void window_home_deinit(Window *window) {
}

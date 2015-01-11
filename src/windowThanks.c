#include <pebble.h>
#include <windowThanks.h>
#include <windowHome.h>

// external globals
Window *g_window_thanks;

// static globals
static TextLayer *s_ty_layer;
//static GBitmap *s_smiley_bitmap;

static void tap_handler(AccelAxisType axis, int32_t direction) {
  APP_LOG(APP_LOG_LEVEL_INFO, "help: tap_handler()");
  switch (axis) {
  case ACCEL_AXIS_X:
  case ACCEL_AXIS_Y:
  case ACCEL_AXIS_Z:
    if (direction != 0) {
	  window_stack_remove(g_window_home, true);
      window_stack_push(g_window_home, true);
	  APP_LOG(APP_LOG_LEVEL_INFO, "push: home window");
	}
    break;
  }
}

void window_thanks_load(Window *window) {
	APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: window_thanks_load()");
	
	// Add bitmap smiley face
	
	// Thanks text layer
	Layer *window_layer = window_get_root_layer(window);
	s_ty_layer = text_layer_create(GRect(0, 100, 144, 86));
	text_layer_set_font(s_ty_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text(s_ty_layer, "Thank you! Please tap to confirm.");
    text_layer_set_text_alignment(s_ty_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(s_ty_layer)); 
	accel_tap_service_subscribe(tap_handler); 
}

void window_thanks_unload(Window *window) {
	APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: window_thanks_unload()");
	text_layer_destroy(s_ty_layer);
	accel_tap_service_unsubscribe();
	//gbitmap_destroy(s_smiley_bitmap);
}

void window_thanks_init(Window *window) {
	APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: window_thanks_init()");
  // Create thanks Window
  g_window_thanks = window_create();
  window_set_fullscreen(g_window_thanks,true);
  window_set_window_handlers(g_window_thanks, (WindowHandlers) {
    .load = window_thanks_load,
    .unload = window_thanks_unload
  });
}

void window_thanks_deinit(Window *window) {
	APP_LOG(APP_LOG_LEVEL_INFO, "ALARM: window_thanks_deinit()");
	window_destroy(g_window_thanks);
}

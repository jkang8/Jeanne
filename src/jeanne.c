#include <pebble.h> 

Window *window;
TextLayer *text_layer;

static void init() {
   s_window = window_create();

   window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = main_window_load,
      .unload = main_window_unload
   });
   window_stack_push(s_main_window, true);
   text_layer = text_layer_create(GRect(0, 0, 144, 154));
}

static void deinit() {
   text_layer_destroy(text_layer); 
   window_destroy(window);
}

int main(void) {
   init();
   app_event_loop();
   deinit();
}

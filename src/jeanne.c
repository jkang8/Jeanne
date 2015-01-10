#include <pebble.h> 

Window *window;
TextLayer *text_layer;
MenuLayer *menu_layer;

  
void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context){
   switch(cell_index->row){
      case 0:
         menu_cell_basic_draw(ctx, cell_layer, "Meds", "Set Meds", NULL);
         break;
      case 1:
         menu_cell_basic_draw(ctx, cell_layer, "Exit", NULL, NULL);
		     break;
      
  }
}

uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
	return 2;
}

static void window_load(Window *window){
   //create later
   menu_layer = menu_layer_create(GRect(0, 0, 144, 168 - 16));
   //let menu recieve clicks
   menu_layer_set_click_config_onto_window(menu_layer, window);
   //callbacks
   	MenuLayerCallbacks callbacks = {
		   .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
		   .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback
	};
  menu_layer_set_callbacks(menu_layer, NULL, callbacks);
  layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer))
}

static void window_unload(Window *window)

static void init() {
   window = window_create();

   window_stack_push(window, true); 
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

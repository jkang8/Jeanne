#include <pebble.h>

#define WAKEUP_REASON 0
#define PERSIST_KEY_WAKEUP_ID 42
#define KEY_MESSAGE 1
#define KEY_TIME 2

	
static Window *s_main_window;
static TextLayer *s_output_layer;

static WakeupId s_wakeup_id;

static void wakeup_handler(WakeupId id, int32_t reason) {
	// The app has woken!
	text_layer_set_text(s_output_layer, persist_read_int(KEY_MESSAGE));

	// Delete the ID
	persist_delete(PERSIST_KEY_WAKEUP_ID);
}


void inbox_received_callback(DictionaryIterator *iterator, void *context) {
	// Read first item
	Tuple *t = dict_read_first(iterator);
					 
	

	while(t != NULL) {
		switch(t->key) {
			case KEY_MESSAGE:

				persist_write_int(KEY_MESSAGE, t->value->cstr);

			break;

			case KEY_TIME:

				wakeup_cancel_all();
				WakeupId x_wakeup_id = wakeup_schedule(_time,1,true);
				persist_write_int(PERSIST_KEY_WAKEUP_ID, x_wakeup_id);
				APP_LOG(APP_LOG_LEVEL_DEBUG, "made Wakup");

			break;
		}
		t = dict_read_next(iterator);
	}

}


static void select_click_handler(ClickRecognizerRef recognizer, void *context) {

}



static void click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void main_window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect window_bounds = layer_get_bounds(window_layer);

	// Create output TextLayer
	s_output_layer = text_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
	text_layer_set_text_alignment(s_output_layer, GTextAlignmentCenter);
	text_layer_set_text(s_output_layer, "Press SELECT to schedule a Wakeup.");
	layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
}

static void main_window_unload(Window *window) {
	// Destroy output TextLayer
	text_layer_destroy(s_output_layer);
}

static void init(void) {
	app_message_register_inbox_received(inbox_received_callback);

	app_message_open(
		app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

	// Create main Window
	s_main_window = window_create();
	window_set_click_config_provider(s_main_window, click_config_provider);
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload,
	});

	window_stack_push(s_main_window, true);

	// Subscribe to Wakeup API
	wakeup_service_subscribe(wakeup_handler);

	// Was this a wakeup launch?
	if (launch_reason() == APP_LAUNCH_WAKEUP) {
		// The app was started by a wakeup
		WakeupId id = 0;
		int32_t reason = 0;

		// Get details and handle the wakeup
		wakeup_get_launch_event(&id, &reason);
		wakeup_handler(id, reason);
	} 

}

static void deinit(void) {
	// Destroy main Window
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}



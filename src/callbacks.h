#pragma once
#include <pebble.h>

extern void inbox_received_callback(DictionaryIterator *iterator, void *context);
extern void inbox_dropped_callback(AppMessageResult reason, void *context);
extern void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
extern void outbox_sent_callback(DictionaryIterator *iterator, void *context);
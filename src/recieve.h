#pragma once
#include <pebble.h>
#define PERSIST_KEY_DRUG 4
#define PERSIST_KEY_TIME 5
#define PERSIST_KEY_AMOUNT 6
void receive_key_value(Tuple *t);
void make_wakeup(int time);
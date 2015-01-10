#include <pebble.h>
#include "recieve.h"

void receive_key_value(Tuple *t) {
  //Function will put the time drug and amount values into persistent data
  switch(t->key) {
    case 0:
      persist_write_int(PERSIST_KEY_TIME, (int)t->value);
      break;
    case 1:
      persist_write_string(PERSIST_KEY_DRUG,t->value->cstring);
      break;
    case 2:
      persist_write_int(PERSIST_KEY_AMOUNT,(int)t->value);
      break;
  }
}

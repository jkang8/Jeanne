#include <pebble.h>
#include "recieve.h"

void receive_key_value(int key,void value) {
  //Function will put the time drug and amount values into persistent data
  switch(key) {
    case 0:
      persist_write_int(PERSIST_KEY_TIME,(int)value);
      break;
    case 1:
      persist_write_string(PERSIST_KEY_DRUG,(char*)value);
      break;
    case 2:
      persist_write_int(PERSIST_KEY_AMOUNT,(int)value);
      break;
  }
}

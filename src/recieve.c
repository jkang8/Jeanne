#include <pebble.h>
#include <alarm.h>
#include "recieve.h"
extern int PERSIST_KEY_TIME;
extern int PERSIST_KEY_DRUG;
extern int PERSIST_KEY_AMOUNT;
void recieve_key_value(int key,void* value){
  switch(key) {
    case 0:
      persist_write_int(PERSIST_KEY_TIME,value);
    case 1:
       persist_write_int(PERSIST_KEY_DRUG,value);
    case 2:
       persist_write_int(PERSIST_KEY_AMOUNT,value);
    
  }
}

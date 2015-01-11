#include <pebble.h>
#include "recieve.h"
#include <alarm.h>

void receive_key_value(Tuple *t) {
  //Function will put the time drug and amount values into persistent data
  switch(t->key) {
    case 0:
      persist_write_int(PERSIST_KEY_TIME, t->value->int32);
  	  make_wakeup(persist_read_int(PERSIST_KEY_TIME));
	  //make_wakeup(time(NULL)+5);
      break;
    case 1:
      persist_write_string(PERSIST_KEY_DRUG,t->value->cstring);
      break;
    case 2:
      persist_write_int(PERSIST_KEY_AMOUNT,t->value->int32);
      break;
  }
}

void make_wakeup(int time){
   wakeup_cancel_all();
   APP_LOG(APP_LOG_LEVEL_DEBUG, "first");
   WakeupId x_wakeup_id = wakeup_schedule(time,1,true);
   APP_LOG(APP_LOG_LEVEL_DEBUG, "second");
   persist_write_int(PERSIST_KEY_WAKEUP_ID, x_wakeup_id);
   APP_LOG(APP_LOG_LEVEL_DEBUG, "third");
}

#include <pebble.h>

#include "communication.h"
#include "icon.h"
#include "menuset.h"
#include "menurest.h"
#include "menuicon.h"
#include "menuact.h"
#include "Ambivibe2.h"

  
extern uint8_t participant_num;
extern uint8_t icon;
extern AppTimer *timer_handle;
extern int guessedIndex;
extern int certainty;
extern uint8_t lastMinSent;
extern uint8_t lastHourSent;
extern uint8_t lastSecSent;
DataLoggingSessionRef data_log;

extern Window *window_set;
extern Window *window_rest;
extern Window *window_icon;
extern Window *window_act;

//////////////////////////////////////////////////
// Sending Message

// Constructs a message to record details of trail and outcome
void construct_and_send_message(uint8_t userResponse, uint8_t activityIndex) {
  //if response felt, set guessed_icon_index to saved value (associated with window_icon)
  //if user did not feel response/did not have time set the guessedIndex to NUM_ICONS_PER_SETS(garbage value)
  uint8_t iconIndex =  (userResponse != USER_RESPONSE_FELT) ? NUM_ICONS_PER_SETS : guessedIndex;
  uint8_t guessedIcon = (iconIndex == NUM_ICONS_PER_SETS) ? 0 : iconIndex + 1; // '?' is 10th index but returned as 0
   
  time_t rawtime;
  time(&rawtime);
  struct tm *info = localtime( &rawtime );
  uint8_t respHour = (uint8_t) (info->tm_hour); //current hour
  uint8_t respMin = (uint8_t) (info->tm_min); //current min
  uint8_t respSec = (uint8_t) (info->tm_sec); //current sec
  uint8_t day = (uint8_t) (info->tm_mday); // day of month
  uint8_t month = (uint8_t) (info->tm_mon) + 1; //month
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "sent icon: %d \n user response: %d \n guessed icon: %d \n activity index: %d", icon + 1, userResponse, guessedIcon, activityIndex);
  uint8_t message[NUM_RET_MSG_FIELDS] = {participant_num, userResponse, icon + 1, (uint8_t) guessedIcon, activityIndex, (uint8_t)(certainty + 1), lastHourSent, lastMinSent, lastSecSent, respHour, respMin, respSec, month, day};
  
  DataLoggingResult r = data_logging_log(data_log, message, 1);
  if (r != DATA_LOGGING_SUCCESS){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "LOGGING ERROR\n"); 
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "DATA LOGGED\n"); 
  }
  
  send_message(NUM_RET_MSG_FIELDS, message); //send response to server
  
  window_stack_pop_all(false);
  window_stack_push(window_set, false);
  window_stack_push(window_rest, true);
  icon = NO_VIBRATION_SENT;
}

//called when user does not respond to a message after 1 minute
void construct_and_send_no_response_message(void *data) {
  certainty = -1;
  construct_and_send_message(USER_NO_RESPONSE, NUM_ACTIVITIES_PER_SETS);
}


// Send results of trial to phone
void send_message(uint8_t length, uint8_t *message) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  if (iter == NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "null iter");
    return;
  }
  Tuplet tuple = (message == NULL)
                      ? TupletInteger(0, 0)
                      : TupletBytes(0, message, length);
  dict_write_tuplet(iter, &tuple);
  dict_write_end(iter);

  app_message_outbox_send();
  return;
}


static void in_received_handler(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Received!");
  setupFirstLaunch();
  
}

//////////////////////////////////////////////////

static void in_dropped_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Dropped!");
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Failed to Send!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void init_communication()
{
  data_log = data_logging_create(
    /* tag */                 LOG_TAG,
    /* DataLoggingItemType */ DATA_LOGGING_BYTE_ARRAY,
    /* length */              NUM_RET_MSG_FIELDS,
    /* resume */              true
  );
  
  // Register message handlers
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_failed(out_failed_handler);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  
}

void deinit_communication(){
  data_logging_finish(data_log);
}

//////////////////////////////////////////////////

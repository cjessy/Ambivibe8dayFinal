#include <pebble.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
  
  
#include "menuset.h"
#include "setup.h"
#include "menuact.h"
#include "menuicon.h"
#include "icon.h"
#include "communication.h"
#include "menurest.h"
#include "timeutils.h"
#include "menucert.h"
  
//#define ICON_INDEX_KEY 41
#define ICON_ARRAY_KEY 42
#define IS_FIRST_INIT_KEY 43
#define PARTICIPANT_NUM_KEY 45
#define LAST_MIN_KEY 46
#define LAST_HOUR_KEY 47
#define NOT_FELT_KEY 48
#define NEXT_SCHEDULED_KEY 49
#define LAST_SEC_KEY 50
#define WAKEUP_REASON 0

extern Window *window_set;
extern Window *window_setup;
extern Window *window_icon;
extern Window *window_rest;
extern Window *window_act;
extern int guessedIndex;
extern int notFelt;

uint8_t participant_num;
uint8_t icon = NO_VIBRATION_SENT; //default = no vibration sent
uint8_t lastMinSent;
uint8_t lastHourSent;
uint8_t lastSecSent;
//int icon_index;
AppTimer *timer_handle;
time_t next_time;

//static uint8_t icons[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; 
static WakeupId s_wakeup_id;


extern DataLoggingSessionRef data_log;

// returns a random int from 0 to 54 
// (leave a few minutes at end to prevent edge case where two vibrations are very close)
static int rand_min(){
//   srand((unsigned)time(NULL));
  return (int) ((rand() / (RAND_MAX + 1.)) * 55);
}

static int rand_icon(){
  return (rand() % 10);
}

// schedules a vibration for the given day and hour (random minute) and returns 
// the random minute of the hour the vibration should be sent
static int schedule_vibration(WeekDay day, int newHour){
  int newMin = rand_min(); //random minute of the hour
  //time_t future_time = clock_to_timestamp(day, newHour, newMin);
  next_time = clock_to_timestamp(day, newHour, newMin);
  s_wakeup_id = wakeup_schedule(next_time, WAKEUP_REASON, true); //schedule next vibration
  
  char buff[20];
  strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&next_time));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "vibration scheduled for: %s \n", buff);
  return newMin;
}


// shuffles the icons array (should be called once before the beginning of each test day)
// static void shuffle() {    
//     srand((unsigned)time(NULL));
//     for (size_t i = NUM_ICONS_PER_SETS - 1; i > 0; i--) {
//       double random = rand() / (RAND_MAX + 1.);
//       size_t j = (unsigned int) (random * (i+1));
//       uint8_t t = icons[j];
//       icons[j] = icons[i];
//       icons[i] = t;
//     }
// }

static void wakeup_handler(WakeupId id, int32_t reason) {
  if (window_stack_get_top_window() == window_set && icon != NO_VIBRATION_SENT){ //no response to last vibration
     app_timer_cancel(timer_handle);
     construct_and_send_no_response_message(NULL);
  } 
  icon = (uint8_t) rand_icon();
  play_icon(participant_num % 2, icon); //odd participant num = presignal
  
//   if (icon_index >= 9){
//     shuffle();
//     icon_index = 0;
//   }
  
  set_last_sent_time();
  //lastMinSent = (uint8_t)get_curr_min();
  //lastHourSent = (uint8_t)get_curr_hour();
  
  //Clear previous windows
  window_stack_pop_all(false);
  // Load the questionnaire
  window_stack_push(window_set, true);
  
  // If no response, log no response
  timer_handle = app_timer_register(TIMEOUT_MS, construct_and_send_no_response_message, NULL);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "icon is: %d \n", icon + 1);
  //icon_index++;
  
  int curr_hour = get_curr_hour();
  
  if (curr_hour >= END_TIME){
//     shuffle();
//     icon_index = 0;
    schedule_vibration(get_tomorrow(), START_TIME);
  } else {
    schedule_vibration(TODAY, curr_hour + 1); 
  }
}

static void getPersistentData(){
  if (persist_exists(NEXT_SCHEDULED_KEY)) {
     persist_read_data(NEXT_SCHEDULED_KEY, &next_time, sizeof(next_time));
  }
//   if (persist_exists(ICON_ARRAY_KEY)) { 
//      persist_read_data(ICON_ARRAY_KEY, icons, sizeof(icons)); //get array of icons from last launch
//   }
  
//   if (persist_exists(ICON_INDEX_KEY)) { 
//      icon_index = persist_read_int(ICON_INDEX_KEY); //get index in array from last launch
//   }
  
  if (persist_exists(PARTICIPANT_NUM_KEY)) { 
     participant_num = persist_read_int(PARTICIPANT_NUM_KEY); //get index in array from last launch
  }
    
  if (persist_exists(LAST_MIN_KEY)) { 
     lastMinSent = (uint8_t) persist_read_int(LAST_MIN_KEY); //get index in array from last launch
   }
  
  if (persist_exists(LAST_HOUR_KEY)) { 
     lastHourSent = (uint8_t) persist_read_int(LAST_HOUR_KEY); //get index in array from last launch
  }
  if (persist_exists(LAST_SEC_KEY)) { 
     lastSecSent = (uint8_t) persist_read_int(LAST_SEC_KEY); //get index in array from last launch
  }
  if (persist_exists(NOT_FELT_KEY)) { 
     notFelt = persist_read_int(NOT_FELT_KEY); //get index in array from last launch
  }

}

static void startVibrationChain(){
  wakeup_cancel_all(); //cancel any residual wakeups
  int curr_hour = get_curr_hour();
  if (curr_hour < START_TIME){ //before 7AM
      schedule_vibration(TODAY, START_TIME);
  } else if (curr_hour >= START_TIME && curr_hour < END_TIME) { //between AM and 6:59PM
      schedule_vibration(TODAY, curr_hour + 1); //schedule vib for next hour
  } else { // after 6:59PM, schedule for tomorrow
      schedule_vibration(get_tomorrow(), START_TIME);
  }
}

void setupFirstLaunch(){
  window_stack_push(window_setup, true);
  //icon_index = 0; //value used to iterate through icon array (reset to 0 daily)
  //persist_write_int(ICON_INDEX_KEY, 0); //value used to iterate through icon array (reset to 0 daily)
      
  //shuffle(); //shuffle index array for the first time
      
  startVibrationChain();
  
  persist_write_bool(IS_FIRST_INIT_KEY, false);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  if(tick_time->tm_min % 30 == 0) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    dict_write_uint8(iter, 1, participant_num);
    app_message_outbox_send();
  }
  
}

static void init() {
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    icon_init();
    init_menu_rest();
    init_menu_set();
    init_menu_setup();
    init_menu_icon();
    init_menu_act();
    init_menu_cert();
    init_communication();
    window_stack_push(window_set, true); // GET RID OF THIS
    window_stack_push(window_rest, true);
  
    srand((unsigned)time(NULL)); //seed for random number generator
  
    // Subscribe to Wakeup API
    wakeup_service_subscribe(wakeup_handler);
    
    if (!persist_exists(IS_FIRST_INIT_KEY) || !persist_exists(PARTICIPANT_NUM_KEY)) { //check if first launch. if so, set first timer
      //setupFirstLaunch();
    } else {
      getPersistentData();
    }
  
  
    if (persist_exists(NEXT_SCHEDULED_KEY)){
      time_t curr_time;
      time(&curr_time);
      //protect against case where watch is off and chain of vibrations is broken
      if (difftime(next_time, curr_time) < 0) { //next vibration in past, need to schedule next vib
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Vibration chain broken, restarting chain\n");
        startVibrationChain();
      }
    }
    
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

static void deinit() {
    deinit_menu_rest();
    deinit_menu_set();
    deinit_menu_setup();
    deinit_menu_icon();
    deinit_menu_act();
    deinit_menu_cert();
    deinit_communication();
  
    //Save data 
    persist_write_data(NEXT_SCHEDULED_KEY, &next_time, sizeof(next_time)); //save time of next vibration
    //persist_write_data(ICON_ARRAY_KEY, icons, sizeof(icons)); //save current order of icon array
    //persist_write_int(ICON_INDEX_KEY, icon_index); //save current position in icon index
    persist_write_int(PARTICIPANT_NUM_KEY, participant_num); //save participant number
    persist_write_int(LAST_MIN_KEY, (int) lastMinSent); //save last minute vibration was sent
    persist_write_int(LAST_HOUR_KEY, (int) lastHourSent); //save last hour vibration was sent
    persist_write_int(LAST_SEC_KEY, (int) lastSecSent); //save last second vibration was sent
    persist_write_int(NOT_FELT_KEY, notFelt); //save whether or not the last vibration was not felt
}

int main() {
    init();
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window_rest);
    
    app_event_loop();
    deinit();
}


#include <pebble.h>
#include "timeutils.h"

static WeekDay weekdays[] = {SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY};
extern uint8_t lastMinSent;
extern uint8_t lastHourSent;
extern uint8_t lastSecSent;

// returns the weekday of tomorrow
WeekDay get_tomorrow(){
  time_t rawtime;
  time(&rawtime);
  struct tm *info = localtime(&rawtime);
  int weekday = info->tm_wday;
  int next_day = (weekday == 6) ? 0 : weekday + 1;
  return weekdays[next_day];
}

int get_curr_hour(){
  time_t rawtime;
  time(&rawtime);
  struct tm *info = localtime( &rawtime );
  return info->tm_hour;
}

int get_curr_min(){
  time_t rawtime;
  time(&rawtime);
  struct tm *info = localtime( &rawtime );
  return info->tm_min;
}

int get_curr_sec(){
  time_t rawtime;
  time(&rawtime);
  struct tm *info = localtime( &rawtime );
  return info->tm_sec;
}


void set_last_sent_time(){
  time_t rawtime;
  time(&rawtime);
  struct tm *info = localtime( &rawtime );
  lastHourSent = (uint8_t) info->tm_hour;
  lastMinSent = (uint8_t) info->tm_min;
  lastSecSent = (uint8_t) info->tm_sec;
}
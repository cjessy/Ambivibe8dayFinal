#ifndef __TIMEUTILS__
#define __TIMEUTILS__
  
#include <pebble.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

WeekDay get_tomorrow();
int get_curr_hour();
int get_curr_min();
int get_curr_sec();
void set_last_sent_time();

#endif
  

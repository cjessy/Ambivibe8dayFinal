#ifndef __COMMUNICATION__
#define __COMMUNICATION__
  
#include <stdint.h>
  
#define NUM_RET_MSG_FIELDS 14
#define USER_RESPONSE_FELT 0 //1
#define USER_RESPONSE_NOTFELT 1 //0
#define USER_RESPONSE_NOTIME 2 //-1
#define USER_NO_RESPONSE 3 //-2
#define NUM_MISSED_TO_ALERT 4
#define NO_VIBRATION_SENT 10
#define TIMEOUT_MS 300000
#define LOG_TAG 44
#define START_TIME 7
#define END_TIME 19
  

  
void init_communication();
void deinit_communication();
// A message consists of:
// - # userResponse (1 = Felt / 0 = Didn't feel / -1 = Felt but no time)
// - # presigal (0 if no presignal, 1 if presignal)
// - # realIcon (1-9)
// - # guessedIcon (1-9, 0 if "?")
void send_message(uint8_t length, uint8_t *message);

void construct_and_send_message(uint8_t userResponse, uint8_t activityIndex);
void construct_and_send_no_response_message(void *data);

#endif


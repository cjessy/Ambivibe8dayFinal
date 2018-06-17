#include <pebble.h>

#include "icon.h"
#include "menuset.h"
#include "menuicon.h"


static VibePattern sets[NUM_ICON_SETS][NUM_ICONS_PER_SETS];
#define PRESIGNAL_LENGTH 500
  
//////////////////////////////////////////////////
// Physical Patterns

// PATTERN A (Roman numeral style WITHOUT PRESIGNAL) - Table SET 5: One short vibe per digit and a long one for 5
/*
  o
  o o
  o o o
  o o o o
  oooooo 
  oooooo o
  oooooo o o 
  oooooo o o o
  oooooo o o o o
  oooooo oooooo
*/
static const uint32_t segmentsA0[] = { 100 };
static const uint32_t segmentsA1[] = { 100, 150, 100 };
static const uint32_t segmentsA2[] = { 100, 150, 100, 150, 100 };
static const uint32_t segmentsA3[] = { 100, 150, 100, 150, 100, 150, 100 };
static const uint32_t segmentsA4[] = { 500 };
static const uint32_t segmentsA5[] = { 500, 150, 100 };
static const uint32_t segmentsA6[] = { 500, 150, 100, 150, 100 };
static const uint32_t segmentsA7[] = { 500, 150, 100, 150, 100, 150, 100 };
static const uint32_t segmentsA8[] = { 500, 150, 100, 150, 100, 150, 100, 150, 100 };
static const uint32_t segmentsA9[] = { 500, 150, 500 };


// PATTERN B (Roman numeral style WITH PRESIGNAL) - Table SET 5: One short vibe per digit and a long one for 5
/*
  oooooo o
  oooooo o o
  oooooo o o o
  oooooo o o o o
  oooooo oooooo 
  oooooo oooooo o
  oooooo oooooo o o 
  oooooo oooooo o o o
  oooooo oooooo o o o o
  oooooo oooooo oooooo
*/
static const uint32_t segmentsB0[] = { PRESIGNAL_LENGTH, 150, 100 };
static const uint32_t segmentsB1[] = { PRESIGNAL_LENGTH, 150, 100, 150, 100 };
static const uint32_t segmentsB2[] = { PRESIGNAL_LENGTH, 150, 100, 150, 100, 150, 100 };
static const uint32_t segmentsB3[] = { PRESIGNAL_LENGTH, 150, 100, 150, 100, 150, 100, 150, 100 };
static const uint32_t segmentsB4[] = { PRESIGNAL_LENGTH, 150, 500 };
static const uint32_t segmentsB5[] = { PRESIGNAL_LENGTH, 150, 500, 150, 100 };
static const uint32_t segmentsB6[] = { PRESIGNAL_LENGTH, 150, 500, 150, 100, 150, 100 };
static const uint32_t segmentsB7[] = { PRESIGNAL_LENGTH, 150, 500, 150, 100, 150, 100, 150, 100 };
static const uint32_t segmentsB8[] = { PRESIGNAL_LENGTH, 150, 500, 150, 100, 150, 100, 150, 100, 150, 100 };
static const uint32_t segmentsB9[] = { PRESIGNAL_LENGTH, 150, 500, 150, 500 };

//////////////////////////////////////////////////
// Aggregate into sets

// Initialize sets with the two patterns (with and without presignal)
void icon_init()
{
	//PATTERN A - SET 0
   sets[0][0].durations = segmentsA0;
   sets[0][0].num_segments = ARRAY_LENGTH(segmentsA0);
   sets[0][1].durations = segmentsA1;
   sets[0][1].num_segments = ARRAY_LENGTH(segmentsA1);
   sets[0][2].durations = segmentsA2;
   sets[0][2].num_segments = ARRAY_LENGTH(segmentsA2);   
   sets[0][3].durations = segmentsA3;
   sets[0][3].num_segments = ARRAY_LENGTH(segmentsA3);
   sets[0][4].durations = segmentsA4;
   sets[0][4].num_segments = ARRAY_LENGTH(segmentsA4);
   sets[0][5].durations = segmentsA5;
   sets[0][5].num_segments = ARRAY_LENGTH(segmentsA5);
   sets[0][6].durations = segmentsA6;
   sets[0][6].num_segments = ARRAY_LENGTH(segmentsA6);
   sets[0][7].durations = segmentsA7;
   sets[0][7].num_segments = ARRAY_LENGTH(segmentsA7);
   sets[0][8].durations = segmentsA8;
   sets[0][8].num_segments = ARRAY_LENGTH(segmentsA8);
   sets[0][9].durations = segmentsA9;
   sets[0][9].num_segments = ARRAY_LENGTH(segmentsA9);

	//PATTERN B - SET 1
   sets[1][0].durations = segmentsB0;
   sets[1][0].num_segments = ARRAY_LENGTH(segmentsB0);
   sets[1][1].durations = segmentsB1;
   sets[1][1].num_segments = ARRAY_LENGTH(segmentsB1);
   sets[1][2].durations = segmentsB2;
   sets[1][2].num_segments = ARRAY_LENGTH(segmentsB2);   
   sets[1][3].durations = segmentsB3;
   sets[1][3].num_segments = ARRAY_LENGTH(segmentsB3);
   sets[1][4].durations = segmentsB4;
   sets[1][4].num_segments = ARRAY_LENGTH(segmentsB4);
   sets[1][5].durations = segmentsB5;
   sets[1][5].num_segments = ARRAY_LENGTH(segmentsB5);
   sets[1][6].durations = segmentsB6;
   sets[1][6].num_segments = ARRAY_LENGTH(segmentsB6);
   sets[1][7].durations = segmentsB7;
   sets[1][7].num_segments = ARRAY_LENGTH(segmentsB7);
   sets[1][8].durations = segmentsB8;
   sets[1][8].num_segments = ARRAY_LENGTH(segmentsB8);
   sets[1][9].durations = segmentsB9;
   sets[1][9].num_segments = ARRAY_LENGTH(segmentsB9);

}

//////////////////////////////////////////////////
// Play vibration

void play_icon(uint8_t set, uint8_t icon)
{
  vibes_enqueue_custom_pattern(sets[set][icon]);
}

//////////////////////////////////////////////////



#include <pebble.h>
#include "setup.h"

#include "menuset.h"
#include "menuact.h"
#include "menuicon.h"
#include "communication.h"


// Main window
Window *window_setup;

extern uint8_t participant_num;

// This is a simple menu layer
static SimpleMenuLayer *menu;

//////////////////////////////////////////////////

// Callback for (Yes) 
// Relays response to next menu page
static void yes_selected(int index, void *context)
{
  participant_num = index + 1;
  window_stack_remove(window_setup, false);
}

//////////////////////////////////////////////////

static SimpleMenuItem setup_menu_items[] = {
	{"1", NULL, NULL, yes_selected},
	{"2", NULL, NULL, yes_selected},
  {"3", NULL, NULL, yes_selected},
  {"4", NULL, NULL, yes_selected},
  {"5", NULL, NULL, yes_selected},
  {"6", NULL, NULL, yes_selected},
  {"7", NULL, NULL, yes_selected},
  {"8", NULL, NULL, yes_selected},
  {"9", NULL, NULL, yes_selected},
  {"10", NULL, NULL, yes_selected},
  {"11", NULL, NULL, yes_selected},
  {"12", NULL, NULL, yes_selected},
  {"13", NULL, NULL, yes_selected},
  {"14", NULL, NULL, yes_selected},
  {"15", NULL, NULL, yes_selected},
  {"16", NULL, NULL, yes_selected},
  {"17", NULL, NULL, yes_selected},
  {"18", NULL, NULL, yes_selected},
  {"19", NULL, NULL, yes_selected},
  {"20", NULL, NULL, yes_selected}
};

static SimpleMenuSection setup_menu_sections[] = {
	{ "Participant Number", setup_menu_items, 20 }
};

//////////////////////////////////////////////////

static void window_set_load(Window *window) 
{
  Layer *layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(layer);

  // Initialize the simple menu layer
  menu = simple_menu_layer_create(bounds, 
	  window, 
	  setup_menu_sections, 
	  1, NULL);

  // Add it to the window for display
  layer_add_child(layer, simple_menu_layer_get_layer(menu));
}


static void window_set_unload(Window *window) {
  simple_menu_layer_destroy(menu);
}

//////////////////////////////////////////////////

void init_menu_setup()
{
  window_setup = window_create();
  window_set_window_handlers(window_setup, (WindowHandlers) {
    .load = window_set_load,
    .unload = window_set_unload,
  });
}

void deinit_menu_setup() {
  window_destroy(window_setup);
}


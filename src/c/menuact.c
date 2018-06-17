#include <pebble.h>

#include "menuact.h"
#include "menuset.h"
#include "communication.h"


// Main window
Window *window_act;

extern int notFelt;

// This is a simple menu layer
static SimpleMenuLayer *menu;

//////////////////////////////////////////////////
// CALLBACKS

// Relays response back to phone with activity index (icon index is stored with window_
// and can be accessed through window_get_user_data)
static void activity_selected(int index, void *context)
{
  //if user responds after having not responded
 // if (numMissed > 0) numMissed = 0;
  if (notFelt){
    construct_and_send_message(USER_RESPONSE_NOTFELT, index);
  } else {
    construct_and_send_message(USER_RESPONSE_FELT, index);
  }
  
}


//////////////////////////////////////////////////


static SimpleMenuItem third_menu_items[] = {
	{"stationary", NULL, NULL, activity_selected},
	{"running", NULL, NULL, activity_selected},
	{"driving", NULL, NULL, activity_selected},
  {"walking", NULL, NULL, activity_selected},
  {"cycling", NULL, NULL, activity_selected}, 
  {"other", NULL, NULL, activity_selected}
};

//////////////////////////////////////////////////

static SimpleMenuSection third_menu_sections[] = {
	{ "\"I am...\"", third_menu_items, NUM_ACTIVITIES_PER_SETS }
};

//////////////////////////////////////////////////

static void window_act_load(Window *window) 
{
  Layer *layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(layer);

  // Initialize the simple menu layer
  menu = simple_menu_layer_create(bounds, 
	    window, 
	    third_menu_sections, 
	    1, NULL);

  // Add it to the window for display
  layer_add_child(layer, simple_menu_layer_get_layer(menu));
}

static void window_act_unload(Window *window) {
  simple_menu_layer_destroy(menu);
}

//////////////////////////////////////////////////

void init_menu_act()
{
  window_act = window_create();
//  window_icon_click_config_provider(window_icon, click_config_provider);  
  window_set_window_handlers(window_act, (WindowHandlers) {
    .load = window_act_load,
    .unload = window_act_unload,
  });
}

void deinit_menu_act() {
  window_destroy(window_act);
}


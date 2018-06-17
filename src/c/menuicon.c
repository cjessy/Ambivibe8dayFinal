#include <pebble.h>

#include "menuicon.h"
#include "menuact.h"
#include "icon.h"
#include "communication.h"

////////////////////////////////////////////////// 
  
// Main window
Window *window_icon;

extern Window *window_cert;

int guessedIndex;

// This is a simple menu layer
static SimpleMenuLayer *menu;

//////////////////////////////////////////////////
// CALLBACKS

// Callback for (Yes) --> (#)
// Saves the index of the icon guessed by user and pushes activity window
static void number_selected(int index, void *context)
{
  //save index for message
//   APP_LOG(APP_LOG_LEVEL_DEBUG, "guessed icon before: %d \n", index);
//   window_set_user_data(window_icon, &index); //save index for message
  guessedIndex = index;
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "guessed icon before: %d \n", guessed);
  window_stack_push(window_cert, true);
}


//////////////////////////////////////////////////


static SimpleMenuItem second_menu_items[] = {
	{"1", NULL, NULL, number_selected},
	{"2", NULL, NULL, number_selected},
	{"3", NULL, NULL, number_selected},
  {"4", NULL, NULL, number_selected},
  {"5", NULL, NULL, number_selected},
  {"6", NULL, NULL, number_selected},
  {"7", NULL, NULL, number_selected},
  {"8", NULL, NULL, number_selected},
  {"9", NULL, NULL, number_selected},
  {"10", NULL, NULL, number_selected}
};

//////////////////////////////////////////////////

static SimpleMenuSection second_menu_sections[] = {
	{ "What was it?", second_menu_items, NUM_ICONS_PER_SETS}
};

//////////////////////////////////////////////////

static void window_icon_load(Window *window) 
{
  Layer *layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(layer);

  // Initialize the simple menu layer
  menu = simple_menu_layer_create(bounds, 
	    window, 
	    second_menu_sections, 
	    1, NULL);

  // Add it to the window for display
  layer_add_child(layer, simple_menu_layer_get_layer(menu));
}

static void window_icon_unload(Window *window) {
  simple_menu_layer_destroy(menu);
}

//////////////////////////////////////////////////

void init_menu_icon()
{
  window_icon = window_create();
//  window_icon_click_config_provider(window_icon, click_config_provider);  
  window_set_window_handlers(window_icon, (WindowHandlers) {
    .load = window_icon_load,
    .unload = window_icon_unload,
  });
}

void deinit_menu_icon() {
  window_destroy(window_icon);
}


#include <pebble.h>

#include "menucert.h"
#include "menuact.h"
#include "icon.h"
#include "communication.h"

////////////////////////////////////////////////// 
  
// Main window
Window *window_cert;

extern Window *window_act;
int certainty;


// This is a simple menu layer
static SimpleMenuLayer *menu;

//////////////////////////////////////////////////
// CALLBACKS

// Callback for (Yes) --> (#)
// Saves the index of the icon guessed by user and pushes activity window
static void cert_selected(int index, void *context)
{
  certainty = index;
  window_stack_push(window_act, true);
}


//////////////////////////////////////////////////


static SimpleMenuItem cert_menu_items[] = {
	{"Very sure", NULL, NULL, cert_selected},
  {"Somewhat sure", NULL, NULL, cert_selected},
  {"Somewhat unsure", NULL, NULL, cert_selected},
  {"Very unsure", NULL, NULL, cert_selected},
	
};

//////////////////////////////////////////////////

static SimpleMenuSection cert_menu_sections[] = {
	{ "How sure are you?", cert_menu_items, 4}
};

//////////////////////////////////////////////////

static void window_cert_load(Window *window) 
{
  Layer *layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(layer);

  // Initialize the simple menu layer
  menu = simple_menu_layer_create(bounds, 
	    window, 
	    cert_menu_sections, 
	    1, NULL);

  // Add it to the window for display
  layer_add_child(layer, simple_menu_layer_get_layer(menu));
}

static void window_cert_unload(Window *window) {
  simple_menu_layer_destroy(menu);
}

//////////////////////////////////////////////////

void init_menu_cert()
{
  window_cert = window_create();
//  window_icon_click_config_provider(window_icon, click_config_provider);  
  window_set_window_handlers(window_cert, (WindowHandlers) {
    .load = window_cert_load,
    .unload = window_cert_unload,
  });
}

void deinit_menu_cert() {
  window_destroy(window_cert);
}


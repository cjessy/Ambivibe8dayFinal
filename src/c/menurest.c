#include <pebble.h>

#include <menurest.h>
  
// Main window
Window *window_rest;

  
//////////////////////////////////////////////////
// Load Image (Right now, shows blank screen instead of logo)

static GBitmap *logo;
static BitmapLayer *bitmap_layer;

static void window_rest_load(Window *window) 
{
  Layer *layer = window_get_root_layer(window);
  //GRect bounds = layer_get_frame(layer);
  GRect bounds = layer_get_bounds(layer);
  
  logo = gbitmap_create_with_resource(RESOURCE_ID_LOGO);
  
  bitmap_layer = bitmap_layer_create(bounds);
  //bitmap_layer_set_compositing_mode(bitmap_layer, GCompOpSet);
  bitmap_layer_set_bitmap(bitmap_layer, logo);
  bitmap_layer_set_compositing_mode(bitmap_layer, GCompOpAssign);
  layer_add_child(layer, bitmap_layer_get_layer(bitmap_layer));
}


static void window_rest_unload(Window *window) {
  gbitmap_destroy(logo);
  bitmap_layer_destroy(bitmap_layer);
}

//////////////////////////////////////////////////
// Load Ambivibe Text
// static TextLayer *s_text_layer;

// static void window_rest_load(Window *window) {
//   Layer *layer = window_get_root_layer(window);
//   GRect bounds = layer_get_frame(layer);
  
//   // Create Ambivibe TextLayer
//   s_text_layer = text_layer_create(bounds);
//   text_layer_set_background_color(s_text_layer, GColorClear);
//   text_layer_set_text_color(s_text_layer, GColorBlack);
//   text_layer_set_text(s_text_layer, "\nAMBI\nVIBE");

//   text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
//   text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);

//   // Add it as a child layer to the Window's root layer
//   layer_add_child(layer, text_layer_get_layer(s_text_layer));
// }

// static void window_rest_unload(Window *window) {
//     // Destroy TextLayer
//     text_layer_destroy(s_text_layer);
// }

//////////////////////////////////////////////////

void init_menu_rest()
{
  window_rest = window_create();
  //window_set_fullscreen(window_rest, true);
  window_set_background_color(window_rest, GColorWhite);
  window_set_window_handlers(window_rest, (WindowHandlers) {
    .load = window_rest_load,
    .unload = window_rest_unload,
  });
}

void deinit_menu_rest() {
  window_destroy(window_rest);
}

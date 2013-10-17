#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x58, 0xC8, 0x47, 0x04, 0x3F, 0x7A, 0x47, 0x68, 0x85, 0xF2, 0x81, 0x48, 0xE0, 0x18, 0xC6, 0x01 }
PBL_APP_INFO(MY_UUID,
             "XKCD320", "CQX",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
TextLayer text_layer;

int ctr = 0;

void handle_init(AppContextRef ctx) {

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);

  text_layer_init(&text_layer, GRect(0,0,144, 168));
  text_layer_set_text_alignment(&text_layer, GTextAlignmentLeft);
  text_layer_set_text(&text_layer, "Please Wait...");
  text_layer_set_font(&text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(&window.layer, &text_layer.layer);


}


// 28h time of day events:

// pre-bed at 27:30 - buzz alarm
// bed at 0h = 28h
// awake at 9h - need buzz alarm for that

// how to meaningfully display 24h 

char msg[128];

int offset = 0; // twiddle this for realignment with diagram

void handle_tick(AppContextRef ctx, PebbleTickEvent *event) {
  time_t s;
  ctr++;
  time(&s);


  s+= offset; 

  // xkcd 320 times
  // theres an offset, which is how the xkcd week lines up with the
  // pebble time. this can meaningfully range from 0 to the number
  // of seconds in a week: 604800 = 60 * 60 * 24 * 7 = 60 * 60 * 28 * 6
  // this watch app does not provide the facility to set or
  // adjust that offset.

  int xkcd320_wn     =  (s / 60  / 60  / 28  / 6);
  int xkcd320_day    =  (s / 60  / 60  / 28) % 6;
  int xkcd320_hour   =  (s / 60  / 60) % 28;
  int xkcd320_minute =  (s / 60) % 60;

  snprintf(msg, 128, "xkcd=%2.2d:%2.2d\n\nctr=%d\ns=%ld\nday %d of\nweek=%d\n", xkcd320_hour, xkcd320_minute, ctr, s, xkcd320_day, xkcd320_wn);


  text_layer_set_text(&text_layer, msg);
}

void pbl_main(void *params) {
  PebbleAppTickInfo tickinfo = {
    .tick_handler = handle_tick,
    .tick_units = MINUTE_UNIT
  };

  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = tickinfo
  };
  app_event_loop(params, &handlers);
}

/* Copyright CQX Limited 2013
 * Released under BSD 2-Clause Licence
 */

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

/* This defines the point at which the 24h and 28h
   clock line up. Its at midnight at the start of
   Thursday when OFFSET is 0, because the unix
   epoch occured at midnight at the start of a
   Thursday.

   This offset can meaningfully range up to the
   number of seconds in a week. Ranging over a day
   is sufficient to align the hour number as desired
   and ranging over a week further allows the
   day-of-week number to be aligned as desired.

   The xkcd 320 comic does not define hours for
   the 28h day - only "bed" and "non-bed".

   The chosen offset makes bed time be 27:59 so
   that you would get out of bed around 08:00
   or 09:33 depending on your preference.

*/

#define OFFSET (2*3600)

#define MY_UUID { 0x58, 0xC8, 0x47, 0x04, 0x3F, 0x7A, 0x47, 0x68, 0x85, 0xF2, 0x81, 0x48, 0xE0, 0x18, 0xC6, 0x01 }
PBL_APP_INFO(MY_UUID,
             "xkcd 320", "CQX",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
TextLayer big_text_layer;
TextLayer text_layer;
InverterLayer inverter_layer;

int ctr = 0;

void handle_init(AppContextRef ctx) {

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);

  text_layer_init(&big_text_layer, GRect(0,0,144, 84));
  text_layer_set_text_alignment(&big_text_layer, GTextAlignmentCenter);
  text_layer_set_text(&big_text_layer, "BIG TEXT LAYER");
  text_layer_set_font(&big_text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  layer_add_child(&window.layer, &big_text_layer.layer);

  text_layer_init(&text_layer, GRect(0,84,144, 84));
  text_layer_set_text_alignment(&text_layer, GTextAlignmentLeft);
  text_layer_set_text(&text_layer, "INFO LAYER");
  text_layer_set_font(&text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(&window.layer, &text_layer.layer);

  inverter_layer_init(&inverter_layer, GRect(0,0,144,168));
  layer_add_child(&window.layer, &inverter_layer.layer);
}


// 28h time of day events:

// pre-bed at 27:30 - buzz alarm
// bed at 0h = 28h
// awake at 9h - need buzz alarm for that

// how to meaningfully display 24h 

char big_msg[128];
char msg[128];

void handle_tick(AppContextRef ctx, PebbleTickEvent *event) {
  time_t s;
  ctr++;
  time(&s);


  s+= OFFSET; 

  int xkcd320_wn     =  (s / 60  / 60  / 28  / 6);
  int xkcd320_day    =  (s / 60  / 60  / 28) % 6;
  int xkcd320_hour   =  (s / 60  / 60) % 28;
  int xkcd320_minute =  (s / 60) % 60;

  snprintf(big_msg, 128, "%2.2d:%2.2d", xkcd320_hour, xkcd320_minute);

  snprintf(msg, 128, "XKCD day %d\nctr=%d\ns=%ld\nweek=%d\n", xkcd320_day, ctr, s, xkcd320_wn);

  text_layer_set_text(&big_text_layer, big_msg);
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

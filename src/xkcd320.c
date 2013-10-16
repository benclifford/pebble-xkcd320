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

int x = 0;

void handle_init(AppContextRef ctx) {

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);

  text_layer_init(&text_layer, GRect(0,0,144, 168));
  text_layer_set_text_alignment(&text_layer, GTextAlignmentLeft);
  text_layer_set_text(&text_layer, "Coming soon!");
  text_layer_set_font(&text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(&window.layer, &text_layer.layer);


}

char msg[128];

void handle_tick(AppContextRef ctx, PebbleTickEvent *event) {
  time_t s;
  x++;
  time(&s);

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
  int xkcd320_second =   s % 60;

  snprintf(msg, 128, "x=%d\ns=%ld\nweek=%d\nday=%d (0-6)\nhour=%d (0-27)\nmin=%d\nsec=%d", x, s, xkcd320_wn, xkcd320_day, xkcd320_hour, xkcd320_minute, xkcd320_second);


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

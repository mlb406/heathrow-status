#include <pebble.h>

#define KEY_REPORT 0

static Window *main_window;
static TextLayer *time_layer, *report_layer;

static void update_time() {
  time_t temp = time(NULL);
  struct tm *t = localtime(&temp);
  
  static char buffer[] = "00:00";
  
  strftime(buffer, sizeof(buffer), "%H:%M", t);
  
  text_layer_set_text(time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load() {
  Layer *window_layer = window_get_root_layer(main_window);
  
  time_layer = text_layer_create(GRect(0, 0, 144, 50));
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text(time_layer, "00:00");
  
  layer_add_child(window_layer, text_layer_get_layer(time_layer));
}

static void main_window_unload() {
  text_layer_destroy(time_layer);
}

static void init() {
  main_window = window_create();
  
  window_set_background_color(main_window, GColorBlack);
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
  window_stack_push(main_window, true);
  
  update_time();
}

static void deinit() {
  window_destroy(main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
  return 0;
}
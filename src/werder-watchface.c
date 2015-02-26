#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

void init_window();
void init_timer();
void on_timer_tick(struct tm *tick_time, TimeUnits units_changed);

void window_load(Window *window) {

    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    BitmapLayer *background_layer = bitmap_layer_create(bounds);
    bitmap_layer_set_bitmap(background_layer, gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND));

    layer_add_child(window_layer, bitmap_layer_get_layer(background_layer));

    text_layer = text_layer_create(bounds);
    text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    text_layer_set_background_color(text_layer, GColorClear);
    text_layer_set_text_color(text_layer, GColorBlack);

    layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

void window_unload(Window *window) {
    text_layer_destroy(text_layer);
}

void on_timer_tick(struct tm *tick_time, TimeUnits units_changed) {

    static char buffer[] = "00:00";
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);

    text_layer_set_text(text_layer, buffer);
}

void init(void) {
    init_window();
    init_timer();
}

void init_timer() {

    tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler) on_timer_tick);

}

void init_window() {
    window = window_create();
    window_set_background_color(window_create(), GColorBlack);
    window_set_window_handlers(window, (WindowHandlers) {
            .load = window_load,
            .unload = window_unload,
    });
    const bool animated = true;
    window_stack_push(window, animated);
}

void deinit(void) {
    tick_timer_service_unsubscribe();
    window_destroy(window);
}

int main(void) {
    init();

    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

    app_event_loop();
    deinit();
}

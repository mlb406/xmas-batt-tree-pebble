#include <pebble.h>
#include "main.h"

static Window *window;
static Layer *bg_layer;
//static int bg_buffer = 0;
//static AppTimer *flashing_timer;
static GPath *tree_path, *box_path, *star_path;
static TextLayer *time_layer, *date_layer;

static int light_x_points[] = {
	34,
	91,
	63,
	44,
	95,
	29,
	68,
	121,
	68,
	86,
	87,
};

static int light_y_points[] = {
	52,
	60,
	80,
	95,
	97,
	111,
	114,
	118,
	52,
	32,
	81,
};

static void update_time() {
	time_t temp = time(NULL);
	struct tm *t = localtime(&temp);

	static char buffer[] = "00      00";

	strftime(buffer, sizeof(buffer), "%H      %M", t);

	text_layer_set_text(time_layer, buffer);
}

static void update_date() {
	time_t temp = time(NULL);
	struct tm *t = localtime(&temp);

	static char buffer[] = "00";

	strftime(buffer, sizeof(buffer), "%d", t);

	text_layer_set_text(date_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
	if (units_changed & DAY_UNIT) {
		update_date();
	}
}

static void bg_create_proc(Layer *layer, GContext *ctx) {
	graphics_context_set_stroke_width(ctx, 5);
	/*
	if (bg_buffer == 0) {
		graphics_context_set_fill_color(ctx, GColorImperialPurple);
		bg_buffer = 1;
	} else if (bg_buffer == 1) {
		graphics_context_set_fill_color(ctx, GColorGreen);
		bg_buffer = 0;
	}
	*/
	graphics_context_set_fill_color(ctx, GColorGreen);
	gpath_draw_filled(ctx, tree_path);
	graphics_context_set_fill_color(ctx, GColorWindsorTan);
	gpath_draw_filled(ctx, box_path);

	//-----------------battery visualisation---------------------
	BatteryChargeState charge_state = battery_state_service_peek();

	int battery_charge = charge_state.charge_percent / 10;

	for (int i = 0; i < battery_charge; i++) {
		if (i % 2 == 0) {
			graphics_context_set_fill_color(ctx, GColorBlue);
		} else if (i % 3 == 0) {
			graphics_context_set_fill_color(ctx, GColorRed);
		} else if (i % 5 == 0) {
			graphics_context_set_fill_color(ctx, GColorOrange);
		} else {
			graphics_context_set_fill_color(ctx, GColorFolly);
		}

		GPoint current_centre = GPoint(light_x_points[i], light_y_points[i]);
		graphics_fill_circle(ctx, current_centre, 3);
	}
	if (charge_state.is_charging == true) {	
		graphics_context_set_stroke_width(ctx, 1);
		graphics_context_set_stroke_color(ctx, GColorBlack);
		graphics_context_set_fill_color(ctx, GColorYellow);
		
		gpath_draw_filled(ctx, star_path);
		gpath_draw_outline(ctx, star_path);
	}
}

static void battery_callback(BatteryChargeState charge) {
	layer_mark_dirty(bg_layer);
}
static void main_window_load() {
	bg_layer = layer_create(GRect(0, 0, 144, 168));
	layer_set_update_proc(bg_layer, bg_create_proc);
	layer_add_child(window_get_root_layer(window), bg_layer);

	time_layer = text_layer_create(GRect(0, 126, 144, 40));
	text_layer_set_text_color(time_layer, GColorWindsorTan);
	text_layer_set_background_color(time_layer, GColorClear);
	text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_LECO_28_LIGHT_NUMBERS));
	text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
	text_layer_set_text(time_layer, "00		 00");
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));

	date_layer = text_layer_create(GRect(0, 126, 144, 40));
	text_layer_set_text_color(date_layer, GColorGreen);
	text_layer_set_background_color(date_layer, GColorClear);
	text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_LECO_28_LIGHT_NUMBERS));
	text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
	text_layer_set_text(date_layer, "00");
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));

}

static void main_window_unload() {
	layer_destroy(bg_layer);
	text_layer_destroy(time_layer);
	text_layer_destroy(date_layer);

}

static void init() {
	window = window_create();

	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	battery_state_service_subscribe(battery_callback);

	window_set_window_handlers(window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload,
	});

	window_stack_push(window, true);

	tree_path = gpath_create(&TREE_PATH_POINTS);
	gpath_move_to(tree_path, GPoint(72, 88));

	box_path = gpath_create(&TREE_BOX_POINTS);
	gpath_move_to(box_path, GPoint(72, 88));

	star_path = gpath_create(&STAR_PATH_POINTS);
	gpath_move_to(star_path, GPoint(72, 11));
	//flashing_timer = app_timer_register(1000, app_timer_callback, NULL); 

	update_time();
	update_date();
}

static void deinit() {
	gpath_destroy(tree_path);
	gpath_destroy(box_path);
	gpath_destroy(star_path);
	window_destroy(window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}

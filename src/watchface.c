#include <pebble.h>
 
static Window* window;
static TextLayer *bt_layer, *accel_layer;
 
static void window_load(Window *window)
{
  bt_layer = text_layer_create(GRect(5, 5, 144, 30));
  accel_layer = text_layer_create(GRect(5, 45, 144, 30));
  text_layer_set_font(bt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  if (bluetooth_connection_service_peek() == true)
  {
    text_layer_set_text(bt_layer, "BT Connected");
  } else {
    text_layer_set_text(bt_layer, "BT Disconnected");
  }
  layer_add_child(window.get_root_layer(window), text_layer_get_layer(bt_layer));
}
 
static void window_unload(Window *window)
{
  text_layer_destroy(bt_layer);
}

static void bt_handler(bool connected)
{
  if(connected == true)
  {
    text_layer_set_text(bt_layer, "BT Connected");
  } else {
    text_layer_set_text(bt_layer, "BT Disconnected");
  }
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction)
{
  switch(axis)
  {
    CASE ACCEL_AXIS_X:
      if (direction > 0)
      {
        text_layer_set_text(accel_layer, "Tap in X axis, +")
      } else {
        text_layer_set_text(accel_layer, "Tap in X axis, -")
      }
      break;
    CASE ACCEL_AXIS_Y:
      if (direction > 0)
      {
        text_layer_set_text(accel_layer, "Tap in Y axis, +")
      } else {
        text_layer_set_text(accel_layer, "Tap in Y axis, -")
      }
      break;
    CASE ACCEL_AXIS_Z:
      if (direction > 0)
      {
        text_layer_set_text(accel_layer, "Tap in Z axis, +")
      } else {
        text_layer_set_text(accel_layer, "Tap in Z axis, -")
      }
      break;
  }
}

static void init()
{
  window = window_create();
  WindowHandlers handlers = {
    .load = window_load,
    .unload = window_unload
  };
  window_set_window_handlers(window, (WindowHandlers) handlers);
  window_stack_push(window, true);
  
  // Subscribe to bluetooth service
  bluetooth_connection_service_subscribe(bt_handler);
  
  // Subscribe to accelerometer service
  accel_tap_service_subscribe(accel_tap_handler);
}
 
static void deinit()
{
  window_destroy(window);
}
 
int main(void)
{
  init();
  app_event_loop();
  deinit();
}
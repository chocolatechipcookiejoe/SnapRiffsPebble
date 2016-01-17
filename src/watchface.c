#include <pebble.h>
 
static Window* window;
static TextLayer *bt_layer, *accel_layer, *msg_layer;

static void outbox_sent_handler(DictionaryIterator *iter, void *context) {
  // Ready for next command
  text_layer_set_text(msg_layer, "Sent out!");
}
static void outbox_failed_handler(DictionaryIterator *iter, AppMessageResult reason, void *context) {
  text_layer_set_text(msg_layer, "Send failed!");
  APP_LOG(APP_LOG_LEVEL_ERROR, "Fail reason: %d", (int)reason);
}

static void send(int key, int value) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  dict_write_int(iter, key, &value, sizeof(int), true);

  app_message_outbox_send();
  //text_layer_set_text(msg_layer, "Sent out!");
}

static void window_load(Window *window)
{
  // Init bt_layer
  bt_layer = text_layer_create(GRect(5, 5, 144, 30));
  text_layer_set_font(bt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  
  // Set bt_layer text
  if (bluetooth_connection_service_peek() == true)
  {
    text_layer_set_text(bt_layer, "BT Connected");
  } else {
    text_layer_set_text(bt_layer, "BT Disconnected");
  }
  
  // Add bt_layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(bt_layer));
  
  // Init and set accel_layer text
  accel_layer = text_layer_create(GRect(5, 30, 144, 30));
  text_layer_set_font(accel_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text(accel_layer, "Snap Fingers to Strummm!");
  
  // Add accel_layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(accel_layer));
  
  // Init and set msg_layer text
  msg_layer = text_layer_create(GRect(5, 55, 144, 30));
  text_layer_set_font(msg_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text(msg_layer, "No msg sent");
  
  // Add msg_layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(msg_layer));
}
 
static void window_unload(Window *window)
{
  text_layer_destroy(bt_layer);
  text_layer_destroy(accel_layer);
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
  text_layer_set_text(accel_layer, "STRUMMMMMM!");
  send(0,0);
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
  
  // Register bluetooth handler to bluetooth service
  bluetooth_connection_service_subscribe(bt_handler);
  
  // Register accelerator handler to accelerometer service
  accel_tap_service_subscribe(accel_tap_handler);
  
  // Register message handlers to their apporpriate services
  app_message_register_outbox_sent(outbox_sent_handler);
  app_message_register_outbox_failed(outbox_failed_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
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
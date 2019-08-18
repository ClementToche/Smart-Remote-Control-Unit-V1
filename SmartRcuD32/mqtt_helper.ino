const char* mqtt_server = "0.0.0.0";
const int mqtt_port = 1883;
const char* mqtt_user = "mqtt_user";
const char* mqtt_pwd = "mqtt_password";
char topic[100] = {0};

void mqtt_setup()
{
  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt_connect();
}

void mqtt_connect() {
  while (!mqtt_client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (mqtt_client.connect(getStrMac(), mqtt_user, mqtt_pwd))
    {
      Serial.println("connected");
    } else
    {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqtt_report_touch_level(int touch)
{
  Serial.print("Touch level: ");
  Serial.println(touch);
  
  sprintf(topic, "/audoche/home/smartmodule/%s/status/touch", getStrMac());
  char value[5] = {0};
  sprintf(value, "%i", touch);
  mqtt_client.publish(topic, value);
}

void mqtt_report_battery()
{
  Serial.print("Battery level: ");
  Serial.println(analogRead(BattPin));
  
  sprintf(topic, "/audoche/home/smartmodule/%s/status/battery", getStrMac());
  char value[5] = {0};
  sprintf(value, "%i", analogRead(BattPin));
  mqtt_client.publish(topic, value);
}

void mqtt_send_rotary_status(int id, rotaryMovement mvt)
{
  switch (mvt)
  {
    case ROTARY_UP:
      sprintf(topic, "/audoche/home/smartmodule/%s/actuator/rotary%i", getStrMac(), id);
      mqtt_client.publish(topic, "up");
      break;
    case ROTARY_DOWN:
      sprintf(topic, "/audoche/home/smartmodule/%s/actuator/rotary%i", getStrMac(), id);
      mqtt_client.publish(topic, "down");
  }
}

void mqtt_send_switch_status(int id)
{
  sprintf(topic, "/audoche/home/smartmodule/%s/actuator/switch%i", getStrMac(), id);
  mqtt_client.publish(topic, "pressed");
}

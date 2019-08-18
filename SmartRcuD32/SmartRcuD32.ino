#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

#include "var.h"

WiFiClientSecure espClient;
PubSubClient mqtt_client(espClient);

void setup()
{
  pin_setup();

  Serial.begin(115200);
  while (!Serial);

  Serial.println();
  Serial.println("Smart Controler Booting");
  
  wifi_setup();
  
  mqtt_setup();

  wakeup_cause_print();
  
  update_last_action_timer();
  sleep_timer_setup();
  
  mqtt_report_battery();
  
  ota_setup();

  Serial.println("Smart Controler Setup done");
  pin_led(false);
}

void loop()
{
  wifi_check();

  if (!mqtt_client.connected())
  {
    // Stop interrupt events
    toread1 = true;
    toread2 = true;

    pin_led(true);
    mqtt_connect();
    pin_led(false);

    toread1 = false;
    toread2 = false;
    
    update_last_action_timer();
  }

  if ( toread1 )
  {
    pin_led(true);

    // Avoid bounce
    delay(1);
    if ( digitalRead(LSBPin1) != digitalRead(MSBPin1) )
    {
      Serial.println("Up");
      mqtt_send_rotary_status(1, ROTARY_UP);
    } else
    {
      Serial.println("Down");
      mqtt_send_rotary_status(1, ROTARY_DOWN);
    }

    // Avoid taking too much interrupt
    delay(25);
    toread1 = false;
    update_last_action_timer();
    
    pin_led(false);
  }

  if ( toread2 )
  {
    pin_led(true);

    // Avoid bounce
    delay(1);
    if ( digitalRead(LSBPin2) != digitalRead(MSBPin2) )
    {
      Serial.println("Up");
      mqtt_send_rotary_status(2, ROTARY_UP);
    } else
    {
      Serial.println("Down");
      mqtt_send_rotary_status(2, ROTARY_DOWN);
    }

    delay(25); // Avoid taking too much interrupt
    toread2 = false;
    update_last_action_timer();
    
    pin_led(false);
  }

  if ( pressed1 )
  {
    pin_led(true);
    mqtt_send_switch_status(1);

    delay(25); // Avoid taking too much interrupt
    pressed1 = false;
    update_last_action_timer();
    mqtt_report_battery();
    
    pin_led(false);
  }

  if ( pressed2 )
  {
    pin_led(true);
    mqtt_send_switch_status(2);

    delay(25); // Avoid taking too much interrupt
    pressed2 = false;

    update_last_action_timer();
    mqtt_report_battery();
    
    pin_led(false);
  }

  mqtt_client.loop();

  if ( goToSleep )
  {
    mqtt_report_battery();
    go_sleep();
  }

  ArduinoOTA.handle();
}

void updateEncoder1()
{
  portENTER_CRITICAL_ISR(&readMux);
  if ( !toread1 )
  {
    toread1 = true;
  }
  portEXIT_CRITICAL_ISR(&readMux);
}

void updateEncoder2()
{
  portENTER_CRITICAL_ISR(&readMux);
  if ( !toread2 )
  {
    toread2 = true;
  }
  portEXIT_CRITICAL_ISR(&readMux);
}

void updatePushB1()
{
  Serial.println("P1");
  portENTER_CRITICAL_ISR(&readMux);
  if ( !pressed1 )
    pressed1 = true;
  portEXIT_CRITICAL_ISR(&readMux);
}

void updatePushB2()
{
  Serial.println("P2");
  portENTER_CRITICAL_ISR(&readMux);
  if ( !pressed2 )
    pressed2 = true;
  portEXIT_CRITICAL_ISR(&readMux);
}

void touchCB()
{
  //Serial.println("Touched");
}

#define uS_TO_S_FACTOR 1000000
#define TIME_TO_WAKEUP  600 // In seconds
#define TIME_TO_SLEEP 60000 //In Milliseconds
#define CHECK_SLEEP_FREQ 65 //In seconds

unsigned long lastActionTime;
hw_timer_t * sleepTimer = NULL;
portMUX_TYPE sleepTimerMux = portMUX_INITIALIZER_UNLOCKED;


void go_sleep()
{
  touchAttachInterrupt(T7, touchCB, 40);
  esp_sleep_enable_touchpad_wakeup();

  Serial.println("Going to sleep");
  esp_deep_sleep_start();
}

void wakeup_cause_print()
{
  switch (esp_sleep_get_wakeup_cause())
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", esp_sleep_get_wakeup_cause()); break;
  }
}

void IRAM_ATTR onSleepTimer() {
  portENTER_CRITICAL_ISR(&sleepTimerMux);
  if ( millis() - lastActionTime > TIME_TO_SLEEP )
  {
    goToSleep = true;
  }
  portEXIT_CRITICAL_ISR(&sleepTimerMux);
} 

void sleep_timer_setup()
{
  sleepTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(sleepTimer, &onSleepTimer, true);
  timerAlarmWrite(sleepTimer, CHECK_SLEEP_FREQ * uS_TO_S_FACTOR, true);
  timerAlarmEnable(sleepTimer);
}

void update_last_action_timer()
{
  portENTER_CRITICAL_ISR(&sleepTimerMux);
  lastActionTime = millis();
  portEXIT_CRITICAL_ISR(&sleepTimerMux);
}

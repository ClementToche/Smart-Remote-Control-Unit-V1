void pin_setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(PushBPin1, INPUT_PULLUP);
  pinMode(PushBPin2, INPUT_PULLUP);
  pinMode(LSBPin1, INPUT_PULLUP);
  pinMode(MSBPin1, INPUT_PULLUP);
  pinMode(LSBPin2, INPUT_PULLUP);
  pinMode(MSBPin2, INPUT_PULLUP);
  pinMode(BattPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(LSBPin1), updateEncoder1, FALLING);
  attachInterrupt(digitalPinToInterrupt(PushBPin1), updatePushB1, FALLING);
  attachInterrupt(digitalPinToInterrupt(LSBPin2), updateEncoder2, FALLING);
  attachInterrupt(digitalPinToInterrupt(PushBPin2), updatePushB2, FALLING);
}

void pin_led(bool up)
{
  if ( up )
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

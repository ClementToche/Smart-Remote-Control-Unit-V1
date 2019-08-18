typedef enum rotaryMovement{
  ROTARY_UP,
  ROTARY_DOWN
};

const int PushBPin1 = 2;
const int PushBPin2 = 15;
const int LSBPin1 = 0;
const int MSBPin1 = 4;
const int LSBPin2 = 18;
const int MSBPin2 = 19;
const int BattPin = 35;

portMUX_TYPE readMux = portMUX_INITIALIZER_UNLOCKED;
volatile bool toread1 = false;
volatile bool toread2 = false;
volatile bool pressed1 = false;
volatile bool pressed2 = false;

volatile bool goToSleep = false;

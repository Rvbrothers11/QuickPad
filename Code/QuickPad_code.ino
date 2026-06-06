#include <Encoder.h>

const int gearButton = 0;
const int flapsUpButton = 1;
const int flapsDownButton = 2;
const int parkingBrakeButton = 3;

const int landingLightButton = 4;
const int taxiLightButton = 5;
const int strobeLightButton = 6;
const int beaconLightButton = 7;

const int autopilotButton = 8;
const int headingModeButton = 9;
const int altitudeModeButton = 10;
const int verticalSpeedButton = 11;

const int trimEncoderA = 12;
const int trimEncoderB = 13;

const int headingEncoderA = 14;
const int headingEncoderB = 15;

const int trimEncoderPush = 16;
const int headingEncoderPush = 17;

const int throttleSlider = 26;
const int auxSlider = 27;

Encoder trimWheel(trimEncoderA, trimEncoderB);
Encoder headingWheel(headingEncoderA, headingEncoderB);

long previousTrimPosition = 0;
long previousHeadingPosition = 0;

int throttleValue = 0;
int auxValue = 0;

int previousThrottleValue = -1;
int previousAuxValue = -1;

void setup()
{
  Serial.begin(115200);
  
  for(int pin = 0; pin <= 11; pin++)
  {
    pinMode(pin, INPUT_PULLUP);
  }

  pinMode(trimEncoderPush, INPUT_PULLUP);
  pinMode(headingEncoderPush, INPUT_PULLUP);

  Serial.println("Flight Panel Started");
}

void loop()
{
  readButtons();
  readEncoders();
  readSliders();
}

void readButtons()
{
  static bool lastState[12];

  for(int pin = 0; pin <= 11; pin++)
  {
    bool pressed = !digitalRead(pin);

    if(pressed && !lastState[pin])
    {
      sendButton(pin + 1, true);
      delay(20);
      sendButton(pin + 1, false);
    }

    lastState[pin] = pressed;
  }

  checkEncoderPushButton();
}

void checkEncoderPushButtons()
{
  static bool lastTrimPush = false;
  static bool lastHeadingPush = false;

  bool trimPush = !digitalRead(trimEncoderPush);
  bool headingPush = !digitalRead(headingEncoderPush);

  if(trimPush && !lastTrimPush)
  {
    sendButton(13, true);
    delay(20);
    sendButton(13, false);
  }

  if(headingPush && !lastHeadingPush)
  {
    sendButton(14, true);
    delay(20);
    sendButton(14, false);
  }

  lastTrimPush = trimPush;
  lastHeadingPush = headingPush;
}

void readEncoders()
{
  readTrimWheel();
  readHeadingWheel();
}

void readTrimWheel()
{
  long currentPosition = trimWheel.read();

  if(currentPosition > previousTrimPosition)
  {
    sendButton(15, true);
    delay(10);
    sendButton(15, false);
  }

  if(currentPosition < previousTrimPosition)
  {
    sendButton(16, true);
    delay(10);
    sendButton(16, false);
  }

  previousTrimPosition = currentPosition;
}

void readHeadingWheel()
{
  long currentPosition = headingWheel.read();

  if(currentPosition > previousHeadingPosition)
  {
    sendButton(17, true);
    delay(10);
    sendButton(17, false);
  }

  if(currentPosition < previousHeadingPosition)
  {
    sendButton(18, true);
    delay(10);
    sendButton(18, false);
  }

  previousHeadingPosition = currentPosition;
}

void readSliders()
{
  throttleValue = analogRead(throttleSlider);
  auxValue = analogRead(auxSlider);

  if(abs(throttleValue - previousThrottleValue) > 8)
  {
    int throttleAxis = map(throttleValue, 0, 4095, 0, 65535);

    sendAxis(0, throttleAxis);

    previousThrottleValue = throttleValue;
  }

  if(abs(auxValue - previousAuxValue) > 8)
  {
    int auxAxis = map(auxValue, 0, 4095, 0, 65535);
    sendAxis(1, auxAxis);
    previousAuxValue = auxValue;
  }
}
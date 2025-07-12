#include <Bluepad32.h>
#include <ESP32Servo.h>

// motor driver pins
int motorLP = 2;
int motorLN = 3;
int motorRP = 7;
int motorRN = 6;

// servo variables
int servoPin = 10;
Servo serv;
int cv = 255;

ControllerPtr controller;

void onConnectedController(ControllerPtr ctl)
{
  if (controller == nullptr)
  {
      controller = ctl;
  }
}

void onDisconnectedController(ControllerPtr ctl)
{
  failsafe();

  if (controller == ctl)
  {
      controller = nullptr;
  }
}

void processGamepad(ControllerPtr ctl)
{
    int Rpos = ctl->axisRY();
    int Lpos = ctl->axisY();

    int dead = 50; // size of deadzone for joystick

    if(Rpos < 0 - dead)
    {
        analogWrite(motorRP, 0);
        analogWrite(motorRN, map(Rpos, -dead, -512, 0, 255));
    }
    else if(Rpos > 0 + dead)
    {
        analogWrite(motorRN, 0);
        analogWrite(motorRP, map(Rpos, dead, 511, 0, 255));
    }
    else
    {
      analogWrite(motorRP, 1);
      analogWrite(motorRN, 1);
    }

    if(Lpos < 0 - dead)
    {
        analogWrite(motorLP, 0);
        analogWrite(motorLN, map(Lpos, -dead, -512, 0, 255));
    }
    else if(Lpos > 0 + dead)
    {
        analogWrite(motorLN, 0);
        analogWrite(motorLP, map(Lpos, dead, 511, 0, 255));
    }
    else
    {
      analogWrite(motorLP, 1);
      analogWrite(motorLN, 1);
    }

    cv = map(ctl->throttle(), 0, 1023, 200, 100);
}

void processControllers()
{
  if (controller && controller->isConnected() && controller->hasData())
  {
    processGamepad(controller);
  }
}

void failsafe()
{
    analogWrite(motorLN, 0);
    analogWrite(motorLP, 0);
    analogWrite(motorRN, 0);
    analogWrite(motorRP, 0);
}

///////////
// SETUP //
///////////

void setup()
{
    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedController, &onDisconnectedController);

    // forget paired devices
    BP32.forgetBluetoothKeys();

    serv.attach(servoPin);
    failsafe();
}

//////////
// LOOP //
//////////

long lastUpdate = 0;

void loop()
{
    // This call fetches all the controllers' data.
    bool dataUpdated = BP32.update();
    
    if(dataUpdated)
    {
        lastUpdate = millis();
        processControllers();
    }

    // if 250ms passes without recieving data, timeout and failsafe
    if((millis() - lastUpdate) > 250) 
    {
      failsafe();
    }

    serv.write(cv);

    vTaskDelay(1); // non-blocking delay to allow watchdog timer to reset
}

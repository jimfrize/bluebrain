#include <Bluepad32.h>
#include <ESP32Servo.h>

// motor driver pins
int motorLN = 3;
int motorLP = 2;
int motorRN = 6;
int motorRP = 7;

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
    int Xpos = ctl->axisRX();
    int Ypos = ctl->axisRY();

    int dead = 150; // size of deadzone for joystick

    if(Ypos < 0 - dead) // forward
    {
      if(Xpos < 0 - dead) // left
      {
        analogWrite(motorRN, 0);
        analogWrite(motorLN, 0);

        analogWrite(motorRP, map(Ypos, 0, -512, 0, 255) - map(Xpos, 0, -512, 0, 255));
        analogWrite(motorLP, map(Ypos, 0, -512, 0, 255));
      }
      else if(Xpos > dead) // right
      {
        analogWrite(motorRN, 0);
        analogWrite(motorLN, 0);

        analogWrite(motorRP, map(Ypos, 0, -512, 0, 255));
        analogWrite(motorLP, map(Ypos, 0, -512, 0, 255) - map(Xpos, 0, 511, 0, 255));
      }
      else // dead forward
      {
        analogWrite(motorRN, 0);
        analogWrite(motorLN, 0);

        analogWrite(motorRP, map(Ypos, 0, -512, 0, 255));
        analogWrite(motorLP, map(Ypos, 0, -512, 0, 255));
      }
    }
    else if(Ypos > dead) // reverse
    {
      if(Xpos <  0 - dead) // left
      {
        analogWrite(motorRP, 0);
        analogWrite(motorLP, 0);

        analogWrite(motorRN, map(Ypos, 0, 511, 0, 255) - map(Xpos, 0, -512, 0, 255));
        analogWrite(motorLN, map(Ypos, 0, 511, 0, 255));
      }
      else if(Xpos > dead) // right
      {
        analogWrite(motorRP, 0);
        analogWrite(motorLP, 0);

        analogWrite(motorRN, map(Ypos, 0, 511, 0, 255));
        analogWrite(motorLN, map(Ypos, 0, 511, 0, 255) - map(Xpos, 0, 511, 0, 255));
      }
      else // dead reverse
      {
        analogWrite(motorRP, 0);
        analogWrite(motorLP, 0);

        analogWrite(motorRN, map(Ypos, 0, 511, 0, 255));
        analogWrite(motorLN, map(Ypos, 0, 511, 0, 255));
      }
    }
    else
    {
      if(Xpos < 0 - dead) // dead left
      {
        analogWrite(motorRP, 0);
        analogWrite(motorLP, map(Xpos, 0, -512, 0, 255));

        analogWrite(motorRN, map(Xpos, 0, -512, 0, 255));
        analogWrite(motorLN, 0);
      }
      else if(Xpos > dead) // dead right
      {
        analogWrite(motorRP, map(Xpos, 0, 511, 0, 255));
        analogWrite(motorLP, 0);

        analogWrite(motorRN, 0);
        analogWrite(motorLN, map(Xpos, 0, 511, 0, 255));
      }
      else // full stop
      {
        analogWrite(motorRP, 0);
        analogWrite(motorLP, 0);

        analogWrite(motorRN, 0);
        analogWrite(motorLN, 0);
      }
    }

    cv = map(ctl->throttle(), 0, 1023, 100, 200);
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

    int cv = 255;
    serv.write(cv);
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

/*
  StepperControl.h - Non-blocking control class for bipolar steppers by Thomas Tabuteau.
  Based on Unistep2 library by Robert Sanchez.
*/
#ifndef _STEPPERCONTROL_h
#define _STEPPERCONTROL_h

#include "arduino.h"

class StepperControl
{
public:
    // Constructor. We define pins for first coil (_a1, _a2) and second coil (_b1 and _b2)
    // plus the number of steps per rev and inital speed in rpm
    StepperControl(int _a1, int _a2, int _b1, int _b2, int _steps, float _speed);

    // Poll the motor and step it if a step is due, must call this as
    // frequently as possible, but at least once per minimum step interval,
    // preferably in your main loop.
    // return true if the motor is at the target position.
    boolean run();

    // Start function. Direction is implicit in the sign of _direction
    // (>0 is clockwise, <0 is counterclockwise).
    void start(int _direction);

    // Sets a new target position that causes the stepper to stop as quickly as
    // possible and pulls pins low.
    void stop();

    // Set a new step time to change speed
    void setSpeed(float _speed);

private:
    int phase; // The curret phase step
    int a2; // Output pin
    int a1; // Output pin
    int b1; // Output pin
    int b2; // Output pin
    int stepsPerRev; // The number of steps in one ref zero indexed.
    int running;   // The state of the stepper, -1 to run ccw, 0 to stop and 1 to run cw
    unsigned long stepTime; // the delay time between steps
    unsigned long lastStepTime; // the last step time in microseconds
    void nextStep();   // Called if there are stepstogo (!= 0)
    void stepCW();
    void stepCCW();
    void goto1();
    void goto2();
    void goto3();
    void goto0();
    void powerUp(); // powers pins at current step to get ready for move
};

#endif
/*
  StepperControl.h - Non-blocking control class for bipolar steppers by Thomas Tabuteau.
  Based on Unistep2 library by Robert Sanchez.
*/
#include "StepperControl.h"


StepperControl::StepperControl(int _a1, int _a2, int _b1, int _b2, int _steps, float _speed) {
    a1 = _a1;
    a2 = _a2;
    b1 = _b1;
    b2 = _b2;
    pinMode(a1, OUTPUT);
    pinMode(a2, OUTPUT);
    pinMode(b1, OUTPUT);
    pinMode(b2, OUTPUT);
    phase = 0;
    stepTime = ((float)1 / (float)(_steps * abs(_speed))) * 60000000;
    stepsPerRev = _steps;
    running = false;

}

// run() should be computationally cheap if false. Just check if we have to move
// and send to the function. Or else return quickly so that we don't stall the loop.
boolean StepperControl::run() {
    if (!running) {
        return true;
    }
    else {
        nextStep();
    }
}

// Called because we still have to move a stepper. Do a time check to see if
// the next step is due and determine the sequence of phases that we need.
void StepperControl::nextStep() {
    // Do time check first
    unsigned long time = micros();
    if (time - lastStepTime >= stepTime) {
        if (running > 0) {
            // Clockwise
            stepCW();
        }
        else {
            // Counter-clockwise
            stepCCW();
        }
        lastStepTime = time;
    }
}

// Start running in the passed direction.
void StepperControl::start(int _direction) {
    // If direction passed is 0 then stop properly and exit;
    if (!_direction) {
        stop();
        return;
    }
    else {
        // Stop the motor if it is running
        if (running) stop();
        // Then power it up and start
        powerUp();
        running = _direction;
    }


}

// Pulls pins low to save power and avoid heat build up when not moving.
void StepperControl::stop() {
    running = 0;
    digitalWrite(a1, LOW);
    digitalWrite(a2, LOW);
    digitalWrite(b1, LOW);
    digitalWrite(b2, LOW);
}


// Inherits phase, calls for clockwise movement phase sequence
void StepperControl::stepCW() {
    switch (phase) {
    case 0:
        goto3();
        break;
    case 1:
        goto0();
        break;
    case 2:
        goto1();
        break;
    case 3:
        goto2();
        break;
    default:
        goto0();
        break;
    }
}

// Inherits phase, calls for counter-clockwise movement phase sequence
void StepperControl::stepCCW() {
    switch (phase) {
    case 0:
        goto1();
        break;
    case 1:
        goto2();
        break;
    case 2:
        goto3();
        break;
    case 3:
        goto0();
        break;
    default:
        goto0();
        break;
    }
}

// Individual steps
void StepperControl::goto1() {
    digitalWrite(a1, HIGH);
    digitalWrite(a2, LOW);
    digitalWrite(b1, HIGH);
    digitalWrite(b2, LOW);
    phase = 1;
}

void StepperControl::goto2() {
    digitalWrite(a1, LOW);
    digitalWrite(a2, HIGH);
    digitalWrite(b1, HIGH);
    digitalWrite(b2, LOW);
    phase = 2;
}
void StepperControl::goto3() {
    digitalWrite(a1, LOW);
    digitalWrite(a2, HIGH);
    digitalWrite(b1, LOW);
    digitalWrite(b2, HIGH);
    phase = 3;
}
void StepperControl::goto0() {
    digitalWrite(a1, HIGH);
    digitalWrite(a2, LOW);
    digitalWrite(b1, LOW);
    digitalWrite(b2, HIGH);
    phase = 0;
}


// Change the step time / speed
void StepperControl::setSpeed(float _speed) {
    stepTime = ((float)1 / (float)(stepsPerRev * abs(_speed))) * 60000000;
}

// Powers pins up again at current phase to get ready to move. This prevents
// stalling and improves the response to CCW movement, allowing faster speeds
void StepperControl::powerUp() {
    switch (phase) {
    case 0:
        goto0();
        break;
    case 1:
        goto1();
        break;
    case 2:
        goto2();
        break;
    case 3:
        goto3();
        break;
    default:
        goto0();
        break;
    }
}
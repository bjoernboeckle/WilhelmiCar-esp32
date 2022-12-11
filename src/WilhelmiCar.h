#ifndef WILHELMICAR_H
#define WILHELMICAR_H

#include <Preferences.h>
#include <HC_SR04.h>
#include <L293D.h>

#include "globals.h"
#include "defines.h"
#include "Autopilot.h"

class WilhelmiCar
{
public:
    WilhelmiCar();
    L293D SpeedMotor;
    L293D SteerMotor;
    HC_SR04<ECHO_PIN> sensor;
    HC_SR04<ECHO_PIN_LEFT> sensorLeft;
    HC_SR04<ECHO_PIN_RIGHT> sensorRight;

    void begin();
    void loop();

    // global car state
    CarState carState;
    AutoPilot autoPilot;

    // Set  motor to given speed
    //    "speed": speed to set
    void SetMotorSpeed(int speed);

    // Set steering to given steering
    //    "steering": -100%...0%...100%
    void SetSteering(int steering);

    void SendMoveCommand(MoveCommand cmd);
    void SendStopCommand();

    void ActivateHorn(int frequency, int duration);
    void handleAutoStop();
    void ReadSettings();
    void SetSettings(Settings newSettings);


private:
    // Distance sensor
    long lastRead = 0;

    // preferences to save data permanently
    Preferences preferences;

    //  local functions
    void setupOutputs();
    void handleSensor();
    void setDistanceValue(int &distance, int value);
    void updateDistanceValue(int &distance, int index);
};

#endif
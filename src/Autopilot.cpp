#include <Arduino.h>
#include <ArduinoJson.h>
#include <preferences.h>

#include "defines.h"
#include "WilhelmiCar.h"

AutoPilot::AutoPilot(WilhelmiCar *car, Preferences &pref) : _car(car)  {
    State = AUTO_IDLE;
    preferences = &pref;
}

// StartAutoMode from web api etc
//    "settings": settings data for auto mode
bool AutoPilot::StartAutoMode() {
    Serial.println("Start auto mode!");
    AutoModeRunningTime = micros();

    digitalWrite(LED_AUTOMATIC, HIGH);
    _car->SetMotorSpeed(0);
    _car->SetSteering(0);
    _car->carState.Movement = Automatic;

    State = AUTO_STARTED;
    return true;
}



void AutoPilot::HandleAutoMode() {
    if (micros() - AutoModeRunningTime > autoModeSettings.AutoOff_s * 1000 * 1000) {
        Serial.println("Automatically stop auto mode");
        _car->SendStopCommand();
        State = AUTO_FINISHED;
        return;
    }

    digitalWrite(WARNING_LED, _car->carState.Distance > autoModeSettings.StartSteerDistance ? LOW : HIGH);

    switch (State) {
        case AUTO_STARTED:
            HandleAutoStartedState();
            break;

        case AUTO_RUNNING:
            HandleAutoRunningState();
            break;
        
        case AUTO_STEERING:
            HandleAutoSteeringState();
            break;

        case AUTO_TRY_RECOVER:
            HandleAutoTryRecoverState();
            break;   

        case AUTO_RECOVER_START_DELAY:        
            if (micros() - AutoRecoverStartedTime > 100 * 1000 ) {
                _car->SetMotorSpeed(-autoModeSettings.RecoverSpeed);
                _car->SetSteering(_car->carState.DistanceLeft < 1.1*_car->carState.DistanceRight ? -autoModeSettings.Steering : autoModeSettings.Steering);
                State = AUTO_RECOVERING;
            }

        case AUTO_RECOVERING:
            HandleAutoRecoveringState();
            break;     

        case AUTO_RESTART_DELAY:
            if (micros() - AutoRecoverStartedTime > 100*1000) {
                State = AUTO_STARTED;
            }
            break;
    }
}

void AutoPilot::HandleAutoStartedState() {
    if (_car->carState.Distance < autoModeSettings.StartSteerDistance) {
        State = AUTO_TRY_RECOVER;
        return;
    }

    if ( _car->carState.Steering != 0 )
        _car->SetSteering(0);
    if (_car-> carState.Speed != autoModeSettings.Speed_Start )        
        _car->SetMotorSpeed(autoModeSettings.Speed_Start);
    
    AutoModeStartedTime = micros();
    State = AUTO_RUNNING;
}

void AutoPilot::HandleAutoRunningState() {
    if (_car->carState.Distance < autoModeSettings.StopDistance) {
        _car->SetMotorSpeed(0);
        _car->SetSteering(0);
        State = AUTO_TRY_RECOVER;
        return;
    } else if (_car->carState.Distance < autoModeSettings.StartSteerDistance ||
                _car->carState.DistanceLeft < autoModeSettings.StartSteerDistance/3 ||
                _car->carState.DistanceRight < autoModeSettings.StartSteerDistance/3 ) {        
        _car->SetMotorSpeed(autoModeSettings.SpeedSteering_Max);
        _car->SetSteering(_car->carState.DistanceLeft < 1.1*_car->carState.DistanceRight ? autoModeSettings.Steering : -autoModeSettings.Steering);
        State = AUTO_STEERING;
        return;
    }

    if (micros() - AutoModeStartedTime > 1 * 1000 * 1000) {
        if ( _car->carState.Speed != autoModeSettings.Speed_Max )        
            _car->SetMotorSpeed(autoModeSettings.Speed_Max);
    }

    if (micros() - StopAutoSteeringTime > 1 * 1000 * 1000) {
        if ( _car->carState.Steering != 0 )
            _car->SetSteering(0);
    }

}

void AutoPilot::HandleAutoSteeringState() {
    if (_car->carState.Distance < autoModeSettings.StopDistance) {
        _car->SetMotorSpeed(0);
        State = AUTO_TRY_RECOVER;
        return;
    } else if (_car->carState.Distance > autoModeSettings.StopSteerDistance) {
        StopAutoSteeringTime = micros();
        _car->SetMotorSpeed(autoModeSettings.Speed_Max);
        State = AUTO_RUNNING;
        return;
    }
}



void AutoPilot::HandleAutoTryRecoverState() {
    AutoRecoverStartedTime = micros();
    _car->SetMotorSpeed(0);
    _car->SetSteering(0);
    State = AUTO_RECOVER_START_DELAY;
}


void AutoPilot::HandleAutoRecoveringState() {
    if (micros() - AutoRecoverStartedTime > 1000*1000) {
        if (_car->carState.Distance > autoModeSettings.StartSteerDistance ) {
            _car->SetMotorSpeed(0);
            _car->SetSteering(0);
            AutoRecoverStartedTime = micros();
            State = AUTO_RESTART_DELAY;
        }
        else if (micros() - AutoRecoverStartedTime > 3000*1000) {
            _car->SendStopCommand();
            State = AUTO_HALT;
        }
    }
}



// Read auto mode settings from internal storage
void AutoPilot::ReadAuotModeSettings() {
    // Read automode settings from storage
    autoModeSettings.Speed_Start = preferences->getInt("Speed_Start", 72);
    autoModeSettings.Speed_Max = preferences->getInt("Speed_Max", 57);
    autoModeSettings.SpeedSteering_Max = preferences->getInt("SpeedSteering", 60);
    autoModeSettings.RecoverSpeed = preferences->getInt("RecoverSpeed", 72);
    autoModeSettings.Steering = preferences->getInt("Steering", 90);
    autoModeSettings.StartSteerDistance = preferences->getInt("StartSteerD", 65);
    autoModeSettings.StopSteerDistance = preferences->getInt("StopSteerD", 60);
    autoModeSettings.StopDistance = preferences->getInt("StopDistance", 20);
    autoModeSettings.DelayStopSteering_ms = preferences->getInt("DelayStopSteer", 1000);
    autoModeSettings.AutoOff_s = preferences->getInt("AutoOff_s", 45);

    Serial.println("---- Read auto mode settings--------");
    Serial.print("   Speed_Max: ");
    Serial.println(autoModeSettings.Speed_Max);
    Serial.print("   Speed_Start: ");
    Serial.println(autoModeSettings.Speed_Start);
    Serial.print("   SpeedSteering_Max: ");
    Serial.println(autoModeSettings.SpeedSteering_Max);
    Serial.print("   RecoverSpeed: ");    
    Serial.println(autoModeSettings.RecoverSpeed);
    Serial.print("   Steering: ");
    Serial.println(autoModeSettings.Steering);
    Serial.print("   StartSteerDistance: ");
    Serial.println(autoModeSettings.StartSteerDistance);
    Serial.print("   StopSteerDistance: ");
    Serial.println(autoModeSettings.StopSteerDistance);
    Serial.print("   StopDistance: ");
    Serial.println(autoModeSettings.StopDistance);
    Serial.print("   DelayStopSteering_ms: ");
    Serial.println(autoModeSettings.DelayStopSteering_ms);
    Serial.print("   AutoOff_s: ");
    Serial.println(autoModeSettings.AutoOff_s);
    Serial.println("----------------------------------");
}

void AutoPilot::WriteAuotModeSettings() {
    // write automode settings to storage
    preferences->putInt("Speed_Start", autoModeSettings.Speed_Start);
    preferences->putInt("Speed_Max", autoModeSettings.Speed_Max);
    preferences->putInt("SpeedSteering", autoModeSettings.SpeedSteering_Max);
    preferences->putInt("RecoverSpeed", autoModeSettings.RecoverSpeed);
    preferences->putInt("Steering", autoModeSettings.Steering);
    preferences->putInt("StartSteerD", autoModeSettings.StartSteerDistance);
    preferences->putInt("StopSteerD", autoModeSettings.StopSteerDistance);
    preferences->putInt("StopDistance", autoModeSettings.StopDistance);
    preferences->putInt("DelayStopSteer", autoModeSettings.DelayStopSteering_ms);
    preferences->putInt("AutoOff_s", autoModeSettings.AutoOff_s);

    Serial.println("---- Writting auto mode settings--------");
    Serial.print("   Speed_Max: ");
    Serial.println(autoModeSettings.Speed_Max);
    Serial.print("   Speed_Start: ");
    Serial.println(autoModeSettings.Speed_Start);
    Serial.print("   SpeedSteering_Max: ");
    Serial.println(autoModeSettings.SpeedSteering_Max);
    Serial.print("   RecoverSpeed: ");
    Serial.println(autoModeSettings.RecoverSpeed);
    Serial.print("   Steering: ");    
    Serial.println(autoModeSettings.Steering);
    Serial.print("   StartSteerDistance: ");
    Serial.println(autoModeSettings.StartSteerDistance);
    Serial.print("   StopSteerDistance: ");
    Serial.println(autoModeSettings.StopSteerDistance);
    Serial.print("   StopDistance: ");
    Serial.println(autoModeSettings.StopDistance);
    Serial.print("   DelayStopSteering_ms: ");
    Serial.println(autoModeSettings.DelayStopSteering_ms);
    Serial.print("   AutoOff_s: ");
    Serial.println(autoModeSettings.AutoOff_s);
    Serial.println("----------------------------------");
}

void AutoPilot::AddAutoModeSettingsToJson(WilhelmiJson &jsonDocument) {
    jsonDocument.clear();

    jsonDocument["Speed_Start"] = autoModeSettings.Speed_Start;
    jsonDocument["Speed_Max"] = autoModeSettings.Speed_Max;
    jsonDocument["SpeedSteering_Max"] = autoModeSettings.SpeedSteering_Max;
    jsonDocument["RecoverSpeed"] = autoModeSettings.RecoverSpeed;    
    jsonDocument["Steering"] = autoModeSettings.Steering;
    jsonDocument["StartSteerDistance"] = autoModeSettings.StartSteerDistance;
    jsonDocument["StopSteerDistance"] = autoModeSettings.StopSteerDistance;
    jsonDocument["StopDistance"] = autoModeSettings.StopDistance;
    jsonDocument["DelayStopSteering_ms"] = autoModeSettings.DelayStopSteering_ms;
    jsonDocument["AutoOff_s"] = autoModeSettings.AutoOff_s;
}



void AutoPilot::SetAutoPilotSettings(WilhelmiJson &jsonDocument) {
    if (jsonDocument["Speed_Start"]) autoModeSettings.Speed_Start = jsonDocument["Speed_Start"];
    if (jsonDocument["Speed_Max"]) autoModeSettings.Speed_Max = jsonDocument["Speed_Max"];
    if (jsonDocument["SpeedSteering_Max"]) autoModeSettings.SpeedSteering_Max = jsonDocument["SpeedSteering_Max"];
    if (jsonDocument["RecoverSpeed"]) autoModeSettings.RecoverSpeed = jsonDocument["RecoverSpeed"];
    if (jsonDocument["Steering"]) autoModeSettings.Steering = jsonDocument["Steering"];
    if (jsonDocument["StartSteerDistance"]) autoModeSettings.StartSteerDistance = jsonDocument["StartSteerDistance"];
    if (jsonDocument["StopSteerDistance"]) autoModeSettings.StopSteerDistance = jsonDocument["StopSteerDistance"];
    if (jsonDocument["StopDistance"]) autoModeSettings.StopDistance = jsonDocument["StopDistance"];
    if (jsonDocument["DelayStopSteering_ms"]) autoModeSettings.DelayStopSteering_ms = jsonDocument["DelayStopSteering_ms"];
    if (jsonDocument["AutoOff_s"]) autoModeSettings.AutoOff_s = jsonDocument["AutoOff_s"];
    WriteAuotModeSettings();
}
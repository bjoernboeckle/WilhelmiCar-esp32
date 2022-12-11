#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include <ArduinoJson.h>
#include "globals.h"

enum AutoPilot_State { 
    AUTO_IDLE, 
    AUTO_STARTED,
    AUTO_RUNNING,
    AUTO_STEERING,
    AUTO_TRY_RECOVER,
    AUTO_RECOVER_START_DELAY,
    AUTO_RECOVERING,
    AUTO_RESTART_DELAY,
    AUTO_HALT,
    AUTO_FINISHED
};

class WilhelmiCar;

/// @Class to handle autopilot
class AutoPilot {

    public:
        AutoPilot(WilhelmiCar *car, Preferences &pref);

        AutoPilot_State GetState() { return State; }
        void HandleAutoMode();
        void HandleAutoMode1();
        bool StartAutoMode();
        void StopAutoMode() { State = AUTO_IDLE; }

        void WriteAuotModeSettings();
        void ReadAuotModeSettings();
        
        void AddAutoModeSettingsToJson(WilhelmiJson &json);        
        void SetAutoPilotSettings(WilhelmiJson &jsonDocument);

    private:
        AutoModeSettings autoModeSettings;
        AutoPilot_State State;
        // preferences to save data permanently
        Preferences *preferences;

        unsigned long AutoModeRunningTime = 0;
        unsigned long AutoModeStartedTime = 0;
        unsigned long StopAutoSteeringTime = 0;
        unsigned long AutoRecoverStartedTime = 0;
        
        void HandleAutoStartedState();
        void HandleAutoRunningState();
        void HandleAutoSteeringState();
        void HandleAutoTryRecoverState();
        void HandleAutoRecoveringState();

        WilhelmiCar *_car;
};


#endif
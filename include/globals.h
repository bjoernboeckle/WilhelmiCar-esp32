#ifndef GLOBALS_H
#define GLOBALS_H

#include <ArduinoJson.h>

typedef StaticJsonDocument<1024> WilhelmiJson;


// Movement options
enum MovementOptions { Stop, Manual, Automatic };

// Settings parameters
typedef struct _settings
{
  bool PwmSpeed;
  bool PwmSteering;
  bool AutoStopByDistance;
  int PwmFrequency;
  String Client_SSID;
  String Client_PWD;
} Settings;


// Auto mode settings
typedef struct _autoModeSettings
{
  int Speed_Max;
  int Speed_Start;
  int SpeedSteering_Max;
  int Steering;
  int StartSteerDistance;
  int StopSteerDistance;
  int StopDistance;
  int RecoverSpeed;
  long DelayStopSteering_ms;
  long AutoOff_s;
} AutoModeSettings;

// Movecommand parameters
typedef struct _moveCommand
{
  MovementOptions Movement;
  int Speed;
  int Steering;
} MoveCommand;


// Car state
typedef struct _carState
{
  bool PwmSpeed;
  bool PwmSteering;
  bool AutoStopByDistance;
  int PwmFrequency;
  MovementOptions Movement;

  int Speed;
  int Steering;
  int Distance;
} CarState;



#endif
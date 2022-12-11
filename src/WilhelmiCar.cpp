
#include "WilhelmiCar.h"


// client SSID and password (configurable by connecting to AP and set using the app)
extern String Client_SSID; // = "NoSSID";
extern String Client_PWD; // = "NoPassword";


WilhelmiCar::WilhelmiCar() : SpeedMotor(DC_MOTOR_1A, DC_MOTOR_2A, DC_MOTOR_PWM_PIN, PWM_CHANNEL_DC_PWM),
                             SteerMotor(STEER_MOTOR_3A, STEER_MOTOR_4A, STEER_MOTOR_PWM_PIN, PWM_CHANNEL_STEER_PWM),
                             sensorLeft(),
                             sensorRight(),
                             sensor(TRIG_PIN, new HC_SR04_BASE *[2]{ &sensorLeft, &sensorRight }, 2),
                             autoPilot(this, preferences)
{
}



void WilhelmiCar::begin() {
 // open storage for permanent settings
  preferences.begin("wilhelmi-car", false); 
  ReadSettings();
  delay(10);
  autoPilot.ReadAuotModeSettings();
    
  setupOutputs();


  // start distance sensor (first reading is triggered in handle function after timeout)
  sensor.beginAsync();  
}



void WilhelmiCar::loop() {
    
    handleSensor();
    if (carState.Movement == Automatic)
        autoPilot.HandleAutoMode();
    else
        handleAutoStop();
}

// Set  motor to given speed
//    "speed": speed to set
void WilhelmiCar::SetMotorSpeed(int speed) {   
  SpeedMotor.SetMotorSpeed(speed);
  carState.Speed = speed; 
}


// Set steering to given steering 
//    "steering": -100%...0%...100%
void WilhelmiCar::SetSteering(int steering) {
  SteerMotor.SetMotorSpeed(steering);
  carState.Steering = steering;
}


// SendMoveCommand from web api etc
//    "cmd": command data
void WilhelmiCar::SendMoveCommand(MoveCommand cmd) {
  
  Serial.print("Command: ");
  Serial.print(cmd.Movement);
  Serial.print(" / Speed: ");
  Serial.print(cmd.Speed);
  Serial.print(" / Steering: ");
  Serial.println(cmd.Steering);

  carState.Movement = cmd.Movement;


  digitalWrite(LED_AUTOMATIC, carState.Movement == Automatic ? HIGH : LOW);
  
  // Handle Automatic
  if (carState.Movement == Automatic)
    autoPilot.StartAutoMode();
  else if (carState.Movement == Stop) {
    SetMotorSpeed(0);
    SetSteering(0);
  }
  else {
    // Handle Speed
    SetMotorSpeed(cmd.Speed);
    // Handle Steering
    SetSteering(cmd.Steering);
  }
}




// SetSettings from web api etc
//    "newSettings": settings data
void WilhelmiCar::SetSettings(Settings newSettings) {
  // Get data
  bool modeChanged = false;

  if (newSettings.Client_SSID.length() > 0  && (newSettings.Client_SSID != Client_SSID || newSettings.Client_PWD != Client_PWD) ) {
    Client_SSID = newSettings.Client_SSID;
    Client_PWD = newSettings.Client_PWD;
    preferences.putString("Client_SSID", Client_SSID);
    preferences.putString("Client_PWD", Client_PWD);  

    Serial.print("New SSID settings for ");
    Serial.println(Client_SSID);
  }


  if (newSettings.AutoStopByDistance != carState.AutoStopByDistance) {
    carState.AutoStopByDistance = newSettings.AutoStopByDistance;
    preferences.putBool("AutoStopByDistance", carState.AutoStopByDistance);
  }

if (newSettings.PwmFrequency != carState.PwmFrequency) {
    carState.PwmFrequency = newSettings.PwmFrequency;
    preferences.putInt("PwmFrequency", carState.PwmFrequency);
    Serial.print("New PWM frequency ");
    Serial.println(carState.PwmFrequency);
    modeChanged = true;
  }


  if (newSettings.PwmSteering != carState.PwmSteering) {
    Serial.print("Set steering pwm mode from ");
    Serial.print(carState.PwmSteering);
    Serial.print(" to ");
    Serial.println(newSettings.PwmSteering);    
    carState.PwmSteering = newSettings.PwmSteering;
    preferences.putBool("PwmSteering", carState.PwmSteering);
    modeChanged = true;
  }

  if (newSettings.PwmSpeed != carState.PwmSpeed) {
    Serial.print("Set speed pwm mode from ");
    Serial.print(carState.PwmSpeed);
    Serial.print(" to ");
    Serial.println(newSettings.PwmSpeed);    
    carState.PwmSpeed = newSettings.PwmSpeed;
    preferences.putBool("PwmSteering", carState.PwmSpeed);
    modeChanged = true;
  }  

  if (modeChanged) {
    setupOutputs();
  }
  else {
    Serial.println("Pwm mode not changed");
  }
}




void WilhelmiCar::SendStopCommand() {
    MoveCommand cmd;
    cmd.Movement = Stop;
    cmd.Speed = 0;
    cmd.Steering = 0;
    SendMoveCommand(cmd);
}



void WilhelmiCar::setDistanceValue(int &distance, int value) 
{
  // Ignore 0
    if (value > 0 )
      distance = value;
}

// global data to filter sensor input
#define NUM_DISTANCE_VALUES  4
static int distanceCounter[3] = {0, 0, 0};
static std::array<int, NUM_DISTANCE_VALUES> distanceData [3];

// GetFilteredDistance calculate distance based on input measurement,
// try to remove noise and outliers by median and average filter
int GetFilteredDistance(std::array<int, NUM_DISTANCE_VALUES> values) {
  std::sort(values.begin(), values.end());  
  int start = NUM_DISTANCE_VALUES / 2;
  return values[start];
  //return ( values[start] + values[start+1] + values[start+2] ) / 3;
}


void WilhelmiCar::updateDistanceValue(int &distance, int index) 
{
  int value = sensor.getDist_cm(index);
  if (value <= 0)
    return;

  distanceData[index][distanceCounter[index]++] = value;
  distance = GetFilteredDistance(distanceData[index]);
  
  distanceCounter[index] %= NUM_DISTANCE_VALUES;
}


// handleSensor handle distance sensor measurement
void WilhelmiCar::handleSensor() {
  // check timeout for measurement
  if (lastRead == 0 && sensor.isFinished()) 
  {
    updateDistanceValue(carState.Distance, 0);
    updateDistanceValue(carState.DistanceLeft, 1);
    updateDistanceValue(carState.DistanceRight, 2);
    //setDistanceValue(carState.Distance, sensor.getDist_cm(0));
    //setDistanceValue(carState.DistanceLeft, sensor.getDist_cm(1));
    //setDistanceValue(carState.DistanceRight, sensor.getDist_cm(2));
    lastRead = micros();    
  } else if (lastRead != 0 && micros() - lastRead > 20000 ) {
    // Normal restart after measurement (delay between measurements)
    lastRead = 0;
    sensor.startAsync(30000);
  }
}


// Read settings from internal storage
void WilhelmiCar::ReadSettings() {
  // Read settings from storage
  Client_SSID = preferences.getString("Client_SSID", "NoSSID");
  Client_PWD = preferences.getString("Client_PWD", "NoPassword");
  carState.AutoStopByDistance = preferences.getBool("AutoStopByDistance", false);
  carState.PwmFrequency = preferences.getInt("PwmFrequency", PWM_FREQ);  
  carState.PwmSpeed = preferences.getBool("PwmSpeed", true);
  carState.PwmSteering = preferences.getBool("PwmSteering", true);
  carState.Movement =  Stop;
  carState.Speed = 0;
  carState.Steering = 0;
  carState.Distance = 0;
}




// handle auto stop
void WilhelmiCar::handleAutoStop() {
  digitalWrite(WARNING_LED, carState.Distance > 60 ? LOW : HIGH);
  
  if (carState.AutoStopByDistance) {
    if(carState.Distance < 60 && carState.Speed > 0) {
      SendStopCommand();
    }
  }
}


// Activate horn
void WilhelmiCar::ActivateHorn(int frequency, int duration) {
  tone(HORN_PIN, frequency, duration);
}


// setupOutputs set output configuration depending on settings
void WilhelmiCar::setupOutputs() {
  pinMode(ACCESS_POINT_SELECT, INPUT_PULLUP);
  pinMode(LED_AUTOMATIC, OUTPUT);
  pinMode(WARNING_LED, OUTPUT); 


  bool bSpeedMotor = SpeedMotor.begin(carState.PwmSpeed, carState.PwmFrequency, PWM_RESOLUTION);
  bool bSteerMotor = SteerMotor.begin(carState.PwmSteering, carState.PwmFrequency, PWM_RESOLUTION);

  Serial.print("Init SpeedMotor: ");
  Serial.println(bSpeedMotor);
  Serial.print("Init SteerMotor: ");
  Serial.println(bSteerMotor);
  
  SetMotorSpeed(0);
  SetSteering(0);  
}




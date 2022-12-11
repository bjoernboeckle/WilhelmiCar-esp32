# WilhelmiCar-esp32

App Remote controlled car based on ESP32 controller

<img src="WilhelmiCarLogo.png" alt="logo" style="width:100px;"/>

The project was created to extend a arduino based student work.
An Android app or online web app can now be used to control the the self driving car either manually or start/stop auto mode.

## ESP32 code

https://github.com/bjoernboeckle/WilhelmiCar-esp32.git


## Web App 
https://wilhelmicar.azurewebsites.net/


## .Net Maui App 

### Playstore
https://play.google.com/store/apps/details?id=com.bjoernboeckle.wilhelmicar

### Code on Azure DevOps
(TODO)



# Setup
The car is based on an ESP32 D2 Mini, two AC motors and an HC-SR04 ultrasonic sensor.

|IO Pin|      Function                 |  Direction   |
|------|-------------------------------|-------------:|
|      | **Misc**                                     |
| 4    | wifi AP / client mode         | in           |
| 19   | LED Distance warning          | out          |
| 16   | Horn                          | out          |
| 2    | internal Led automatic mode   | out          |
|      | **Motor**                                    |
| 22   | L293D Motor EN1,2             | out, pwm     |
| 17   | L293D Motor 1A                | out          |
| 21   | L293D Motor 2A                | out          |
| 5    | L293D Motor EN3,4             | out, pwm     |
| 26   | L293D Motor 3A                | out          |
| 23   | L293D Motor 4A                | out          |
|      | **Distance**                                 |
| 27   | HC-SR04 Trigger               | out          |
| 25   | HC-SR04 Echo                  | in, interrupt|
| 32   | HC-SR04 Echo                  | in, interrupt|
| 12   | HC-SR04 Echo                  | in, interrupt|

#ifndef DEFINES_H
#define DEFINES_H

#define VERSION "1.9.2"


// Set these to your desired accesspoint SSID/PWD.
#define SSID_AP "Wilhelmi-Car"
// need to be >=8 and <63
#define  PWD_AP  NULL                  



// Define Pins
#define ACCESS_POINT_SELECT  4
#define LED_AUTOMATIC        2
#define WARNING_LED         19
#define DC_MOTOR_PWM_PIN    22
#define DC_MOTOR_1A         17
#define DC_MOTOR_2A         21
#define HORN_PIN            32

#define STEER_MOTOR_PWM_PIN  5
#define STEER_MOTOR_3A      26
#define STEER_MOTOR_4A      23

#define TRIG_PIN            16
#define ECHO_PIN            18

// PWM Channels
//12000
#define PWM_FREQ       50 
#define PWM_RESOLUTION 8
#define PWM_CHANNEL_DC_PWM 6
#define PWM_CHANNEL_STEER_PWM   7


#endif

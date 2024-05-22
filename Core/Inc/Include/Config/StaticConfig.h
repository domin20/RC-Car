#pragma once

#define WIRELESS_BAUDRATE 9600

// BUTTON
#define BUTTON_DEBOUNCE_TIME 0
#define BUTTON_WAIT_FOR_HOLD_TIME 1000
#define BUTTON_REPEAT_INTERVAL 100

#define MAX_ON_HOLD_FUNCTIONS 5

#define SERVO_TEST_HOLD_TIME 2000
#define MOTOR_TEST_HOLD_TIME 5000

// SERWO
#define SERVO_MIN_POSITION 500
#define SERVO_ZERO_POSITION 1500
#define SERVO_MAX_POSITION 2500

// MOTOR
#define MOTOR_DATA_MAX_DELAY 50

// ADC Parameters
#define ADC_CONVERSION_COUNT 2
#define MAX_ADC_RAW_VALUE 4096

#define MICROCONTROLLER_VOLTAGE 3.3f

#define R1_MAIN_SUPPLY 10000.0f
#define R2_MAIN_SUPPLY 1000.0f
#define R1_DC_MOTOR 4700.0f
#define R2_DC_MOTOR 1000.0f

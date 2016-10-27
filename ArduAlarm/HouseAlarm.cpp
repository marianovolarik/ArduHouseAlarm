#include <Arduino.h>
#include "HouseAlarm.h"
#include "pitches.h"


#define LED_PIN         13

#define ONE_SECOND_MS  1000

#define PRE_ACTIVATED_TIME_S    10
#define PRE_ACTIVATED_TIME_MS   PRE_ACTIVATED_TIME_S*1000

#define PRE_RINGING_TIME_S      5
#define PRE_RINGING_TIME_MS     PRE_RINGING_TIME_S*1000

#define MAX_DEACTIVATION_CODE_LEN    10
#define DEACTIVATION_CODE            "1364*"


HouseAlarm::HouseAlarm(int BuzzerPin)
{
    pin_buzzer = BuzzerPin;
    pinMode(pin_buzzer, OUTPUT);
    state = PRE_ACTIVATED;
}

void HouseAlarm::begin(){
    startTime = millis();
    secondsCounter = startTime + ONE_SECOND_MS;
}

void HouseAlarm::run(){
    unsigned long currentTime = 0;

    currentTime = millis();
    switch(state){
        case PRE_ACTIVATED:

            /* 1 Hz beep */
            if (currentTime > secondsCounter){
                secondsCounter += ONE_SECOND_MS; /* 1 second added */
                pip();
            }

            if (currentTime > startTime + PRE_ACTIVATED_TIME_MS){
                state = ACTIVATED;
                startTime = millis();
                secondsCounter = startTime + ONE_SECOND_MS;
            }
            break;

        case ACTIVATED:

            /* 1 Hz beep */
            if (currentTime > secondsCounter){
                secondsCounter += ONE_SECOND_MS; /* 1 second added */
                //switchLed = !switchLed;
            }

            //digitalWrite(LED_PIN, HIGH ? switchLed : LOW);
            break;

        case PRE_RINGING:

            /* 1 Hz beep */
            if (currentTime > secondsCounter){
                secondsCounter += ONE_SECOND_MS; /* 1 second added */
                pip();
            }

            if (currentTime > startTime + PRE_RINGING_TIME_MS){
                state = RINGING;
                startTime = millis();
                secondsCounter = startTime + ONE_SECOND_MS;
            }
            break;

        case DEACTIVATED:
            noTone(pin_buzzer);
            break;

    };
}

void HouseAlarm::activate(){
    if (state == DEACTIVATED){
        state = PRE_ACTIVATED;
        startTime = millis();
        secondsCounter = startTime + ONE_SECOND_MS;
    }
}

void HouseAlarm::ring(){
    if (state == ACTIVATED){
        state = PRE_RINGING;
        startTime = millis();
        secondsCounter = startTime + ONE_SECOND_MS;
    }
}

bool HouseAlarm::is_ringing(){
    return state == RINGING;
}

bool HouseAlarm::deactivate(char *pwd){
    if (strcmp(pwd, DEACTIVATION_CODE) == 0){
        state = DEACTIVATED;
    }
}

/*
 * Private methods
 */

void HouseAlarm::pip(){
    tone(pin_buzzer, NOTE_C5, 250);
}
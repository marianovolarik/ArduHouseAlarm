#include "pitches.h"

/* Pin assignments */
#define REED_SWITCH_PIN 2
#define BUZZER_PIN      3
#define LED_PIN         13

#define ONE_SECOND_MS  1000

#define PRE_ACTIVATED_TIME_MS   10*1000
#define PRE_RINGING_TIME_MS     5*1000


enum states_t {PRE_ACTIVATED, ACTIVATED, PRE_RINGING, RINGING, DEACTIVATED};
enum states_t state = PRE_ACTIVATED;

unsigned long startTime = 0;
unsigned long currentTime = 0;
unsigned long secondsCounter = 0;

boolean switchTone = false;
boolean switchLed = false;


void setup() {
    pinMode(REED_SWITCH_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    startTime = millis();
    secondsCounter = startTime + 1000;
}


void loop(){

    currentTime = millis();
    switch(state){
        case PRE_ACTIVATED:

            /* 1 Hz beep */
            if (currentTime > secondsCounter){
                secondsCounter += ONE_SECOND_MS; /* 1 second added */
                tone(BUZZER_PIN, NOTE_C5, 250);
            }

            if (currentTime > startTime + PRE_ACTIVATED_TIME_MS){
                state = ACTIVATED;
                startTime = millis();
                secondsCounter = startTime + ONE_SECOND_MS;
            }

            /*
             * TO DO: If the correct password was entered,
             *        go to the state DEACTIVATED
             */
            break;

        case ACTIVATED:

            /* 1 Hz beep */
            if (currentTime > secondsCounter){
                secondsCounter += ONE_SECOND_MS; /* 1 second added */
                switchLed = !switchLed;
            }

            digitalWrite(LED_PIN, HIGH ? switchLed : LOW);

            if (digitalRead(REED_SWITCH_PIN) == LOW){
                state = PRE_RINGING;
                startTime = millis();
                secondsCounter = startTime + ONE_SECOND_MS;
            }

            /*
             * TO DO: If the correct password was entered,
             *        go to the state DEACTIVATED
             */
            break;

        case PRE_RINGING:

            /* 1 Hz beep */
            if (currentTime > secondsCounter){
                secondsCounter += ONE_SECOND_MS; /* 1 second added */
                tone(BUZZER_PIN, NOTE_B5, 250);
            }

            if (currentTime > startTime + PRE_RINGING_TIME_MS){
                state = RINGING;
                startTime = millis();
                secondsCounter = startTime + ONE_SECOND_MS;
            }

            /*
             * TO DO: If the correct password was entered,
             *        go to the state DEACTIVATED
             */
            break;

        case RINGING:

            /* 1 Hz beep */
            if (currentTime > secondsCounter){
                secondsCounter += ONE_SECOND_MS; /* 1 second added */
                switchTone = !switchTone;
            }

            if (switchTone){
                tone(BUZZER_PIN, NOTE_B5);
            }
            else{
                tone(BUZZER_PIN, NOTE_G5);
            }

            /*
             * TO DO: If the correct password was entered,
             *        go to the state DEACTIVATED
             */
            break;

        case DEACTIVATED:
            noTone(BUZZER_PIN);
            break;

    };
}

#include <Keypad.h>
#include "pitches.h"

/* Pin assignments */

// Keypad
#define ROW0_PIN    2
#define ROW1_PIN    3
#define ROW2_PIN    4
#define ROW3_PIN    5

#define COL0_PIN    6
#define COL1_PIN    7
#define COL2_PIN    8

#define BUZZER_PIN      9
#define REED_SWITCH_PIN 10
#define LED_PIN         13


#define ONE_SECOND_MS  1000

#define PRE_ACTIVATED_TIME_S    10
#define PRE_ACTIVATED_TIME_MS   PRE_ACTIVATED_TIME_S*1000

#define PRE_RINGING_TIME_S      5
#define PRE_RINGING_TIME_MS     PRE_RINGING_TIME_S*1000

#define MAX_DEACTIVATION_CODE_LEN    10
#define DEACTIVATION_CODE            "1364*"


const byte rows = 4;
const byte cols = 3;
char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[rows] = {ROW0_PIN, ROW1_PIN, ROW2_PIN, ROW3_PIN};
byte colPins[cols] = {COL0_PIN, COL1_PIN, COL2_PIN};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

/* Alarm states */
enum states_t {PRE_ACTIVATED, ACTIVATED, PRE_RINGING, RINGING, DEACTIVATED};
enum states_t state = PRE_ACTIVATED;

unsigned long startTime = 0;
unsigned long currentTime = 0;
unsigned long secondsCounter = 0;

boolean switchTone = false;
boolean switchLed = false;

char code[MAX_DEACTIVATION_CODE_LEN];
char code_index = 0;


void setup() {
    pinMode(REED_SWITCH_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    startTime = millis();
    secondsCounter = startTime + 1000;

    code[0] = '\0';
}


void loop(){

    char key = keypad.getKey();

    if (key != NO_KEY){
        tone(BUZZER_PIN, NOTE_A6, 125);
        code[code_index] = key;
        code_index++;

        if (key == '#'){
            code_index = 0;
            code[code_index] = '\0';

            /* Jump to state PRE_ACTIVATED */
            state = PRE_ACTIVATED;
            startTime = millis();
            secondsCounter = startTime + 1000;
        }

        if (key == '*'){
            /* Check code */
            code[code_index] = '\0';
            if (strcmp(code, DEACTIVATION_CODE) == 0){
                /* Jump to state DEACTIVATED */
                state = DEACTIVATED;
            }

            code_index = 0;
            code[code_index] = '\0';
        }
    }

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
            break;

        case DEACTIVATED:
            noTone(BUZZER_PIN);
            break;

    };
}

#include <Keypad.h>

#include "HouseAlarm.h"
#include "pitches.h"

#define BUZZER_PIN      9
#define REED_SWITCH_PIN 10

// Keypad
#define N_ROWS  (byte) 4
#define N_COLS  (byte) 3

#define ROW0_PIN    2
#define ROW1_PIN    3
#define ROW2_PIN    4
#define ROW3_PIN    5

#define COL0_PIN    6
#define COL1_PIN    7
#define COL2_PIN    8


char keys[N_ROWS][N_COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[N_ROWS] = {ROW0_PIN, ROW1_PIN, ROW2_PIN, ROW3_PIN};
byte colPins[N_COLS] = {COL0_PIN, COL1_PIN, COL2_PIN};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, N_ROWS, N_COLS);


HouseAlarm alarm(BUZZER_PIN);

char code[10];
char code_index = 0;

void setup() {
    pinMode(REED_SWITCH_PIN, INPUT_PULLUP);
    alarm.begin();
    code[0] = '\0';
}


void loop(){

    char key = keypad.getKey();
    if (key != NO_KEY){
        tone(BUZZER_PIN, NOTE_B5, 125);
        code[code_index] = key;
        code_index++;

        if (key == '#'){
            alarm.activate();
            code_index = 0;
            code[code_index] = '\0';
        }

        if (key == '*'){
            /* Check code */
            code[code_index] = '\0';
            alarm.deactivate(code);
            code_index = 0;
            code[code_index] = '\0';
        }
    }


    if (digitalRead(REED_SWITCH_PIN) == HIGH){
        alarm.ring();
    }
    alarm.run();

    if (alarm.is_ringing()){
        tone(BUZZER_PIN, NOTE_B7, 250);
    }
}

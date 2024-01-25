// -------------------------------------------------
// Copyright (c) 2022 HiBit <https://www.hibit.dev>
// -------------------------------------------------

#include "pitches.h"
#include <Servo.h>

#define BUZZER_PIN 12
#define PHOTO_RESISTOR A0
#define LED_PIN 5
#define LED_2PIN 6


Servo myservo;
Servo myservo2;

int pos = 0;
unsigned long lastTime = 0;
unsigned long dt = 1000; // dt in milliseconds

// Star Wars melody tunes
int melody[] = {
  NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_F5, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_F5, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,

  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5, NOTE_C5, NOTE_C5,
  NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,
  NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_C5,
  NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,

  NOTE_C6, NOTE_G5, NOTE_G5, REST, NOTE_C5,
  NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,
  NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_D5, NOTE_E5, NOTE_C6, NOTE_C6,
  NOTE_F6, NOTE_DS6, NOTE_CS6, NOTE_C6, NOTE_AS5, NOTE_GS5, NOTE_G5, NOTE_F5,
  NOTE_C6
};
// time
int durations[] = {
  8, 8, 8,
  2, 2,
  8, 8, 8, 2, 4,
  8, 8, 8, 2, 4,
  8, 8, 8, 2, 8, 8, 8,
  2, 2,
  8, 8, 8, 2, 4,

  8, 8, 8, 2, 4,
  8, 8, 8, 2, 8, 16,
  4, 8, 8, 8, 8, 8,
  8, 8, 8, 4, 8, 4, 8, 16,
  4, 8, 8, 8, 8, 8,

  8, 16, 2, 8, 8,
  4, 8, 8, 8, 8, 8,
  8, 8, 8, 4, 8, 4, 8, 16,
  4, 8, 4, 8, 4, 8, 4, 8,
  1
};

int lightAmount;
const int buttonPin = 4;  // the number of the pushbutton pin
int buttonState = 0;  // variable for reading the pushbutton status



void setup()
{
  pinMode(BUZZER_PIN, OUTPUT);// audio pin
  pinMode(A0, INPUT); //photo resistor (not used)
  pinMode(LED_PIN, OUTPUT); //blue light saber LED
  pinMode(LED_2PIN, OUTPUT); //red light saber LED

  pinMode(buttonPin, INPUT); //button


  Serial.begin(9600);
  myservo.attach(3);
  myservo2.attach(2);

}

void loop()
{
  lightAmount = analogRead(A0); //read light amount
  Serial.println(lightAmount); //print the values, used to debug
  buttonState = digitalRead(buttonPin); // read button state

  //detect when button is pressed
  if (buttonState==HIGH){


    // duration of the melody
    int size = sizeof(durations) / sizeof(int);
    //go throught the duration for each note
    for (int note = 0; note < size; note++) {
      
      //to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int duration = 1000 / durations[note];
      //to distinguish the notes, set a minimum time between them.
      //the note's duration + 30% seems to work well:
      int pauseBetweenNotes = duration * 1.30;

      //Turn on both light sabers
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(LED_2PIN, HIGH);
      // Set both motors away from each other 
      myservo.write(90);
      myservo2.write(10);

      //proper time delay for motor control and light control
      if (millis() - lastTime  >= dt)   // wait for dt milliseconds
      {
        lastTime = millis();
        //switch state of the motors and LEDs 
        myservo.write(10);
        myservo2.write(90);
        digitalWrite(LED_PIN, LOW);
        digitalWrite(LED_2PIN, LOW);

      }
      //play given note on the buzer
      tone(BUZZER_PIN, melody[note], duration);
      

      
      //read states within the loop (not used)
      lightAmount = analogRead(A0);
      buttonState = digitalRead(buttonPin);
      //pauses between the notes. The values are inditacted above
      delay(pauseBetweenNotes);
      
      
      

      
      // if (buttonState==LOW){
      //   break;
      // }
    }
  }else{
    //stop the tone playing and turn off the lighs:
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LED_2PIN, LOW);
    noTone(BUZZER_PIN);
  
  }
}
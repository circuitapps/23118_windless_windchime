/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

#define PIN_0 0
#define PIN_1 1
#define PIN_2 2
#define PIN_3 3
#define PIN_4 4

#define MAX_REPS 20

void pulse_drive(int output_pin, int on_duration_ms, int off_duration_ms)
{
  digitalWrite(output_pin, HIGH);  // turn pin on (HIGH is the voltage level)
  delay(on_duration_ms);           // wait
  digitalWrite(output_pin, LOW);   // turn pin off by making the voltage LOW
  delay(off_duration_ms);          // wait
}

void pulse_sequence_feature()
{

  int i;

  pulse_drive(PIN_0, 50, 50);

  for(i=0;i<MAX_REPS; ++i)
  {
    pulse_drive(PIN_3, 200, 100);
  }

  pulse_drive(PIN_0, 50, 50);

  for(i=0;i<MAX_REPS; ++i)
  {
    pulse_drive(PIN_3, 100, 100);
  }

  pulse_drive(PIN_0, 50, 50);

  for(i=0;i<MAX_REPS; ++i)
  {
    pulse_drive(PIN_3, 50, 100);
  }

  pulse_drive(PIN_0, 50, 50);

  for(i=0;i<MAX_REPS; ++i)
  {
    pulse_drive(PIN_3, 25, 100);
  }

  pulse_drive(PIN_0, 50, 50);

  for(i=0;i<MAX_REPS; ++i)
  {
    pulse_drive(PIN_3, 10, 100);
  }

}


void windless_chime_feature()
{
  int i;
  int off_duration_ms = 5000;

  pulse_drive(PIN_0, 50, 50);

  for(i=0; i<5; ++i)
  {
    pulse_drive(PIN_3, 25, 25);
  }

  delay(off_duration_ms);   // wait

}


// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(PIN_0, OUTPUT);
  pinMode(PIN_3, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  
  //pulse_sequence_feature();

  windless_chime_feature();

}

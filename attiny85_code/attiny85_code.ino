
/*
  Windless windchime control code

  Operation: ON_OFF_PIN turns the device on if it is off and vice versa. While the device is off, LEVEL_UP_PIN and LEVEL_DOWN_PIN presses are ignored.
  When the device is turned on, the COIL_OUT_PIN is activated every DEFAULT_PULSE_RATE x TIME_BASE_MSEC milliseconds. To make the coil activation
  more frequent, user needs to press the LEVEL_UP_PIN. To reduce the coil activation rate, user needs to press the LEVEL_DOWN_PIN. Each button press 
  increases or descreases the coil activation rate by TIME_BASE_MSEC milliseconds. The slowest coil activation rate is MIN_COIL_PULSE_RATE x TIME_BASE_MSEC milliseconds.
  The fastest coil activation rate is MAX_COIL_PULSE_RATE x TIME_BASE_MSEC milliseconds.

  Each time any of the ON_OFF_PIN, LEVEL_UP_PIN or LEVEL_DOWN_PINs are pressed the LED_OUT_PIN goes high twice; one for the rising edge and one for the falling edge as
  a result of mechanically pressing and depressing the pushbutton. All the pin interactions are interrupt driven.

  by circuitapps
  October 2023
*/
#include <avr/io.h>

// PORT B DEFINITIONS
#define PB5_PIN1 PB5
#define PB4_PIN3 PB4
#define PB3_PIN2 PB3
#define PB2_PIN7 PB2
#define PB1_PIN6 PB1
#define PB0_PIN5 PB0

// Application pin definitions
#define COIL_OUT_PIN PB3_PIN2
#define LED_OUT_PIN PB0_PIN5
#define LEVEL_UP_PIN PB1_PIN6
#define LEVEL_DOWN_PIN PB4_PIN3
#define ON_OFF_PIN PB2_PIN7

#define PC_INT_VECTOR PCINT0_vect  // On ATtiny85, there is ONE interrupt vector (i.e., one interrupt service routine) that gets called when ANY of the ctive interrupt pins are triggered !

//#define MAX_REPS 20
#define MAX_COIL_PULSE_RATE 1  // Pulse once every second
#define MIN_COIL_PULSE_RATE 10  // Pulse once every ten seconds
#define TIME_BASE_MSEC 1000  // Maximum time delay (in milliseconds) before the coil can be reactivated
#define DEFAULT_PULSE_RATE 5  // initialization value at power up

#define PULSE_DRIVE_ON_PERIOD_MSEC 25  // How long the COIL_OUT_PIN is kept HIGH in milliseconds
#define PULSE_DRIVE_OFF_PERIOD_MSEC 25  // How long the COIL_OUT_PIN is kept LOW in milliseconds

#define POWER_ON 1
#define POWER_OFF 0

// Global variables follow
int current_pulse_rate = 5;  // default coil activation rate is once per 5 seconds
int current_on_off_state = POWER_OFF;

void pulse_drive(int output_pin, int on_duration_ms, int off_duration_ms)
{
  digitalWrite(output_pin, HIGH);  // turn pin on (HIGH is the voltage level)
  delay(on_duration_ms);           // wait
  digitalWrite(output_pin, LOW);   // turn pin off by making the voltage LOW
  delay(off_duration_ms);          // wait
}

void pin_interrupt_test_setup()
{
  int current_pulse_rate = 5;  // default coil activation rate is once per 5 seconds
  current_on_off_state = POWER_OFF;

  cli();  // disable interrupts during set up
  // Three pin interrupts are enabled below
  PCMSK |= (1 << LEVEL_UP_PIN) | (1 << LEVEL_DOWN_PIN) | (1 << ON_OFF_PIN);
  GIFR  |= (1 << PCIF);  // clear any outstanding interrupts
  GIMSK |= (1 << PCIE);  // Pin Change Interrupts are Enabled
  pinMode(LEVEL_UP_PIN, INPUT_PULLUP);  // 20K pullup enabled. Pin needs to operate in "active low"
  pinMode(LEVEL_DOWN_PIN, INPUT_PULLUP);  // 20K pullup enabled. Pin needs to operate in "active low"
  pinMode(ON_OFF_PIN, INPUT_PULLUP);  // 20K pullup enabled. Pin needs to operate in "active low"
  pinMode(LED_OUT_PIN, OUTPUT);
  pinMode(COIL_OUT_PIN, OUTPUT);
  sei();  // enable interrupts after set up (last line of set up)
}


ISR(PC_INT_VECTOR)
{
  // One of the interrupt pins got triggered. Identify which.

  // Turn LED on at the start
  digitalWrite(LED_OUT_PIN, HIGH);  // turn pin on (HIGH is the voltage level)

  if(digitalRead(LEVEL_UP_PIN) == LOW)
  {
    if(current_on_off_state == POWER_ON)
    {
      // This interrupt will work ONLY IF power is ON
      if(current_pulse_rate > MAX_COIL_PULSE_RATE)
      {
        // Pulse rate not at fastest rate yet 
        --current_pulse_rate;  // pulse one second faster
      }
    }
    else
      current_pulse_rate = DEFAULT_PULSE_RATE;

  }
  else if(digitalRead(LEVEL_DOWN_PIN) == LOW)
  {
      if(current_on_off_state == POWER_ON)
      {
        // This interrupt will work ONLY IF power is ON
        if(current_pulse_rate < MIN_COIL_PULSE_RATE)
        {
          // Pulse rate not at slowest rate yet 
          ++current_pulse_rate;  // pulse one second faster
        }
      }
      else
        current_pulse_rate = DEFAULT_PULSE_RATE;

  }
  else
  {
    // ON_OFF_PIN must have fired
    if(digitalRead(ON_OFF_PIN) == LOW)
    {
      if(current_on_off_state == POWER_OFF)
      {
        // Power turned ON
        current_pulse_rate = DEFAULT_PULSE_RATE;
        //last_trigger = 3;
        current_on_off_state = POWER_ON;  // Power now turned ON
      }
      else
      {
        // Power now turned OFF
        current_pulse_rate = DEFAULT_PULSE_RATE;
        //last_trigger = 0;
        current_on_off_state = POWER_OFF;  // Power now turned OFF
      }
    }

  }

  // Turn LED off at the end
  digitalWrite(LED_OUT_PIN, LOW);   // turn pin off by making the voltage LOW

}

void coil_activation()
{
  int i;

  delay(TIME_BASE_MSEC * current_pulse_rate);   // wait based on current coil activation delay state
  
  for(i=0; i<5; ++i)
  {
    pulse_drive(COIL_OUT_PIN, PULSE_DRIVE_ON_PERIOD_MSEC, PULSE_DRIVE_OFF_PERIOD_MSEC);
  }

}

// the setup function runs once when you press reset or power the board
void setup() {

  // windless_chime_setup();
  pin_interrupt_test_setup();

  delay(250);   // wait
  current_pulse_rate = DEFAULT_PULSE_RATE;
  current_on_off_state = POWER_OFF;
  digitalWrite(LED_OUT_PIN, LOW);
  digitalWrite(COIL_OUT_PIN, LOW);
}

// the loop function runs over and over again forever

void loop() {

  if(current_on_off_state == POWER_ON)
    coil_activation();  // main functional routine

}

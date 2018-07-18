#include "InfraredTracing.h"

byte dat = 0;
byte ItPins[5];
// Provides ISR
#ifndef __AVR_ATmega32U4__
#include <avr/interrupt.h>
/*
ISR(IT_TIMER_INTR_NAME)
{
    // Serial.println("ISR");
    // Serial.println(millis());
    dat = 0;
    dat = digitalRead(ItPins[0]);
    dat |= digitalRead(ItPins[1]) << 1;
    dat |= digitalRead(ItPins[2]) << 2;
    dat |= digitalRead(ItPins[3]) << 3;
    dat |= digitalRead(ItPins[4]) << 4;
    //Serial.println(dat,BIN);
}
*/
/**
 * Alternate Constructor which can call your own function to map the IR to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
InfraredTracing::InfraredTracing(byte pin1, byte pin2)
{
    InfraredTracingPin1 = pin1;
    InfraredTracingPin2 = pin2;
    pinMode(InfraredTracingPin1, INPUT);
    pinMode(InfraredTracingPin2, INPUT);
    ItPins[0] = InfraredTracingPin1;
    ItPins[1] = InfraredTracingPin2;
    value = 0;
    InfraredTracingMode = E_INFRARED_SENSOR_2;
    begin();
}

InfraredTracing::InfraredTracing(byte pin1, byte pin2, byte pin3)
{
    InfraredTracingPin1 = pin1;
    InfraredTracingPin2 = pin2;
    InfraredTracingPin3 = pin3;
    pinMode(InfraredTracingPin1, INPUT);
    pinMode(InfraredTracingPin2, INPUT);
    pinMode(InfraredTracingPin3, INPUT);
    ItPins[0] = InfraredTracingPin1;
    ItPins[1] = InfraredTracingPin2;
    ItPins[2] = InfraredTracingPin3;
    value = 0;
    InfraredTracingMode = E_INFRARED_SENSOR_3;
    begin();
}

InfraredTracing::InfraredTracing(byte pin1, byte pin2, byte pin3, byte pin4)
{
    InfraredTracingPin1 = pin1;
    InfraredTracingPin2 = pin2;
    InfraredTracingPin3 = pin3;
    InfraredTracingPin4 = pin4;
    pinMode(InfraredTracingPin1, INPUT);
    pinMode(InfraredTracingPin2, INPUT);
    pinMode(InfraredTracingPin3, INPUT);
    pinMode(InfraredTracingPin4, INPUT);
    ItPins[0] = InfraredTracingPin1;
    ItPins[1] = InfraredTracingPin2;
    ItPins[2] = InfraredTracingPin3;
    ItPins[3] = InfraredTracingPin4;
    value = 0;
    InfraredTracingMode = E_INFRARED_SENSOR_4;
	  begin();
}

InfraredTracing::InfraredTracing(byte pin1, byte pin2, byte pin3, byte pin4, byte pin5)
{
    InfraredTracingPin1 = pin1;
    InfraredTracingPin2 = pin2;
    InfraredTracingPin3 = pin3;
    InfraredTracingPin4 = pin4;
    InfraredTracingPin5 = pin5;
    pinMode(InfraredTracingPin1, INPUT);
    pinMode(InfraredTracingPin2, INPUT);
    pinMode(InfraredTracingPin3, INPUT);
    pinMode(InfraredTracingPin4, INPUT);
    pinMode(InfraredTracingPin5, INPUT);
    ItPins[0] = InfraredTracingPin1;
    ItPins[1] = InfraredTracingPin2;
    ItPins[2] = InfraredTracingPin3;
    ItPins[3] = InfraredTracingPin4;
    ItPins[4] = InfraredTracingPin5;
    value = 0;
    InfraredTracingMode = E_INFRARED_SENSOR_5;
    begin();
}
/**
 * \par Function
 *    begin
 * \par Description
 *    Initialize interrupt.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void InfraredTracing::begin()
{

  cli();
  // setup pulse clock timer interrupt
  //Prescale /8 (16M/8 = 0.5 microseconds per tick)
  // Therefore, the timer interval can range from 0.5 to 128 microseconds
  // depending on the reset value (255 to 0)
  IT_TIMER_CONFIG_NORMAL();

  //Timer2 Overflow Interrupt Enable
  IT_TIMER_ENABLE_INTR;

  // TIMER_RESET;

  sei();  // enable interrupts

  // initialize state machine variables
  value = 0;

}

/**
 * \par Function
 *    end
 * \par Description
 *    Close the interrupt.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void InfraredTracing::end()
{
  EIMSK &= ~(1 << INT0);
}

byte InfraredTracing::getValue()
{
  value = dat;
	return value;
}
#endif

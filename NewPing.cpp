/*
*	NewPing.cpp - Handles the timers. Based on the timers of NewPing library
*	Version: 0.3
*	Author: Dimitris Platis (based on the work by Tim Eckel - teckel@leethost.com)
* 	License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
*/
#include "Smartcar_sensors.h"

// Variables used for timer functions
void (*intFunc)();
void (*intFunc2)();
unsigned long _ms_cnt_reset;
volatile unsigned long _ms_cnt;

void NewPing::timer_start(unsigned long frequency, void (*userFunc)(void)) {
	timer_setup();                       // Configure the timer interrupt.
	intFunc = NewPing::timer_start_cntdwn;  // Timer events are sent here once every ms till user's frequency is reached.
	intFunc2 = userFunc;                 // User's function to call when user's frequency is reached.
	_ms_cnt = _ms_cnt_reset = frequency; // Current ms counter and reset value.

#if defined (__AVR_ATmega32U4__) // Use Timer4 for ATmega32U4 (Teensy/Leonardo).
	OCR4C = 249;         // Every count is 4uS, so 1ms = 250 counts - 1.
	TIMSK4 = (1<<TOIE4); // Enable Timer4 interrupt.
#else
	OCR2A = 249;           // Every count is 4uS, so 1ms = 250 counts - 1.
	TIMSK2 |= (1<<OCIE2A); // Enable Timer2 interrupt.
#endif
}

void NewPing::timer_stop() { // Disable timer interrupt.
#if defined (__AVR_ATmega32U4__) // Use Timer4 for ATmega32U4 (Teensy/Leonardo).
	TIMSK4 = 0;
#else
	TIMSK2 &= ~(1<<OCIE2A);
#endif
}

void NewPing::timer_setup() {
#if defined (__AVR_ATmega32U4__) // Use Timer4 for ATmega32U4 (Teensy/Leonardo).
	timer_stop(); // Disable Timer4 interrupt.
	TCCR4A = TCCR4C = TCCR4D = TCCR4E = 0;
	TCCR4B = (1<<CS42) | (1<<CS41) | (1<<CS40) | (1<<PSR4); // Set Timer4 prescaler to 64 (4uS/count, 4uS-1020uS range).
	TIFR4 = (1<<TOV4);
	TCNT4 = 0;    // Reset Timer4 counter.
#else
	timer_stop();           // Disable Timer2 interrupt.
	ASSR &= ~(1<<AS2);      // Set clock, not pin.
	TCCR2A = (1<<WGM21);    // Set Timer2 to CTC mode.
	TCCR2B = (1<<CS22);     // Set Timer2 prescaler to 64 (4uS/count, 4uS-1020uS range).
	TCNT2 = 0;              // Reset Timer2 counter.
#endif
}


void NewPing::timer_start_cntdwn() {
	if (!_ms_cnt--) {            // Count down till we reach zero.
		intFunc2();              // Scheduled time reached, run the main timer event function.
		_ms_cnt = _ms_cnt_reset; // Reset the ms timer.
	}
}


#if defined (__AVR_ATmega32U4__) // Use Timer4 for ATmega32U4 (Teensy/Leonardo).
ISR(TIMER4_OVF_vect) {
#else
ISR(TIMER2_COMPA_vect) {
#endif
	if(intFunc) intFunc(); // If wrapped function is set, call it.
}

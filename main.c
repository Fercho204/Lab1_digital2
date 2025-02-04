//Digital 2
//Laboratorio 1
//Fernando Palomo - 22357
// Juego de carreras con display de 7 segmentos y contador de décadas para cada jugador

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Display7seg/Display7seg.h"

#define BTN_START PC4  // Botón de inicio en PC4
#define BTN_P1    PB4  // Botón Jugador 1 en PB4
#define BTN_P2    PB5  // Botón Jugador 2 en PB5

volatile uint8_t start_race = 0;
volatile uint8_t score_p1 = 1;  // Iniciar en 1
volatile uint8_t score_p2 = 1;  // Iniciar en 1
volatile uint8_t winner = 0;

// === SECUENCIA DE INICIO ===
void start_sequence() {
	PORTC = 0;
	PORTB = 0;
	for (int8_t i = 5; i >= 1; i--) {
		display_number(i);
		_delay_ms(1000);
	}
	start_race = 1;
	winner = 0;
	display_number(0);
}

// === CONFIGURACIÓN DEL SISTEMA ===
void setup() {
	cli();
	display_init();
	UCSR0B &= ~((1 << RXEN0) | (1 << TXEN0));  // Deshabilitar UART
	// Configurar LEDs como salida
	DDRC |= 0x0F;  // LEDs de P1
	DDRB |= 0x0F;  // LEDs de P2
	
	// Configurar botones como entrada
	DDRB &= ~(1 << BTN_P1);
	DDRB &= ~(1 << BTN_P2);
	DDRC &= ~(1 << BTN_START);
	
	// Habilitar pull-ups internos
	PORTB |= (1 << BTN_P1);
	PORTB |= (1 << BTN_P2);
	PORTC |= (1 << BTN_START);

	// Configurar interrupciones de cambio de estado
	PCICR |= (1 << PCIE0);  // Habilitar interrupciones en PCINT0 y PCINT1
	PCICR |= (1 << PCIE1);
	PCMSK0 |= (1 << PCINT4);  // Habilitar PB4 y PB5 (botones de jugadores)
	PCMSK0 |= (1 << PCINT5);
	PCMSK1 |= (1 << PCINT12);  // Habilitar PC4 (botón de inicio)

	sei();  // Habilitar interrupciones globales
	display_number(0);
}

// === VERIFICACIÓN DE GANADOR ===
void check_winner() {
	if (score_p1 > 8) {
		winner = 1;
		PORTC = 0x0F;  // Enciende LEDs de P1
		PORTB = 0x00;  // Apaga LEDs de P2
		display_number(1);
		score_p1 = 1;
	}
	if (score_p2 > 8) {
		winner = 2;
		PORTB = 0x0F;  // Enciende LEDs de P2
		PORTC = 0x00;  // Apaga LEDs de P1
		display_number(2);
		score_p2 = 1;
	}
}



// === BUCLE PRINCIPAL ===
void loop() {
	check_winner(); // todo está manejado por interrupciones.
}

// === PROGRAMA PRINCIPAL ===
int main() {
	setup();
	while (1) {
		loop();
	}
	return 0;
}

// === INTERRUPCIÓN BOTONES DE JUGADORES ===
ISR(PCINT0_vect) {
	_delay_ms(60);
	if (start_race && !winner){
		if((PINB&(1<<BTN_P1))==0){
			PORTC = score_p1;
			score_p1 <<= 1;
			return;
		}
		if((PINB&(1<<BTN_P2))==0){
			PORTB = score_p2;
			score_p2 <<= 1;
			return;
		}
	}
	
}

// === INTERRUPCIÓN BOTÓN DE INICIO ===
ISR(PCINT1_vect) {
	if((PINC&(1<<BTN_START))==0)  {
		start_sequence();
	}
}

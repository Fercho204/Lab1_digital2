//Librería de display de 7 segmentos
#include "Display7seg.h"

// Mapeo de números al display de 7 segmentos (ánodo común)
const uint8_t SEGMENT_MAP[] = {
	0b01000000, // 0
	0b11111001, // 1
	0b10100100, // 2
	0b00110000, // 3
	0b10011001, // 4
	0b10010010, // 5
};

// Configurar el puerto D como salida para el display
void display_init() {
	DDRD = 0xFF; // Puerto D como salida
}

// Mostrar número en el display
void display_number(uint8_t num) {
	PORTD = SEGMENT_MAP[num];
}

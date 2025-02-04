//Librería de display de 7 segmentos (llamar a las funciones e incluir la librería)
#ifndef Display7seg_h_
#define Display7seg_h_
#include <stdint.h>
#include <avr/io.h>

// Inicializar el display
void display_init();

// Mostrar un número en el display
void display_number(uint8_t num);

#endif

#include "tablero.h"

void calcularMovimientosTorre(Casilla tablero[8][8], int fila, int col);
void calcularMovimientosRey(Casilla tablero[8][8], int fila, int col, bool reyNoSeHaMovido, bool torreCortaNoSeHaMovido, bool torreLargaNoSeHaMovido);
void calcularMovimientosPeon(Casilla tablero[8][8], int fila, int col);
void calcularMovimientosAlfil(Casilla tablero[8][8], int fila, int col);
void calcularMovimientosReina(Casilla tablero[8][8], int fila, int col);
void calcularMovimientosCaballo(Casilla tablero[8][8], int fila, int col);

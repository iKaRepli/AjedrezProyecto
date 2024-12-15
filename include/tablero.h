#ifndef TABLERO_H
#define TABLERO_H

#define TAMANO_CASILLA 100


struct Casilla {
    int pieza;
    int color;
    bool movimiento_valido;
};

enum ColorCasilla {
    COLOR_BLANCO = 0,
    COLOR_NEGRO,
    COLOR_CASILLA_SELECCIONADA,
    COLOR_MOVIMIENTO_VALIDO,
    COLOR_MOVIMIENTO_INVALIDO,
    COLOR_TOMAR_PIEZA
};

enum Turno{
    BLANCAS = 1,
    NEGRAS = -1
};

enum Pieza {
    CASILLA_VACIA = 0,
    PEON_BLANCO = 1,
    ALFIL_BLANCO = 2,
    CABALLO_BLANCO = 3,
    TORRE_BLANCA = 4,
    REINA_BLANCA = 5,
    REY_BLANCO = 6,
    PEON_NEGRO = -1,
    ALFIL_NEGRO = -2,
    CABALLO_NEGRO = -3,
    TORRE_NEGRA = -4,
    REINA_NEGRA = -5,
    REY_NEGRO = -6
};




// Declaración de funciones
void inicializarRecursosTablero(); // Carga las imágenes necesarias
void liberarRecursosTablero();
void inicializarTablero(Casilla tablero[8][8]);
void dibujarTablero(Casilla tablero[8][8],int turno);
void dibujarPieza(int pieza, int x, int y,int turno);




#endif

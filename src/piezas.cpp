#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "../include/tablero.h"
#include "../include/piezas.h"
#include <stdexcept>


bool esMovimientoValidoPeon(Casilla tablero[8][8], int fila, int col, int reyFila, int reyCol)
{
    // Determinar dirección del peón (1 para blanco, -1 para negro)
    int direccion = (tablero[fila][col].pieza > 0) ? -1 : 1;

    // Captura diagonal: verificar si el rey está en una posición capturable
    for (int dx = -1; dx <= 1; dx += 2) // -1 para izquierda, 1 para derecha
    {
        int nuevaFila = fila + direccion;
        int nuevaCol = col + dx;

        if (nuevaFila == reyFila && nuevaCol == reyCol)
        {
            return true; // El rey está en peligro
        }
    }

    return false; // El movimiento no amenaza al rey
}

bool esMovimientoValidoTorre(Casilla tablero[8][8], int fila, int col, int reyFila, int reyCol)
{
    const int direcciones[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (int d = 0; d < 4; ++d)
    {
        int dx = direcciones[d][0];
        int dy = direcciones[d][1];
        int x = fila, y = col;

        while (true)
        {
            x += dx;
            y += dy;

            // Verificar límites del tablero
            if (x < 0 || x >= 8 || y < 0 || y >= 8)
                break;

            if (tablero[x][y].pieza == 0)
            {
                continue; // Casilla vacía, seguir explorando
            }
            else if (x == reyFila && y == reyCol)
            {
                return true; // Rey en peligro
            }
            else
            {
                break; // Encontramos otra pieza, detener
            }
        }
    }
    return false;
}
bool esMovimientoValidoCaballo(Casilla tablero[8][8], int fila, int col, int reyFila, int reyCol) {
    // Movimientos posibles del caballo (en forma de "L")
    const int movimientos[8][2] = {
        {2, 1},   // Abajo derecha
        {2, -1},  // Abajo izquierda
        {-2, 1},  // Arriba derecha
        {-2, -1}, // Arriba izquierda
        {1, 2},   // Derecha abajo
        {1, -2},  // Izquierda abajo
        {-1, 2},  // Derecha arriba
        {-1, -2}  // Izquierda arriba
    };

    for (int m = 0; m < 8; ++m) {
        int x = fila + movimientos[m][0];
        int y = col + movimientos[m][1];

        // Verificar si el movimiento alcanza al rey
        if (x == reyFila && y == reyCol) {
            return true; // El rey está en peligro
        }
    }

    return false; // El rey no está amenazado por este caballo
}


bool esMovimientoValidoReina(Casilla tablero[8][8], int fila, int col, int reyFila, int reyCol) {
    // Direcciones de la reina: diagonales y rectas
    const int direcciones[8][2] = {
        {1, 1},   // Arriba derecha
        {1, -1},  // Abajo derecha
        {-1, 1},  // Arriba izquierda
        {-1, -1}, // Abajo izquierda
        {-1, 0},  // Arriba
        {1, 0},   // Abajo
        {0, -1},  // Izquierda
        {0, 1}    // Derecha
    };

    for (int d = 0; d < 8; ++d) {
        int dx = direcciones[d][0];
        int dy = direcciones[d][1];
        int x = fila, y = col;

        while (true) {
            x += dx;
            y += dy;

            // Verificar límites del tablero
            if (x < 0 || x >= 8 || y < 0 || y >= 8)
                break;

            // Si encuentra al rey en la dirección, retorna true
            if (x == reyFila && y == reyCol) {
                return true; // El rey está en peligro
            }

            // Si encuentra otra pieza, detiene la exploración
            if (tablero[x][y].pieza != CASILLA_VACIA) {
                break; // Bloqueado por otra pieza
            }
        }
    }

    return false; // No hay amenaza al rey en estas direcciones
}

bool esMovimientoValidoAlfil(Casilla tablero[8][8], int fila, int col, int reyFila, int reyCol) {
    // Direcciones de movimiento del alfil: diagonales
    const int direcciones[4][2] = {
        {1, 1},   // Arriba derecha
        {1, -1},  // Abajo derecha
        {-1, 1},  // Arriba izquierda
        {-1, -1}  // Abajo izquierda
    };

    // Explorar cada dirección diagonal
    for (int d = 0; d < 4; ++d) {
        int dx = direcciones[d][0];
        int dy = direcciones[d][1];
        int x = fila;
        int y = col;

        while (true) {
            x += dx;
            y += dy;

            // Verificar si está fuera del tablero
            if (x < 0 || x >= 8 || y < 0 || y >= 8)
                break;

            // Si encuentra al rey en la dirección, retorna true
            if (x == reyFila && y == reyCol) {
                return true; // El rey está en peligro
            }

            // Si encuentra otra pieza, detiene la exploración
            if (tablero[x][y].pieza != 0) {
                break; // Bloqueado por otra pieza
            }
        }
    }

    return false; // No hay amenaza al rey en estas diagonales
}

bool exponeReyAJaque(Casilla tablero[8][8], int origenFila, int origenCol, int destinoFila, int destinoCol)
{
    // Guardar estado actual del tablero
    int piezaOriginal = tablero[destinoFila][destinoCol].pieza;
    tablero[destinoFila][destinoCol].pieza = tablero[origenFila][origenCol].pieza;
    tablero[origenFila][origenCol].pieza = CASILLA_VACIA;

    // Buscar al rey del jugador actual
    int reyFila = -1, reyCol = -1;
    int color = (tablero[destinoFila][destinoCol].pieza > 0) ? 1 : -1;

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if ((color > 0 && tablero[i][j].pieza == REY_BLANCO) ||
                (color < 0 && tablero[i][j].pieza == REY_NEGRO))
            {
                reyFila = i;
                reyCol = j;
                break;
            }
        }
    }

    // Verificar si el rey queda en jaque
    bool enJaque = false;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            // Solo considerar piezas enemigas
            if ((color > 0 && tablero[i][j].pieza < 0) ||
                (color < 0 && tablero[i][j].pieza > 0))
            {
                // Verificar ataques de torres enemigas
                switch (tablero[i][j].pieza)
                {
                case TORRE_BLANCA:
                case TORRE_NEGRA:
                    if (esMovimientoValidoTorre(tablero, i, j, reyFila, reyCol))
                    {
                        enJaque = true;
                    }
                    break;

                case REY_BLANCO:
                case REY_NEGRO:
                    if (abs(reyFila - i) <= 1 && abs(reyCol - j) <= 1)
                    {
                        enJaque = true;
                    }
                    break;
                case ALFIL_BLANCO:
                case ALFIL_NEGRO:

                 if (esMovimientoValidoAlfil(tablero, i, j, reyFila, reyCol))
                    {
                        enJaque = true;
                    }
                    break;
                case REINA_BLANCA:
                case REINA_NEGRA:

                 if (esMovimientoValidoReina(tablero, i, j, reyFila, reyCol))
                    {
                        enJaque = true;
                    }
                    break;
                case PEON_BLANCO:
                case PEON_NEGRO:

                 if (esMovimientoValidoPeon(tablero, i, j, reyFila, reyCol))
                    {
                        enJaque = true;
                    }
                    break;
                case CABALLO_BLANCO:
                case CABALLO_NEGRO:

                 if (esMovimientoValidoCaballo(tablero, i, j, reyFila, reyCol))
                    {
                        enJaque = true;
                    }
                    break;

                default:
                    break;
                }

                // Aquí puedes agregar lógica para otros tipos de piezas si es necesario
                // Ejemplo: ataques de reyes enemigos
                if (abs(tablero[i][j].pieza) == 6)
                {
                }

                if (enJaque)
                    break;
            }
        }
        if (enJaque)
            break;
    }

    // Restaurar estado original del tablero
    tablero[origenFila][origenCol].pieza = tablero[destinoFila][destinoCol].pieza;
    tablero[destinoFila][destinoCol].pieza = piezaOriginal;

    return enJaque;
}
void calcularMovimientosCaballo(Casilla tablero[8][8], int fila, int col) {
    // Limpiar movimientos válidos antes de calcular
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            tablero[i][j].movimiento_valido = false;
        }
    }

    // Movimientos posibles del caballo (en forma de "L")
    const int movimientos[8][2] = {
        {2, 1},   // Abajo derecha
        {2, -1},  // Abajo izquierda
        {-2, 1},  // Arriba derecha
        {-2, -1}, // Arriba izquierda
        {1, 2},   // Derecha abajo
        {1, -2},  // Izquierda abajo
        {-1, 2},  // Derecha arriba
        {-1, -2}  // Izquierda arriba
    };

    for (int m = 0; m < 8; ++m) {
        int x = fila + movimientos[m][0];
        int y = col + movimientos[m][1];

        // Verificar si está dentro del tablero
        if (x < 0 || x >= 8 || y < 0 || y >= 8)
            continue;

        // Simular el movimiento y verificar jaque
        if (tablero[x][y].pieza == CASILLA_VACIA) {
            if (!exponeReyAJaque(tablero, fila, col, x, y)) {
                tablero[x][y].movimiento_valido = true;
            }
        } else {
            // Si es una pieza enemiga, verificar captura
            if ((tablero[fila][col].pieza > 0 && tablero[x][y].pieza < 0) ||
                (tablero[fila][col].pieza < 0 && tablero[x][y].pieza > 0)) {
                if (!exponeReyAJaque(tablero, fila, col, x, y)) {
                    tablero[x][y].movimiento_valido = true;
                }
            }
        }
    }
}


void calcularMovimientosReina(Casilla tablero[8][8], int fila, int col) {
    // Limpiar movimientos válidos antes de calcular
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            tablero[i][j].movimiento_valido = false;
        }
    }

    // Direcciones de la reina: combinamos diagonales y rectas
    const int direcciones[8][2] = {
        {1, 1},   // Arriba derecha
        {1, -1},  // Abajo derecha
        {-1, 1},  // Arriba izquierda
        {-1, -1}, // Abajo izquierda
        {-1, 0},  // Arriba
        {1, 0},   // Abajo
        {0, -1},  // Izquierda
        {0, 1}    // Derecha
    };

    for (int d = 0; d < 8; ++d) {
        int dx = direcciones[d][0];
        int dy = direcciones[d][1];
        int x = fila, y = col;

        while (true) {
            x += dx;
            y += dy;

            // Verificar si está fuera del tablero
            if (x < 0 || x >= 8 || y < 0 || y >= 8)
                break;

            // Simular el movimiento y verificar jaque
            if (tablero[x][y].pieza == CASILLA_VACIA) {
                if (!exponeReyAJaque(tablero, fila, col, x, y)) {
                    tablero[x][y].movimiento_valido = true;
                }
            } else {
                // Si es una pieza enemiga, verificar captura
                if ((tablero[fila][col].pieza > 0 && tablero[x][y].pieza < 0) ||
                    (tablero[fila][col].pieza < 0 && tablero[x][y].pieza > 0)) {
                    if (!exponeReyAJaque(tablero, fila, col, x, y)) {
                        tablero[x][y].movimiento_valido = true;
                    }
                }
                break; // Detener al encontrar cualquier pieza
            }
        }
    }
}

void calcularMovimientosRey(Casilla tablero[8][8], int fila, int col, bool reyNoSeHaMovido, bool torreCortaNoSeHaMovido, bool torreLargaNoSeHaMovido)
{
    // Limpiar movimientos válidos antes de calcular
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            tablero[i][j].movimiento_valido = false;
        }
    }

    // Direcciones posibles del rey (todas las adyacentes)
    const int direcciones[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    for (int d = 0; d < 8; ++d)
    {
        int nuevaFila = fila + direcciones[d][0];
        int nuevaColumna = col + direcciones[d][1];

        // Verificar límites del tablero
        if (nuevaFila < 0 || nuevaFila >= 8 || nuevaColumna < 0 || nuevaColumna >= 8)
            continue;

        // Si la casilla está vacía o contiene una pieza enemiga
        if (tablero[nuevaFila][nuevaColumna].pieza == CASILLA_VACIA ||
            (tablero[fila][col].pieza > 0 && tablero[nuevaFila][nuevaColumna].pieza < 0) ||
            (tablero[fila][col].pieza < 0 && tablero[nuevaFila][nuevaColumna].pieza > 0))
        {
            // Simular el movimiento y verificar si expone al rey al jaque
            if (!exponeReyAJaque(tablero, fila, col, nuevaFila, nuevaColumna))
            {
                tablero[nuevaFila][nuevaColumna].movimiento_valido = true;
            }
        }
    }

    // Lógica para el enroque
    if (!reyNoSeHaMovido)
    {
        // Enroque corto
        if (!torreCortaNoSeHaMovido &&
            tablero[fila][col + 1].pieza == CASILLA_VACIA &&
            tablero[fila][col + 2].pieza == CASILLA_VACIA &&
            !exponeReyAJaque(tablero, fila, col, fila, col + 1) &&
            !exponeReyAJaque(tablero, fila, col, fila, col + 2))
        {
            tablero[fila][col + 2].movimiento_valido = true; // Casilla de destino del rey
        }

        // Enroque largo
        if (!torreLargaNoSeHaMovido &&
            tablero[fila][col - 1].pieza == CASILLA_VACIA &&
            tablero[fila][col - 2].pieza == CASILLA_VACIA &&
            tablero[fila][col - 3].pieza == CASILLA_VACIA &&
            !exponeReyAJaque(tablero, fila, col, fila, col - 1) &&
            !exponeReyAJaque(tablero, fila, col, fila, col - 2))
        {
            tablero[fila][col - 2].movimiento_valido = true; // Casilla de destino del rey
        }
    }
}

void calcularMovimientosAlfil(Casilla tablero[8][8], int fila, int col) {
    // Limpiar movimientos válidos antes de calcular
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            tablero[i][j].movimiento_valido = false;
        }
    }

    // Direcciones de movimiento del alfil
    const int direcciones[4][2] = {
        {1, 1},   // Arriba derecha
        {1, -1},  // Abajo derecha
        {-1, 1},  // Arriba izquierda
        {-1, -1}  // Abajo izquierda
    };

    for (int d = 0; d < 4; ++d) {
        int dx = direcciones[d][0];
        int dy = direcciones[d][1];
        int x = fila;
        int y = col;

        while (true) {
            x += dx;
            y += dy;

            // Verificar si está fuera del tablero
            if (x < 0 || x >= 8 || y < 0 || y >= 8)
                break;

            // Simular el movimiento y verificar jaque
            if (tablero[x][y].pieza == CASILLA_VACIA) {
                if (!exponeReyAJaque(tablero, fila, col, x, y)) {
                    tablero[x][y].movimiento_valido = true;
                }
            } else {
                // Si es una pieza enemiga, verificar captura
                if ((tablero[fila][col].pieza > 0 && tablero[x][y].pieza < 0) ||
                    (tablero[fila][col].pieza < 0 && tablero[x][y].pieza > 0)) {
                    if (!exponeReyAJaque(tablero, fila, col, x, y)) {
                        tablero[x][y].movimiento_valido = true;
                    }
                }
                break; // Detener al encontrar cualquier pieza
            }
        }
    }
}

void calcularMovimientosTorre(Casilla tablero[8][8], int fila, int col)
{
    // Limpiar movimientos válidos antes de calcular
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            tablero[i][j].movimiento_valido = false;
        }
    }

    // Direcciones: arriba, abajo, izquierda, derecha
    const int direcciones[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (int d = 0; d < 4; ++d)
    {
        int dx = direcciones[d][0];
        int dy = direcciones[d][1];
        int x = fila, y = col;

        while (true)
        {
            x += dx;
            y += dy;

            // Verificar límites del tablero
            if (x < 0 || x >= 8 || y < 0 || y >= 8)
                break;

            // Si la casilla está vacía, es válida
            if (tablero[x][y].pieza == CASILLA_VACIA)
            {
                // Simulación: verificar si este movimiento expone al rey a un jaque
                if (!exponeReyAJaque(tablero, fila, col, x, y))
                {
                    tablero[x][y].movimiento_valido = true;
                }
            }
            // Si hay una pieza, verificar si es enemiga
            else
            {
                if ((tablero[fila][col].pieza > 0 && tablero[x][y].pieza < 0) ||
                    (tablero[fila][col].pieza < 0 && tablero[x][y].pieza > 0))
                {
                    // Simulación: verificar si capturar expone al rey
                    if (!exponeReyAJaque(tablero, fila, col, x, y))
                    {
                        tablero[x][y].movimiento_valido = true;
                    }
                }
                break; // Detenerse al encontrar cualquier pieza
            }
        }
    }
}

// Calcular movimientos válidos para un peón
void calcularMovimientosPeon(Casilla tablero[8][8], int fila, int col)
{
    // Limpiar movimientos válidos antes de calcular
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            tablero[i][j].movimiento_valido = false;
        }
    }

    // Determinar dirección del peón (1 para blanco, -1 para negro)
    int direccion = (tablero[fila][col].pieza > 0) ? -1 : 1;

    // Movimiento simple hacia adelante
    int nuevaFila = fila + direccion;
    if (nuevaFila >= 0 && nuevaFila < 8 && tablero[nuevaFila][col].pieza == CASILLA_VACIA)
    {
        if (!exponeReyAJaque(tablero, fila, col, nuevaFila, col))
        {
            tablero[nuevaFila][col].movimiento_valido = true;
        }
    }

    // Movimiento inicial de dos casillas
    if ((fila == 6 && direccion == -1) || (fila == 1 && direccion == 1))
    {
        int dobleFila = fila + 2 * direccion;
        if (tablero[nuevaFila][col].pieza == CASILLA_VACIA && tablero[dobleFila][col].pieza == CASILLA_VACIA)
        {
            if (!exponeReyAJaque(tablero, fila, col, dobleFila, col))
            {
                tablero[dobleFila][col].movimiento_valido = true;
            }
        }
    }

    // Captura diagonal
    for (int dx = -1; dx <= 1; dx += 2) // -1 para izquierda, 1 para derecha
    {
        int nuevaCol = col + dx;
        if (nuevaFila >= 0 && nuevaFila < 8 && nuevaCol >= 0 && nuevaCol < 8)
        {
            if ((tablero[fila][col].pieza > 0 && tablero[nuevaFila][nuevaCol].pieza < 0) ||
                (tablero[fila][col].pieza < 0 && tablero[nuevaFila][nuevaCol].pieza > 0))
            {
                if (!exponeReyAJaque(tablero, fila, col, nuevaFila, nuevaCol))
                {
                    tablero[nuevaFila][nuevaCol].movimiento_valido = true;
                }
            }
        }
    }
}




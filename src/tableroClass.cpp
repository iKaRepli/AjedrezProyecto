#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <vector>
#include "../include/tablero.h"
#include "../include/piezas.h"
#include <stdexcept>

class Tablero
{
private:
    Casilla tablero[8][8];
    bool holding = false;
    int fila_down = -1, col_down = -1;
    int piezaInicial = 0;

public:
    Tablero()
    {
        inicializarTablero(tablero);
        inicializarRecursosTablero();
    }

    ~Tablero()
    {
        liberarRecursosTablero();
    }

    void manejarEvento(ALLEGRO_EVENT &event)
    {
        switch (event.type)
        {
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            manejarMouseDown(event);
            printf("Peedamsd");
            break;

        case ALLEGRO_EVENT_MOUSE_AXES:
            manejarMouseMove(event);
            break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            manejarMouseUp(event);
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            throw std::runtime_error("Cierre de ventana solicitado.");
        default:
            break;
        }
    }

    void dibujar()
    {
        al_clear_to_color(al_map_rgb(255, 255, 255));
        dibujarTablero(tablero);
        ALLEGRO_MOUSE_STATE mouseState;
        al_get_mouse_state(&mouseState); // Capturamos el estado del mouse
        int mouse_x = mouseState.x;     // Obtenemos la posición X
        int mouse_y = mouseState.y;     // Obtenemos la posición Y
        dibujarPieza(piezaInicial, mouse_x, mouse_y);
        al_flip_display();
    }

private:
    void manejarMouseDown(ALLEGRO_EVENT &event)
{
    fila_down = event.mouse.y / TAMANO_CASILLA;
    col_down = event.mouse.x / TAMANO_CASILLA;

    // Verifica si se seleccionó una torre
    if (tablero[fila_down][col_down].pieza == TORRE_BLANCA || tablero[fila_down][col_down].pieza == TORRE_NEGRA)
    {
        calcularMovimientosTorre(tablero, fila_down, col_down);
        piezaInicial = tablero[fila_down][col_down].pieza;
        tablero[fila_down][col_down].pieza = 0;
        holding = true;
    }
    // Verifica si se seleccionó un rey
    else if (tablero[fila_down][col_down].pieza == REY_BLANCO || tablero[fila_down][col_down].pieza == REY_NEGRO)
    {
        calcularMovimientosRey(tablero, fila_down, col_down); 
        piezaInicial = tablero[fila_down][col_down].pieza;
        tablero[fila_down][col_down].pieza = 0;
        holding = true;
    }else if (tablero[fila_down][col_down].pieza == ALFIL_BLANCO || tablero[fila_down][col_down].pieza == ALFIL_NEGRO)
    {
        calcularMovimientosAlfil(tablero, fila_down, col_down); 
        piezaInicial = tablero[fila_down][col_down].pieza;
        tablero[fila_down][col_down].pieza = 0;
        holding = true;
    }
    else
    {
        limpiarMovimientosValidos();
    }
}

    void manejarMouseMove(ALLEGRO_EVENT &event)
    {
        if (holding)
        {
            dibujar();
        }
    }

    void manejarMouseUp(ALLEGRO_EVENT &event)
    {
        int fila_up = event.mouse.y / TAMANO_CASILLA;
        int col_up = event.mouse.x / TAMANO_CASILLA;

        if (tablero[fila_up][col_up].movimiento_valido)
        {
            tablero[fila_up][col_up].pieza = piezaInicial;
        }
        else
        {
            tablero[fila_down][col_down].pieza = piezaInicial;
        }

        limpiarMovimientosValidos();
        holding = false;
        piezaInicial = 0;
        dibujar();
    }

    void limpiarMovimientosValidos()
    {
        for (int fila = 0; fila < 8; ++fila)
        {
            for (int col = 0; col < 8; ++col)
            {
                tablero[fila][col].movimiento_valido = false;
            }
        }
    }
};
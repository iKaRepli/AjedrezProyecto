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
    int turno = BLANCAS;

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
        dibujarTablero(tablero,turno);
        ALLEGRO_MOUSE_STATE mouseState;
        al_get_mouse_state(&mouseState); // Capturamos el estado del mouse
        int mouse_x = mouseState.x;     // Obtenemos la posición X
        int mouse_y = mouseState.y;     // Obtenemos la posición Y
        printf("%d %d\n",turno,piezaInicial);
        if(turno * piezaInicial > 0){
        dibujarPieza(piezaInicial, mouse_x, mouse_y, turno);
    }
        al_flip_display();
    }

private:
    void manejarMouseDown(ALLEGRO_EVENT &event)
{
    fila_down = event.mouse.y / TAMANO_CASILLA;
    col_down = event.mouse.x / TAMANO_CASILLA;
    piezaInicial = tablero[fila_down][col_down].pieza;
    if ((turno == BLANCAS && piezaInicial > 0) ||
        (turno == NEGRAS && piezaInicial < 0)) {
        
        int tipoPieza = std::abs(piezaInicial); // Ignorar el color para identificar tipo

        switch (tipoPieza) {
            case 4:
                calcularMovimientosTorre(tablero, fila_down, col_down);
                break;

            case 6:
                calcularMovimientosRey(tablero, fila_down, col_down);
                break;

            case 2:
                calcularMovimientosAlfil(tablero, fila_down, col_down);
                break;

            case 1:
                calcularMovimientosPeon(tablero, fila_down, col_down);
                break;

            default:
                printf("Pieza no manejada: %d\n", tipoPieza);
                return; // Salir si la pieza no está implementada
        }

        // Configurar estado "holding" y limpiar la casilla
        piezaInicial = piezaInicial;
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
            if(piezaInicial * turno > 0){
                int asd = piezaInicial * turno;
                printf("%d",asd);
                
            }
        }
    }

    void manejarMouseUp(ALLEGRO_EVENT &event)
    {
        int fila_up = event.mouse.y / TAMANO_CASILLA;
        int col_up = event.mouse.x / TAMANO_CASILLA;

        if (tablero[fila_up][col_up].movimiento_valido)
        {
            tablero[fila_up][col_up].pieza = piezaInicial;
            turno = turno * -1;
        }
        else
        {
            tablero[fila_down][col_down].pieza = piezaInicial;
        }

        limpiarMovimientosValidos();
        holding = false;
        piezaInicial = 0;
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
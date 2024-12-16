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


    bool torreCortaBlancaMovida = false;
    bool torreLargaBlancaMovida = false;
    bool torreCortaNegraMovida = false;
    bool torreLargaNegraMovida = false;
    bool reyBlancoMovido = false;
    bool reyNegroMovido = false;


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

        if (tablero[fila_down][col_down].pieza == TORRE_BLANCA )
    {
        calcularMovimientosTorre(tablero, fila_down, col_down);
        piezaInicial = tablero[fila_down][col_down].pieza;
        tablero[fila_down][col_down].pieza = 0;
        holding = true;
    }else if(tablero[fila_down][col_down].pieza == TORRE_NEGRA){
        calcularMovimientosTorre(tablero, fila_down, col_down);
    }
    // Verifica si se seleccionó un rey
    else if (tablero[fila_down][col_down].pieza == REY_BLANCO )
    {
        calcularMovimientosRey(tablero, fila_down, col_down, reyBlancoMovido, torreCortaBlancaMovida, torreLargaBlancaMovida); 
        piezaInicial = tablero[fila_down][col_down].pieza;
        tablero[fila_down][col_down].pieza = 0;
        holding = true;
    }else if(tablero[fila_down][col_down].pieza == REY_NEGRO){
        calcularMovimientosRey(tablero, fila_down, col_down, reyNegroMovido, torreCortaNegraMovida, torreLargaNegraMovida); 
        piezaInicial = tablero[fila_down][col_down].pieza;
        tablero[fila_down][col_down].pieza = 0;
        holding = true;
        
    }else if (tablero[fila_down][col_down].pieza == ALFIL_BLANCO || tablero[fila_down][col_down].pieza == ALFIL_NEGRO)
    {
        calcularMovimientosAlfil(tablero, fila_down, col_down); 
        piezaInicial = tablero[fila_down][col_down].pieza;
        tablero[fila_down][col_down].pieza = 0;
        holding = true;
    }else if (tablero[fila_down][col_down].pieza == PEON_BLANCO || tablero[fila_down][col_down].pieza == PEON_NEGRO)
    {
        calcularMovimientosPeon(tablero, fila_down, col_down);
        piezaInicial = tablero[fila_down][col_down].pieza;
        tablero[fila_down][col_down].pieza = 0;
        holding = true;
    }else if (tablero[fila_down][col_down].pieza == REINA_BLANCA || tablero[fila_down][col_down].pieza == REINA_NEGRA)
    {
        calcularMovimientosReina(tablero, fila_down, col_down);
        piezaInicial = tablero[fila_down][col_down].pieza;
        tablero[fila_down][col_down].pieza = 0;
        holding = true;
    }
    else if (tablero[fila_down][col_down].pieza == CABALLO_BLANCO || tablero[fila_down][col_down].pieza == CABALLO_NEGRO)
    {
        calcularMovimientosCaballo(tablero, fila_down, col_down);
        piezaInicial = tablero[fila_down][col_down].pieza;
        tablero[fila_down][col_down].pieza = 0;
        holding = true;
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
   
                
            }
        }
    }

    void manejarMouseUp(ALLEGRO_EVENT &event)
{
    int fila_up = event.mouse.y / TAMANO_CASILLA;
    int col_up = event.mouse.x / TAMANO_CASILLA;

    if (tablero[fila_up][col_up].movimiento_valido)
    {
        printf("Moverás las piezas sí o sí de esta posición: %d %d\n", fila_down, col_down);
        printf("Estado de las torres blancas: corta=%d, larga=%d\n", torreCortaBlancaMovida, torreLargaBlancaMovida);
        printf("Estado del rey blanco movido: %d\n", reyBlancoMovido);

        bool esBlanca = piezaInicial > 0;

        // Enroque: Si movemos el rey y el destino está en una columna de enroque
        if (std::abs(piezaInicial) == 6 && (col_up == 2 || col_up == 6)) 
        {
            if (esBlanca && fila_down == 7) // Rey blanco
            {
                reyBlancoMovido = true;
                if (col_up == 2) // Enroque largo (hacia la izquierda)
                {
                    tablero[7][3].pieza = tablero[7][0].pieza; // Mover torre
                    tablero[7][0].pieza = 0;
                    torreLargaBlancaMovida = true;
                }
                else if (col_up == 6) // Enroque corto (hacia la derecha)
                {
                    tablero[7][5].pieza = tablero[7][7].pieza; // Mover torre
                    tablero[7][7].pieza = 0;
                    torreCortaBlancaMovida = true;
                }
            }
            else if (!esBlanca && fila_down == 0) // Rey negro
            {
                reyNegroMovido = true;
                if (col_up == 2) // Enroque largo (hacia la izquierda)
                {
                    tablero[0][3].pieza = tablero[0][0].pieza; // Mover torre
                    tablero[0][0].pieza = 0;
                    torreLargaNegraMovida = true;
                }
                else if (col_up == 6) // Enroque corto (hacia la derecha)
                {
                    tablero[0][5].pieza = tablero[0][7].pieza; // Mover torre
                    tablero[0][7].pieza = 0;
                    torreCortaNegraMovida = true;
                }
            }
        }
        else
        {
            // Determinar si se movió el rey o alguna torre
            if (esBlanca)
            {
                if (fila_down == 7 && col_down == 7) { // Torre corta blanca
                    torreCortaBlancaMovida = true;
                }
                else if (fila_down == 7 && col_down == 0) { // Torre larga blanca
                    torreLargaBlancaMovida = true;
                }
                else if (fila_down == 7 && col_down == 4) { // Rey blanco
                    reyBlancoMovido = true;
                }
            }
            else
            {
                if (fila_down == 0 && col_down == 7) { // Torre corta negra
                    torreCortaNegraMovida = true;
                }
                else if (fila_down == 0 && col_down == 0) { // Torre larga negra
                    torreLargaNegraMovida = true;
                }
                else if (fila_down == 0 && col_down == 4) { // Rey negro
                    reyNegroMovido = true;
                }
            }
        }

        // Mover la pieza seleccionada
        tablero[fila_up][col_up].pieza = piezaInicial;
        turno = turno * -1; // Cambiar de turno
    }
    else
    {
        // Si no es un movimiento válido, devolver la pieza a su posición inicial
        tablero[fila_down][col_down].pieza = piezaInicial;
    }

    // Limpiar movimientos válidos y resetear el estado
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
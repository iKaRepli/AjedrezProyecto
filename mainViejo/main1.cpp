#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "../include/tablero.h"
#include <allegro5/allegro_image.h>


void calcularMovimientosTorre(Casilla tablero[8][8], int fila, int col)
{
    // Limpiar el atributo "movimiento_valido" de todo el tablero
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
            if (tablero[x][y].pieza == 0)
            {
                tablero[x][y].movimiento_valido = true;
            }
            // Si hay una pieza, verificar si es enemiga
            else
            {
                if ((tablero[fila][col].pieza > 0 && tablero[x][y].pieza < 0) ||
                    (tablero[fila][col].pieza < 0 && tablero[x][y].pieza > 0))
                {
                    tablero[x][y].movimiento_valido = true;
                }
                break; // Detenerse al encontrar cualquier pieza
            }
        }
    }
}

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
    // Crear una consola para usar printf en Windows
    AllocConsole();
    FILE *fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
#endif
    
    printf("Consola inicializada\n");

    if (!al_init())
    {
        printf("No se pudo inicializar Allegro.\n");
        return -1;
    }

    // Inicializar los primitivos de Allegro
    if (!al_init_primitives_addon())
    {
        printf("No se pudieron inicializar los primitivos de Allegro.\n");
        return -1;
    }

    if (!al_init_image_addon()) {
        printf("No se pudo inicializar el módulo de imágenes.\n");
        return -1;
    }

    // Crear una ventana
    ALLEGRO_DISPLAY *display = al_create_display(8 * TAMANO_CASILLA, 8 * TAMANO_CASILLA);
    if (!display)
    {
        printf("No se pudo crear la ventana.\n");
        return -1;
    }

    // Inicializar el mouse y la cola de eventos
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_install_mouse();
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(display)); // Registrar eventos de ventana

    // Inicializar el tablero de ajedrez
    Casilla tablero[8][8];
    inicializarTablero(tablero);
    inicializarRecursosTablero();

    // Colocar algunas piezas
    tablero[0][3].pieza = 4;  // Torre blanca
    tablero[1][3].pieza = -4; // Pieza negra
    tablero[7][1].pieza = -4; // Pieza negra
    tablero[1][6].pieza = 2; // Pieza negra
    tablero[6][3].pieza = 4;  // Pieza blanca
    al_clear_to_color(al_map_rgb(255, 255, 255)); // Limpiar pantalla
    dibujarTablero(tablero);

    // Bucle principal

    int x = 0;
    bool running = true;
    int fila_down = -1, col_down = -1;   // Variables globales, fuera del switch
    int fila_while = -1, col_while = -1; // Variables globales, fuera del switch
    int fila_up = -1, col_up = -1;
    int xp = 0;
    int yp = 0;
    bool holding = false;
    int piezaInicial = 0;

    while (running)
    {
        ALLEGRO_EVENT event;
        // Dibujar el tablero y las piezas

        while (al_get_next_event(queue, &event))
        {

            switch (event.type)
            {
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                // Detectar clic

                fila_down = event.mouse.y / TAMANO_CASILLA;
                col_down = event.mouse.x / TAMANO_CASILLA;

                // Si clic en una torre
                if (tablero[fila_down][col_down].pieza == 4 || tablero[fila_down][col_down].pieza == -4)
                {
                    calcularMovimientosTorre(tablero, fila_down, col_down);
                    piezaInicial = tablero[fila_down][col_down].pieza;
                    tablero[fila_down][col_down].pieza = 0;
                    holding = true;
                }
                else
                {
                    // Limpiar movimientos válidos
                    for (int fila = 0; fila < 8; ++fila)
                    {
                        for (int col = 0; col < 8; ++col)
                        {
                            tablero[fila][col].movimiento_valido = false;
                        }
                    }
                }

                printf("Holding position: %d, %d\n", fila_down, col_down);
                al_clear_to_color(al_map_rgb(255, 255, 255)); // Limpiar pantalla
                dibujarTablero(tablero);
                if(piezaInicial != 0){

                    dibujarPieza(piezaInicial, event.mouse.x, event.mouse.y);
                }

                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                // Si el mouse está siendo "holdeado", verificar constantemente la posición
                if (holding)
                {
                    fila_while = event.mouse.y / TAMANO_CASILLA;
                    col_while = event.mouse.x / TAMANO_CASILLA;
                    al_clear_to_color(al_map_rgb(255, 255, 255)); // Limpiar pantalla
                    dibujarTablero(tablero);
                    dibujarPieza(piezaInicial, event.mouse.x, event.mouse.y);

                    printf("Holding position: %d, %d pieza en mano: %d\n", fila_while, col_while,piezaInicial);
                }
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                fila_up = event.mouse.y / TAMANO_CASILLA;
                col_up = event.mouse.x / TAMANO_CASILLA;

                if (tablero[fila_up][col_up].movimiento_valido)
                {
                    // Si la casilla de destino tiene una pieza enemiga, "comerla"
                    if (tablero[fila_up][col_up].pieza != 0)
                    {
                        printf("Comiendo pieza en %d, %d\n", fila_up, col_up);
                    }

                    // Mover la pieza seleccionada
                    tablero[fila_up][col_up].pieza = piezaInicial;
                }
                else
                {
                    // Si el movimiento no es válido, regresar la pieza a su posición inicial
                    tablero[fila_down][col_down].pieza = piezaInicial;
                }

                // Limpiar movimientos válidos y resetear estado
                for (int fila = 0; fila < 8; ++fila)
                {
                    for (int col = 0; col < 8; ++col)
                    {
                        tablero[fila][col].movimiento_valido = false;
                    }
                }
                holding = false;
                piezaInicial = 0;

                printf("Final decision: %d , %d\n", fila_up, col_up);
                al_clear_to_color(al_map_rgb(255, 255, 255)); // Limpiar pantalla
                dibujarTablero(tablero);
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;

            default:
                break;
            }
        }

        if (++x == 720)
        {
            printf("\n");
            for (int fila = 0; fila < 8; ++fila)
            {
                printf("\n");
                for (int col = 0; col < 8; ++col)
                {
                    printf("   |%d %d %d|", tablero[fila][col].pieza, tablero[fila][col].color, tablero[fila][col].movimiento_valido);
                }
            }
            x = 0;
        }
        al_rest(0.004); // 60 FPS
        al_flip_display();
    }

    // Limpiar y cerrar
    liberarRecursosTablero();
    al_destroy_display(display);
    al_destroy_event_queue(queue);

    return 0;
}

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "../include/tablero.h"
#include <stdio.h>

void dibujarMovimientoValido(bool movimiento, int pieza, int fila, int col){
int x = col * TAMANO_CASILLA;
int y = fila * TAMANO_CASILLA;
ALLEGRO_COLOR color = (fila + col) % 2 == 0 ? al_map_rgb(202, 203, 179) : al_map_rgb(99, 128, 70);
if (movimiento) {
                if(pieza != 0){
                    al_draw_circle(x + TAMANO_CASILLA / 2, y + TAMANO_CASILLA / 2, 46, color,8);
                } else{
                    al_draw_filled_circle(x + TAMANO_CASILLA / 2, y + TAMANO_CASILLA / 2, TAMANO_CASILLA / 5.7, color);
                }
            }
}
static ALLEGRO_BITMAP* torre_imagen = nullptr; 
static ALLEGRO_BITMAP* torre_negra_imagen = nullptr;
static ALLEGRO_BITMAP* rey_negro_imagen = nullptr;
static ALLEGRO_BITMAP* rey_blanco_imagen = nullptr;
static ALLEGRO_BITMAP* alfil_imagen = nullptr; 
static ALLEGRO_BITMAP* alfil_negro_imagen = nullptr; 
static ALLEGRO_BITMAP* peon_negro_imagen = nullptr; // Nuevo
static ALLEGRO_BITMAP* peon_blanco_imagen = nullptr; // Nuevo

void inicializarRecursosTablero() {
    torre_imagen = al_load_bitmap("assets/torre.png");
    if (!torre_imagen) {
        printf("Error: No se pudo cargar la imagen torre.png\n");
        exit(1); // Finaliza el programa si no se encuentra la imagen
    }

    torre_negra_imagen = al_load_bitmap("assets/torre_negra.png");
    if (!torre_negra_imagen) {
        printf("Error: No se pudo cargar la imagen torre_negra.png\n");
        exit(1); // Finaliza el programa si no se encuentra la imagen
    }

    rey_blanco_imagen = al_load_bitmap("assets/rey_blanco.png");
    if (!rey_blanco_imagen) {
        printf("Error: No se pudo cargar la imagen rey_blanco.png\n");
        exit(1); // Finaliza el programa si no se encuentra la imagen
    }

    rey_negro_imagen = al_load_bitmap("assets/rey_negro.png");
    if (!rey_negro_imagen) {
        printf("Error: No se pudo cargar la imagen rey_negro.png\n");
        exit(1); // Finaliza el programa si no se encuentra la imagen
    }

    alfil_imagen = al_load_bitmap("assets/alfil_blanco.png"); // Nuevo
    if (!alfil_imagen) {
        printf("Error: No se pudo cargar la imagen alfil_blanco.png\n");
        exit(1); // Finaliza el programa si no se encuentra la imagen
    }

    alfil_negro_imagen = al_load_bitmap("assets/alfil_negro.png"); // Nuevo
    if (!alfil_negro_imagen) {
        printf("Error: No se pudo cargar la imagen alfil_negro.png\n");
        exit(1); // Finaliza el programa si no se encuentra la imagen
    }

    peon_blanco_imagen = al_load_bitmap("assets/peon_blanco.png"); // Nuevo
    if (!peon_blanco_imagen) {
        printf("Error: No se pudo cargar la imagen peon_blanco.png\n");
        exit(1); // Finaliza el programa si no se encuentra la imagen
    }

    peon_negro_imagen = al_load_bitmap("assets/peon_negro.png"); // Nuevo
    if (!peon_negro_imagen) {
        printf("Error: No se pudo cargar la imagen peon_negro.png\n");
        exit(1); // Finaliza el programa si no se encuentra la imagen
    }
}

void liberarRecursosTablero() {
    if (torre_imagen) {
        al_destroy_bitmap(torre_imagen);
        torre_imagen = nullptr;
    }
    if (torre_negra_imagen) {
        al_destroy_bitmap(torre_negra_imagen);
        torre_negra_imagen = nullptr;
    }
    if (rey_blanco_imagen) {
        al_destroy_bitmap(rey_blanco_imagen);
        rey_blanco_imagen = nullptr;
    }
    if (rey_negro_imagen) {
        al_destroy_bitmap(rey_negro_imagen);
        rey_negro_imagen = nullptr;
    }
    if (alfil_imagen) { // Nuevo
        al_destroy_bitmap(alfil_imagen);
        alfil_imagen = nullptr;
    }
    if (alfil_negro_imagen) { // Nuevo
        al_destroy_bitmap(alfil_negro_imagen);
        alfil_negro_imagen = nullptr;
    }
    if (peon_blanco_imagen) { // Nuevo
        al_destroy_bitmap(peon_blanco_imagen);
        peon_blanco_imagen = nullptr;
    }
    if (peon_negro_imagen) { // Nuevo
        al_destroy_bitmap(peon_negro_imagen);
        peon_negro_imagen = nullptr;
    }
}

void dibujarPieza(int pieza, int x, int y,int turno) {
    
    if (pieza == 0) {
        // Casilla vacía, no se dibuja nada.
        return;
    }

    ALLEGRO_BITMAP* bitmap = nullptr;

    switch (pieza) {
    case 4: 
        bitmap = torre_imagen;
        break;
    case -4: 
        bitmap = torre_negra_imagen;
        break;
    case 6: 
        bitmap = rey_blanco_imagen;
        break;
    case -6: 
        bitmap = rey_negro_imagen;
        break;
    case 2: // Nuevo
        bitmap = alfil_imagen;
        break;
    case -2: // Nuevo
        bitmap = alfil_negro_imagen;
        break;
    case 1: // Nuevo
        bitmap = peon_blanco_imagen;
        break;
    case -1: // Nuevo
        bitmap = peon_negro_imagen;
        break;
    default:
        printf("Advertencia: pieza no reconocida (%d).\n", pieza);
        return;
    }

    if (bitmap) {
        al_draw_scaled_bitmap(
            bitmap,
            0, 0,                            
            al_get_bitmap_width(bitmap),
            al_get_bitmap_height(bitmap),
            x - TAMANO_CASILLA / 2,
            y - TAMANO_CASILLA / 2,
            TAMANO_CASILLA,
            TAMANO_CASILLA,
            0
        );
    }
}


void inicializarTablero(Casilla tablero[8][8]) {
    for (int fila = 0; fila < 8; ++fila) {
        for (int col = 0; col < 8; ++col) {
            tablero[fila][col].pieza = 0;
            tablero[fila][col].color = (fila + col) % 2 == 0 ? 0 : 1;
            tablero[fila][col].movimiento_valido = false;
        }
    }
    
    // Peones blancos
// Peones blancos
tablero[6][0].pieza = PEON_BLANCO;
tablero[6][1].pieza = PEON_BLANCO;
tablero[6][2].pieza = PEON_BLANCO;
tablero[6][3].pieza = PEON_BLANCO;
tablero[6][4].pieza = PEON_BLANCO;
tablero[6][5].pieza = PEON_BLANCO;
tablero[6][6].pieza = PEON_BLANCO;
tablero[6][7].pieza = PEON_BLANCO;

// Peones negros
tablero[1][0].pieza = PEON_NEGRO;
tablero[1][1].pieza = PEON_NEGRO;
tablero[1][2].pieza = PEON_NEGRO;
tablero[1][3].pieza = PEON_NEGRO;
tablero[1][4].pieza = PEON_NEGRO;
tablero[1][5].pieza = PEON_NEGRO;
tablero[1][6].pieza = PEON_NEGRO;
tablero[1][7].pieza = PEON_NEGRO;

// Piezas blancas
tablero[7][0].pieza = TORRE_BLANCA;
tablero[7][7].pieza = TORRE_BLANCA;
tablero[7][2].pieza = ALFIL_BLANCO;
tablero[7][5].pieza = ALFIL_BLANCO;
tablero[7][4].pieza = REY_BLANCO;

// Piezas negras
tablero[0][0].pieza = TORRE_NEGRA;
tablero[0][7].pieza = TORRE_NEGRA;
tablero[0][2].pieza = ALFIL_NEGRO;
tablero[0][5].pieza = ALFIL_NEGRO;
tablero[0][4].pieza = REY_NEGRO;


    
}

void dibujarTablero(Casilla tablero[8][8],int turno) {
    for (int fila = 0; fila < 8; ++fila) {
        for (int col = 0; col < 8; ++col) {
            int x = col * TAMANO_CASILLA;
            int y = fila * TAMANO_CASILLA;

            
            ALLEGRO_COLOR colorCasilla;


            switch (tablero[fila][col].color)
            {

            case COLOR_BLANCO:
                colorCasilla = al_map_rgb(235, 236, 208);
                break;
            
            case COLOR_NEGRO:
                colorCasilla = al_map_rgb(115, 149, 82);
                break;
            
            }
            al_draw_filled_rectangle(x, y, x + TAMANO_CASILLA, y + TAMANO_CASILLA, colorCasilla);

           
            dibujarMovimientoValido(tablero[fila][col].movimiento_valido, tablero[fila][col].pieza, fila, col);

            // Dibujar pieza
            if (tablero[fila][col].pieza != 0) {
                dibujarPieza(tablero[fila][col].pieza, x + TAMANO_CASILLA / 2, y + TAMANO_CASILLA / 2, turno);
            }
        }
    }
}

//estoy escribiendo muy mal porque no me acostumbro al teclado cada vez que escrib en este teclado me pasa lo mismo xdxdjjajaj pero todo bien porque poco a poco estoy aprendiendo es esta computdora de mi compañero isaaac tetris y le va bien rico que esa bien chila porque tiene lucesitas led, le costo machin oero la nt si vale la pena aqui juega fortnite 
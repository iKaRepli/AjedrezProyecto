#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "tableroClass.cpp"
#include "../include/tablero.h"
#include "../include/piezas.h"
#include <stdexcept>


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
    if (!al_init() || !al_init_primitives_addon() || !al_init_image_addon())
    {
        printf("Error al inicializar Allegro.\n");
        return -1;
    }

    ALLEGRO_DISPLAY *display = al_create_display(8 * TAMANO_CASILLA, 8 * TAMANO_CASILLA);
    if (!display)
    {
        printf("No se pudo crear la ventana.\n");
        return -1;
    }


    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_install_mouse();
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    try
    {
        Tablero tablero;

        bool running = true;

        while (running)
        {
            ALLEGRO_EVENT event;
            
            while (al_get_next_event(queue, &event))
            {
                try
                {
                    tablero.manejarEvento(event);
                }
                catch (const std::runtime_error &e)
                {
                    running = false;
                }
            }
            tablero.dibujar();
            al_rest(0.004); // 60 FPS
        }
    }
    catch (const std::exception &e)
    {
        printf("Excepción: %s\n", e.what());
    }

    al_destroy_display(display);
    al_destroy_event_queue(queue);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include "wiiuse.h"
#include "arduino-serial-lib.h"

#include "robot-controller.h"
#include "wiimote-handler.h"

#ifndef WIIUSE_WIN32
#include <unistd.h>
#endif // WIIUSE_WIN32

typedef enum {
    MAX_WIIMOTES = 4,
    SEARCH_TIMEOUT_SECONDS = 5
} WiiuseConstants;

/**
 * Conecta com wiimote e arduino e escuta os eventos do wiimote
 */
int main(int argc, char** argv)
{
    wiimote** wiimotes;
    int found, connected, fd;

    /* Inicializar array de wiimotes */
    wiimotes = wiiuse_init(MAX_WIIMOTES);
    found = wiiuse_find(wiimotes, MAX_WIIMOTES, SEARCH_TIMEOUT_SECONDS);

    if (!found) {
        printf("Nenhum Wiimote encontrado!\n");
        return 0;
    }

    /* Conectar wiimotes */
    connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);

    if (connected) {
        printf("Conectado a %d Wiimotes de um total de %d encontrados.\n",
               connected, found);

        setup_wiimote(wiimotes);
    } else {
        printf("Conexão com Wiimotes falhou!\n");
        return 0;
    }

    /* iniciar conexão com o arduino */
    fd = connect_to_robot();
    if (fd == -1) {
        printf("--A conexão com o arduino falhou!--\n");
        return 0;
    } else {
        printf("--Conectado ao arduino--\n");
    }

    /* enquanto existirem wiimotes conectados escutar eventos */
    while (has_wiimote_connections(wiimotes, MAX_WIIMOTES)) {
        if (wiiuse_poll(wiimotes, MAX_WIIMOTES)) {
            int i;

            /* Analisar cada um dos quatro slots */
            for (i = 0; i < MAX_WIIMOTES; i++) {
                switch (wiimotes[i]->event) {

                /* eventos - botões, sensores de movimento ou IR*/
                case WIIUSE_EVENT:
                    handle_event(wiimotes[i]);
                    break;

                /* status - informações do dispositivo */
                case WIIUSE_STATUS:
                    handle_status(wiimotes[i]);
                    break;

                /* desconexão de dispositivo */
                case WIIUSE_DISCONNECT:
                    handle_disconnection(wiimotes[i]);

                default:
                    break;
                }
            }
        }
    }

    /* fechar conexão bluetooth com o arduino */
    disconnect();
    return 0;
}

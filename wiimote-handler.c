/**
 * Lida com os eventos do wiimote.
 */

#include "wiimote-handler.h"

/** \brief  Imprime alerta sobre a desconexão de um dispositivo.
 *
 * \param   wm  --  Ponteiro para uma estrutura wiimote_t.
 */
void handle_disconnection(wiimote* wm)
{
    printf("Wiimote desconectado - ID: %d\n", wm->unid);
}

/** \brief  Monitora os botões, acelerômetro e sensor IR do dispositivo.
 *
 * \param   wm  --  Ponteiro para uma estrura wiimote_t.
 */
void handle_event(wiimote* wm)
{
    /**
     * Aumenta a sensibilidade do sensor IR.
     */
    if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_PLUS)) {
        int level;
        WIIUSE_GET_IR_SENSITIVITY(wm, &level);
        wiiuse_set_ir_sensitivity(wm, level + 1);
        printf("Sensibilidade++\n");
    }

    /**
     * Diminui a sensibilidade do sensor IR.
     */
    if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_MINUS)) {
        int level;
        WIIUSE_GET_IR_SENSITIVITY(wm, &level);
        wiiuse_set_ir_sensitivity(wm, level - 1);
        printf("Sensibilidade--\n");
    }

    /**
     * Marca o wiimote com a flag 'status'.
     */
    if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_A)) {
        wiiuse_status(wm);
    }

    /**
     * Caso o sensor IR esteja habilitado, interpreta os sinais do sensor e
     * envia requisições para a movimentação do carro.
     */
    if (WIIUSE_USING_IR(wm)) {
        int i, visible_dot_count;

        /**
         * Margem de erro para alinhamento do carro, 20% para mais ou para menos
         * do centro da tela, o valor '1023' é baseado na resolução do sensor IR
         * do wiimote que é de 1024 por 768.
         */
        float center_tolerance = 0.2;
        float left_limit = 1023 * (0.5 - center_tolerance);
        float right_limit = 1023 * (0.5 + center_tolerance);

        /**
         * Como a distâancia no eixo z (entre o wiimote e o ponto de referencia
         * entre os leds) varia de acordo com a distância entre os leds, os
         * valores 700 e 800 foram escolhidos arbitrariamente para que o 'zero'
         * do carro se mantivesse a uma distância específica dos leds.
         */
        int lower_distance_limit = 700;
        int upper_distance_limit = 800;

        /**
         * Visita cada um dos 4 possíveis leds e imprime suas coordenadas x e y
         * caso estejam visiveis.
         */
        for (i = 0; i < 4; i++) {
            if (wm->ir.dot[i].visible) {
                printf("\nIR SOURCE %d:  (rx: %d, ry: %d)\n",
                       i, wm->ir.dot[i].rx, wm->ir.dot[i].ry);
            }
        }

        /**
         * Executa correção de posicionamento apenas se existirem um ou mais
         * leds visíveis.
         */
        if (wm->ir.num_dots > 0) {
            printf("IR cursor(a): (ax: %d, ay: %d)\n", wm->ir.ax, wm->ir.ay);
            printf("IR cursor(i): ( x: %d,  y: %d)\n", wm->ir.x, wm->ir.y);
            printf("IR z-distance: %f\n", wm->ir.z);

            /**
             * Correção do alinhamento (eixo x).
             */
            if (wm->ir.ax > right_limit) {
                signal_right();

            } else if (wm->ir.ax < left_limit) {
                signal_left();

            } else {
                printf("MEIO\n");

                /**
                 * Correção da distância (eixo z), necessita de pelo menos dois
                 * pontos de IR visíveis para o cálculo da distância.
                 */
                if (wm->ir.num_dots > 1) {
                    if (wm->ir.z < 700) {
                        signal_back();
                    } else if (wm->ir.z > 800) {
                        signal_forward();
                    }
                }

            }
        } //Captura de um ou mais pontos
    } //Interpretação do sinal IR
}

/** \brief  Imprime na tela informações sobre o dispositivo que levantar um
 *          evento de estatus.
 *
 * \param   wm  --  Ponteiro para estrutura wiimote_t.
 */
void handle_status(struct wiimote_t* wm) {
    int sensitivity;
    WIIUSE_GET_IR_SENSITIVITY(wm, &sensitivity);

	printf("\nSTATUS -- Wiimote ID %i\n", wm->unid);
	printf("acelerometro:    %i\n", WIIUSE_USING_ACC(wm));
	printf("infra-vermelho:  %i\n", WIIUSE_USING_IR(wm));
    printf("sensibilidade:   %i\n", sensitivity);
    printf("bateria:         %f %%\n", (wm->battery_level) * 100);

	printf("leds:            %i %i %i %i\n", WIIUSE_IS_LED_SET(wm, 1),
                                             WIIUSE_IS_LED_SET(wm, 2),
                                             WIIUSE_IS_LED_SET(wm, 3),
                                             WIIUSE_IS_LED_SET(wm, 4));
}

/** \brief  Configura leds, sensor IR e acelerômetro dos wiimotes.
 *
 * \param   wm  --  Array de estruturas wiimote_t.
 */
void setup_wiimote(wiimote** wm)
{
    wiiuse_set_leds(wm[0], WIIMOTE_LED_1);
    wiiuse_set_leds(wm[1], WIIMOTE_LED_2);
    wiiuse_set_leds(wm[2], WIIMOTE_LED_3);
    wiiuse_set_leds(wm[3], WIIMOTE_LED_4);

    /* habilita sensor IR */
    wiiuse_set_ir(wm[0], 1);

    /* habilita sensor de movimento */
    wiiuse_motion_sensing(wm[0], 1);
    wiiuse_set_aspect_ratio(wm[0], WIIUSE_ASPECT_16_9);
}

/** \brief  Verifica se existem conexões com wiimotes abertas.
 *
 * \param   wiimotes Array de estruturas wiimote_t.
 * \param   wiimotes_number Número máximo de wiimotes.
 * \return  1 se existirem conexões, 0 caso contrário.
 */
short has_wiimote_connections(wiimote** wiimotes, int wiimotes_number)
{
    int i;

    if (!wiimotes)
        return 0;

    for (i = 0; i < wiimotes_number; i++) {
        if (wiimotes[i] && WIIMOTE_IS_CONNECTED(wiimotes[i])) {
			return 1;
		}
    }

    return 0;
}

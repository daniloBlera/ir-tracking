/**
 * Lida com a conexão, envio de comandos e desconexão com o bluetooth do
 * arduino.
 */

#include <time.h>

#include "arduino-serial-lib.h"
#include "robot-controller.h"

/* código de erro para tentativa de conexão com o bluetooth */
const int ERROR_CODE = -1;

/**
 * Limite de tempo para evitar que o programa envie mais requisições do que o
 * arduino possa executar, o valor foi escolhido através de tentativa-e-erro, um
 * valor muito alto fará a movimentação ser executada com travamentos enquanto
 * um valor muito baixo irá sobrecarregar o buffer de leitura do arduino ou
 * seja, o carro continuará executando algumas instruções mesmo após o envio ter
 * encerrado no computador.
 *
 * Para mais detalhes ver 'has_enough_time()' e as funções para o envio de
 * requisições para movimentação.
 */
double limit = 3000;

/* tempo quando a última requisição foi enviada */
static clock_t last_sent;
int fd;

/**
 * \brief  Envia comando para mover para frente.
 *
 * Para fim de testes, caso não seja possível se utilizar a conexão com o
 * bluetooth é possível imprimir no console a mensagem '<direcao>-E'
 */
void signal_forward()
{
    if (fd == ERROR_CODE)
        printf("FRENTE-E\n");
    else {
        if (has_enough_time()) {
            serialport_write(fd, "w");
            printf("FRENTE\n");
        }
    }
}

/**
 * \brief  Envia comando para mover para trás.
 *
 * Para fim de testes, caso não seja possível se utilizar a conexão com o
 * bluetooth é possível imprimir no console a mensagem '<direcao>-E'
 */
void signal_back()
{
    if (fd == ERROR_CODE) {
        printf("TRAS-E\n");
    } else {
        if (has_enough_time()) {
            serialport_write(fd, "s");
            printf("TRAS\n");
        }
    }
}

/**
 * \brief  Envia comando para girar para a esquerda.
 *
 * Para fim de testes, caso não seja possível se utilizar a conexão com o
 * bluetooth é possível imprimir no console a mensagem '<direcao>-E'
 */
void signal_left()
{
    if (fd == ERROR_CODE) {
        printf("ESQUERDA-E\n");
    } else {
        if (has_enough_time()) {
            serialport_write(fd, "a");
            printf("ESQUERDA\n");
        }
    }
}

/**
 * \brief  Envia comando para girar para direita.
 *
 * Para fim de testes, caso não seja possível se utilizar a conexão com o
 * bluetooth é possível imprimir no console a mensagem '<direcao>-E'
 */
void signal_right()
{
    if (fd == ERROR_CODE) {
        printf("DIREITA-E\n");
    } else {
        if (has_enough_time()) {
            serialport_write(fd, "d");
            printf("DIREITA\n");
        }
    }
}

/**
 * Verifica se há tempo suficiente desde o envio da última requisição para que
 * uma próxima possa ser enviada.
 *
 * Existe um limite de quantidade de requisições o arduino poderá executar e
 * como a requisição para movimentação será chamada tão rápido quanto o código
 * puder ser executado no computador, é necessária uma maneira de evitar que o
 * buffer de leitura no arduino seja sobrecarregado com instruções.
 */
int has_enough_time(void)
{
    clock_t now;
    now = clock();

    if ( ((double)(now - last_sent)) > limit ) {
        last_sent = now;
        return 1;
    }

    return 0;
}

/**
 * \brief  Inicia uma conexão com a porta serial.
 * \returns  fd válido ou -1 caso contrário.
 */
int connect_to_robot(void)
{
    return fd = serialport_init("/dev/rfcomm0", 9600);
    last_sent = clock();
}

/**
 * \brief  Encerra, se houver, a conexão com a porta serial.
 */
void disconnect(void)
{
    if (fd != -1) {
        serialport_close(fd);
        printf("CONEXÃO BLUETOOTH ENCERRADA\n");
    }
}

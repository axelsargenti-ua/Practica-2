/*
 * PRÁCTICA 2 - COMUNICACIONES EN RED - CLIENTE
 *
 * DOCUMENTACIÓN (cómo lo voy a hacer):
 * Sigo la estructura de toda aplicación cliente-servidor del enunciado:
 *   1. socket()   -> creo el canal de comunicación
 *   2. connect()  -> pido la conexión al servidor (su IP la recibo como
 *                    argumento y la convierto con inet_addr(); puerto 9999)
 *   3. read()     -> leo lo que envía el servidor, hasta que cierra la
 *                    conexión, y lo muestro por pantalla con write()
 *   4. close()    -> cierro el canal
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PUERTO 9999

int main(int argc, char *argv[])
{
    int conector, n;
    struct sockaddr_in dir_servidor;
    char buffer[1024];

    if (argc != 2) {
        printf("Uso: Cliente IP_Servidor\n");
        exit(1);
    }

    /* 1. socket */
    conector = socket(AF_INET, SOCK_STREAM, 0);
    if (conector < 0) {
        perror("Error en socket");
        exit(1);
    }

    /* Dirección del servidor: IP recibida como argumento y puerto 9999 */
    memset(&dir_servidor, 0, sizeof(dir_servidor));
    dir_servidor.sin_family = AF_INET;
    dir_servidor.sin_port = htons(PUERTO);
    dir_servidor.sin_addr.s_addr = inet_addr(argv[1]);

    /* 2. connect */
    if (connect(conector, (struct sockaddr *)&dir_servidor, sizeof(dir_servidor)) < 0) {
        perror("Error en connect");
        exit(1);
    }

    /* 3. read del socket y write por pantalla (descriptor 1) */
    while ((n = read(conector, buffer, sizeof(buffer))) > 0)
        write(1, buffer, n);

    /* 4. close */
    close(conector);
    return 0;
}
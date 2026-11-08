/*
 * PRÁCTICA 2 - COMUNICACIONES EN RED - SERVIDOR
 *
 * DOCUMENTACIÓN (cómo lo voy a hacer):
 * Sigo exactamente la estructura de toda aplicación cliente-servidor del
 * enunciado, en el orden de las llamadas al sistema:
 *   1. socket()  -> creo el canal de comunicación
 *   2. bind()    -> comunico a la red la dirección (puerto 9999)
 *   3. listen()  -> me pongo disponible para recibir peticiones
 *   4. accept()  -> me bloqueo esperando a que llegue un cliente
 * Cuando llega un cliente creo un hijo con fork(). El hijo se encarga de
 * leer el archivo "Google.html" y enviarlo con write() por la conexión.
 * El padre cierra esa conexión y vuelve a accept(), así el servidor está
 * siempre escuchando por el puerto 9999.
 */


 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PUERTO 9999

int main()
{
    int conector, conexion, fichero, n;
    struct sockaddr_in dir_servidor;
    char buffer[1024];
    pid_t pid;

    /* 1. socket: familia AF_INET, tipo SOCK_STREAM (TCP) */
    conector = socket(AF_INET, SOCK_STREAM, 0);
    if (conector < 0) {
        perror("Error en socket");
        exit(1);
    }

    /* Dirección del servidor: cualquier IP de la máquina, puerto 9999
       (htons/htonl pasan los números al formato de la red) */
    memset(&dir_servidor, 0, sizeof(dir_servidor));
    dir_servidor.sin_family = AF_INET;
    dir_servidor.sin_port = htons(PUERTO);
    dir_servidor.sin_addr.s_addr = htonl(INADDR_ANY);

    /* 2. bind */
    if (bind(conector, (struct sockaddr *)&dir_servidor, sizeof(dir_servidor)) < 0) {
        perror("Error en bind");
        exit(1);
    }

    /* 3. listen: cola de 5 peticiones pendientes */
    if (listen(conector, 5) < 0) {
        perror("Error en listen");
        exit(1);
    }
    printf("Servidor escuchando en el puerto %d\n", PUERTO);

    while (1) {
        /* 4. accept: espera a que llegue un cliente */
        conexion = accept(conector, NULL, NULL);
        if (conexion < 0) {
            perror("Error en accept");
            continue;
        }

        pid = fork();
        if (pid == 0) {
            /* HIJO: transfiere el archivo al cliente */
            close(conector);
            fichero = open("Google.html", O_RDONLY);
            if (fichero < 0) {
                perror("Error al abrir Google.html");
                exit(1);
            }
            while ((n = read(fichero, buffer, sizeof(buffer))) > 0)
                write(conexion, buffer, n);
            close(fichero);
            close(conexion);
            exit(0);
        }
        /* PADRE: no usa esta conexión, vuelve a esperar clientes */
        close(conexion);
    }
}
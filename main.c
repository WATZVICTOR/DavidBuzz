// Servidor TCP
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "servidor.h"

#define MAX_CLIENTES 5

int id_cliente = 0;
pthread_mutex_t lock;

// Vector en el que almacenamos todos los clientes conectados.
struct Cliente* VectorClientes[MAX_CLIENTES];

void Arreglos() {
    VectorClientesInicializa(VectorClientes, MAX_CLIENTES);
}

// Funcion que atiende a los clientes.
void *AtiendeCliente(void* ptr_cliente) {

    // Creamos una copia en el ambito del hilo para que la maneje.
    struct Cliente cliente = *((struct Cliente*) ptr_cliente);

    // Creamos una estrcutura auxiliar.
    struct Auxiliar aux;
    aux.bytes_entrada = 0;
    aux.bytes_salida = 0;
    memset(&(aux.buffer_entrada),0,sizeof(aux.buffer_entrada));
    memset(&(aux.buffer_salida),0,sizeof(aux.buffer_salida));

    //
    // PASO 1 - Registramos un puntero de este cliente al Vector de Clientes.
    //

    VectorClientesAddCliente(&cliente, VectorClientes, MAX_CLIENTES);



    //
    // PASO 2 - El cliente nos envia su nombre de usuario.
    //

    // Esperamos a que hable el cliente. (Nos da Tipo de usuario y Nombre;)
    aux.bytes_entrada = recv(cliente.Descriptor, aux.buffer_entrada, sizeof(aux.buffer_entrada),0);

    // Chequeamos los bytes de entrada.
    if (aux.bytes_entrada == -1) {
        ImprimeErrorClienteAnonimo(&cliente);
        perror("Error en la funcion AtiendeCliente");
        close(cliente.Descriptor);
        return NULL;
    }
    if (aux.bytes_entrada == 0) {
        ImprimeInfoDesconexionAnonima(&cliente);
        return NULL;
    }
    if (aux.bytes_entrada > 0 && aux.bytes_entrada <= 32) {
        strcpy(cliente.Nombre, aux.buffer_entrada);
        ImprimeInfoRegistroCliente(&cliente);
    } else {
        strcpy(aux.buffer_salida, "El nombre no puede ser mayor 32 Caracteres. Cerrando conexión.");
        aux.bytes_salida = send(cliente.Descriptor, aux.buffer_salida, sizeof(aux.buffer_salida), 0);
        ImprimeErrorClienteNombre32(&cliente);
        close(cliente.Descriptor);
        return NULL;
    }

    //
    // PASO 3 - El cliente nos envia que tipo de usuario es A - Mentor | B - Mentorizado | C - Sin Asignar
    //







}

// Este servidor ha sido construido usando la web..
// https://www.gta.ufrj.br/ensino/eel878/sockets/intro.html#platform
int main(int argc, char** argv) {

    Arreglos();
    // AREA DE DATOS
    int error;
    char* IP;
    short int PORT;
    int server_descriptor, client_descriptor, client_size;
    struct sockaddr_in server;
    struct sockaddr_in client;
    // Control de errores.
    if (argc!=2) {
        printf("Use: %s <PORT>\n", argv[0]);
        return 1;
    }
    // Parseamos los arguemtnos que nos pasan
    PORT = atoi(argv[1]);
    // Creamos la informacion con la que el servidor va a servir peticiones.
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    // Inicializamos un mutex para poder crear secciones criticas, hacinedo que un solo hilo ejecute esa parte del codigo.
    error = pthread_mutex_init(&lock, NULL);
    if (error ==-1) {
        perror("Error en la funcion MUTEX: \n");
        return 1;
    }

    // Ahora ya podemos pasar a construir el servidor.

    // 1º Socket
    server_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (server_descriptor == -1) {
        perror("Error en la funcion SOCKET: \n");
        return 1;
    }

    // 2º Bind
    error = bind(server_descriptor, (struct sockaddr *)&server, sizeof(server));
    if (error == -1) {
        perror("Error en la funcion BIND: \n");
        return 1;
    }

    // 3º Listen
    error = listen(server_descriptor, 500);
    if (error == -1) {
        perror("Error en la funcion LISTEN: \n");
        return 1;
    }
    printf("Esperando conexiones entrantes...\n");

    // 4º Accept
    // A partir de aqui hacemos un bucle eterno que se dedique a atender peticiones.
    for(;;) {
        // Esperamos la conexion de un cliente.
        client_descriptor = accept(server_descriptor, (struct sockaddr *)&client, &client_size);
        if (client_descriptor == -1) {
            perror("Error en la funcion ACCEPT: \n");
            return 1;
        }
        // Creamos una estrcutura de tipo clinete para pasarle toda la informacion al hilo.
        struct Cliente cliente;
        cliente.Conexion = client;
        cliente.ID_Hilo;
        cliente.ID_Cliente = id_cliente++;
        cliente.Descriptor = client_descriptor;
        strcpy(cliente.Nombre, "Anonymous");
        //cliente.Nombre = "Anonymous";
        cliente.Tipo = 'C';

        ImprimeInfoNuevaConexion(&(cliente.Conexion));
        // Cuando tengamos una conexion se la pasamos a un hilo y que la gestione el.
        error = pthread_create(&(cliente.ID_Hilo), NULL, AtiendeCliente, (void *)&cliente);
        if (error == -1) {
        perror("Error en la funcion THREAD_CREATE: \n");
        return 1;
        }
    }
}

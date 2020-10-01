// Servidor TCP
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "servidor.h"


int david_conectado = 0;
struct Cliente ptr_david;
int id_cliente = 0;
pthread_mutex_t lock;



// Funciones a usar.
void RegistrarCliente(struct Cliente* cliente);
void *AtiendeCliente(void* ptr_cliente);
void AtenderDavid();
void AtenderGuia(struct Cliente* cliente);
void CerrarConexion(struct Cliente* cliente);

// Este servidor ha sido construido usando mucho amor.
int main(int argc, char** argv) {

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
	printf("DEBUG\n");
        // Creamos una estrcutura de tipo cliente para pasarle toda la informacion al hilo.
        struct Cliente cliente;
        cliente.Conexion = client;
        cliente.ID_Hilo;
        cliente.ID_Cliente = id_cliente++;
        cliente.Descriptor = client_descriptor;
        cliente.Tipo = '2';

        // Cuando tengamos una conexion se la pasamos a un hilo y que la gestione el.
        error = pthread_create(&(cliente.ID_Hilo), NULL, AtiendeCliente, (void *)&cliente);
        if (error == -1) {
        perror("Error en la funcion THREAD_CREATE: \n");
        return 1;
        }
    }
}

void *AtiendeCliente(void* ptr_cliente) {

    // Creamos una copia en el ambito del hilo para que la maneje.
    struct Cliente cliente = *((struct Cliente*) ptr_cliente);

    RegistrarCliente(&cliente);

    if (cliente.Tipo == '1') AtenderDavid();
    if (cliente.Tipo == '0') AtenderGuia(&cliente);

}

// Registra el cliente
// Esta funcion hace...
// (1) - Espera por un byte que ha de enviar el cleinte
// (2) - Checeka si el byte se lee bien.
// (3) - Mira que el byte se adapta a nuestro protcolo. '0' o '1' son los unicos bytes acepatdos
// (4) - Mira si de que tipo es el byte
// (4.1) - Si el byte es 1, indica que la conexion la esta haciendo david y por lo tanto se abre a que se conecten usuarios que le guien. Si david ya esta conectado de antes, se expulsa a este usuario. Se guarda a david en la varaible global *ptr_david
// (4.2) - Si el byte es 0, mira si david se ha conectado, si se ha conectado mete a este usuario a ser un guia de david, si no, le dice al usaurio que david aun no se conecto y cierra la conexion. Se guarda a los guias en el vector global *ptr_guia[];
void RegistrarCliente(struct Cliente* cliente) {

    struct Auxiliar aux;
    //
    // (1)
    //
    // ¿De que tipo es?
    aux.bytes_entrada = recv(cliente->Descriptor, &(aux.buffer_entrada), sizeof(char), 0);
    //
    // (2)
    //
    // Controlamos errores
    if (aux.bytes_entrada==-1) {
        ErrorBytesEntrada(cliente);
        return;
    }
    if (aux.bytes_entrada==0) {
        CeroBytesEntrada(cliente);
        return;
    }
    if (aux.bytes_entrada!=1) {
        FueraDeProtocolo(cliente);
        return;
    }
    //
    // (3)
    //
    // Miramos si el byte forma parte de nuestro protocolo
    if (!(aux.buffer_entrada == '0' || aux.buffer_entrada == '1')) {
        FueraDeProtocolo(cliente);
        return;
    }
    //
    // (4)
    //
    // Miramos si es un guia, o es david.
    if (aux.buffer_entrada == '1') { // Es DAVID
        //
        // (4.1)
        //
        if (david_conectado) { // Se ha conectado ya david?
            DavidYaConectado(cliente);
            return;
        }
        david_conectado = 1;
        cliente->Tipo = aux.buffer_entrada;
        ptr_david = *cliente;
        printf("<SERVIDOR/INFO> %d -> DAVID SE ACABA DE CONECTAR. DESCRIPTOR: %d\n", cliente->ID_Cliente, cliente->Descriptor);
    } else { // Es un GUIA
        //
        // (4.2)
        //
        // ¿Esta ya david conectado?
        if (!david_conectado) {
            DavidNoConectado(cliente);
            return;
        }

        cliente->Tipo = aux.buffer_entrada;
        printf("<SERVIDOR/INFO> %d -> Un guia se acaba de conectar.\n", cliente->ID_Cliente);

    }

}

// Atiende a David
void AtenderDavid() {
    // IMPLEMENTAR
    struct Auxiliar aux;

    for (;;) {
        aux.bytes_entrada = recv(ptr_david.Descriptor, &(aux.buffer_entrada), sizeof(char), 0);

        if (aux.bytes_entrada==-1) {
            ErrorBytesEntrada(&ptr_david);
            david_conectado = 0;
            return;
        }
        if (aux.bytes_entrada==0) {
            CeroBytesEntrada(&ptr_david);
            david_conectado = 0;
            return;
        }
        printf("DEBUG\n");

    }


}
// (1) - Esperamos comando del guia.
// (2) - Checeka si el byte se lee bien.
// (3) - Mira que el byte se adapta a nuestro protcolo. '1' o '3' o '7' o '9' son los unicos bytes acepatdos
// (4) - Informa al servidor de que el guia ha enviado el comando x
// (5) - Enviamos el commando a david.
// (6) - Miramos que se haya enviado bien.
void AtenderGuia(struct Cliente* cliente) {
    // IMPLEMENTAR
    struct Auxiliar aux;

    for(;;) {
        //
        // (1)
        //
        // Esperamos comando
        aux.bytes_entrada = recv(cliente->Descriptor, &(aux.buffer_entrada), sizeof(char), 0);
        //
        // (2)
        //
        if (aux.bytes_entrada==-1) {
            ErrorBytesEntrada(cliente);
            return;
        }
        if (aux.bytes_entrada==0) {
            CeroBytesEntrada(cliente);
            return;
        }
        if (aux.buffer_entrada == 'x') {
            david_conectado = 0;
            ResetDavid(cliente);
            continue;
        }
        //
        // (3)
        //
        if (!(aux.buffer_entrada == '1' || aux.buffer_entrada == '3' || aux.buffer_entrada == '7' || aux.buffer_entrada == '9')) {
            FueraDeProtocolo(cliente);
            return;
        }

        //
        // (4)
        //
        GuiaEnviaComando(cliente, aux.buffer_entrada);
        //
        // (5)
        //
        aux.bytes_salida = send(ptr_david.Descriptor, &(aux.buffer_entrada), sizeof(char), 0);
        //
        // (6)
        //
        if (aux.bytes_salida==-1) {
            ErrorBytesSalida(cliente);
        }
    }
}

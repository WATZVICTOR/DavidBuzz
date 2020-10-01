#include <arpa/inet.h>
// PROYECTO DAVIDBUZZ
// Uso militar.

//
// ESTRUCTURAS
//

// Estrcutura que almacena los datos de un cliente conectado al servidor.
struct Cliente {
    struct sockaddr_in Conexion;
    pthread_t ID_Hilo;
    int ID_Cliente;
    int Descriptor;
    char Tipo; // 1 - David | 0 - Guia
};

// Estrcutura auxiliar que usaremos para alamcenar estructuras de datos.
struct Auxiliar {
    int bytes_entrada;
    int bytes_salida;
    char buffer_entrada;
    char buffer_salida;
};

//
// IMPRIMIR ESTRUCTURAS
//

// Imprime estrcutura Cliente
void ImprimeCliente(struct Cliente* cliente) {
    printf("<SERVIDOR/DEBUG> #####\n");
    printf("Tipo: %c\n",cliente->Tipo);
    printf("Descriptor: %d\n",cliente->Descriptor);
    printf("ID_Cliente: %d\n",cliente->ID_Cliente);
    printf("ID_Hilo: %ld\n",cliente->ID_Hilo);
    printf("Conexion:\n");
    printf("    IP: %s\n", inet_ntoa(cliente->Conexion.sin_addr));
    printf("    Puerto: %d\n",ntohs(cliente->Conexion.sin_port));
    printf("<SERVIDOR/DEBUG> #####\n");
}

// Imprime estrcutura auxiliar.
void ImprimeAuxiliar(struct Auxiliar* auxiliar) {
    printf("Bytes Entrada: %d\n",auxiliar->bytes_entrada);
    printf("Bytes Salida: %d\n",auxiliar->bytes_salida);
    printf("Buffer Entrada: %c\n",auxiliar->buffer_entrada);
    printf("Buffer Salida: %c\n",auxiliar->buffer_salida);
}

//
// Log
//

void CeroBytesEntrada(struct Cliente* cliente) {
    printf("<SERVIDOR/INFO> %d -> Usuario desconectado.\n", cliente->ID_Cliente);
    sleep(1);
    close(cliente->Descriptor);
}

void ErrorBytesEntrada(struct Cliente* cliente) {
    printf("<SERVIDOR/ERROR> %d -> Error en la lectura del cliente.\n", cliente->ID_Cliente);
    CeroBytesEntrada(cliente);
}

void FueraDeProtocolo(struct Cliente* cliente) {
    printf("<SERVIDOR/INFO> %d -> Usuario ha enviado msg fuera del protocolo.\n", cliente->ID_Cliente);
    CeroBytesEntrada(cliente);
}

void DavidNoConectado(struct Cliente* cliente) {
    printf("<SERVIDOR/INFO> %d -> El usuario ha intenado ser guia de David, pero este aun no esta conectado.\n", cliente->ID_Cliente);
    char msg[] = "David aun no esta conectado.\n";
    send(cliente->Descriptor, msg, sizeof(msg), 0);
    CeroBytesEntrada(cliente);
}

void DavidYaConectado(struct Cliente* cliente) {
    printf("<SERVIDOR/INFO> %d -> SE HA INTENTADO CONECTAR OTRO DAVID.\n", cliente->ID_Cliente);
    char msg[] = "David aun ya se encuentra conectado.\n";
    send(cliente->Descriptor, msg, sizeof(msg), 0);
    CeroBytesEntrada(cliente);
}

//
// AtenderGuia
//

void GuiaEnviaComando(struct Cliente* cliente, char cmd) {
    switch(cmd) {
    case '1':
        printf("<SERVIDOR/INFO> %d -> %c (Abajo izquierda)\n", cliente->ID_Cliente, cmd);
        break;
    case '3':
        printf("<SERVIDOR/INFO> %d -> %c (Abajo derecha)\n", cliente->ID_Cliente, cmd);
        break;
    case '7':
        printf("<SERVIDOR/INFO> %d -> %c (Arriba izquierda)\n", cliente->ID_Cliente, cmd);
        break;
    case '9':
        printf("<SERVIDOR/INFO> %d -> %c (Arriba derecha)\n", cliente->ID_Cliente, cmd);
        break;
    default:
        printf("<SERVIDOR/INFO> %d -> %c (DESCONOCIDO)\n", cliente->ID_Cliente, cmd);
        break;
    }
    return;
}

void ErrorBytesSalida(struct Cliente* cliente) {
    printf("<SERVIDOR/ERROR> %d -> Error en el envio de datos a DAVID.\n", cliente->ID_Cliente);
    perror("Error: ");
}

void ResetDavid(struct Cliente* cliente) {
    printf("<SERVIDOR/ERROR> %d -> HA RESETEADO A DAVID\n", cliente->ID_Cliente);
}

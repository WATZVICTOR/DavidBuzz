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
    char Nombre[512];
    char Tipo; // A - Mentor | B - Mentorizado | C - Sin Asignar
};

// Estrcutura auxiliar que usaremos para alamcenar estructuras de datos.
struct Auxiliar {
    int bytes_entrada;
    int bytes_salida;
    char buffer_entrada[512];
    char buffer_salida[512];
};

//
// IMPRIMIR ESTRUCTURAS
//

// Imprime estrcutura Cliente
void ImprimeCliente(struct Cliente* cliente) {
    printf("Nombre: %s\n",cliente->Nombre);
    printf("Tipo: %c\n",cliente->Tipo);
    printf("Descriptor: %d\n",cliente->Descriptor);
    printf("ID_Cliente: %d\n",cliente->ID_Cliente);
    printf("ID_Hilo: %ld\n",cliente->ID_Hilo);
    printf("Conexion:\n");
    printf("    IP: %s\n", inet_ntoa(cliente->Conexion.sin_addr));
    printf("    Puerto: %d\n",ntohs(cliente->Conexion.sin_port));
}

// Imprime estrcutura auxiliar.
void ImprimeAuxiliar(struct Auxiliar* auxiliar) {
    printf("Bytes Entrada: %d\n",auxiliar->bytes_entrada);
    printf("Bytes Salida: %d\n",auxiliar->bytes_salida);
    printf("Buffer Entrada: %s\n",auxiliar->buffer_entrada);
    printf("Buffer Salida: %s\n",auxiliar->buffer_salida);
}

//
// IMPRIMIR MENSAJES DE LOG
//


// Imprime una nueva conexion.
void ImprimeInfoNuevaConexion(struct sockaddr_in* conexion) {
    printf("<SERVIDOR/INFO> IP: %s Conectado.\n",inet_ntoa(conexion->sin_addr));
}

// Imprime una desconexion de un usuario sin registrar.
void ImprimeInfoDesconexionAnonima(struct Cliente* cliente) {
        printf("<SERVIDOR/INFO> IP: %s Desconectado.\n",inet_ntoa(cliente->Conexion.sin_addr));

}

// Imprime un error respecto a una conexion de un usuario.
void ImprimeErrorClienteAnonimo(struct Cliente* cliente) {
    printf("<SERVIDOR/ERROR> IP: %s Desconectado. Error en la lectura del socket %d\n",inet_ntoa(cliente->Conexion.sin_addr), cliente->Descriptor);
}

// Imprime el registro de un usuario. (Ha metido su nombre)
void ImprimeInfoRegistroCliente(struct Cliente* cliente) {
    printf("<SERVIDOR/INFO> IP: %s Registrado como %s\n",inet_ntoa(cliente->Conexion.sin_addr), cliente->Nombre);
}

// Imprime un error del cliente por execer el numero de caracteres en el registro del nombre.
void ImprimeErrorClienteNombre32(struct Cliente* cliente) {
    printf("<SERVIDOR/INFO> IP: %s Desconectado. Excedio la longitud de nombre.\n",inet_ntoa(cliente->Conexion.sin_addr));

}

//
//  ADMINISTRAR VECTOR DE CLIENTES
//

// Imprime todos los clientes del vectro que le pasemos.
void VectorClientesImprime(struct Cliente** clientes, int n) {
    struct Cliente aux;
    memset(&aux,0,sizeof(struct Cliente));
    for (int i=0;i<n;i++) {
        if (memcmp(clientes[i],&aux,sizeof(struct Cliente))==0) {
            ImprimeCliente(clientes[i]);
        }
    }
}
// Añade al Vector clientes el cliente que le pasamos.
void VectorClientesAddCliente(struct Cliente* cliente, struct Cliente** clientes, int n) {
    clientes[cliente->ID_Cliente] = cliente;
}

// Inicializa el vector clientes a ceros.
void VectorClientesInicializa(struct Cliente** clientes, int n) {
    memset(clientes,0,n*sizeof(struct Cliente));
}

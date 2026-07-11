#ifndef CLIENTES_H //para evitar que se incluya varias veces el mismo archivo
#define CLIENTES_H //si no existe entonces se define y la crea
typedef struct { //estructura para almacenar los datos de los clientes
    char cedula[15];
    char nombre_cliente[60];
    int puntos_acumulados;
} Cliente; //se llama Cliente a estos datos y se lo debe llamar con ese nombre mas adelante
void registrarCliente(); //funcion para registrar un nuevo cliente
Cliente buscarClientePorCedula(const char* id_buscar);  //funcion para buscar un cliente por su cedula
void mostrarClientes(); //funcion para mostrar todos los clientes registrados
#endif // CLIENTES_H
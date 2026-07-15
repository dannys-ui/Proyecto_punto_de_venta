//                                                                                                                            //
//          Modulo de clientes          //
#ifndef CLIENTES_H //si no esta definida ejecuta las lineas de abajo
#define CLIENTES_H //lo define y crea
typedef struct { //estructura de datos
    char cedula[15];
    char nombre_cliente[60];
    int puntos_acumulados;
} Cliente; //nombre de la estructura de datos
void registrarCliente(); //funcion para registrar un nuevo cliente
void eliminarCliente(); //funcion para eliminar un cliente
Cliente buscarClientePorCedula(const char* id_buscar);//recibe un parametro (id_buscar) y devuelve un objeto cliente
void mostrarClientes(); //funcion para mostrar todos los clientes registrados
#endif // CLIENTES_H
//                                                                                                                            //
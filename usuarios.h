//                                                                                                                            //
//          Declaracion modulo de usuarios del sistma          //
#ifndef USUARIOS_H //si no esta definido procede a ejecutar lo que sigue
#define USUARIOS_H //lo define y crea
typedef enum { //estructura de datos
    ADMINISTRADOR = 1, //rol administrador = 1
    CAJERO, //valor 2 automáticamente asignado
    BODEGUERO, // valor 3 automaticamente asignado
} Rol; //nombre de la estrutura de datos
typedef struct { 
    char uid_nfc[24]; //arreglo para guardar la calva NFC
    char nombre[50]; //arreglo para el nombre 
    char rol_usuario[20]; //arreglo para el rol que ocupa
    Rol rol; //se veulve a llamar al enum de arriba
} Usuario; //nombre de la estructura de datos

void crearArchivoSemilla(); //funcion para crear un archivo inicial - revisar en usuarios.c
void leerUsuariosRegistrados(); //funcion para ver los usuarios del sitema, cuyos datos vienen del archivo semilla
void registrarUsuario(); //funcion para agregar un nuevo usuario
void eliminarUsuarioPorNombre(const char* nombre); //funcion para eliminar un usuario
#endif // USUARIOS_H
//                                                                                                                            //
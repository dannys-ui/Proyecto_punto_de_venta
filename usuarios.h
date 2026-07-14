#ifndef USUARIOS_H //para evitar que se incluya varias veces el mismo archivo
#define USUARIOS_H //si no existe entonces se define y la crea
typedef enum { //se define un tipo de dato enumerado llamado Rol
    ADMINISTRADOR = 1, //rol administrador = 1, los otros valores se asignan automáticamente a partir de 1
    CAJERO, //rol cajero con el valor 2 automáticamente asignado
    BODEGUERO, // igual que cajera se asigno automáticamente el valor 3
} Rol; //se llama rol a estos datos y se lo debe llamar con ese nombre mas adelante
typedef struct { //se define un tipo de dato estructurado llamado Usuario
    char uid_nfc[24]; //arreglo de tamaño 24 para guardar la clave de las tarjetas nfc
    char nombre[50]; //arreglo para el nombre 
    char rol_usuario[20]; //arreglo para el rol que ocupa
    Rol rol; //cada usuario tendra un campo llamado rol y ese campo solo puede tomar un valor de rol.
} Usuario; //se llama Usuario a estos datos y se lo debe llamar con ese nombre mas adelante

void crearArchivoSemilla(); //archivo "seed" o semilla que se genera por primera con datos de prueba.
void leerUsuariosRegistrados(); //funcion que lee los usuarios registrados en el archivo de usuarios
#endif // USUARIOS_H
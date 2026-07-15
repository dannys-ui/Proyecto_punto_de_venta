//                                                                                                                           //
#include <stdio.h>
#include <string.h>
#include "usuarios.h"
//          Funcion para crear el archivo binario donde se guardaran los 3 usuarios         //
void crearArchivoSemilla() {
    FILE *archivo = fopen("usuarios.dat", "wb"); //si existe lo abre, si no crea el archivo binario "usuarios"
    if (archivo == NULL) { //control de seguridad por si no se encuentra el archivo o no se puede abrir
        printf("Error: No se pudo crear el archivo de usuarios.\n");
        return;
    }
    Usuario lista_semilla[3] = { //arreglo de 3 elementos del tipo Usuario
        {"04:9D:24:25:CA:2A:81", "Jorgue", "Administrador", ADMINISTRADOR}, //tarjeta 1
        {"04:D2:E3:24:CA:2A:81", "Daniel", "Cajero", CAJERO}, //tarjeta 2
        {"04:D3:22:24:CA:2A:81", "Oscar", "Bodeguero", BODEGUERO}, //tarjeta 3
    };
    fwrite(lista_semilla, sizeof(Usuario), 3, archivo); //se escribe la lista de usuarios en el archivo
    fclose(archivo); //cierra el archivo para guardar los cambios
        printf("Archivo semilla 'usuarios.dat' creado con exito.\n");
}
//                                                                                                                           //
//          Implementacion de la funcion para leer el archivo binario y ver los usuarios registrados          //
void leerUsuariosRegistrados() {
    FILE *archivo = fopen("usuarios.dat", "rb"); //se abre el archivo usuarios en modo de lectura (read binary)
    if (archivo == NULL) { //control de seguridad igual que el anterior
        printf("Error: No se pudo abrir el archivo de usuarios.\n");
        return;
    }
    Usuario u; //u es un usuario individual que se puede llenar con datos
    printf("\n----Lista de usuarios registrados en el sistema----\n");
    while (fread(&u, sizeof(Usuario), 1, archivo) == 1) { //lee un objeto Usuario desde el archivo, si no hay mas datos devuleve 0 y termina el bucle
        printf("Nombre: %s\n", u.nombre);//mostrar los datos de los usuarios en orden
        printf("UID NFC: %s\n", u.uid_nfc);
        printf("Rol (texto): %s\n", u.rol_usuario);
        printf("Rol (codigo): ");
        switch (u.rol)
        {
        case ADMINISTRADOR:
            printf("Administrador (1)\n");
            break;
        case CAJERO:
            printf("Cajero (2)\n");
            break;
        case BODEGUERO:                         //imprime el rol usando el campo rol del enum
            printf("Bodeguero (3)\n");
            break;
        default:
            printf("Desconocido\n");
            break;
        }
        printf("-----------------------------\n");
    }
    fclose(archivo); //cierra el archivo para guardar los cambios
}
//                                                                                                                               //
//          Funcion para registar a un nuevo usuario en el sistema          //
void registrarUsuario() {
    FILE *archivo = fopen("usuarios.dat", "ab"); // abrir en modo append binario
    if (archivo == NULL) {//control de errores
        printf("Error al abrir el archivo de usuarios.\n");
        return;
    }
    Usuario nuevo;//variable "nuevo" de tipo Usuario donde se guardaran los nuevos datos
    printf("\n----REGISTRO DE NUEVO USUARIO----\n");
    printf("Ingrese UID NFC: ");//para registrar la clave de acceso
    scanf("%23s", nuevo.uid_nfc);
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // limpiar buffer
    printf("Ingrese nombre del usuario: ");
    fgets(nuevo.nombre, sizeof(nuevo.nombre), stdin);//para ingresar una linea entera con espacios
    nuevo.nombre[strcspn(nuevo.nombre, "\n")] = 0;
    printf("Ingrese rol (1=ADMINISTRADOR, 2=CAJERO, 3=BODEGUERO): ");//pide que rol se dara al nuevo usuario
    int rol;
    scanf("%d", &rol);
    nuevo.rol = (Rol)rol;//convierte ese numero al tipo enum Rol
    switch (nuevo.rol) {
        case ADMINISTRADOR: strcpy(nuevo.rol_usuario, "ADMINISTRADOR"); break;
        case CAJERO: strcpy(nuevo.rol_usuario, "CAJERO"); break;//copia el nombre del rol segun el valor del enum
        case BODEGUERO: strcpy(nuevo.rol_usuario, "BODEGUERO"); break;
        default: strcpy(nuevo.rol_usuario, "DESCONOCIDO"); break;
    }
    fwrite(&nuevo, sizeof(Usuario), 1, archivo);//escribe la estructura "nuevo" en el archivo binario
    fclose(archivo);//cierra el archivo
    printf("Usuario registrado exitosamente.\n"); //mensaje de exito
}
//                                                                                                                              //
//          Funcion para eliminar usuarios por nombre          //
void eliminarUsuarioPorNombre(const char* nombre) {
    FILE *archivo = fopen("usuarios.dat", "rb");//abre el archivo de usuarios en modo lectura
    FILE *temp = fopen("temp.dat", "wb");//abre un archivo temporal en modo escritura
    if (archivo == NULL || temp == NULL) {//control de errores
        printf("Error al abrir los archivos.\n");
        return;
    }
    Usuario u;//variable donde se leeran los usuarios del archivo
    int encontrado = 0;//indicador para saber si encontro el usuario
    while (fread(&u, sizeof(Usuario), 1, archivo) == 1) {//lee cada usuario del archivo
        if (strcmp(u.nombre, nombre) == 0) {//compara el nombre con el nombre a eliminar
            encontrado = 1;//si no coincide se copia al archivo temporal
        } else {
            fwrite(&u, sizeof(Usuario), 1, temp);
        }
    }
    fclose(archivo);
    fclose(temp);//cierra ambos archivos
    remove("usuarios.dat");//elimina el archivo de usuarios (aqui se mantiene el nombre a eliminar)
    rename("temp.dat", "usuarios.dat");//renombra el archivo temporal y pasa a ser el nuevo archivo de usuarios (aqui no se copio el nombre que se queria eliminar)
    if (encontrado)
        printf("Usuario '%s' eliminado exitosamente.\n", nombre);//mensaje de exito
    else
        printf("Usuario con nombre '%s' no encontrado.\n", nombre);//si no se encontro indica que el usuario buscado no existe
}
//                                                                                                                               //
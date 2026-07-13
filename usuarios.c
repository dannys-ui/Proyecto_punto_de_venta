#include <stdio.h>
#include <string.h>
#include "usuarios.h"
//! Funciona, no tocar :3
void crearArchivoSemilla() {
    FILE *archivo = fopen("usuarios.dat", "wb"); //se abre el archivo en modo binario para escritura
    if (archivo == NULL) { //control de seguridad por si no se encuentra el archivo o no se puede abrir
        printf("Error: No se pudo crear el archivo de usuarios.\n");
        return;
    }
    Usuario lista_semilla[4] = {
        {"04:9D:24:25:CA:2A:81", "Jorgue", "Administrador", ADMINISTRADOR, 0}, //tarjeta 1
        {"04:D2:E3:24:CA:2A:81", "Daniel", "Cajero", CAJERO, 0}, //tarjeta 2
        {"04:D3:22:24:CA:2A:81", "Oscar", "Bodeguero", BODEGUERO, 0}, //tarjeta 3
        {"04:CA:E4:24:CA:2A:81", "0000", "Comprador", COMPRADOR, 10} //tarjeta 4
    };
    fwrite(lista_semilla, sizeof(Usuario), 4, archivo); //se escribe la lista de usuarios en el archivo
    fclose(archivo); //cierra el archivo para guardar los cambios
        printf("Archivo semilla 'usuarios.dat' creado con exito.\n");
}
void leerUsuariosRegistrados() { //segunda funcion de lectura
    FILE *archivo = fopen("usuarios.dat", "rb"); //se abre el archivo en modo binario para lectura
    if (archivo == NULL) { //control de seguridad igual que el anterior
        printf("Error: No se pudo abrir el archivo de usuarios.\n");
        return;
    }
    Usuario u; //contenedor temporal
    printf("\n----Lista de usuarios registrados en el sistema----\n");
    while (fread(&u, sizeof(Usuario), 1, archivo) == 1) {
        printf("Nombre: %s\n", u.nombre_usuario);
        printf("UID NFC: %s\n", u.uid_nfc);
        printf("Clave: %s\n", u.clave_teclado);
        printf("Rol: ");
        switch (u.rol)
        {
        case ADMINISTRADOR:
            printf("Administrador (1)\n");
            break;
        case CAJERO:
            printf("Cajero (2)\n");
            break;
        case BODEGUERO:
            printf("Bodeguero (3)\n");
            break;
        case COMPRADOR:
            printf("Comprador (4)\n");
            break;
        default:
            printf("Desconocido\n");
            break;
        }
        printf("Puntos: %d\n", u.puntos);
        printf("-----------------------------\n");
    }
    fclose(archivo); //cierra el archivo para guardar los cambios
}
#include <stdio.h>
#include <string.h>
#include "usuarios.h"
void crearArchivoSemilla() {
    FILE *archivo = fopen("usuarios.dat", "wb"); //se abre el archivo en modo binario para escritura
    if (archivo == NULL) { //control de seguridad por si no se encuentra el archivo o no se puede abrir
        printf("Error: No se pudo crear el archivo de usuarios.\n");
        return;
    }
    Usuario lista_semilla[4] = {
        {"04:9D:24:25:CA:2A:81", "admin123", "Administrador", ADMINISTRADOR, 0},
        {"04:D2:E3:24:CA:2A:81", "cajero123", "Cajero", CAJERO, 0},
        {"04:D3:22:24:CA:2A:81", "bodeguero123", "Bodeguero", BODEGUERO, 0},
        {"04:CA:E4:24:CA:2A:81", "0000", "Comprador", COMPRADOR, 10}
    };

    fwrite(lista_semilla, sizeof(Usuario), 4, archivo); //se escribe la lista de usuarios en el archivo
    }
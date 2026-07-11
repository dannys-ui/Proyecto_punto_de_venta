#include <stdio.h>
#include "usuarios.h"
//!Solo es una prueba inicial para ver si se crean los archivos y se leen correctamente, no tocar :3
int main() {
    printf ("Prueba inicial para los usuarios...\n");
    crearArchivoSemilla();
    leerUsuariosRegistrados();
    return 0;
}
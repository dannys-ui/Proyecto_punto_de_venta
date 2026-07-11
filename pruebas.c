//!archivo para pruebas, todo se ira al main despues
#include <stdio.h>
#include "productos.h"
int main(){ 
    int opcion;
    do {
        printf("Prueba de modulo de productos.\n");
        printf("1. Crear inventario semilla\n");
        printf("2. Mostrar productos\n");
        printf("3. Registrar producto\n");
        printf("4. Actualizar producto\n");
        printf("5. Eliminar producto\n");
        printf("6. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                crearInventarioSemilla();
                break;
            case 2:
                mostrarProductos();
                break;
            case 3:
                registrarProducto();
                break;
            case 4:
                actualizaProducto();
                break;
            case 5:
                eliminarProducto();
                break;
            case 6:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (opcion != 6);
    return 0;
}
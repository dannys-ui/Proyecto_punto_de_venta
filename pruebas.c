//!archivo para pruebas, todo se ira al main despues
#include <stdio.h>
#include "productos.h"
#include "clientes.h"
int main(){ 
    int opcion;
    do {
        printf("Prueba de modulo de productos.\n");
        printf("1. Crear inventario semilla\n");
        printf("2. Mostrar productos\n");
        printf("3. Registrar producto\n");
        printf("4. Actualizar producto\n");
        printf("5. Eliminar producto\n");
        printf("6. Realizar Venta\n");
        printf("7. Historial de ventas\n");
        printf("8. Salir\n");
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
                realizarVenta();
                break;
            case 7:
                mostrarVentasRealizadas();
                break;
            case 8:
                printf("Saliendo.\n");
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (opcion != 8);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuarios.h"
#include "productos.h"
#include "clientes.h"
#include "caja.h"
#include "reportes.h"
#include "ventas.h"

void menuAdministrador();
void menuCajero();
void menuBodeguero();
int main() {
    FILE *comprobarInventario = fopen("productos.dat", "rb");
    if(comprobarInventario == NULL) {
        printf("[SISTEMA] Creando base de datos inicial de productos.\n");
        crearInventarioSemilla();
    } else {
        fclose(comprobarInventario);
    }
    FILE *comprobarUsuarios = fopen("usuarios.dat", "rb");
    if(comprobarUsuarios == NULL) {
        printf("[SISTEMA] Creando base de datos inicial de usuarios.\n");
        crearArchivoSemilla();
    } else {
        fclose(comprobarUsuarios);
    }
    printf("[SISTEMA] Bases de datos listas.\n");
    char uid_ingresado[24];
    int autenticado = 0;
    Rol rolUsuario;
    printf("\n    BIENVENIDO AL SISTEMA DE VENTAS    \n");
    //! De momento se ingresa a mano hasta recibir el modulo de tarjetas NFC
    //Administrador = 04:9D:24:25:CA:2A:81
    //Cajero = 04:D2:E3:24:CA:2A:81
    //Bodeguero = 04:D3:22:24:CA:2A:81
    printf("Ingrese el UID NFC de su tarjeta para ingresar: ");
    scanf("%23s", uid_ingresado);
    FILE *archivo = fopen("usuarios.dat", "rb");
    if(archivo == NULL) {
        printf("Error: No se pudo abrir la base de datos de usuarios.\n");
        return 1;
    }
    Usuario u;
    while(fread(&u, sizeof(Usuario), 1, archivo) == 1) {
        if(strcmp(u.uid_nfc, uid_ingresado) == 0) {
            autenticado = 1;
            rolUsuario = u.rol;
            printf("\nAcceso concedido. Bienvenido: %s [%s]\n", u.rol_usuario, u.nombre);
            break;
        }
    }
    fclose(archivo);
    if(!autenticado) {
        printf("Error: Tarjeta NFC no registrada en el sistema.\n");
        return 0;
    }
    switch (rolUsuario) {
    case ADMINISTRADOR:
        menuAdministrador();
        break;
    case CAJERO:
        menuCajero();
        break;
    case BODEGUERO:
        menuBodeguero();
        break;
    default:
        printf("Los roles desconocidos no tienen acceso al sistema de gestion.\n");
        break;
    }
    printf("\nSistema cerrado correctamente\n");
    return 0;
}
void menuAdministrador() {
    int opcion;
    do {
        printf("\n    MENU ADMINISTRADOR    \n");
        printf("1. Ver inventario completo.\n");
        printf("2. Registrar nuevo producto.\n");
        printf("3. Actualizar producto.\n");
        printf("4. Eliminar producto.\n");
        printf("5. Ver lista de clientes.\n");
        printf("6. Registrar nuevo cliente.\n");
        printf("7. Abrir caja.\n");
        printf("8. Realizar venta.\n");
        printf("9. Historial de ventas del dia.\n");
        printf("10. Cerrar caja.\n");
        printf("11. Reportes avanzados.\n");
        printf("12. Ver usuarios del sistema.\n");
        printf("13. Eliminar usuarios del sistema.\n");
        printf("14. Historial completo de ventas.\n");
        printf("15. Salir del sistema.\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1: mostrarProductos(); break;
            case 2: registrarProducto(); break;
            case 3: actualizaProducto(); break;
            case 4: eliminarProducto(); break;
            case 5: mostrarClientes(); break;
            case 6: registrarCliente(); break;
            case 7: abrirCaja(); break;
            case 8: realizarVenta(); break;
            case 9: mostrarVentas(); break;
            case 10: cerrarCaja(); break;
            case 11: menuReportesAvanzado(); break;
            case 12: leerUsuariosRegistrados(); break;
            case 13: {
                char nombre[50];
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF); // limpiar el "\n" que dejo el scanf de la opcion
                printf("Ingrese el nombre del usuario a eliminar: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = 0;
                eliminarUsuarioPorNombre(nombre);
                break;
            }
            case 14: mostrarHistorialVentas(); break;
            case 15: printf("Cerrando sesion de ADMINISTRADOR.\n"); break;
            default: printf("Opcion no valida.\n");
        }
    } while (opcion != 15);
}
void menuCajero() {
    int opcion;
    do {
        printf("\n      MENU CAJERO     \n");
        printf("1. Abrir caja.\n");
        printf("2. Realizar facturacion.\n");
        printf("3. Consultar clientes.\n");
        printf("4. Registrar nuevo cliente.\n");
        printf("5. Historial de ventas del dia.\n");
        printf("6. Cerrar caja.\n");
        printf("7. Salir del sistema.\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1: abrirCaja(); break;
            case 2: realizarVenta(); break;
            case 3: mostrarClientes(); break;
            case 4: registrarCliente(); break;
            case 5: mostrarVentas(); break;
            case 6: cerrarCaja(); break;
            case 7: printf("Cerrando sesion de CAJERO.\n"); break;
            default: printf("Opcion no valida.\n");
        }
    } while (opcion != 7);
}
void menuBodeguero() {
    int opcion;
    do {
        printf("\n      MENU BODEGUERO      \n");
        printf("1. Ver stock actual de productos.\n");
        printf("2. Registrar ingreso de nuevo producto.\n");
        printf("3. Actualizar producto.\n");
        printf("4. Reporte de stock bajo minimo.\n");
        printf("5. Inventario ordenado alfabeticamente.\n");
        printf("6. Salir del sistema.\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1: mostrarProductos(); break;
            case 2: registrarProducto(); break;
            case 3: actualizaProducto(); break;
            case 4: {
                int minimo;
                printf("Ingrese el stock minimo: ");
                scanf("%d", &minimo);
                mostrarStockBajoMinimo(minimo);
                break;
            }
            case 5: mostrarInventarioOrdenado(); break;
            case 6: printf("Cerrando sesion de BODEGUERO.\n"); break;
            default: printf("Opcion no valida.\n");
        }
    } while (opcion != 6);
}
#include "reportes.h"
#include <stdio.h>
#include <stdlib.h>
#include "clientes.h"
#include <string.h>
int calcularTotalUnidades(FILE *archivo) {
    Producto p;
    if (fread(&p, sizeof(Producto), 1, archivo) != 1) {
        return 0;
    }
    return p.stock + calcularTotalUnidades(archivo);
}
void reporteStockBajo() {
    FILE *archivo = fopen("productos.dat", "rb");
    if(archivo == NULL) {
        printf("Error al abrir el inventario.\n");
        return;
    }
    int gran_total = calcularTotalUnidades(archivo);
    fclose(archivo);
    printf("\n          REPORTE         \n");
    printf("=============================\n");
    printf("Total de productos fisicos en la tienda: %d unidades.\n", gran_total);
    printf("==============================\n");
}
void reporteProductosPorPrecio() {
    FILE *archivo = fopen("productos.dat", "rb");
    if (archivo == NULL) {
        printf("Error al abrir el inventario.\n");
        return;
    }
    Producto lista[500];
    int total_productos = 0;
    while (fread(&lista[total_productos], sizeof(Producto), 1, archivo) == 1) {
        total_productos++;
        if(total_productos >= 500) break;
    }
    fclose(archivo);
    if(total_productos == 0) {
        printf("\nNo hay productos registrados para ordenar.\n");
        return;
    }
    for(int i = 0; i < total_productos - 1; i++) {
        for(int j = 0; j < total_productos - i - 1; j++) {
            if(lista[j].precio < lista[j + 1].precio) {
                Producto temp = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = temp;
            }
        }
    }
    printf("\n        REPORTE PRODUCTOS POR PRECIO            \n");
    printf("%-5s | %-20s | %-10s | %-6s\n", "ID", "Nombre", "Precio", "Stock");
    for(int i = 0; i < total_productos; i++) {
        printf("%-5d | %-20s | $%-9.2f | %-6d\n", lista[i].id_producto, lista[i].nombre_producto, lista[i].precio, lista[i].stock);
    }
}
void menuReportes() {
    int opcion;
    printf("\n======================================\n");
    printf("            MODULO DE REPORTES          \n");
    printf("1. Total unidades fisicas.\n");
    printf("2. Listar productos por precio.\n");
    printf("3. Historial de ventas facturadas.\n");
    printf("4. Consulta de clientes.\n");
    printf("0. Volver al menu principal.\n");
    printf("========================================\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);
    char cedula[15];
    Cliente c;
    switch (opcion)
    {
    case 1:
        reporteStockBajo();
        break;
    case 2:
        reporteProductosPorPrecio();
        break;
    case 3:
        printf("\n---- HISTORIAL DE FACTURACION ----\n");
        FILE *archivoVentas = fopen("ventas.dat", "rb");
        if(archivoVentas == NULL) {
            printf("No hay registros de ventas aun.\n");
        } else {
            Factura f;
            printf("%-10s | %-15s\n", "ID venta", "Total Pagado");
            printf("------------------------------------------\n");
            while (fread(&f, sizeof(Factura), 1, archivoVentas) == 1) {
                printf("%-10d | $%-15.2f\n", f.id_venta, f.total_pagado);
            }
            fclose(archivoVentas);
        }
        break;
    case 4:
        printf("\n---- CONSULTA DE CLIENTES ----\n");
        printf("Ingrese la cedula a consultar: ");
        scanf("%s", cedula);
        c = buscarClientePorCedula(cedula);
        if (strcmp(c.nombre_cliente, "No registrado") == 0) {
            printf("La cedula ingresada no se encuentra regisgtrada en la base de datos.\n");
        } else {
            printf("Cliente: %s | Cedula: %s\n", c.nombre_cliente, c.cedula);
        }
        break;
    case 0:
        return;
    default:
        printf("Opcion invalida.\n");
        break;
    }
}
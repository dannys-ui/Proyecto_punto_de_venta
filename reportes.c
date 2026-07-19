#include <stdio.h>
#include <string.h>
#include "reportes.h"
#include "ventas.h"
//                                                                                                                                //
//          Funcion para buscar una venta por fecha          //
void mostrarVentasPorFecha(const char *fecha_buscar) {//funcion que va a recibir el parametro de la fecha
    FILE *archivo = fopen("ventas.dat", "rb");//abre el archivo en modo de lectura
    if (archivo == NULL) {//control de error
        printf("\nNo hay ventas registradas.\n");
        return;
    }
    Factura f;//f estructura para leer cada factura
    float total_fecha = 0;//acumulador de total vendido en cada fecha
    int contador = 0;//cuantas facturas se hicieron en esa fecha
    printf("\n---- VENTAS DEL %s ----\n", fecha_buscar);
    printf("%-10s | %-12s | %-15s\n", "Num Ticket", "Total Pagado", "Cliente");//imprime el encabezado y datos en columnas
    printf("---------------------------------------------------\n");
    while (fread(&f, sizeof(Factura), 1, archivo) == 1) {//recorre todas las facturas del archivo
        if (strcmp(f.fecha, fecha_buscar) == 0) {//compara la fecha de la factura con la fecha buscada
            printf("Ticket #%-4s | $%-11.2f | %s\n", f.id_factura, f.total_pagado, f.cedula_cliente);//si coincide imprime el ticket, el total y el cliente
            total_fecha += f.total_pagado;//suma total
            contador++;//aumenta el contador
        }
    }
    if (contador == 0) {
        printf("No se encontraron ventas en esa fecha.\n");//si no hay ventas entonces no hay facturas
    } else {
        printf("---------------------------------------------------\n");
        printf("TOTAL VENDIDO EN ESA FECHA: $%.2f\n", total_fecha);//imprime el total vendido en esa fecha
    }
    fclose(archivo);//cierra el archivo
}
//                                                                                                                                //
//          Funcion para mostrar las ventas en un rango de tiempo          //
void mostrarVentasPorRango(const char *fecha_inicio, const char *fecha_fin) {
    FILE *archivo = fopen("ventas.dat", "rb"); //abre el archivo en modo lectura binaria
    if (archivo == NULL) { //control de error
        printf("\nNo hay ventas registradas.\n");
        return;
    }
    Factura f; //estructura para leer cada factura
    float total_rango = 0; //acumulador del total vendido en el rango
    int contador = 0; //facturas que se encontraron en ese rango
    printf("\n---- VENTAS DEL %s AL %s ----\n", fecha_inicio, fecha_fin); //imprime titulo y columnas para organizar los datos
    printf("%-10s | %-12s | %-15s | %-10s\n", "Num Ticket", "Total Pagado", "Cliente", "Fecha");
    printf("---------------------------------------------------------------\n");
    while (fread(&f, sizeof(Factura), 1, archivo) == 1) { //recorre todas las facturas
        if (strcmp(f.fecha, fecha_inicio) >= 0 && strcmp(f.fecha, fecha_fin) <= 0) {
            printf("Ticket #%-4s | $%-11.2f | %s | %s\n", f.id_factura, f.total_pagado, f.cedula_cliente, f.fecha);
            total_rango += f.total_pagado; //suma el total de esa factura
            contador++; //aumenta el contador
        }
    }
    if (contador == 0) {
        printf("No se encontraron ventas en ese rango.\n"); //si no hubo coincidencias
    } else {
        printf("---------------------------------------------------------------\n");
        printf("TOTAL VENDIDO EN EL RANGO: $%.2f\n", total_rango); //imprime el total acumulado
    }
    fclose(archivo); // cierra el archivo
}
//                                                                                                                                //
//          Funcion para mostrar el producto mas vendido          //
void mostrarProductosMasVendidos() {
    FILE *archivo = fopen("ventas.dat", "rb"); // abre archivo de ventas en modo lectura
    if (archivo == NULL) { //control de error
        printf("\nNo hay ventas registradas.\n");
        return;
    }
    typedef struct { //estructura temporal para contar cuantas veces se vendio cada producto
        char nombre_producto[50];
        int cantidad_total;
    } ProductoVenta; //nombre de la estructura
    ProductoVenta productos[100]; // arreglo auxiliar para guardar hasta 100 productos
    int num_productos = 0; //cuenta cuantos productos unicos se han registrado
    Factura f;
    while (fread(&f, sizeof(Factura), 1, archivo) == 1) { //lee cada factura del archivo
        int encontrado = 0;
        for (int i = 0; i < num_productos; i++) {
            if (strcmp(productos[i].nombre_producto, f.nombre_producto) == 0) { //compara los productos de la lista con los productos de la factura
                productos[i].cantidad_total += f.cantidad; //suma la cantidad
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            if (num_productos < 100) {
                strcpy(productos[num_productos].nombre_producto, f.nombre_producto); //copia el nombre de la factura al auxiliar productos
                productos[num_productos].cantidad_total = f.cantidad;
                num_productos++;
            } else {
                printf("Aviso: se alcanzo el limite de 100 productos distintos en el reporte.\n");
            }
        }
    }
    fclose(archivo); //cierra el archivo
    for (int i = 0; i < num_productos - 1; i++) { //bubble sort para ordenar de mayor a menor
        for (int j = 0; j < num_productos - i - 1; j++) {
            if (productos[j].cantidad_total < productos[j+1].cantidad_total) { 
                ProductoVenta temp = productos[j];
                productos[j] = productos[j+1];
                productos[j+1] = temp;
            }
        }
    }
    printf("\n---- PRODUCTOS MÁS VENDIDOS ----\n");
    printf("%-20s | %-10s\n", "Producto", "Cantidad"); //muestra la lista de los productos mas vendidos
    printf("---------------------------------\n");
    for (int i = 0; i < num_productos; i++) {
        printf("%-20s | %-10d\n", productos[i].nombre_producto, productos[i].cantidad_total);
    }
}
//                                                                                                                                //
//          Funcion para alertar de estock bajo          //
void mostrarStockBajoMinimo(int minimo) {
    FILE *archivo = fopen("productos.dat", "rb"); //abre archivo de productos
    if (archivo == NULL) { //control de error
        printf("\nNo hay productos registrados.\n");
        return;
    }
    Producto p; //estructura de producto
    int contador = 0; //cuena cuantos productos cumplen la condicion
    printf("\n---- PRODUCTOS CON STOCK BAJO (%d unidades) ----\n", minimo);
    printf("%-10s | %-20s | %-10s\n", "ID", "Nombre", "Stock"); //imprimir con formato de tabla
    printf("-------------------------------------------------\n");
    while (fread(&p, sizeof(Producto), 1, archivo) == 1) { //lee cada producto
        if (p.stock < minimo) { //condición: stock menor al mínimo
            printf("%-10d | %-20s | %-10d\n", p.id_producto, p.nombre_producto, p.stock);
            contador++; //aumenta el contador
        }
    }
    if (contador == 0) {
        printf("No se encontraron productos con stock bajo.\n"); //si ningun producto cumple la condicion
    }
    fclose(archivo); //cierra el archivo
}
//                                                                                                                                 //
//          Funcion para el ranking de clientes          //
void mostrarRankingClientes() {
    FILE *archivo = fopen("ventas.dat", "rb"); // abre archivo de ventas en modo lectura
    if (archivo == NULL) { //control de error
        printf("\nNo hay ventas registradas.\n");
        return;
    }
    typedef struct { //estructura auxiliar para acumular los datos de los clientes
        char cedula_cliente[20];
        float total_gastado;
    } ClienteRanking; //nombre de la estructura
    ClienteRanking clientes[100]; //arreglo auxiliar para guardar hasta 100 clientes
    int num_clientes = 0; //conte de usuarios unicos registrados
    Factura f; //variable para leer cada factura
    while (fread(&f, sizeof(Factura), 1, archivo) == 1) { //lee cada factura
        int encontrado = 0; //bandera para saber si el cliente ya estaba en la lista
        for (int i = 0; i < num_clientes; i++) {  //si el cliente ya esta en el arreglo suma el gasto de la factura al total
            if (strcmp(clientes[i].cedula_cliente, f.cedula_cliente) == 0) {
                clientes[i].total_gastado += f.total_pagado;
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) { //si el cliente no esta en la lista copia la cedula, inicia su gasto con el total de la factura 
            if(num_clientes < 100) {
                strcpy(clientes[num_clientes].cedula_cliente, f.cedula_cliente);
                clientes[num_clientes].total_gastado = f.total_pagado;
                num_clientes++;
            } else {
                printf("Aviso se alcanzo el limite de 100 clientes en el reporte.\n");
            }
        }
    }
    fclose(archivo); //cierra el archivo de ventas
    for (int i = 0; i < num_clientes - 1; i++) { //bubble sort para ordenar de mayor a menor
        for (int j = 0; j < num_clientes - i - 1; j++) {
            if (clientes[j].total_gastado < clientes[j+1].total_gastado) {
                ClienteRanking temp = clientes[j];
                clientes[j] = clientes[j+1];
                clientes[j+1] = temp;
            }
        }
    }
    printf("\n---- RANKING DE CLIENTES POR MONTO TOTAL ----\n");
    printf("%-20s | %-12s\n", "Cliente (Cedula)", "Total Gastado");
    printf("---------------------------------------------\n");
    for (int i = 0; i < num_clientes; i++) { //recorre el arreglo ya ordenado y muestra cada cliente con su gasto toal
        printf("%-20s | $%-11.2f\n", clientes[i].cedula_cliente, clientes[i].total_gastado);
    }
}
//                                                                                                                                //
//          Menu de reportes avanzados          //
void menuReportesAvanzado() {
    int opcion;
    do {
        printf("\n    MENU REPORTES AVANZADOS    \n");
        printf("1. Ventas por fecha.\n");
        printf("2. Ventas por rango.\n");
        printf("3. Productos mas vendidos.\n");
        printf("4. Stock bajo minimo.\n");
        printf("5. Ranking de clientes.\n");
        printf("6. Volver.\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1: {
                char fecha[11];
                printf("Ingrese la fecha (YYYY-MM-DD): ");
                scanf("%10s", fecha);
                mostrarVentasPorFecha(fecha);
                break;
            }
            case 2: {
                char inicio[11], fin[11];
                printf("Ingrese fecha inicio (YYYY-MM-DD): ");
                scanf("%10s", inicio);
                printf("Ingrese fecha fin (YYYY-MM-DD): ");
                scanf("%10s", fin);
                mostrarVentasPorRango(inicio, fin);
                break;
            }
            case 3: mostrarProductosMasVendidos(); break;
            case 4: {
                int minimo;
                printf("Ingrese el stock minimo: ");
                scanf("%d", &minimo);
                mostrarStockBajoMinimo(minimo);
                break;
            }
            case 5: mostrarRankingClientes(); break;
            case 6: printf("Volviendo al menu principal...\n"); break;
            default: printf("Opcion no válida.\n");
        }
    } while (opcion != 6);
}
//                                                                                                                                //
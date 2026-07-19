#include <stdio.h>
#include <stdlib.h>
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
//          Funcion para alertar de estock bajo (ordenado por stock con Seleccion)          //
void mostrarStockBajoMinimo(int minimo) {
    FILE *archivo = fopen("productos.dat", "rb"); //abre archivo de productos
    if (archivo == NULL) { //control de error
        printf("\nNo hay productos registrados.\n");
        return;
    }
    Producto p; //estructura de producto
    Producto bajos[100]; //arreglo auxiliar para guardar los productos que cumplen la condicion
    int contador = 0; //cuenta cuantos productos cumplen la condicion
    while (fread(&p, sizeof(Producto), 1, archivo) == 1) { //lee cada producto
        if (p.stock < minimo && contador < 100) { //condición: stock menor al mínimo (y que quepa en el arreglo)
            bajos[contador] = p; //se guarda el producto encontrado
            contador++; //aumenta el contador
        }
    }
    fclose(archivo); //cierra el archivo, ya no se necesita: el resto del trabajo es en memoria
    //          Ordenamiento por SELECCION (de menor a mayor stock)          //
    for (int i = 0; i < contador - 1; i++) {
        int pos_menor = i; //asume que el menor esta en la posicion actual
        for (int j = i + 1; j < contador; j++) {
            if (bajos[j].stock < bajos[pos_menor].stock) { //busca si hay uno mas pequeño en el resto del arreglo
                pos_menor = j; //actualiza la posicion del menor encontrado
            }
        }
        if (pos_menor != i) { //si el menor no era el actual, se intercambian
            Producto temp = bajos[i];
            bajos[i] = bajos[pos_menor];
            bajos[pos_menor] = temp;
        }
    }
    printf("\n---- PRODUCTOS CON STOCK BAJO (%d unidades) ----\n", minimo);
    printf("%-10s | %-20s | %-10s\n", "ID", "Nombre", "Stock"); //imprimir con formato de tabla
    printf("-------------------------------------------------\n");
    for (int i = 0; i < contador; i++) { //recorre el arreglo ya ordenado de menor a mayor stock
        printf("%-10d | %-20s | %-10d\n", bajos[i].id_producto, bajos[i].nombre_producto, bajos[i].stock);
    }
    if (contador == 0) {
        printf("No se encontraron productos con stock bajo.\n"); //si ningun producto cumple la condicion
    }
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
    printf("%-20s | %-12s\n", "Cliente (Cédula)", "Total Gastado");
    printf("---------------------------------------------\n");
    for (int i = 0; i < num_clientes; i++) { //recorre el arreglo ya ordenado y muestra cada cliente con su gasto toal
        printf("%-20s | $%-11.2f\n", clientes[i].cedula_cliente, clientes[i].total_gastado);
    }
}
//                                                                                                                                //
//          Funcion de particion para el Quicksort (ordena por nombre de producto)          //
int particionarProductos(Producto *arreglo, int inicio, int fin) {
    Producto pivote = arreglo[fin]; //se toma el ultimo elemento del tramo como pivote
    int i = inicio - 1; //marca la frontera entre "menores al pivote" y el resto
    for (int j = inicio; j < fin; j++) {
        if (strcmp(arreglo[j].nombre_producto, pivote.nombre_producto) < 0) { //compara nombres alfabeticamente
            i++;
            Producto temp = arreglo[i]; //intercambia para dejar los menores al inicio del tramo
            arreglo[i] = arreglo[j];
            arreglo[j] = temp;
        }
    }
    Producto temp = arreglo[i + 1]; //coloca el pivote en su posicion final correcta
    arreglo[i + 1] = arreglo[fin];
    arreglo[fin] = temp;
    return i + 1; //devuelve la posicion donde quedo el pivote
}
//                                                                                                                                //
//          Funcion RECURSIVA de Quicksort para ordenar productos por nombre          //
void quicksortProductos(Producto *arreglo, int inicio, int fin) {
    if (inicio < fin) { //caso recursivo: el tramo tiene mas de un elemento
        int pos_pivote = particionarProductos(arreglo, inicio, fin);
        quicksortProductos(arreglo, inicio, pos_pivote - 1); //ordena recursivamente la mitad izquierda del pivote
        quicksortProductos(arreglo, pos_pivote + 1, fin); //ordena recursivamente la mitad derecha del pivote
    }
    //caso base implicito: si inicio >= fin (0 o 1 elemento), no hace nada y no se vuelve a llamar
}
//                                                                                                                                //
//          Funcion para mostrar el inventario ordenado alfabeticamente (Quicksort + memoria dinamica)          //
void mostrarInventarioOrdenado() {
    FILE *archivo = fopen("productos.dat", "rb");
    if (archivo == NULL) {
        printf("\nNo hay productos registrados.\n");
        return;
    }
    int total = 0; //contador de cuantos productos hay en total
    Producto temp;
    while (fread(&temp, sizeof(Producto), 1, archivo) == 1) { //primera pasada: solo cuenta cuantos productos hay
        total++;
    }
    if (total == 0) {
        printf("\nNo hay productos registrados.\n");
        fclose(archivo);
        return;
    }
    Producto *productos = malloc(total * sizeof(Producto)); //reserva memoria dinamica del tamaño exacto que se necesita
    if (productos == NULL) { //control de error si no hay memoria disponible
        printf("Error: no se pudo reservar memoria para el inventario.\n");
        fclose(archivo);
        return;
    }
    rewind(archivo); //regresa al inicio del archivo para la segunda pasada
    int i = 0;
    while (fread(&temp, sizeof(Producto), 1, archivo) == 1) { //segunda pasada: carga los productos a memoria dinamica
        productos[i] = temp;
        i++;
    }
    fclose(archivo); //ya no se necesita el archivo, el resto del trabajo es en memoria
    quicksortProductos(productos, 0, total - 1); //ordena el arreglo dinamico alfabeticamente por nombre
    printf("\n---- INVENTARIO ORDENADO ALFABETICAMENTE (Quicksort) ----\n");
    printf("%-6s | %-25s | %-7s | %-5s\n", "ID", "Nombre del Producto", "Precio", "Stock");
    printf("-------------------------------------------------------------\n");
    for (int k = 0; k < total; k++) {
        printf("%-6d | %-25s | $%-6.2f | %-5d\n", productos[k].id_producto, productos[k].nombre_producto, productos[k].precio, productos[k].stock);
    }
    printf("-------------------------------------------------------------\n");
    free(productos); //libera la memoria reservada dinamicamente, ya no se necesita
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
        printf("6. Inventario ordenado alfabeticamente.\n");
        printf("7. Volver.\n");
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
            case 6: mostrarInventarioOrdenado(); break;
            case 7: printf("Volviendo al menu principal...\n"); break;
            default: printf("Opcion no válida.\n");
        }
    } while (opcion != 7);
}
//                                                                                                                                //
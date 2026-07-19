#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ventas.h"
#include "caja.h"
#include "clientes.h"
//                                                                                                                                 //
char* obtenerSiguienteIdFactura() {
    static int contador = 1;
    static char id[10];
    sprintf(id, "F%04d", contador++);
    return id;
}
//          Funcion para realizar ventas          //
void realizarVenta() {
    if (esCajaAbierta() == 0) {//verifica si la caja esta abierta para poder realizar la venta
        printf("Error: La caja aun no ha sido abierta.\n");//mensaje de error
        return;
    }
    FILE *archivoProds = fopen("productos.dat", "rb+");//abre productos en modo lectura/escritura
    FILE *archivoVentas = fopen("ventas.dat", "ab");//abre el archivo de ventas en modo append
    if (archivoProds == NULL || archivoVentas == NULL) {//control de errores
        printf("Error al abrir las bases de datos de venta.\n");
        if (archivoProds) fclose(archivoProds);
        if (archivoVentas) fclose(archivoVentas);
        return;
    }
    char cedula_buscar[15];
    printf("\n---- NUEVA VENTA (CAJA) ----\n");
    printf("Ingrese la cedula del cliente, o escriba '9999999999' para Consumidor Final: ");
    scanf("%14s", cedula_buscar);//pide la cedula del cliente o 9999999999 para realizar un consumidor final
    Cliente c;
    if (strcmp(cedula_buscar, "9999999999") == 0) {//si es consumidor final se asigna directamente
        strcpy(c.nombre_cliente, "Consumidor Final");
        strcpy(c.cedula, "9999999999");
    } else {
        c = buscarClientePorCedula(cedula_buscar);
        if (strcmp(c.nombre_cliente, "No Registrado") == 0) {//si no es consumidor final lo busca en clientes
            printf("El cliente no existe. Use Consumidor Final.\n");//si no lo encuentra se cancela la venta y se sugiere usar consumidor final
            fclose(archivoProds);
            fclose(archivoVentas);//se cierran los archivos
            return;
        }
    }
    printf("\n=============================\n");
    printf("        TICKET DE VENTA         \n");
    printf("Cliente: %s\n", c.nombre_cliente);
    printf("Cedula: %s\n", c.cedula);//datos del cliente en el ticket
    printf("----------------------------------\n");
    int mas_productos = 1;
    float subtotal_global = 0.0;
    while (mas_productos == 1) {//bucle para vender varios productos
        int id_buscar, cantidad_vender;
        int encontrado = 0;
        Producto p;
        printf("\nIngrese el ID del producto a vender: ");
        scanf("%d", &id_buscar);//pide el id del producto a vender
        rewind(archivoProds);//mueve el puntero al inicio del archivo de productos
        while (fread(&p, sizeof(Producto), 1, archivoProds) == 1) {//recorre el archivo de productos buscando el id
            if (p.id_producto == id_buscar) {
                encontrado = 1;//si lo encuentra muestra que producto es
                printf("Producto: %s | Precio: %.2f | Stock: %d\n", p.nombre_producto, p.precio, p.stock);
                if (p.stock == 0) {
                    printf("Error: Stock insuficiente.\n");//error por si no hay stock
                    break;
                }
                printf("Ingrese la cantidad a vender: ");
                scanf("%d", &cantidad_vender);//pide la cantidad a vender
                if (cantidad_vender <=0) {
                    printf("Error: La cantidad a vender debe ser mayor a cero.\n");
                    break;
                }
                if (cantidad_vender > p.stock) {//error si la cantidad a vender es mayor al stock
                    printf("Error: Stock insuficiente. Quedan %d unidades.\n", p.stock);
                    break;
                }
                float subtotal_item = p.precio * cantidad_vender;
                subtotal_global += subtotal_item;//calcula el subtotal y lo suma para el pago total
                printf("Agregado: %s x %d = $%.2f\n", p.nombre_producto, cantidad_vender, subtotal_item);//muestra el detalle del producto agregado
                p.stock -= cantidad_vender;//actualiza el stock
                fseek(archivoProds, -sizeof(Producto), SEEK_CUR);//retrocede para reescribir el producto leido
                fwrite(&p, sizeof(Producto), 1, archivoProds);
                Factura nuevaVenta;
                strcpy(nuevaVenta.id_factura, obtenerSiguienteIdFactura());
                snprintf(nuevaVenta.id_producto, sizeof(nuevaVenta.id_producto), "%d", p.id_producto);
                strcpy(nuevaVenta.cedula_cliente, c.cedula);
                strcpy(nuevaVenta.nombre_producto, p.nombre_producto);
                nuevaVenta.cantidad = cantidad_vender;                      //crea factura unica con ID global unico
                nuevaVenta.precio_unitario = p.precio;
                nuevaVenta.total_pagado = subtotal_item * 1.15; // incluye IVA 15%, para que coincida con lo realmente cobrado
                strcpy(nuevaVenta.usuario, "cajeroX");
                time_t t = time(NULL);//para guardar la fecha
                struct tm *tm_info = localtime(&t);
                strftime(nuevaVenta.fecha, 11, "%Y-%m-%d", tm_info);
                fwrite(&nuevaVenta, sizeof(Factura), 1, archivoVentas);
                break;
            }
        }
        if (!encontrado) {
            printf("Error: El ID ingresado no existe.\n");//error si el id ingresado no existe
        }
        printf("\nDesea escanear otro producto? (1 = Si, 0 = No): ");//si se quiere seguir vendiendo se repite el bucle del while
        scanf("%d", &mas_productos);
    }
    if (subtotal_global > 0) {
        float iva = subtotal_global * 0.15; //! IVA 15%
        float total = subtotal_global + iva;
        printf("\n---------------------------\n");
        printf("Subtotal: $%.2f\n", subtotal_global);
        printf("IVA (15%%): $%.2f\n", iva);             //calcula el subtotal de los productos, el iva y el valor final
        printf("Total a pagar: $%.2f\n", total);
        printf("------------------------------\n");
        if (strcmp(c.cedula, "9999999999") != 0) {//si no es consumidor final acumula puntos
            int puntos_ganados = (int)(total / 10.0);//1 punto por cada %10
            if (puntos_ganados > 0) {
                c.puntos_acumulados += puntos_ganados;
                actualizarCliente(c);
                printf("Ha acumulado %d puntos. Total actual: %d\n", puntos_ganados, c.puntos_acumulados);
            }
        }
        printf("Venta realizada con exito.\n");//mensaje de exito
    } else {
        printf("Venta cancelada o carrito vacio.\n");//mensaje de venta vacia
    }
    fclose(archivoProds);
    fclose(archivoVentas);//se cierran los archivos
}
//                                                                                                                                //
//          Funcion RECURSIVA para sumar el total de ventas          //
float sumarTotalVentasRecursivo(FILE *archivo) {
    Factura f;//variable local, distinta en cada llamada recursiva (cada nivel de la recursion tiene la suya)
    if (fread(&f, sizeof(Factura), 1, archivo) != 1) {//caso base: ya no hay mas registros que leer
        return 0.0;//no suma nada mas, aqui termina la recursion
    }
    return f.total_pagado + sumarTotalVentasRecursivo(archivo);//caso recursivo: suma esta factura + el resultado de sumar el resto del archivo
}
//                                                                                                                                //
//          Funcion para mostrar ventas realizadas          //
void mostrarVentas() {
    FILE *archivo = fopen("ventas.dat", "rb");//abre el archivo de ventas en modo lectura
    if (archivo == NULL) {//control de error
        printf("\nNo se han realizado ventas todavia hoy.\n");
        return;
    }
    Factura f;//variable "f" de tipo Factura
    printf("\n---- HISTORIAL DE VENTAS ----\n");
    printf("%-10s | %-12s\n", "Num Ticket", "Total Pagado");//alinear el texto en columnas
    printf("--------------------------------\n");
    while (fread(&f, sizeof(Factura), 1, archivo) == 1) {//recorre todas las facturas del archivo para imprimirlas
        printf("Ticket #%-4s | $%-11.2f\n", f.id_factura, f.total_pagado);//muestra el ID de la factura y el total pagado
    }
    rewind(archivo);//regresa el puntero al inicio del archivo para volver a recorrerlo con la funcion recursiva
    float gran_total = sumarTotalVentasRecursivo(archivo);//calcula el total, esta vez de forma recursiva en vez de con un acumulador iterativo
    printf("---------------------------\n");
    printf("GANANCIAS TOTALES DEL DIA: $%.2f\n", gran_total);//total acumulado de todo el dia
    fclose(archivo);//cierra el archivo
}
//                                                                                                                                //
void actualizarCliente(Cliente c) {
    FILE *archivo = fopen("clientes.dat", "rb+");
    if (archivo == NULL) {
        printf("Error: no se pudo abrir clientes.dat\n");
        return;
    }

    Cliente temp;
    while (fread(&temp, sizeof(Cliente), 1, archivo) == 1) {
        if (strcmp(temp.cedula, c.cedula) == 0) {
            fseek(archivo, -sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, archivo);
            printf("Cliente %s actualizado correctamente.\n", c.cedula);
            break;
        }
    }
    fclose(archivo);
}
void mostrarHistorialVentas() {
    FILE *archivo = fopen("ventas.dat", "rb");
    if (archivo == NULL) {
        printf("No hay ventas registradas.\n");
        return;
    }

    Factura f;
    printf("\n--- HISTORIAL COMPLETO DE VENTAS ---\n");
    printf("%-10s | %-15s | %-10s | %-10s\n", "ID", "Cliente", "Fecha", "Total");
    printf("-------------------------------------------------------------\n");

    while (fread(&f, sizeof(Factura), 1, archivo) == 1) {
        printf("%-10s | %-15s | %-10s | $%-9.2f\n",
               f.id_factura, f.cedula_cliente, f.fecha, f.total_pagado);
    }

    fclose(archivo);
}
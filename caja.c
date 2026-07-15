#include "caja.h"
#include "productos.h"
#include <stdio.h>
#include <stdlib.h>
//                                                                                                                                //
//          Estado inicial de la caja          //
static Caja miCaja = {0.0, 0.0, 0.0, 0};//solo se puede usar aqui, no se puede ver ni acceder a ella aunque se llame a caja.h
int esCajaAbierta() {
    return miCaja.caja_abierta;
}
//                                                                                                                                //
//          Funcion para abrir la caja          //
void abrirCaja() {
    if (miCaja.caja_abierta == 1){//verifica si la caja ya esta abierta para no abrirla de nuevo
        printf("Error: La caja ya se encuentra abierta para hoy.\n");//mensaje de error
        return;
    }
    printf("\n----- APERTURA DE CAJA -----\n");
    printf("Ingrese el monto inicial en efectivo para cambio: $");//monto con el que inicia la caja para dar vueltos
    scanf("%f", &miCaja.monto_apertura);
    if (miCaja.monto_apertura < 0) {//evitar valores negativos
        printf("Error: El monto inicial no puede ser negativo.\n");//mensaje de error
        miCaja.monto_apertura = 0.0;//establece el monto inicial de la caja en $0.0
        return; 
    }
    FILE *archivoVentas = fopen("ventas.dat", "rb");//abre el archivo de ventas del dia anterior
    if(archivoVentas != NULL) {//si existe el archivoVentas lo abre en modo append
        FILE *archivoHistorial = fopen("historial_ventas.dat", "ab");
        if(archivoHistorial != NULL) {
            FacturaResumen f;
            while(fread(&f, sizeof(FacturaResumen), 1, archivoVentas) == 1) {//copia todas las facturas al historialVentas
                fwrite(&f, sizeof(FacturaResumen), 1, archivoHistorial);
            }
            fclose(archivoHistorial);//cierra el historial de ventas
        }
        fclose(archivoVentas);//cierra el archivo de ventas
    }
    FILE *limpiarVentas = fopen("ventas.dat", "wb");//abre ventas.dat en modo de escritura para borrar todo
    if(limpiarVentas != NULL) {
        fclose(limpiarVentas);//cierra el archivo, asi cada dia empieza desde cero pero se guarda el registro de dias anteriores
    }
    miCaja.caja_abierta = 1;//abre la caja
    miCaja.monto_cierre_calculado = miCaja.monto_apertura;//inicia el monto cierre calculado igual al de apertura, mas adelante con las ventas se ajustara
    printf("Caja abierta con exito. Monto inicial: $%.2f\n", miCaja.monto_apertura);//mensaje de exito con el valor inicial ingresado
}
//                                                                                                                               //
//          Funcion para cerrar caja          //
void cerrarCaja() {
    if(miCaja.caja_abierta == 0) {//verifica si la caja ya esta cerrada para no cerrarla de nuevo
        printf("\nError: La caja ya esta cerrada o no ha sido abierta aun.\n");//mensaje de error
        return;
    }
    FILE *archivoVentas = fopen("ventas.dat", "rb");//abre el archivo de ventas en modo lectura
    float total_ventas_dia = 0.0;//ventas del dia = 0.0
    if(archivoVentas != NULL) {//verifica si archivoVentas existe
        FacturaResumen f;
        while(fread(&f, sizeof(FacturaResumen), 1, archivoVentas) == 1) {//recorre todas las facturas y suma el total pagado para la suma total
            total_ventas_dia += f.total_pagado;
        }
        fclose(archivoVentas);//cierra el archivo de ventas del dia
    } else {
        total_ventas_dia = 0.0;//si no hubo ventas en ese dia el archivoVentas no existe, por lo tanto el total de ventas es 0
    }
    miCaja.monto_cierre_calculado = miCaja.monto_apertura + total_ventas_dia;//suma las ventas del dia con el valor inicial con el que se abrio la caja
    printf("\n--- CIERRE DE CAJA ---\n");
    printf("Monto de apertura: $%.2f\n", miCaja.monto_apertura);
    if (total_ventas_dia < 0.0001f) { // si no hubo ventas
        printf("Total ventas registradas: $0.00\n");
    } else {
        printf("Total ventas registradas: $%.2f\n", total_ventas_dia);
    }
    if (miCaja.monto_cierre_calculado < 0.0001f) {
        printf("Monto esperado en caja: $0.00\n");
    } else {
        printf("Monto esperado en caja: $%.2f\n", miCaja.monto_cierre_calculado);
    }
    printf("-------------------------------\n");//muestra los valores que se esperan tener al final del dia
    printf("Ingrese el monto fisico real en efectivo: $");//pide al cajero ingresar el monto fisico que tiene
    scanf("%f", &miCaja.monto_cierre_fisico);
    printf("===============================\n");
    float tolerancia = 0.009f;//un valor de tolerancia para la suma de flotantes
    float diferencia = miCaja.monto_cierre_fisico - miCaja.monto_cierre_calculado;//calcula la diferencia entre el monto esperado y el monto fisico
    if (diferencia >= -tolerancia && diferencia <= tolerancia) {//si la diferencia esta dentro del rango de tolerancia la caja esta cuadrada
        printf("Caja cuadrada\n");
    } else if (diferencia > tolerancia) {//si esta demas indica cuanto dinero sobra
        printf("La caja no esta cuadrada. Sobran: $%.2f\n", diferencia);
    } else {//si falta indica cuanto dinero falta
        printf("La caja no esta cuadrada. Faltan: $%.2f\n", diferencia * -1);
    }
    miCaja.caja_abierta = 0;//marca la caja como cerrada
}
//                                                                                                                               //
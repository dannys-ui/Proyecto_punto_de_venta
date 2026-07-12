#include "caja.h"
#include "productos.h"
#include <stdio.h>
#include <stdlib.h>
static Caja miCaja = {0.0, 0.0, 0.0, 0};
int esCajaAbierta() {
    return miCaja.caja_abierta;
}
void abrirCaja() {
    if (miCaja.caja_abierta == 1){
        printf("Error: La caja ya se encuentra abierta para hoy.\n");
        return;
    }
    printf("\n----- APERTURA DE CAJA -----\n");
    printf("Ingrese el monto inicail en efectivo para cambio: $");
    scanf("%f", &miCaja.monto_apertura);
    if (miCaja.monto_apertura < 0) {
        printf("Error: El monto inicial no puede ser negativo.\n");
        miCaja.monto_apertura = 0.0;
        return;
    }
    miCaja.caja_abierta = 1;
    miCaja.monto_cierre_calculado = miCaja.monto_apertura;
    printf("Caja abierta con exito. Monto inicial: $%.2f\n", miCaja.monto_apertura);
}
void cerrarCaja() {
    if(miCaja.caja_abierta == 0) {
        printf("\nError: La caja ya esta cerrada o no ha sido abierta aun.\n");
        return;
    }
    FILE *archivoVentas = fopen("ventas.dat", "rb");
    float total_ventas_dia = 0.0;
    if(archivoVentas != NULL) {
        Factura f;
        while(fread(&f, sizeof(Factura), 1, archivoVentas) == 1) {
            total_ventas_dia += f.total_pagado;
        }
        fclose(archivoVentas);
    } else {
        total_ventas_dia = 0.0;
    }
    miCaja.monto_cierre_calculado = miCaja.monto_apertura + total_ventas_dia;
    printf("\n--- CIERRE DE CAJA ---\n");
    printf("Monto de apaertura: $%.2f\n", miCaja.monto_apertura);
    printf("Total ventas registradas: $%.2f\n", total_ventas_dia);
    printf("Monto esperado en caja: $%.2f\n", miCaja.monto_cierre_calculado);
    printf("-------------------------------\n");
    printf("Ingrese el monto fisico real en efectivo: $");
    scanf("%f", &miCaja.monto_cierre_fisico);
    printf("===============================\n");
    float diferencia = miCaja.monto_cierre_fisico - miCaja.monto_cierre_calculado;
    if (diferencia == 0.0) {
        printf("Caja cuadrada\n");
    } else if (diferencia < 0.0) {
        printf("La caja no esta cuadrada. Faltan: $%.2f\n", -diferencia);
    } else {
        printf("La caja no esta cuadrada. Sobran: $%.2f\n", diferencia);
    }
    miCaja.caja_abierta = 0;
}
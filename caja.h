#ifndef CAJA_H
#define CAJA_H
#include <stdio.h>
typedef struct {
    float monto_apertura;
    float monto_cierre_calculado;
    float monto_cierre_fisico;
    int caja_abierta;
} Caja;
void abrirCaja();
void cerrarCaja();
int esCajaAbierta();
#endif
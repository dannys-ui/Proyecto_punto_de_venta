#ifndef REPORTES_H
#define REPORTES_H
#include <stdio.h>
#include "productos.h"
void menuReportes();
void reporteStockBajo();
int calcularTotalUnidades(FILE *archivo); //* funcion recursiva
void reporteProductosPorPrecio();
#endif
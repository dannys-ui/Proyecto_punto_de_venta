//                                                                                                                                //
//          Modulo de reportes avanzados          //
#ifndef REPORTES_H //si no esta definido ejecuta las lineas de abajo
#define REPORTES_H //lo define y crea
#include "ventas.h"
#include "productos.h"
#include "clientes.h"
void mostrarVentasPorFecha(const char *fecha_buscar); //funcion para el reporte de ventas por fecha
void mostrarVentasPorRango(const char *fecha_inicio, const char *fecha_fin); //funcion para ver ventas por rango de fecha
void mostrarProductosMasVendidos(); //funcion para productos mas vendidos
void mostrarStockBajoMinimo(int minimo); //funcion para alertar de stock bajo
void mostrarRankingClientes(); //funcion para ranking de clientes
void menuReportesAvanzado(); //funcion para el menu de reportes
#endif
//                                                                                                                                //
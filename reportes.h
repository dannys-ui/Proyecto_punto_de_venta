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
int particionarProductos(Producto *arreglo, int inicio, int fin); //funcion de particion para el quicksort
void quicksortProductos(Producto *arreglo, int inicio, int fin); //funcion recursiva de ordenamiento quicksort
void mostrarInventarioOrdenado(); //funcion para ver el inventario ordenado alfabeticamente (usa quicksort y memoria dinamica)
void menuReportesAvanzado(); //funcion para el menu de reportes
#endif
//                                                                                                                                //
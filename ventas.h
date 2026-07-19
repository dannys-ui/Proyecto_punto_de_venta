//                                                                                                                                //
//          Modulo de ventas          //
#ifndef VENTAS_H//si no esta definido ejecuta las lineas que sigue
#define VENTAS_H//define y lo crea
#include <stdio.h>
#include "productos.h"
#include "clientes.h"
#include "usuarios.h"
typedef struct {//estructura de datos
    char id_factura[10];//id unico para cada factura
    char cedula_cliente[15];//cedula del cliente a quien sale la factura
    char id_producto[10];//id del producto vendido
    char nombre_producto[50];//nombre del producto vendido
    int cantidad;//cantidad del producto vendido
    float precio_unitario;//precio unitario del producto vendido
    float total_pagado;//total pagado que saldra en la factura
    char usuario[30];//nombre del cajero o usuario que realizo la venta
    char fecha[11];//fecha en formato "YYYY/MM/DD"
} Factura;//nombre de la estructura
void realizarVenta();//funcion para registrar la venta
void mostrarVentas();//funcion para mostrar las ventas
Factura buscarVentaPorId(int id_factura);//buscar la venta
char* obtenerSiguienteIdFactura();
void actualizarCliente(Cliente c);
void mostrarHistorialVentas();
float sumarTotalVentasRecursivo(FILE *archivo);//funcion recursiva: suma el total de ventas leyendo el archivo registro por registro
#endif // VENTAS_H
//                                                                                                                                //
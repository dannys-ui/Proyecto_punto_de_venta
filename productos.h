#ifndef PRODUCTOS_H
#define PRODUCTOS_H
typedef struct {
    int id_producto;
    char nombre_producto[50];
    float precio;
    int stock;
} Producto;
typedef struct {
    int id_producto;
    char nombre_producto[50];
    int cantidad;
    float precio_unitario;
    float subtotal;
} DetalleVenta;
typedef struct {
    int id_venta;
    float total_pagado;
} Factura;
void crearInventarioSemilla();
void mostrarProductos();
int existeProducto(int id);
void registrarProducto();
void actualizaProducto();
void eliminarProducto();
void realizarVenta();
void mostrarVentasRealizadas();
#endif // PRODUCTOS_H
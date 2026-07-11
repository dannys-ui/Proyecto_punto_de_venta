#ifndef PRODUCTOS_H
#define PRODUCTOS_H
typedef struct {
    int id_producto;
    char nombre_producto[50];
    float precio;
    int stock;
} Producto;

void crearInventarioSemilla();
void mostrarProductos();
void registrarProducto();
void actualizaProducto();
void eliminarProducto();
#endif // PRODUCTOS_H
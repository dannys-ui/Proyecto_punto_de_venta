//                                                                                                                            //
//          Modulo de productos          //
#ifndef PRODUCTOS_H//si no esta definido ejecuta las siguientes lineas
#define PRODUCTOS_H//lo define y lo crea
typedef struct {//estructura de datos
    int id_producto;//entero para registrar el id de un producto
    char nombre_producto[50];//arreglo para registrar el nombre de un producto
    float precio;//flotante para registrar el precio de un producto
    int stock;//entero para registrar el stock de un producto
} Producto;//nombre de la estructura
void crearInventarioSemilla();//funcion para crear la lista inicial 
void mostrarProductos();//funcion para mostrar la lista de productos
int existeProducto(int id);
void registrarProducto();//funcion para ingresar nuevos productos
void actualizaProducto();//funcion para actualizar productos existentes
void eliminarProducto();//funcion para eliminar un producto
#endif // PRODUCTOS_H
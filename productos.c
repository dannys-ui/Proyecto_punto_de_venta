#include <stdio.h>
#include <string.h>
#include "clientes.h"
#include "productos.h"
//                                                                                                                               //
//          Funcion para crear el archivo de productos inicial          //
void crearInventarioSemilla() {
    FILE *archivo = fopen("productos.dat", "wb");//crea el archivo de productos en binario, si ya existia lo sobreescribe
    if (archivo == NULL) {//control de error
        printf("Error: No se pudo crear el archivo de productos.\n");
        return;
    }
    Producto inventario_inicial[30] = {//arreglo de 30 productos del tipo Producto, id - nombre - precio unitario - stock
        {101, "Aceite vegetal", 2.50, 100}, {102, "Arroz", 1.10, 200},
        {103, "Azucar", 1.20, 150},        {104, "Cafe molido", 2.50, 80},
        {105, "Carne de res", 6.00, 50},    {106, "Carne de pollo", 4.00, 70},
        {107, "Fideos", 1.60, 120},         {108, "Harina de trigo", 1.40, 90},
        {109, "Leche", 1.10, 200},          {110, "Mantequilla", 2.00, 60},
        {111, "Pan blanco", 1.50, 100},     {112, "Sal", 0.80, 300},
        {113, "Salsa de tomate", 1.50, 110},{114, "Queso", 3.00, 70},
        {115, "Yogurt", 1.20, 90},          {116, "Jugo de naranja", 2.00, 80},
        {117, "Galletas", 1.50, 150},       {118, "Cereal", 3.50, 60},
        {119, "Mostaza", 1.50, 90},         {120, "Mayonesa", 2.00, 80},
        {121, "Agua mineral", 0.90, 300},   {122, "Cerveza", 2.00, 100},
        {123, "Vino tinto", 5.00, 40},       {124, "Pasta dental", 1.50, 100},
        {125, "Jabon de tocador", 1.20, 150},{126, "Shampoo", 3.00, 80},
        {127, "Detergente", 2.50, 90},      {128, "Papel higienico", 0.80, 200},
        {129, "Toallas sanitarias", 2.00, 70},{130, "Pañales", 5.00, 60}
    };
    fwrite(inventario_inicial, sizeof(Producto), 30, archivo);//escribe los 30 productos en el archivo binario de productos
    fclose(archivo);//cierra el archivo
    printf("Archivo semilla 'productos.dat' creado exitosamente.\n");//mensaje de exito
}
//                                                                                                                              //
//          Funcion para mostrar la lista de productos          //
void mostrarProductos() {
    FILE *archivo = fopen("productos.dat", "rb");//abre el archivo binario productos en modo lectura
    if (archivo == NULL) {//control de errores
        printf("Error: No se pudo abrir el inventario de productos.\n");
        return; 
    }
    Producto p;//variable "p" de tipo Producto
    printf("\n----- INVENTARIO DE PRODUCTOS -----\n");
    printf("%-6s | %-25s | %-7s | %-5s\n", "ID", "Nombre del Producto", "Precio", "Stock");//alinear texto en columnas
    printf("-------------------------------------------------------------\n");
    while (fread(&p, sizeof(Producto), 1, archivo) == 1) {//lee los 30 productos y se detiene cuando devuelve 0
        printf("%-6d | %-25s | $%-6.2f | %-5d\n", p.id_producto, p.nombre_producto, p.precio, p.stock);//imprime cada parte de la estructura en forma de tabla
    }
    printf("-------------------------------------------------------------\n");
    fclose(archivo);//cierra el archivo
}
//                                                                                                                                //
//          Funcion para verificar la existencia de un producto          //
int existeProducto(int id) {
    FILE *archivo = fopen("productos.dat", "rb");//abre el archivo de productos en modo lectura
    if (archivo == NULL) return 0;//control de error
    Producto p;//variable p del tipo Producto para leer cada producto
    while (fread(&p, sizeof(Producto), 1, archivo) == 1) {//le un producto del archivo en cada iteracion 
        if (p.id_producto == id) {//si el id ingresado coincide con un producto cierra el archivo y devuelve 1
            fclose(archivo);
            return 1;//el prodcuto existe
        }
    }
    fclose(archivo);//cierra el archivo
    return 0;//el producto no existe
}
//                                                                                                                                //
//          Funcion para registrar un nuevo producto          //
void registrarProducto() {
    Producto nuevo;//variable "nuevo" de tipo Producto
    printf("\n----REGISTRO DE NUEVO PRODUCTO----\n");
    printf("Ingrese el ID del producto: ");//solicita el id para el nuevo producto
    scanf("%d", &nuevo.id_producto);
    if (existeProducto(nuevo.id_producto)) {
        printf("Error: Ya existe un producto con el ID %d.\n", nuevo.id_producto);//si el id ya existe mensaje de advertencia y termina la funcion
        return;
    }
    int c;
    while((c = getchar()) != '\n' && c != EOF);//limpieza del buffer
    printf("Ingrese el nombre del producto: ");
    fgets(nuevo.nombre_producto, sizeof(nuevo.nombre_producto), stdin);//leer el nombre del producto con espacios
    nuevo.nombre_producto[strcspn(nuevo.nombre_producto, "\n")] = 0;//elimina el salto de linea
    printf("Ingrese el precio del producto: ");//ingreso del precio y el stock inicial
    scanf("%f", &nuevo.precio);
    printf("Ingrese el Stock inicial: ");
    scanf("%d", &nuevo.stock);
    if (nuevo.precio < 0 || nuevo.stock < 0) {//evitar precios o stock negativo
    printf("Error: Precio y stock deben ser valores positivos.\n");
    return;
    }
    FILE *archivo = fopen("productos.dat", "ab");//abre el archivo productos en modo append binario para escribir en la ultima linea sin borrar el resto
    if (archivo == NULL) {//control de error
        printf("Error al abrir el archivo de productos.\n");
        return;
    }
    fwrite(&nuevo, sizeof(Producto), 1, archivo);//escribe el nuevo producto al final del archivo
    fclose(archivo);//cierra el archivo
    printf("Producto registrado exitosamente.\n");//mensaje de exito
}
//                                                                                                                                //
//          Funcion para actualizar un producto existente          //
void actualizaProducto() {
    FILE *archivo = fopen("productos.dat", "rb+");//"rb+" para leer y modificar directamente los archivos sin tener que crear un archivo temporal
    if (archivo == NULL) {//control de error
        printf("Error al abrir el archivo de productos.\n");
        return;
    }
    int id_buscar;
    int encontrado = 0;//bandera para saber si encontro el producto
    Producto p;//variable "p" de tipo Producto
    printf("\n--- MODIFICAR PRODUCTO ---\n");
    printf("Ingrese el ID del producto a editar: ");
    scanf("%d", &id_buscar);
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Limpiar buffer
    while (fread(&p, sizeof(Producto), 1, archivo) == 1) {//lee el archivo producto por producto
        if (p.id_producto == id_buscar) {
            encontrado = 1;//si encuentra el id buscado marca la bandera encontrado como 1
            printf("Producto encontrado: %s\n", p.nombre_producto);         
            printf("Nuevo nombre (presione Enter para mantener '%s'): ", p.nombre_producto);//nuevo nombre, si no quiere cambiar el nombre solo se presiona enter y se mantiene
            char nuevo_nombre[50];
            fgets(nuevo_nombre, sizeof(nuevo_nombre), stdin);
            nuevo_nombre[strcspn(nuevo_nombre, "\n")] = 0;
            
            if (strlen(nuevo_nombre) > 0) {
                strcpy(p.nombre_producto, nuevo_nombre);
            }
            printf("Nuevo precio (actual: %.2f / ingrese -1 para mantener): ", p.precio);//permite ingresar el nuevo precio, si se lo quiere mantener se ingresa -1
            float nuevo_precio;
            scanf("%f", &nuevo_precio);
            if (nuevo_precio >= 0) {
                p.precio = nuevo_precio;
            }
            printf("Nuevo stock (actual: %d / ingrese -1 para mantener): ", p.stock);//permite ingresar el nuevo stock
            int nuevo_stock;
            scanf("%d", &nuevo_stock);
            if (nuevo_stock >= 0) {
                p.stock = nuevo_stock;
            }
            fseek(archivo, -sizeof(Producto), SEEK_CUR);//al leer una linea apunta al final de esta, con SEEK_CUR le dice "a partir de donde estas muevete a...", para volver a apuntar al inicio de la linea 
            fwrite(&p, sizeof(Producto), 1, archivo);//como esta apuntando al inicio de la linea sobreescribe lo que ya estaba ahi
            break;
        }        
    }
    fclose(archivo);//cierra el archivo
    if (encontrado) {
        printf("Producto actualizado exitosamente.\n");//mensaje de exito si se actualizo un producto
    } else {
        printf("Producto con ID %d no encontrado.\n", id_buscar);//mensaje de error si no encontro el producto  
    }
}
//                                                                                                                                //
//          Funcion para eliminar productos          //
void eliminarProducto() {
    FILE *archivo = fopen("productos.dat", "rb");//abre el archivo productos en modo lectura
    FILE *temporal = fopen("temporal.dat", "wb");//crea un archivo temporal en modo de escritura
    if (archivo == NULL || temporal == NULL) {//control de error
        printf("Error al abrir los archivos para eliminación.\n");
        return;
    }
    int id_eliminar;//int que se busca eliminar
    int encontrado = 0;//bandera para saber si hallo el producto
    Producto p;//variable p de tipo Producto
    printf("\n--- ELIMINAR PRODUCTO ---\n");
    printf("Ingrese el ID del producto a eliminar: ");
    scanf("%d", &id_eliminar);//pide al usuario el id del producto que quiere eliminar
        while(fread(&p, sizeof(Producto), 1, archivo) == 1) {//lee todo el archivo producto por producto hasta el final
        if (p.id_producto == id_eliminar) {
            encontrado = 1;//si lo encuentra lo marca y no se copia en el temporal
            printf("Producto '%s' eliminado.\n", p.nombre_producto);
        } else {
            fwrite(&p, sizeof(Producto), 1, temporal);
        }
    }   
    fclose(archivo);
    fclose(temporal);//cierra los archivos   
    remove("productos.dat");//elimina el archivo productos donde aun esta el producto que se busco para eliminar
    rename("temporal.dat", "productos.dat");//el archivo temporal ahora se llama como el original y lo reemplaza, en este archivo no se copio el producto que se queria eliminar
    if(encontrado) {
        printf("Producto eliminado del sistema con éxito.\n");//si se encontro el producto muestra el mensaje de exito que se borro
    } else {
        printf("Producto con ID %d no encontrado.\n", id_eliminar);//si no se encontro el producto muestra mensaje de error
    }
}
//                                                                                                                               //
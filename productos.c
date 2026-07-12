#include <stdio.h>
#include <string.h>
#include "clientes.h"
#include "productos.h"

void crearInventarioSemilla() {
    FILE *archivo = fopen("productos.dat", "wb");
    if (archivo == NULL) {
        printf("Error: No se pudo crear el archivo de productos.\n");
        return;
    }
    Producto inventario_inicial[30] = {
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
    fwrite(inventario_inicial, sizeof(Producto), 30, archivo);
    fclose(archivo);
    printf("Archivo semilla 'productos.dat' creado exitosamente.\n");
}
void mostrarProductos() {
    FILE *archivo = fopen("productos.dat", "rb");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el inventario de productos.\n");
        return; 
    }
    Producto p;
    printf("\n----- INVENTARIO DE PRODUCTOS -----\n");
    printf("%-6s | %-25s | %-7s | %-5s\n", "ID", "Nombre del Producto", "Precio", "Stock");
    printf("-------------------------------------------------------------\n");
    while (fread(&p, sizeof(Producto), 1, archivo) == 1) {
        printf("%-6d | %-25s | $%-6.2f | %-5d\n", p.id_producto, p.nombre_producto, p.precio, p.stock);
    }
    printf("-------------------------------------------------------------\n");
    fclose(archivo);
}
int existeProducto(int id) {
    FILE *archivo = fopen("productos.dat", "rb");
    if (archivo == NULL) return 0;
    Producto p;
    while (fread(&p, sizeof(Producto), 1, archivo) == 1) {
        if (p.id_producto == id) {
            fclose(archivo);
            return 1;
        }
    }
    fclose(archivo);
    return 0;
}
void registrarProducto() {
    Producto nuevo;
    printf("\n----REGISTRO DE NUEVO PRODUCTO----\n");
    printf("Ingrese el ID del producto: ");
    scanf("%d", &nuevo.id_producto);
    if (existeProducto(nuevo.id_producto)) {
        printf("Error: Ya existe un producto con el ID %d.\n", nuevo.id_producto);
        return;
    }
    int c;
    while((c = getchar()) != '\n' && c != EOF);
    printf("Ingrese el nombre del producto: ");
    fgets(nuevo.nombre_producto, sizeof(nuevo.nombre_producto), stdin);
    nuevo.nombre_producto[strcspn(nuevo.nombre_producto, "\n")] = 0;
    printf("Ingrese el precio del producto: ");
    scanf("%f", &nuevo.precio);
    printf("Ingrese el Stock inicial: ");
    scanf("%d", &nuevo.stock);
    FILE *archivo = fopen("productos.dat", "ab");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de productos.\n");
        return;
    }
    fwrite(&nuevo, sizeof(Producto), 1, archivo);
    fclose(archivo);
    printf("Producto registrado exitosamente.\n");
}
void actualizaProducto() {
    // Abrimos en modo lectura/escritura binaria con "rb+" para modificar in situ
    FILE *archivo = fopen("productos.dat", "rb+");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de productos.\n");
        return;
    }
    int id_buscar;
    int encontrado = 0;
    Producto p;
    printf("\n--- MODIFICAR PRODUCTO ---\n");
    printf("Ingrese el ID del producto a editar: ");
    scanf("%d", &id_buscar);
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Limpiar buffer
    while (fread(&p, sizeof(Producto), 1, archivo) == 1) {
        if (p.id_producto == id_buscar) {
            encontrado = 1;
            printf("Producto encontrado: %s\n", p.nombre_producto);         
            printf("Nuevo nombre (presione Enter para mantener '%s'): ", p.nombre_producto);
            char nuevo_nombre[50];
            fgets(nuevo_nombre, sizeof(nuevo_nombre), stdin);
            nuevo_nombre[strcspn(nuevo_nombre, "\n")] = 0;
            
            if (strlen(nuevo_nombre) > 0) {
                strcpy(p.nombre_producto, nuevo_nombre);
            }
            printf("Nuevo precio (actual: %.2f / presione -1 para mantener): ", p.precio);
            float nuevo_precio;
            scanf("%f", &nuevo_precio);
            if (nuevo_precio >= 0) {
                p.precio = nuevo_precio;
            }
            fseek(archivo, -sizeof(Producto), SEEK_CUR);
            fwrite(&p, sizeof(Producto), 1, archivo);
            break;
        }
    }
    fclose(archivo);
    if (encontrado) {
        printf("Producto actualizado exitosamente.\n");
    } else {
        printf("Producto con ID %d no encontrado.\n", id_buscar);
    }
}
void eliminarProducto() {
    FILE *archivo = fopen("productos.dat", "rb");
    FILE *temporal = fopen("temporal.dat", "wb");
    if (archivo == NULL || temporal == NULL) {
        printf("Error al abrir los archivos para eliminación.\n");
        return;
    }
    int id_eliminar;
    int encontrado = 0;
    Producto p;
    printf("\n--- ELIMINAR PRODUCTO ---\n");
    printf("Ingrese el ID del producto a eliminar: ");
    scanf("%d", &id_eliminar);
        while(fread(&p, sizeof(Producto), 1, archivo) == 1) {
        if (p.id_producto == id_eliminar) {
            encontrado = 1;
            printf("Producto '%s' eliminado del flujo.\n", p.nombre_producto);
        } else {
            fwrite(&p, sizeof(Producto), 1, temporal);
        }
    }   
    fclose(archivo);
    fclose(temporal);   
    remove("productos.dat");
    rename("temporal.dat", "productos.dat");
    
    if(encontrado) {
        printf("Producto eliminado del sistema con éxito.\n");
    } else {
        printf("Producto con ID %d no encontrado.\n", id_eliminar);
    }
}
void realizarVenta() {
    FILE *archivoProds = fopen("productos.dat", "rb+");
    FILE *archivoVentas = fopen("ventas.dat", "ab");
    if (archivoProds == NULL || archivoVentas == NULL) {
        printf("Error al abrir las bases de datos de venta.\n");
        if(archivoProds) fclose(archivoProds);
        if(archivoVentas) fclose(archivoVentas);
        return;
    }
    char cedula_buscar[15];
    printf("\n---- NUEVA VENTA (CAJA) ----\n");
    printf("Ingrese la cedula del cliente, o escriba '9999999999' para Consumidor Final: ");
    scanf("%s", cedula_buscar);
    Cliente c;
    if (strcmp(cedula_buscar, "9999999999") == 0) {
        strcpy(c.nombre_cliente, "Consumidor Final");
        strcpy(c.cedula, "9999999999");
    } else {
        c = buscarClientePorCedula(cedula_buscar);
        if (strcmp(c.nombre_cliente, "No registrado") == 0) {
            printf("El cliente no existe. Use consumidor Final.\n");
            fclose(archivoProds);
            fclose(archivoVentas);
            return;
        }
    }
    printf("\n=============================\n");
    printf("        TICKET DE VENTA         \n");
    printf("Cliente: %s\n", c.nombre_cliente);
    printf("Cedula: %s\n", c.cedula);
    printf("----------------------------------\n");
    int mas_productos = 1;
    float subtotal_global = 0.0;
    while (mas_productos == 1) {
        int id_buscar, cantidad_vender;
        int encontrado = 0;
        Producto p;
        printf("\n Ingrese el ID del producto a vender: ");
        scanf("%d", &id_buscar);
        rewind(archivoProds); //volver a leer el archivo 
        while (fread(&p, sizeof(Producto), 1, archivoProds) == 1) {
            if (p.id_producto == id_buscar) {
                encontrado = 1;
                printf("Producto: %s | Precio: %.2f | Stock: %d\n", p.nombre_producto, p.precio, p.stock);
                if(p.stock == 0) {
                    printf("Error: Stock insuficiente.\n");
                    break;
                }
                printf("Ingrese la cantidad a vender: ");
                scanf("%d", &cantidad_vender);
                if (cantidad_vender > p.stock) {
                    printf("Error: Stock insuficiente. Quedan %d unidades.\n", p.stock);
                    break;
                }
                float subtotal_item = p.precio * cantidad_vender;
                subtotal_global += subtotal_item;
                printf("Agregado: %s x %d = $%.2f\n", p.nombre_producto, cantidad_vender, subtotal_item);
                p.stock -= cantidad_vender;
                fseek (archivoProds, -sizeof(Producto), SEEK_CUR);
                fwrite(&p, sizeof(Producto), 1, archivoProds);
                break;
            }
        }
        if (!encontrado) {
            printf("Error: El ID ingresado no existe.\n");
        }
        printf("\nDesea escanear otro producto? (1 = Si, 0 = No): ");
        scanf("%d", &mas_productos);
    }
        if (subtotal_global > 0) {
            float iva = subtotal_global * 0.15; //! IVA 15%
            float total = subtotal_global + iva;
            printf("\n---------------------------\n");
            printf("Subtotal: $%.2f\n", subtotal_global);
            printf("IVA (15%%): $%.2f\n", iva);
            printf("Total a pagar: $%.2f\n", total);
            printf("------------------------------\n");
            fseek(archivoVentas, 0, SEEK_END); 
            int num_ventas = ftell(archivoVentas) / sizeof(Factura);
            Factura nuevaVenta;
            nuevaVenta.id_venta = num_ventas + 1;
            nuevaVenta.total_pagado = total;
            fwrite(&nuevaVenta, sizeof(Factura), 1, archivoVentas);
            printf("Venta realizada con exito.\n");
        } else {
            printf("Venta cancelada o carrito vacio.\n");
        }
        fclose(archivoProds);
        fclose(archivoVentas);
}
void mostrarVentasRealizadas() {
    FILE *archivo = fopen("ventas.dat", "rb");
    if (archivo == NULL) {
        printf("\nNo se han realizado ventas todavia hoy.\n");
        return;
    }
    Factura f;
    float gran_total = 0;
    printf("\n---- HISTORIAL DE VENTAS ----\n");
    printf("%-10s | %-12s\n", "Num Ticket", "Total Pagado");
    printf("--------------------------------\n");
    while (fread(&f, sizeof(Factura), 1, archivo) == 1) {
        printf("Ticket #%-4d | $%-11.2f\n", f.id_venta, f.total_pagado);
        gran_total += f.total_pagado;
    }
    printf("---------------------------\n");
    printf("GANANCIAS TOTALES DEL DIA: $%.2f\n", gran_total);
    fclose(archivo);
}
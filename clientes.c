//                                                                                                                           //
#include <stdio.h>
#include <string.h>
#include "clientes.h"
//          Funcion para crear el archivo binario de clientes          //
void registrarCliente() {
    FILE *archivo = fopen("clientes.dat", "ab"); //crea el archibo binario en modo "ab" (append binary), escribe en la ultima linea sin borrar lo anterior
    if (archivo == NULL) {//control de segurida para errores
        printf("Error: No se puede abrir la base de datos de clientes.\n");
        return;
    }
    Cliente nuevo;//nuevo es un cliente individual con sus tres campos para guardar alli los datos
    printf("\n----REGISTRO DE NUEVO CLIENTE----\n");
    printf("Ingrese la cedula del cliente (10 digitos): ");
    scanf("%14s", nuevo.cedula);//lee la cedula y lo gaurda en el campo "cedula" de la estructura nuevo
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // limpiar el buffer de entrada para que el salto de linea interfiera con el fgets
    printf("Ingrese el nombre completo del cliente: ");
    fgets(nuevo.nombre_cliente, sizeof(nuevo.nombre_cliente), stdin);//lee la linea entera incluyendo los espacios y los guarda en el campo "nombre_cliente"
    nuevo.nombre_cliente[strcspn(nuevo.nombre_cliente, "\n")] = 0; // eliminar el salto de línea al final
    nuevo.puntos_acumulados = 0;//cada usuario nuevo empieza con 0 puntos
    fwrite(&nuevo, sizeof(Cliente), 1, archivo); //escribe el nuevo cliente en el archivo
    fclose(archivo);//cierra el archivo
    printf("Cliente registrado exitosamente.\n");//mensaje de exito
}
//                                                                                                                               //
void eliminarCliente() {
    FILE *archivo = fopen("clientes.dat", "rb");//abrir el archivo clientes en modo lectura
    FILE *temporal = fopen("temporal.dat", "wb");//crea y abre un archivo temporal en modo excritura
    if (archivo == NULL || temporal == NULL) {//control de errores
        printf("Error al abrir los archivos para eliminación.\n");
        return;
    }
    char cedula_eliminar[15];
    int encontrado = 0;//bandera para saber si encontro el cliente
    Cliente c;
    printf("\n--- ELIMINAR CLIENTE ---\n");
    printf("Ingrese la cédula del cliente a eliminar: ");
    scanf("%s", cedula_eliminar);//pide al usuario la cedula que desea buscar para eliminar
    while (fread(&c, sizeof(Cliente), 1, archivo) == 1) {//lee cada elemento del archivo
        if (strcmp(c.cedula, cedula_eliminar) == 0) {//compara si alguna cedula del archivo coincide con la cedula ingresada para eliminar
            encontrado = 1;//cambia el valor de la bandera
            printf("Cliente '%s' eliminado.\n", c.nombre_cliente);
        } else {
            fwrite(&c, sizeof(Cliente), 1, temporal);//si no coincide ninguna cedula todas se copian a temporal
        }
    }
    fclose(archivo);
    fclose(temporal);//cierra los archivos
    remove("clientes.dat");//elimina el archivo clientes
    rename("temporal.dat", "clientes.dat");//renombra temporal como el nuevo archivo de clientes, aqui se copio todos los clientes excepto el que se queria eliminar
    if (encontrado) {
        printf("Cliente eliminado del sistema con éxito.\n");//mensaje de exito si se encontro y elimino al cliente
    } else {
        printf("Cliente con cédula %s no encontrado.\n", cedula_eliminar);//mensaje de error si el cliente no se encontro
    }
}
//                                                                                                                              //
//          Funcion para buscar un cliente en el archivo binario          //
    Cliente buscarClientePorCedula(const char* id_buscar) {
        FILE *archivo = fopen("clientes.dat", "rb"); //abre el archivo binario en modo "rb" para leer en modo binario
        Cliente temporal;//para leer cada cliente del archivo
        Cliente no_encontrado;//si no encuentra el cliente se devuelve esto
        strcpy(no_encontrado.cedula, ""); // inicializa con cadena vacía 
        strcpy(no_encontrado.nombre_cliente, "No Registrado"); 
        no_encontrado.puntos_acumulados = 0;
        if (archivo == NULL) { //si no se abre el archivo devuelve no encontrado
            return no_encontrado;
        }
        while (fread(&temporal, sizeof(Cliente), 1, archivo) == 1) {//se lee cliente por cliente
            if (strcmp(temporal.cedula, id_buscar) == 0) {//cada cliente leido se guarda en "temporal", se compara cada temporal.cedula con id_buscar, si coincide se cierra el archivo y devuelve temporal
                fclose(archivo);
                return temporal;
            }
        }
        fclose(archivo);//si no encuentra coincidencias se cierra el archivo
        return no_encontrado;//retorna no_encontrado 
    }
//                                                                                                                               //
//          Funcion para mostrar los clientes          //
void mostrarClientes() {
    FILE *archivo = fopen("clientes.dat", "rb");//abre el archivo en modo lectura
    if (archivo == NULL) {//control de errores
        printf("\nNo hay clientes registrados.\n");
        return;
    }
    Cliente c;//variable c de tipo cliente para ir leyendo de uno en uno
    printf("\n----LISTA DE CLIENTES REGISTRADOS----\n");
    while (fread(&c, sizeof(Cliente), 1, archivo) == 1) {//fread para leer un cliente completo del archivo y guardarlo en c
        printf("Cedula: %s\n", c.cedula);//mientras se puede leer devuelve 1 e imprime los datos del cliente
        printf("Nombre: %s\n", c.nombre_cliente);
        printf("Puntos Acumulados: %d\n", c.puntos_acumulados);
    }
    printf("-----------------------------\n");
    fclose(archivo);//cierra el archivo
}
//                                                                                                                                 //
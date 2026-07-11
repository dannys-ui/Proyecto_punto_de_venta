#include <stdio.h>
#include <string.h>
#include "clientes.h"
void registrarCliente() {
    FILE *archivo = fopen("clientes.dat", "ab"); //ab para agregar sin borrar el contenido anterior
    if (archivo == NULL) {
        printf("Error: No se puede abrir la base de datos de clientes.\n");
        return;
    }
    Cliente nuevo;
    printf("\n----REGISTRO DE NUEVO CLIENTE----\n");
    printf("Ingrese la cedula del cliente (10 digitos): ");
    scanf("%14s", nuevo.cedula);
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // limpiar el buffer de entrada   
    printf("Ingrese el nombre completo del cliente: ");
    fgets(nuevo.nombre_cliente, sizeof(nuevo.nombre_cliente), stdin);
    nuevo.nombre_cliente[strcspn(nuevo.nombre_cliente, "\n")] = 0; // eliminar el salto de línea al final
    nuevo.puntos_acumulados = 0;
    fwrite(&nuevo, sizeof(Cliente), 1, archivo); //escribe el nuevo cliente en el archivo
    fclose(archivo);
    printf("Cliente registrado exitosamente.\n");
}
    Cliente buscarClientePorCedula(const char* id_buscar) {
        FILE *archivo = fopen("clientes.dat", "rb"); //rb para leer en modo binario
        Cliente temporal;
        Cliente no_encontrado;
        strcpy(no_encontrado.cedula, ""); // inicializa con cadena vacía
        strcpy(no_encontrado.nombre_cliente, "No Registrado"); 
        no_encontrado.puntos_acumulados = 0;
        if (archivo == NULL) {
            return no_encontrado;
        }
        while (fread(&temporal, sizeof(Cliente), 1, archivo) == 1) {
            if (strcmp(temporal.cedula, id_buscar) == 0) {
                fclose(archivo);
                return temporal;
            }
        }
        fclose(archivo);
        return no_encontrado;
    }
    void mostrarClientes() {
        FILE *archivo = fopen("clientes.dat", "rb");
        if (archivo == NULL) {
            printf("\nNo hay clientes registrados.\n");
            return;
        }
        Cliente c;
        printf("\n----LISTA DE CLIENTES REGISTRADOS----\n");
        while (fread(&c, sizeof(Cliente), 1, archivo) == 1) {
            printf("Cedula: %s\n", c.cedula);
            printf("Nombre: %s\n", c.nombre_cliente);
            printf("Puntos Acumulados: %d\n", c.puntos_acumulados);
        }
        printf("-----------------------------\n");
        fclose(archivo);
    }
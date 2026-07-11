//!archivo para pruebas, todo se ira al main despues
#include <stdio.h>
#include <string.h>
#include "usuarios.h"
#include "clientes.h"

int main() {
    registrarCliente(); // Llamada a la función para registrar un cliente
    mostrarClientes(); // Llamada a la función para mostrar los clientes registrados
    printf("\nPrueba de clientes\n");
    char id_para_venta[11]="1750725531";
    Cliente cliente_actual = buscarClientePorCedula(id_para_venta);
    if (strcmp(cliente_actual.cedula, "") !=0) {
        printf("Venta vinculada al cliente: %s, Tiene %d puntos.\n", cliente_actual.nombre_cliente, cliente_actual.puntos_acumulados);
    } else {
        printf("El cliente con cedula %s no existe. Se procedera como Consumidor Final.\n", id_para_venta);
    }
    return 0;
}
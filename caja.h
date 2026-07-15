//                                                                                                                                //
//          Modulo de caja          //
#ifndef CAJA_H
#define CAJA_H
#include <stdio.h>
typedef struct {//estructura de datos
    float monto_apertura;//flotante para el valor inicial de la caja para cambio
    float monto_cierre_calculado;//flotante que mostrara la caja al final del dia
    float monto_cierre_fisico;//flotante para que cajero ingreso el valor fisico que tiene
    int caja_abierta;//comprobador de si la caja esta abierta o no
} Caja;//nombre de la estructura
void abrirCaja();//funcion para abrir o iniciar caja
void cerrarCaja();//funcion para cerrar o terminar caja
int esCajaAbierta();//funcion para verificar si la caja esta abierta
#endif
//                                                                                                                                //
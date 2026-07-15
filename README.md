# Sistema de Punto de Venta (POS) con Control de Acceso por Roles

Un sistema modular de gestión de ventas, facturación, control de inventarios y reportes avanzados desarrollado en **C** para entornos de terminal. El software gestiona diferentes niveles de permisos de usuario a través de una arquitectura basada en archivos binarios (`.dat`), controlando el ingreso mediante identificadores únicos (UID) asignados al personal[cite: 2].

---

## Características Principales

*   **Control de Acceso por UID:** Acceso restringido al sistema mediante el ingreso manual en consola de un identificador único (UID) asignado a cada empleado[cite: 2].
*   **Roles de Usuario Diferenciados:**
    *   **Administrador:** Acceso total al sistema, eliminación de usuarios, reportes avanzados de auditoría y ventas[cite: 2].
    *   **Cajero:** Funciones de venta diaria, apertura/cierre de caja, facturación e historial local[cite: 2].
    *   **Bodeguero:** Gestión de stock, reabastecimiento y alertas de inventario mínimo[cite: 2].
*   **Arqueo de Caja Cuadrado:** Monitoreo del flujo de efectivo, validando ingresos, retiros y comparando el dinero físico frente al saldo calculado de forma automática (con tolerancia para redondeos de punto flotante)[cite: 3].
*   **Facturación con Fidelización:** Generación de tickets asignados a clientes recurrentes que acumulan 1 punto por cada $10 consumidos[cite: 7]. También soporta ventas automáticas a Consumidor Final[cite: 7].
*   **Reportes Estadísticos y Ranking:** Listas dinámicas ordenadas por Bubble Sort para hallar los productos más vendidos y clasificar a los clientes más valiosos de la tienda[cite: 5].

---

## Estructura del Proyecto

El proyecto está diseñado de forma modular para un mantenimiento limpio:

*   `main.c`: Hilo de ejecución principal, verificación de bases de datos iniciales y enrutamiento del flujo de menús según el rol del usuario[cite: 2].
*   `caja.c` / `caja.h`: Lógica de apertura, historial diario de turnos y validación de balances monetarios[cite: 3].
*   `clientes.c` / `clientes.h`: Gestión del padrón de clientes, búsquedas secuenciales por cédula y acumulación de puntos de fidelidad[cite: 4, 7].
*   `productos.c` / `productos.h`: Base de datos de catálogo e inventario físico de bodega.
*   `usuarios.c` / `usuarios.h`: Control de accesos por UID, creación automática de usuarios semilla y borrado lógico[cite: 6].
*   `ventas.c` / `ventas.h`: Emisión de facturas por ítem, generación de ID global de tickets y cálculo impositivo local (IVA 15%)[cite: 7].
*   `reportes.c` / `reportes.h`: Generación de analíticas, filtros de fecha/rangos y ranking de clientes/productos[cite: 5].

---

## Arquitectura de Datos (`Structs` Utilizados)

El software persiste los datos mediante flujos de bytes binarios. El diseño de las estructuras principales es el siguiente:

### Usuario
```c
typedef enum {
    ADMINISTRADOR = 1,
    CAJERO,
    BODEGUERO
} Rol;

typedef struct {
    char uid_nfc[24];       // UID único de acceso (ej. "04:9D:24:25:CA:2A:81")
    char nombre[50];        // Nombre del empleado
    char rol_usuario[20];   // Rol en formato texto (ej. "ADMINISTRADOR")
    Rol rol;                // Rol en formato numérico (Enum)
} Usuario;

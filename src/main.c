#include <gtk/gtk.h>
#include <stdio.h>
#include "datos.h"
#include "interfaz.h" // Incluimos nuestro nuevo módulo

int main(int argc, char *argv[]) {
    // 1. Iniciamos las librerías gráficas
    gtk_init(&argc, &argv);

    // 2. Cargamos el motor de datos
    Dataset *mi_dataset = datos_crear(256); 
    int registros = datos_cargarCSV(mi_dataset, "data/dataset.csv");

    if (registros > 0) {
        printf("Datos cargados correctamente: %d filas.\n", registros);
    } else {
        printf("Error: No se pudo cargar el CSV.\n");
    }

    // 3. Arrancamos la interfaz gráfica (le pasamos los datos)
    if (iniciar_interfaz(mi_dataset)) {
        // 4. Si la interfaz cargó bien, entramos al bucle infinito
        gtk_main();
    } else {
        printf("El programa no pudo arrancar la interfaz visual.\n");
    }

    // 5. Cuando el bucle termina (usuario cierra ventana), limpiamos TODO
    datos_liberar(mi_dataset);
    printf("Memoria liberada. Fin del programa.\n");

    return 0;
}
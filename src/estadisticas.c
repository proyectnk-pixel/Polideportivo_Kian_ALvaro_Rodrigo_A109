#include "estadisticas.h"
#include "ventana.h"
#include "datos.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void rellenar_ranking_centros(DatosVentana *v) {
    int i, j, k;
    int num;
    char **centros;
    float medias[100];
    int cont;

    // arrays para ordenar
    char *orden_nombres[100];
    float orden_medias[100];
    float tmp_f;
    char *tmp_s;

    if (v == NULL || v->ds == NULL) {
        return;
    }

    centros = datos_obtenerCentros(v->ds, &num);
    if (centros == NULL || num == 0) {
        return;
    }

    cont = 0;

    // calculo media de cada centro
    for (i = 0; i < num && i < 100; i++) {
        float suma = 0.0f;
        int cuenta = 0;
        for (j = 0; j < v->ds->total; j++) {
            if (strcmp(v->ds->actividades[j].centro, centros[i]) == 0) {
                suma += v->ds->actividades[j].ocupacion;
                cuenta++;
            }
        }
        if (cuenta > 0) {
            medias[i] = suma / cuenta;
        } else {
            medias[i] = 0.0f;
        }
        orden_nombres[i] = centros[i];
        orden_medias[i] = medias[i];
        cont++;
    }

    // ordeno de mayor a menor con burbuja
    for (i = 0; i < cont - 1; i++) {
        for (j = 0; j < cont - 1 - i; j++) {
            if (orden_medias[j] < orden_medias[j + 1]) {
                tmp_f = orden_medias[j];
                orden_medias[j] = orden_medias[j + 1];
                orden_medias[j + 1] = tmp_f;

                tmp_s = orden_nombres[j];
                orden_nombres[j] = orden_nombres[j + 1];
                orden_nombres[j + 1] = tmp_s;
            }
        }
    }

    // meto el ranking en el liststore de estadisticas
    gtk_list_store_clear(v->liststore_ranking);

    GtkTreeIter fila;
    char pct[10];

    for (k = 0; k < cont; k++) {
        sprintf(pct, "%.0f%%", orden_medias[k] * 100);
        gtk_list_store_append(v->liststore_ranking, &fila);
        gtk_list_store_set(v->liststore_ranking, &fila,
            0, orden_nombres[k],
            1, pct,
            -1);
    }

    datos_liberarCentros(centros, num);
}

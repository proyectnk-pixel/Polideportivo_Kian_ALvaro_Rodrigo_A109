#include "datos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Reserva memoria para el dataset y para el array de actividades 
Dataset* datos_crear(void) {
    Dataset *d = malloc(sizeof(Dataset));
    if (d == NULL) {
        return NULL;
    }

    d->actividades = malloc(sizeof(Actividad) * MAX_ACTIVIDADES);
    if (d->actividades == NULL) {
        free(d);// si falla la segunda reserva, libera la primera
        return NULL;
    }
    d->total = 0;
    d->capacidad = MAX_ACTIVIDADES;
    return d;
}
// Abre el CSV, salta la cabecera y lee cada línea con scanf
int datos_cargarCSV(Dataset *d, char *ruta) {
    if (d == NULL) {
        return 0;
    }

    FILE *f = fopen(ruta, "r");
    if (f == NULL) {
        return 0;
    }
    // salto la cabecera
    char cabecera[512];
    fgets(cabecera, sizeof(cabecera), f);

    int i = 0;
    while (i < MAX_ACTIVIDADES) {
        Actividad a;
        int leidos = fscanf(f, "%d %d %d %s %s %s %s %s %s %d %d %d %s", &a.anio, &a.mes, &a.dia, a.diaSemana, a.horaInicio, a.horaFin, a.actividad, a.modalidad, a.centro, &a.plazasTotales, &a.plazasOcupadas, &a.libres, a.tipoActividad);

        if (leidos != 13) {
            break;
        }

        // Construimos la franja horaria como "horaInicio - horaFin"
        strcpy(a.franjaHoraria, a.horaInicio);
        strcat(a.franjaHoraria, "-");
        strcat(a.franjaHoraria, a.horaFin);
         // Calcula la ocupacion entre 0 y 1
        if (a.plazasTotales > 0) {
            a.ocupacion = (float)a.plazasOcupadas / a.plazasTotales;
        } else {
            a.ocupacion = 0.0f;
        }

        d->actividades[i] = a;
        i++;
    }

    d->total = i;
    fclose(f);
    return d->total;
}
// Libera el array de actividades y luego el struct
void datos_liberar(Dataset *d) {
    if (d == NULL) {
        return;
    }
    free(d->actividades);
    free(d);
}

// busco los centros sin repetir y los meto en un array de strings
char** datos_obtenerCentros(Dataset *d, int *num) {
    *num = 0;
    if (d == NULL || d->total == 0) {
        return NULL;
    }

    // reservo para el peor caso
    char **lista = malloc(d->total * sizeof(char *));
    if (lista == NULL) {
        return NULL;
    }

    int n = 0, i, j;
    for (i = 0; i < d->total; i++) {
        int rep = 0;
        for (j = 0; j < n; j++) {
            if (strcmp(lista[j], d->actividades[i].centro) == 0) {
                rep = 1;
                break;
            }
        }
        if (rep == 0) {
            lista[n] = malloc(MAX_NOMBRE);
            if (lista[n] == NULL) {
                break;
            }
            strcpy(lista[n], d->actividades[i].centro);
            n++;
        }
    }

    *num = n;
    return lista;
}
// Igual que obtenerCentros pero para nombres de actividad
char** datos_obtenerActividades(Dataset *d, int *num) {
    *num = 0;
    if (d == NULL || d->total == 0) {
        return NULL;
    }

    char **lista = malloc(d->total * sizeof(char *));
    if (lista == NULL) {
        return NULL;
    }

    int i, j;
    int cont = 0;

    for (i = 0; i < d->total; i++) {
        int rep = 0;

        for (j = 0; j < cont; j++) {
            if (strcmp(lista[j], d->actividades[i].actividad) == 0) {
                rep = 1;
                break;
            }
        }
        if (rep == 0) {
            lista[cont] = malloc(MAX_NOMBRE);
            if (lista[cont] == NULL) {
                break;
            }
            strcpy(lista[cont], d->actividades[i].actividad);
            cont++;
        }
    }

    *num = cont;
    return lista;
}
// Libera cada string de la lista y luego el array de punteros
void datos_liberarCentros(char **lista, int n) {
    if (lista == NULL) {
        return;
    }
    int i;
    for (i = 0; i < n; i++) {
        free(lista[i]);
    }
    free(lista);
}
// Calcula la media de ocupacion de todas las actividades
float datos_ocupacionMedia(Dataset *d) {
    if (d == NULL || d->total == 0) {
        return 0.0f;
    }
    float suma = 0.0f;
    int i;
    for (i = 0; i < d->total; i++) {
        suma += d->actividades[i].ocupacion;
    }
    return suma / d->total;
}
// Suma las plazas libres de todas las actividades
int datos_totalLibres(Dataset *d) {
    if (d == NULL) {
        return 0;
    }
    int suma = 0, i;
    for (i = 0; i < d->total; i++) {
        suma += d->actividades[i].libres;
    }
    return suma;
}
// Cuenta cuantas actividades hay en un dia concreto
int datos_contarPorDia(Dataset *d, char *dia) {
    if (d == NULL || dia == NULL) {
        return 0;
    }
    int cnt = 0, i;
    for (i = 0; i < d->total; i++) {
        if (strcmp(d->actividades[i].diaSemana, dia) == 0) {
            cnt++;
        }
    }
    return cnt;
}
// Cuenta cuantas actividades hay en un mes concreto
int datos_contarPorMes(Dataset *d, int mes) {
    if (d == NULL) {
        return 0;
    }
    int cnt = 0, i;
    for (i = 0; i < d->total; i++) {
        if (d->actividades[i].mes == mes) {
            cnt++;
        }
    }
    return cnt;
}

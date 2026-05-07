#include "datos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* quito el salto de linea que deja fgets */
void quitar_espacios(char *s) {
    int j = (int)strlen(s) - 1;
    while (j >= 0 && (s[j] == '\n' || s[j] == '\r' || s[j] == ' ' || s[j] == '\t')) {
        s[j] = '\0';
        j--;
    }
}

/* para que lunes y Lunes sean lo mismo */
void primera_mayuscula(char *s) {
    if (s == NULL || s[0] == '\0')
        return;
    if (s[0] >= 'a' && s[0] <= 'z')
        s[0] = s[0] - 'a' + 'A';
}

/* creo el dataset vacio */
Dataset* datos_crear() {
    Dataset *ds = malloc(sizeof(Dataset));
    if (ds == NULL)
        return NULL;

    ds->actividades = malloc(sizeof(Actividad) * MAX_ACTIVIDADES);
    if (ds->actividades == NULL) {
        free(ds);
        return NULL;
    }

    ds->total = 0;
    ds->capacidad = MAX_ACTIVIDADES;
    return ds;
}

/* cargo el CSV linea a linea y relleno el dataset */
int datos_cargarCSV(Dataset *ds, const char *ruta) {
    if (ds == NULL)
        return 0;

    FILE *f = fopen(ruta, "r");
    if (f == NULL)
        return 0;

    char linea[512];
    char copia[512];
    char *tok;

    /* salto la cabecera */
    fgets(linea, sizeof(linea), f);

    while (fgets(linea, sizeof(linea), f) != NULL) {

        if (ds->total >= MAX_ACTIVIDADES)
            break;

        /* strtok destruye el string asi que uso una copia */
        strncpy(copia, linea, sizeof(copia) - 1);
        copia[sizeof(copia) - 1] = '\0';

        Actividad *a = &ds->actividades[ds->total];

        tok = strtok(copia, " ");
        if (tok == NULL) continue;
        a->anio = atoi(tok);

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        a->mes = atoi(tok);

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        a->dia = atoi(tok);

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        strcpy(a->diaSemana, tok);
        quitar_espacios(a->diaSemana);
        primera_mayuscula(a->diaSemana);

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        strcpy(a->horaInicio, tok);

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        strcpy(a->horaFin, tok);

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        strcpy(a->actividad, tok);
        quitar_espacios(a->actividad);

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        strcpy(a->modalidad, tok);

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        strcpy(a->centro, tok);
        quitar_espacios(a->centro);

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        a->plazasTotales = atoi(tok);

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        a->plazasOcupadas = atoi(tok);

        a->libres = a->plazasTotales - a->plazasOcupadas;

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        strcpy(a->tipoActividad, tok);

        /* junto hora inicio y fin para la tabla */
        strcpy(a->franjaHoraria, a->horaInicio);
        strcat(a->franjaHoraria, "-");
        strcat(a->franjaHoraria, a->horaFin);

        /* porcentaje de ocupacion, evito dividir por cero */
        if (a->plazasTotales > 0)
            a->ocupacion = (float)a->plazasOcupadas / a->plazasTotales;
        else
            a->ocupacion = 0.0f;

        ds->total++;
    }

    fclose(f);
    return ds->total;
}

/* libero la memoria del dataset */
void datos_liberar(Dataset *ds) {
    if (ds == NULL)
        return;
    free(ds->actividades);
    free(ds);
}

/* devuelve los centros sin repetir */
char** datos_obtenerCentros(Dataset *ds, int *num) {
    *num = 0;
    if (ds == NULL || ds->total == 0) return NULL;

    /* en el peor caso todos los centros son distintos */
    char **lista = malloc(sizeof(char*) * ds->total);
    if (lista == NULL) return NULL;
    int n = 0;

    for (int i = 0; i < ds->total; i++) {
        /* miro si este centro ya lo tengo guardado */
        int repetido = 0;
        for (int j = 0; j < n; j++) {
            if (strcmp(lista[j], ds->actividades[i].centro) == 0) {
                repetido = 1;
                break;
            }
        }
        if (!repetido) {
            lista[n] = malloc(MAX_NOMBRE);
            if (lista[n] != NULL) {
                strcpy(lista[n], ds->actividades[i].centro);
                n++;
            }
        }
    }

    *num = n;
    return lista;
}

/* igual pero con el nombre de la actividad */
char** datos_obtenerActividades(Dataset *ds, int *num) {
    *num = 0;
    if (ds == NULL || ds->total == 0) return NULL;

    char **lista = malloc(sizeof(char*) * ds->total);
    if (lista == NULL) return NULL;
    int n = 0;

    for (int i = 0; i < ds->total; i++) {
        int repetido = 0;
        for (int j = 0; j < n; j++) {
            if (strcmp(lista[j], ds->actividades[i].actividad) == 0) {
                repetido = 1;
                break;
            }
        }
        if (!repetido) {
            lista[n] = malloc(MAX_NOMBRE);
            if (lista[n] != NULL) {
                strcpy(lista[n], ds->actividades[i].actividad);
                n++;
            }
        }
    }

    *num = n;
    return lista;
}

/* libera el array de strings */
void datos_liberarLista(char **lista, int n) {
    if (lista == NULL) return;
    for (int i = 0; i < n; i++)
        free(lista[i]);
    free(lista);
}

void datos_liberarCentros(char **lista, int n) {
    datos_liberarLista(lista, n);
}

/* ocupacion media de todas las actividades */
float datos_ocupacionMedia(Dataset *ds) {
    if (ds == NULL || ds->total == 0)
        return 0.0f;

    float suma = 0.0f;
    for (int i = 0; i < ds->total; i++)
        suma += ds->actividades[i].ocupacion;

    return suma / ds->total;
}

/* cuantas plazas libres hay en total */
int datos_totalLibres(Dataset *ds) {
    if (ds == NULL) return 0;

    int suma = 0;
    for (int i = 0; i < ds->total; i++)
        suma += ds->actividades[i].libres;

    return suma;
}

/* cuantas actividades hay un dia concreto */
int datos_contarPorDia(Dataset *ds, const char *dia) {
    if (ds == NULL || dia == NULL) return 0;

    int n = 0;
    for (int i = 0; i < ds->total; i++)
        if (strcmp(ds->actividades[i].diaSemana, dia) == 0)
            n++;

    return n;
}

/* cuantas actividades hay en un mes */
int datos_contarPorMes(Dataset *ds, int mes) {
    if (ds == NULL) return 0;

    int n = 0;
    for (int i = 0; i < ds->total; i++)
        if (ds->actividades[i].mes == mes)
            n++;

    return n;
}

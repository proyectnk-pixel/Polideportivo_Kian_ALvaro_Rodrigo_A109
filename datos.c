#include "datos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// el fgets deja un \n al final y problemas al compara, y limpiamos tambien otros
void quitar_espacios(char *s) {
    int j = (int)strlen(s) - 1;
    while (j >= 0 && (s[j] == '\n' || s[j] == '\r' || s[j] == ' ' || s[j] == '\t')) {
        s[j] = '\0';
        j--;
    }
}

// sin esto "lunes" y "Lunes" no coincidian en el strcmp(comparar)
void primera_mayuscula(char *s) {
    if (s == NULL || s[0] == '\0') return;
    if (s[0] >= 'a' && s[0] <= 'z')
        s[0] = s[0] - 'a' + 'A';
}

Dataset* datos_crear() {
    Dataset *d = malloc(sizeof(Dataset));
    if (d == NULL) return NULL;

    d->actividades = malloc(sizeof(Actividad) * MAX_ACTIVIDADES);
    if (d->actividades == NULL) {
        free(d);
        return NULL;
    }
    d->total = 0;
    d->capacidad = MAX_ACTIVIDADES;
    return d;
}

int datos_cargarCSV(Dataset *d, const char *ruta) {
    if (d == NULL) return 0;

    FILE *f = fopen(ruta, "r");
    if (f == NULL) return 0;

    char buf[512], cp[512];
    char *tok;

    fgets(buf, sizeof(buf), f); // cabecera

    while (fgets(buf, sizeof(buf), f) != NULL) {

        if (d->total >= MAX_ACTIVIDADES) break;

        strncpy(cp, buf, sizeof(cp) - 1);
        cp[sizeof(cp) - 1] = '\0';

        Actividad *a = &d->actividades[d->total];

        tok = strtok(cp, " ");  if (tok == NULL) continue;
        a->anio = atoi(tok);

        tok = strtok(NULL, " ");  if (tok == NULL) continue;
        a->mes = atoi(tok);

        tok = strtok(NULL, " ");  if (tok == NULL) continue;
        a->dia = atoi(tok);

        tok = strtok(NULL, " ");  if (tok == NULL) continue;
        strcpy(a->diaSemana, tok);
        quitar_espacios(a->diaSemana);
        primera_mayuscula(a->diaSemana);

        tok = strtok(NULL, " ");  if (tok == NULL) continue;
        strcpy(a->horaInicio, tok);

        tok = strtok(NULL, " ");  if (tok == NULL) continue;
        strcpy(a->horaFin, tok);

        tok = strtok(NULL, " ");  if (tok == NULL) continue;
        strcpy(a->actividad, tok);
        quitar_espacios(a->actividad);

        tok = strtok(NULL, " ");  if (tok == NULL) continue;
        strcpy(a->modalidad, tok);

        tok = strtok(NULL, " ");  if (tok == NULL) continue;
        strcpy(a->centro, tok);
        quitar_espacios(a->centro);

        tok = strtok(NULL, " ");  if (tok == NULL) continue;
        a->plazasTotales = atoi(tok);

        tok = strtok(NULL, " ");  if (tok == NULL) continue;
        a->plazasOcupadas = atoi(tok);

        a->libres = a->plazasTotales - a->plazasOcupadas;

        tok = strtok(NULL, " ");  if (tok == NULL) continue;
        strcpy(a->tipoActividad, tok);

        // junto las horas para mostrarlas juntas en la tabla
        strcpy(a->franjaHoraria, a->horaInicio);
        strcat(a->franjaHoraria, "-");
        strcat(a->franjaHoraria, a->horaFin);

        if (a->plazasTotales > 0)
            a->ocupacion = (float)a->plazasOcupadas / a->plazasTotales;
        else
            a->ocupacion = 0.0f;

        d->total++;
    }

    fclose(f);
    return d->total;
}

void datos_liberar(Dataset *d) {
    if (d == NULL) return;
    free(d->actividades);
    free(d);
}

char** datos_obtenerCentros(Dataset *d, int *num) {
    *num = 0;
    if (d == NULL || d->total == 0) return NULL;

    char **res = malloc(sizeof(char*) * d->total);
    if (res == NULL) return NULL;

    int n = 0, i, j, rep;
    for (i = 0; i < d->total; i++) {
        rep = 0;
        for (j = 0; j < n; j++) {
            if (strcmp(res[j], d->actividades[i].centro) == 0) {
                rep = 1; break;
            }
        }
        if (!rep) {
            res[n] = malloc(MAX_NOMBRE);
            if (res[n] != NULL)
                strcpy(res[n++], d->actividades[i].centro);
        }
    }
    *num = n;
    return res;
}

// esta es igual a la de centros pero mirando el campo actividad
char** datos_obtenerActividades(Dataset *d, int *num) {
    *num = 0;
    if (d == NULL || d->total == 0) return NULL;

    int n = 0, i, j, rep;
    char **res = malloc(sizeof(char*) * d->total);
    if (res == NULL) return NULL;

    for (i = 0; i < d->total; i++) {
        rep = 0;
        for (j = 0; j < n; j++) {
            if (strcmp(res[j], d->actividades[i].actividad) == 0) { rep = 1; break; }
        }
        if (!rep) {
            res[n] = malloc(MAX_NOMBRE);
            if (res[n] != NULL) strcpy(res[n++], d->actividades[i].actividad);
        }
    }
    *num = n;
    return res;
}

void datos_liberarLista(char **lista, int n) {
    if (lista == NULL) return;
    int i;
    for (i = 0; i < n; i++) free(lista[i]);
    free(lista);
}

void datos_liberarCentros(char **lista, int n) {
    datos_liberarLista(lista, n);
}

float datos_ocupacionMedia(Dataset *d) {
    if (d == NULL || d->total == 0) return 0.0f;
    float suma = 0.0f;
    int i;
    for (i = 0; i < d->total; i++)
        suma += d->actividades[i].ocupacion;
    return suma / d->total;
}

int datos_totalLibres(Dataset *d) {
    if (d == NULL) return 0;
    int suma = 0, i;
    for (i = 0; i < d->total; i++)
        suma += d->actividades[i].libres;
    return suma;
}

int datos_contarPorDia(Dataset *d, const char *dia) {
    if (d == NULL || dia == NULL) return 0;
    int cnt = 0, i;
    for (i = 0; i < d->total; i++)
        if (strcmp(d->actividades[i].diaSemana, dia) == 0) cnt++;
    return cnt;
}

int datos_contarPorMes(Dataset *d, int mes) {
    if (d == NULL) return 0;
    int cnt = 0, i;
    for (i = 0; i < d->total; i++)
        if (d->actividades[i].mes == mes) cnt++;
    return cnt;
}

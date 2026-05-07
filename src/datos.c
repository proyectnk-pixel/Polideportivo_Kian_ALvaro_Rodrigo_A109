#include "datos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Limpio saltos de linea y espacios que deja fgets */
static void quitar_espacios(char *s) {
    int j = (int)strlen(s) - 1;
    while (j >= 0 && (s[j] == '\n' || s[j] == '\r' || s[j] == ' ' || s[j] == '\t'))
        s[j--] = '\0';
}

// Para evitar que mayusculas y minusculas los interprete distinto
static void primera_mayuscula(char *s) {
    if (s == NULL || s[0] == '\0')
        return;

    if (s[0] >= 'a' && s[0] <= 'z')
        s[0] = s[0] - 'a' + 'A';
}

/* Si el array se queda corto, lo duplico. */
static int hacer_mas_grande(Dataset *ds) {
    int nueva_cap = ds->capacidad * 2;
    Actividad *aux = realloc(ds->actividades, sizeof(Actividad) * nueva_cap);

    /* si falla realloc, mejor no tocar nada */
    if (aux == NULL)
        return 0;

    ds->actividades = aux;
    ds->capacidad = nueva_cap;
    return 1;
}

/* Creo un dataset vacío con capacidad inicial 'cap' */
Dataset* datos_crear(int cap) {
    Dataset *ds = malloc(sizeof(Dataset));
    if (ds == NULL)
        return NULL;

    ds->actividades = malloc(sizeof(Actividad) * cap);
    if (ds->actividades == NULL) {
        free(ds);
        return NULL;
    }

    ds->total = 0;
    ds->capacidad = cap;
    return ds;
}

// Cargo el CSV línea a línea
int datos_cargarCSV(Dataset *ds, const char *ruta) {
    if (ds == NULL)
        return 0;
    FILE *f = fopen(ruta, "r");
    if (f == NULL)
        return 0;
    char linea[512];
    char copia[512];
    char *tok;

    /* primera línea = cabecera, la salto */
    fgets(linea, sizeof(linea), f);

    /* leo el resto del fichero */
    while (fgets(linea, sizeof(linea), f) != NULL) {

        /* si no cabe, lo amplío */
        if (ds->total >= ds->capacidad) {
            if (!hacer_mas_grande(ds))
                break;  /* si no puedo ampliar, paro */
        }

        /* strtok modifica el string, así que trabajo con una copia */
        strncpy(copia, linea, sizeof(copia) - 1);
        copia[sizeof(copia) - 1] = '\0';

        Actividad *a = &ds->actividades[ds->total];

        /* Empiezo a trocear la línea */
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
        strncpy(a->diaSemana, tok, MAX_DIA - 1);
        a->diaSemana[MAX_DIA - 1] = '\0';
        quitar_espacios(a->diaSemana);
        primera_mayuscula(a->diaSemana);

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        strncpy(a->horaInicio, tok, MAX_HORA - 1);
        a->horaInicio[MAX_HORA - 1] = '\0';

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        strncpy(a->horaFin, tok, MAX_HORA - 1);
        a->horaFin[MAX_HORA - 1] = '\0';

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        strncpy(a->actividad, tok, MAX_NOMBRE - 1);
        a->actividad[MAX_NOMBRE - 1] = '\0';
        quitar_espacios(a->actividad);

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        strncpy(a->modalidad, tok, MAX_NOMBRE - 1);
        a->modalidad[MAX_NOMBRE - 1] = '\0';

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        strncpy(a->centro, tok, MAX_NOMBRE - 1);
        a->centro[MAX_NOMBRE - 1] = '\0';
        quitar_espacios(a->centro);

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        a->plazasTotales = atoi(tok);

        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        a->plazasOcupadas = atoi(tok);

        /* calculo libres directamente */
        a->libres = a->plazasTotales - a->plazasOcupadas;

        /* tipo de actividad */
        tok = strtok(NULL, " ");
        if (tok == NULL) continue;
        strncpy(a->tipoActividad, tok, MAX_NOMBRE - 1);
        a->tipoActividad[MAX_NOMBRE - 1] = '\0';

        /* franja horaria: "inicio-fin" */
        snprintf(a->franjaHoraria, MAX_FRANJA, "%s-%s", a->horaInicio, a->horaFin);

        /* ocupación: si totales = 0, evito división por cero */
        if (a->plazasTotales > 0)
            a->ocupacion = (float)a->plazasOcupadas / (float)a->plazasTotales;
        else
            a->ocupacion = 0.0f;

        ds->total++;
    }

    fclose(f);
    return ds->total;
}

/* Libero todo lo que reservé */
void datos_liberar(Dataset *ds) {
    if (ds == NULL)
        return;

    free(ds->actividades);
    free(ds);
}

/* Devuelve un array con los centros que aparecen en el dataset, sin repetir.
   El llamante es responsable de liberar la lista con datos_liberarLista. */
char** datos_obtenerCentros(Dataset *ds, int *num) {
    *num = 0;
    if (ds == NULL || ds->total == 0) return NULL;

    /* reservo el peor caso: todos los centros son distintos */
    char **lista = malloc(sizeof(char*) * ds->total);
    if (lista == NULL) return NULL;
    int n = 0;

    for (int i = 0; i < ds->total; i++) {

        // compruebo si este centro ya está en la lista 
        int repetido = 0;
        for (int j = 0; j < n; j++) {
            if (strcmp(lista[j], ds->actividades[i].centro) == 0) {
                repetido = 1;
                break;
            }
        }

        /* si es nuevo, lo añado */
        if (!repetido) {
            lista[n] = malloc(MAX_NOMBRE);
            if (lista[n] != NULL) {
                strncpy(lista[n], ds->actividades[i].centro, MAX_NOMBRE - 1);
                lista[n][MAX_NOMBRE - 1] = '\0';
                n++;
            }
        }
    }

    *num = n;
    return lista;
}

/* Igual que datos_obtenerCentros pero con el nombre de la actividad. */
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
                strncpy(lista[n], ds->actividades[i].actividad, MAX_NOMBRE - 1);
                lista[n][MAX_NOMBRE - 1] = '\0';
                n++;
            }
        }
    }

    *num = n;
    return lista;
}

// Libera un array de strings creado por datos_obtenerCentros o datos_obtenerActividades 
void datos_liberarLista(char **lista, int n) {
    if (lista == NULL) return;

    for (int i = 0; i < n; i++)
        free(lista[i]);

    free(lista);
}

/* Alias por comodidad */
void datos_liberarCentros(char **lista, int n) {
    datos_liberarLista(lista, n);
}

/* Ocupación media de todas las actividades */
float datos_ocupacionMedia(Dataset *ds) {
    if (ds == NULL || ds->total == 0)
        return 0.0f;

    float suma = 0.0f;
    for (int i = 0; i < ds->total; i++)
        suma += ds->actividades[i].ocupacion;

    return suma / (float)ds->total;
}

/* Suma total de plazas libres */
int datos_totalLibres(Dataset *ds) {
    if (ds == NULL)
        return 0;

    int suma = 0;
    for (int i = 0; i < ds->total; i++)
        suma += ds->actividades[i].libres;

    return suma;
}

/* Cuántas actividades hay en un día concreto */
int datos_contarPorDia(Dataset *ds, const char *dia) {
    if (ds == NULL || dia == NULL)
        return 0;

    int n = 0;
    for (int i = 0; i < ds->total; i++)
        if (strcmp(ds->actividades[i].diaSemana, dia) == 0)
            n++;

    return n;
}

/* Cuántas actividades hay en un mes concreto */
int datos_contarPorMes(Dataset *ds, int mes) {
    if (ds == NULL)
        return 0;

    int n = 0;
    for (int i = 0; i < ds->total; i++)
        if (ds->actividades[i].mes == mes)
            n++;

    return n;
}

/* Implementación de la búsqueda por centro */
Actividad** datos_obtenerActividadesPorCentro(Dataset *ds, const char *centro_buscado, int *num_resultados) {
    if (ds == NULL || centro_buscado == NULL || num_resultados == NULL) {
        if (num_resultados != NULL) *num_resultados = 0;
        return NULL;
    }

    *num_resultados = 0;
    int contador = 0;

    // Contar cuántas coinciden
    for (int i = 0; i < ds->total; i++) {
        if (strcmp(ds->actividades[i].centro, centro_buscado) == 0) {
            contador++;
        }
    }

    if (contador == 0) return NULL; 

    // Reservar array de punteros
    Actividad **resultados = malloc(contador * sizeof(Actividad *));
    if (resultados == NULL) return NULL;

    // Guardar las direcciones
    int indice = 0;
    for (int i = 0; i < ds->total; i++) {
        if (strcmp(ds->actividades[i].centro, centro_buscado) == 0) {
            resultados[indice] = &(ds->actividades[i]);
            indice++;
        }
    }

    *num_resultados = contador;
    return resultados;
}


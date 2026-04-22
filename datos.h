#ifndef DATOS_H
#define DATOS_H

#define MAX_NOMBRE   64
#define MAX_DIA      16
#define MAX_FRANJA   32
#define MAX_HORA     12
#define CAP_INI     256

typedef struct {
    int   anio;
    int   mes;
    int   dia;
    char  diaSemana[MAX_DIA];
    char  horaInicio[MAX_HORA];
    char  horaFin[MAX_HORA];
    char  actividad[MAX_NOMBRE];
    char  modalidad[MAX_NOMBRE];
    char  centro[MAX_NOMBRE];
    int   plazasTotales;
    int   plazasOcupadas;
    int   libres;
    char  tipoActividad[MAX_NOMBRE];
    char  franjaHoraria[MAX_FRANJA];  // "HH:MM - HH:MM", derivado al leer
    float ocupacion;                   // 0.0 a 1.0, calculado al leer
} Actividad;

typedef struct {
    Actividad *actividades;
    int total;       // registros cargados
    int capacidad;   // capacidad antes de realloc
} Dataset;

Dataset* datos_crear(int cap);
int      datos_cargarCSV(Dataset *ds, const char *ruta);
void     datos_liberar(Dataset *ds);

char**   datos_obtenerCentros(Dataset *ds, int *num);
char**   datos_obtenerActividades(Dataset *ds, int *num);
void     datos_liberarLista(char **lista, int n);
void     datos_liberarCentros(char **lista, int n);  /* alias de datos_liberarLista */

float    datos_ocupacionMedia(Dataset *ds);
int      datos_totalLibres(Dataset *ds);
int      datos_contarPorDia(Dataset *ds, const char *dia);
int      datos_contarPorMes(Dataset *ds, int mes);

#endif

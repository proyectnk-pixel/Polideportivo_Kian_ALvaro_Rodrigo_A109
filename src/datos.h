#ifndef DATOS_H
#define DATOS_H

#define MAX_NOMBRE      64
#define MAX_DIA         16
#define MAX_FRANJA      32
#define MAX_HORA        12
#define MAX_ACTIVIDADES 5000

// cada linea del CSV es una actividad 
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
    char  franjaHoraria[MAX_FRANJA];
    float ocupacion;
} Actividad;

// guardo todas las actividades 
typedef struct {
    Actividad *actividades;
    int total;
    int capacidad;
} Dataset;

Dataset* datos_crear(void);
int datos_cargarCSV(Dataset *d, char *ruta);
void datos_liberar(Dataset *d);

char** datos_obtenerCentros(Dataset *d, int *num);
char** datos_obtenerActividades(Dataset *d, int *num);
void datos_liberarCentros(char **lista, int n);

float datos_ocupacionMedia(Dataset *d);
int datos_totalLibres(Dataset *d);
int datos_contarPorDia(Dataset *d, char *dia);
int datos_contarPorMes(Dataset *d, int mes);

#endif


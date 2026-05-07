#ifndef DATOS_H
#define DATOS_H

/* columnas del liststore de GTK */
enum {
    COL_ACTIVIDAD = 0,
    COL_HORARIO,       
    COL_PLAZAS,        
    COL_OCUPACION,     
    NUM_COLUMNAS       
};
#define MAX_NOMBRE  64
#define MAX_DIA     16
#define MAX_FRANJA  32
#define MAX_HORA    12
#define MAX_ACTIVIDADES 5000

/* Cada línea del CSV representa una actividad.
   Aquí guardo todo lo que viene del fichero más un par de cosas calculadas. */
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
    //Ocupación = ocupadas / totales (si totales > 0) 
    float ocupacion;

} Actividad;

// aqui guardo todas las actividades 
typedef struct {
    Actividad *actividades;
    int total;      /* cuántas actividades tengo ahora mismo */
    int capacidad;  /* cuántas caben antes de tener que ampliar */
} Dataset;

Dataset* datos_crear();

/* Lee el CSV y rellena el dataset.
   Devuelve cuántas actividades se cargaron. */
int datos_cargarCSV(Dataset *ds, const char *ruta);

/* Libera todo lo que se reservó dinámicamente */
void datos_liberar(Dataset *ds);

/* Devuelve una lista de centros sin repetir */
char** datos_obtenerCentros(Dataset *ds, int *num);

/* Igual que la anterior pero con nombres de actividad */
char** datos_obtenerActividades(Dataset *ds, int *num);

/* Libera la lista creada por las funciones anteriores */
void datos_liberarLista(char **lista, int n);
void datos_liberarCentros(char **lista, int n);

/* estadisticas */
float datos_ocupacionMedia(Dataset *ds);
int datos_totalLibres(Dataset *ds);
int datos_contarPorDia(Dataset *ds, const char *dia);
int datos_contarPorMes(Dataset *ds, int mes);

/* Devuelve un array de punteros a las actividades de un centro concreto */
Actividad** datos_obtenerActividadesPorCentro(Dataset *ds, const char *centro_buscado, int *num_resultados);

#endif

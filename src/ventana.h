#ifndef VENTANA_H
#define VENTANA_H

#include <gtk/gtk.h>
#include "datos.h"

// struct con todos los widgets de la ventana y el dataset 
typedef struct {
    Dataset *ds;

    GtkWindow  *ventana_principal;
    GtkStack   *stack_principal;
    GtkLabel   *lbl_estado_carga;
    GtkLabel   *lbl_cargando;
    GtkSpinner *spinner;

    GtkButton *btn_ir_listados;
    GtkButton *btn_ir_estadisticas;
    GtkButton *btn_ir_recomendaciones;
    GtkButton *btn_ir_graficas;
    GtkButton *btn_salir_menu;

    GtkComboBoxText *combo_centros;
    GtkListStore    *liststore_actividades;
    GtkListStore    *liststore_ranking;
    GtkSearchEntry  *txt_filtro;
    GtkDrawingArea  *drawing_grafica;

} DatosVentana;

DatosVentana* ventana_crear(void);
void ventana_inicializar(DatosVentana *v, GtkBuilder *builder);
void ventana_liberar(DatosVentana *v);

#endif

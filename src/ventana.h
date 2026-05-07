#ifndef VENTANA_H
#define VENTANA_H

#include <gtk/gtk.h>
#include "datos.h"

/* struct con todos los widgets de la ventana principal
   y el dataset cargado */
typedef struct {
    Dataset *ds;

    /* ventana y navegacion */
    GtkWindow  *ventana_principal;
    GtkStack   *stack_principal;
    GtkLabel   *lbl_estado_carga;
    GtkLabel   *lbl_cargando;
    GtkSpinner *spinner;

    /* botones del menu */
    GtkButton *btn_ir_listados;
    
    
    GtkButton *btn_ir_estadisticas;
    GtkButton *btn_ir_recomendaciones;
    GtkButton *btn_ir_graficas;
    GtkButton *btn_salir_menu;

    /* panel listados */
    GtkComboBoxText    *combo_centros;
    GtkListStore       *liststore_actividades;
    GtkSearchEntry     *txt_filtro;
    GtkTreeModelFilter *filtro_actividades;

} DatosVentana;

DatosVentana* ventana_crear(void);
void ventana_inicializar(DatosVentana *v, GtkBuilder *builder);
void ventana_liberar(DatosVentana *v);

#endif

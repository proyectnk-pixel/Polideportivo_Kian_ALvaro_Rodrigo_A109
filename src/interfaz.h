#ifndef INTERFAZ_H
#define INTERFAZ_H

#include <gtk/gtk.h>
#include "datos.h"

// Tu estructura de contexto de siempre
typedef struct {
    Dataset *ds;
    GtkStack *stack_principal;
    GtkLabel *lbl_estado_carga;
    GtkComboBoxText *combo_centros;
    GtkListStore *liststore_actividades;
    GtkSearchEntry *txt_filtro;
    GtkTreeModelFilter *filtro_actividades;
} AppContext;

// La única función que el main necesita conocer
int iniciar_interfaz(Dataset *ds);

#endif
#include "ventana.h"
#include "botones.h"
#include <stdlib.h>

DatosVentana* ventana_crear(void) {
    DatosVentana *v = malloc(sizeof(DatosVentana));
    if (v == NULL) return NULL;
    v->ds = NULL;
    v->filtro_actividades = NULL;
    return v;
}

void ventana_inicializar(DatosVentana *v, GtkBuilder *b) {

    /* cargo todos los widgets desde el archivo glade */
    v->ventana_principal  = GTK_WINDOW(gtk_builder_get_object(b, "ventana_principal"));
    v->stack_principal    = GTK_STACK(gtk_builder_get_object(b, "stack_principal"));
    v->lbl_estado_carga   = GTK_LABEL(gtk_builder_get_object(b, "lbl_estado_carga"));
    v->lbl_cargando       = GTK_LABEL(gtk_builder_get_object(b, "lbl_cargando"));
    v->spinner            = GTK_SPINNER(gtk_builder_get_object(b, "spinner"));

    v->btn_ir_listados        = GTK_BUTTON(gtk_builder_get_object(b, "btn_ir_listados"));
    v->btn_ir_estadisticas    = GTK_BUTTON(gtk_builder_get_object(b, "btn_ir_estadisticas"));
    v->btn_ir_recomendaciones = GTK_BUTTON(gtk_builder_get_object(b, "btn_ir_recomendaciones"));
    v->btn_ir_graficas        = GTK_BUTTON(gtk_builder_get_object(b, "btn_ir_graficas"));
    v->btn_salir_menu         = GTK_BUTTON(gtk_builder_get_object(b, "btn_salir_menu"));

    v->combo_centros         = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(b, "combo_centros"));
    v->liststore_actividades = GTK_LIST_STORE(gtk_builder_get_object(b, "liststore_actividades"));
    v->txt_filtro            = GTK_SEARCH_ENTRY(gtk_builder_get_object(b, "txt_filtro"));

    /* configuro el filtro para la tabla de actividades */
    GtkTreeView *tabla = GTK_TREE_VIEW(gtk_builder_get_object(b, "tree_actividades"));
    if (tabla != NULL && v->liststore_actividades != NULL) {
        v->filtro_actividades = GTK_TREE_MODEL_FILTER(
            gtk_tree_model_filter_new(GTK_TREE_MODEL(v->liststore_actividades), NULL));
        gtk_tree_model_filter_set_visible_func(
            v->filtro_actividades, filtro_actividades_func, v, NULL);
        gtk_tree_view_set_model(tabla, GTK_TREE_MODEL(v->filtro_actividades));
        /* NO hacemos unref aqui — lo hacemos en ventana_liberar */
    }

    /* conecto las senales de los botones del menu */
    g_signal_connect(v->ventana_principal,      "destroy", G_CALLBACK(gtk_main_quit),                     NULL);
    g_signal_connect(v->btn_ir_listados,        "clicked", G_CALLBACK(on_btn_ir_listados_clicked),        v);
    g_signal_connect(v->btn_ir_estadisticas,    "clicked", G_CALLBACK(on_btn_ir_estadisticas_clicked),    v);
    g_signal_connect(v->btn_ir_recomendaciones, "clicked", G_CALLBACK(on_btn_ir_recomendaciones_clicked), v);
    g_signal_connect(v->btn_ir_graficas,        "clicked", G_CALLBACK(on_btn_ir_graficas_clicked),        v);
    g_signal_connect(v->btn_salir_menu,         "clicked", G_CALLBACK(on_btn_salir_clicked),              v);

    GtkButton *btn_buscar = GTK_BUTTON(gtk_builder_get_object(b, "btn_buscar"));
    GtkButton *btn_volver = GTK_BUTTON(gtk_builder_get_object(b, "btn_volver_menu"));
    if (btn_buscar) g_signal_connect(btn_buscar,  "clicked",        G_CALLBACK(on_btn_buscar_clicked),      v);
    if (btn_volver) g_signal_connect(btn_volver,  "clicked",        G_CALLBACK(on_btn_volver_menu_clicked), v);
    if (v->txt_filtro)
        g_signal_connect(v->txt_filtro, "search-changed", G_CALLBACK(on_txt_filtro_changed), v);
}

void ventana_liberar(DatosVentana *v) {
    if (v == NULL) return;
    /* liberamos el filtro aqui, no en ventana_inicializar */
    if (v->filtro_actividades != NULL)
        g_object_unref(v->filtro_actividades);
    datos_liberar(v->ds);
    free(v);
}

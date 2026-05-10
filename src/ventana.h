#include "ventana.h"
#include "botones.h"
#include "graficas.h"
#include "datos.h"
#include <stdlib.h>

DatosVentana* ventana_crear(void) {
    DatosVentana *v = malloc(sizeof(DatosVentana));
    if (v == NULL) {
        return NULL;
    }
    v->ds = NULL;
    return v;
}

void ventana_inicializar(DatosVentana *v, GtkBuilder *b) {
    GtkButton *btn_buscar;
    GtkButton *btn_volver;
    GtkButton *btn_volver_est;
    GtkButton *btn_volver_gra;

    // saco todos los widgets del glade por su nombre
    v->ventana_principal = GTK_WINDOW(gtk_builder_get_object(b, "ventana_principal"));
    v->stack_principal = GTK_STACK(gtk_builder_get_object(b, "stack_principal"));
    v->lbl_estado_carga = GTK_LABEL(gtk_builder_get_object(b, "lbl_estado_carga"));
    v->lbl_cargando = GTK_LABEL(gtk_builder_get_object(b, "lbl_cargando"));
    v->spinner = GTK_SPINNER(gtk_builder_get_object(b, "spinner"));

    v->btn_ir_listados = GTK_BUTTON(gtk_builder_get_object(b, "btn_ir_listados"));
    v->btn_ir_estadisticas = GTK_BUTTON(gtk_builder_get_object(b, "btn_ir_estadisticas"));
    v->btn_ir_recomendaciones = GTK_BUTTON(gtk_builder_get_object(b, "btn_ir_recomendaciones"));
    v->btn_ir_graficas = GTK_BUTTON(gtk_builder_get_object(b, "btn_ir_graficas"));
    v->btn_salir_menu = GTK_BUTTON(gtk_builder_get_object(b, "btn_salir_menu"));

    v->combo_centros = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(b, "combo_centros"));
    v->liststore_actividades = GTK_LIST_STORE(gtk_builder_get_object(b, "liststore_actividades"));
    v->liststore_ranking = GTK_LIST_STORE(gtk_builder_get_object(b, "liststore_ranking"));
    v->txt_filtro = GTK_SEARCH_ENTRY(gtk_builder_get_object(b, "txt_filtro"));
    v->drawing_grafica = GTK_DRAWING_AREA(gtk_builder_get_object(b, "drawing_grafica"));

    // conecto los botones del menu principal 
    g_signal_connect(v->ventana_principal, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(v->btn_ir_listados, "clicked", G_CALLBACK(on_btn_ir_listados_clicked), v);
    g_signal_connect(v->btn_ir_estadisticas, "clicked", G_CALLBACK(on_btn_ir_estadisticas_clicked), v);
    g_signal_connect(v->btn_ir_recomendaciones, "clicked", G_CALLBACK(on_btn_ir_recomendaciones_clicked), v);
    g_signal_connect(v->btn_ir_graficas, "clicked", G_CALLBACK(on_btn_ir_graficas_clicked), v);
    g_signal_connect(v->btn_salir_menu, "clicked", G_CALLBACK(on_btn_salir_clicked), v);

    // botones del panel listados
    btn_buscar = GTK_BUTTON(gtk_builder_get_object(b, "btn_buscar"));
    btn_volver = GTK_BUTTON(gtk_builder_get_object(b, "btn_volver_menu"));
    g_signal_connect(btn_buscar, "clicked", G_CALLBACK(on_btn_buscar_clicked), v);
    g_signal_connect(btn_volver, "clicked", G_CALLBACK(on_btn_volver_menu_clicked), v);
    g_signal_connect(v->txt_filtro, "search-changed", G_CALLBACK(on_txt_filtro_changed), v);

    // botones volver de estadisticas y graficas
    btn_volver_est = GTK_BUTTON(gtk_builder_get_object(b, "btn_volver_menu_estadisticas"));
    btn_volver_gra = GTK_BUTTON(gtk_builder_get_object(b, "btn_volver_menu_graficas"));
    g_signal_connect(btn_volver_est, "clicked", G_CALLBACK(on_btn_volver_menu_clicked), v);
    g_signal_connect(btn_volver_gra, "clicked", G_CALLBACK(on_btn_volver_menu_clicked), v);

    // señal de dibujo para la grafica Cairo
    g_signal_connect(v->drawing_grafica, "draw", G_CALLBACK(on_area_grafica_draw), v);
}

void ventana_liberar(DatosVentana *v) {
    if (v == NULL) {
        return;
    }
    datos_liberar(v->ds);
    free(v);
}

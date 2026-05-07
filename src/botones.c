#include "botones.h"
#include "ventana.h"
#include "datos.h"
#include <stdio.h>
#include <stdlib.h>

/* --- MENU PRINCIPAL --- */

void on_btn_salir_clicked(GtkButton *btn, gpointer datos) {
    gtk_main_quit();
}

void on_btn_ir_listados_clicked(GtkButton *btn, gpointer datos) {
    DatosVentana *v = (DatosVentana *)datos;

    /* relleno el combo con los centros antes de mostrar el panel */
    gtk_combo_box_text_remove_all(v->combo_centros);
    int num = 0;
    char **centros = datos_obtenerCentros(v->ds, &num);

    // TODO: que pasa si centros es NULL aqui?
    for (int i = 0; i < num; i++)
        gtk_combo_box_text_append_text(v->combo_centros, centros[i]);

    if (num > 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(v->combo_centros), 0);

    datos_liberarCentros(centros, num);

    gtk_stack_set_visible_child_name(v->stack_principal, "listados");
}

void on_btn_ir_estadisticas_clicked(GtkButton *btn, gpointer datos) {
    printf("proximamente estadisticas\n");
}

void on_btn_ir_recomendaciones_clicked(GtkButton *btn, gpointer datos) {
    printf("proximamente recomendaciones\n");
}

void on_btn_ir_graficas_clicked(GtkButton *btn, gpointer datos) {
    printf("proximamente graficas\n");
}

/* --- PANEL LISTADOS --- */

/* devuelve TRUE si la fila debe mostrarse segun lo que hay en el buscador */
gboolean filtro_actividades_func(GtkTreeModel *modelo, GtkTreeIter *fila, gpointer datos) {
    DatosVentana *v = (DatosVentana *)datos;
    const gchar *texto = gtk_entry_get_text(GTK_ENTRY(v->txt_filtro));

    if (texto == NULL || texto[0] == '\0')
        return TRUE;

    gchar *nombre;
    gtk_tree_model_get(modelo, fila, 0, &nombre, -1);
    if (nombre == NULL) return FALSE;

    gchar *busqueda = g_utf8_strdown(texto, -1);
    gchar *nombre_min = g_utf8_strdown(nombre, -1);
    gboolean encontrado = (g_strstr_len(nombre_min, -1, busqueda) != NULL);

    g_free(busqueda);
    g_free(nombre_min);
    g_free(nombre);
    return encontrado;
}

void on_btn_volver_menu_clicked(GtkButton *btn, gpointer datos) {
    DatosVentana *v = (DatosVentana *)datos;
    gtk_stack_set_visible_child_name(v->stack_principal, "menu");
}

void on_btn_buscar_clicked(GtkButton *btn, gpointer datos) {
    DatosVentana *v = (DatosVentana *)datos;

    gchar *centro = gtk_combo_box_text_get_active_text(v->combo_centros);
    if (centro == NULL) return;

    gtk_list_store_clear(v->liststore_actividades);

    GtkTreeIter fila;
    for (int i = 0; i < v->ds->total; i++) {
        Actividad *a = &v->ds->actividades[i];
        if (g_strcmp0(a->centro, centro) == 0) {
            gtk_list_store_append(v->liststore_actividades, &fila);
            gtk_list_store_set(v->liststore_actividades, &fila,
                0, a->actividad,
                1, a->franjaHoraria,
                2, a->plazasTotales,
                3, a->ocupacion,
                4, a->modalidad,
                -1);
        }
    }

    g_free(centro);
}

void on_txt_filtro_changed(GtkSearchEntry *entry, gpointer datos) {
    DatosVentana *v = (DatosVentana *)datos;
    if (v->filtro_actividades != NULL)
        gtk_tree_model_filter_refilter(v->filtro_actividades);
}

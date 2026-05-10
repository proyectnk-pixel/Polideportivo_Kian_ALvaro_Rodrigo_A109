#include "botones.h"
#include "ventana.h"
#include "datos.h"
#include "estadisticas.h"
#include <stdio.h>
#include <string.h>

void on_btn_salir_clicked(GtkButton *btn, gpointer datos) {
    (void)btn;
    (void)datos;
    gtk_main_quit();
}

void on_btn_ir_listados_clicked(GtkButton *btn, gpointer datos) {
    DatosVentana *v = (DatosVentana *)datos;
    int num;
    char **centros;
    int i;

    gtk_combo_box_text_remove_all(v->combo_centros);

    num = 0;
    centros = datos_obtenerCentros(v->ds, &num);

    for (i = 0; i < num; i++) {
        gtk_combo_box_text_append_text(v->combo_centros, centros[i]);
    }

    gtk_combo_box_set_active(GTK_COMBO_BOX(v->combo_centros), 0);
    datos_liberarCentros(centros, num);

    gtk_stack_set_visible_child_name(v->stack_principal, "listados");
}

void on_btn_ir_estadisticas_clicked(GtkButton *btn, gpointer datos) {
    DatosVentana *v = (DatosVentana *)datos;
    (void)btn;
    rellenar_ranking_centros(v);
    gtk_stack_set_visible_child_name(v->stack_principal, "estadisticas");
}

void on_btn_ir_recomendaciones_clicked(GtkButton *btn, gpointer datos) {
    (void)btn;
    (void)datos;
    printf("proximamente recomendaciones\n");
}

void on_btn_ir_graficas_clicked(GtkButton *btn, gpointer datos) {
    DatosVentana *v = (DatosVentana *)datos;
    (void)btn;
    gtk_stack_set_visible_child_name(v->stack_principal, "graficas");
    gtk_widget_queue_draw(GTK_WIDGET(v->drawing_grafica));
}

void on_btn_volver_menu_clicked(GtkButton *btn, gpointer datos) {
    DatosVentana *v = (DatosVentana *)datos;
    (void)btn;
    gtk_stack_set_visible_child_name(v->stack_principal, "menu");
}

void on_btn_buscar_clicked(GtkButton *btn, gpointer datos) {
    DatosVentana *v = (DatosVentana *)datos;
    (void)btn;
    gchar *centro;
    GtkTreeIter fila;
    char pct[10];
    int i;

    centro = gtk_combo_box_text_get_active_text(v->combo_centros);
    if (centro == NULL) {
        return;
    }

    gtk_list_store_clear(v->liststore_actividades);

    for (i = 0; i < v->ds->total; i++) {
        if (g_strcmp0(v->ds->actividades[i].centro, centro) == 0) {
            sprintf(pct, "%.0f%%", v->ds->actividades[i].ocupacion * 100);
            gtk_list_store_append(v->liststore_actividades, &fila);
            gtk_list_store_set(v->liststore_actividades, &fila,
                0, v->ds->actividades[i].actividad,
                1, v->ds->actividades[i].franjaHoraria,
                2, v->ds->actividades[i].plazasTotales,
                3, pct,
                4, v->ds->actividades[i].modalidad,
                -1);
        }
    }

    g_free(centro);
}

/* filtra la tabla segun lo que escribe el usuario en el buscador */
void on_txt_filtro_changed(GtkSearchEntry *entry, gpointer datos) {
    DatosVentana *v = (DatosVentana *)datos;
    (void)entry;
    const gchar *texto;
    GtkTreeIter fila;
    char pct[10];
    int i;

    texto = gtk_entry_get_text(GTK_ENTRY(v->txt_filtro));

    // si esta vacio vuelvo a cargar todo desde el combo seleccionado
    if (texto == NULL || texto[0] == '\0') {
        on_btn_buscar_clicked(NULL, datos);
        return;
    }

    gtk_list_store_clear(v->liststore_actividades);

    for (i = 0; i < v->ds->total; i++) {
        if (strstr(v->ds->actividades[i].actividad, texto) != NULL) {
            sprintf(pct, "%.0f%%", v->ds->actividades[i].ocupacion * 100);
            gtk_list_store_append(v->liststore_actividades, &fila);
            gtk_list_store_set(v->liststore_actividades, &fila,
                0, v->ds->actividades[i].actividad,
                1, v->ds->actividades[i].franjaHoraria,
                2, v->ds->actividades[i].plazasTotales,
                3, pct,
                4, v->ds->actividades[i].modalidad,
                -1);
        }
    }
}


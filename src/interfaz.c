#include "interfaz.h"
#include <stdlib.h>
#include <stdio.h>

// =========================================================
// ZONA 1: CALLBACKS (Tus botones y eventos)
// =========================================================

void on_btn_salir_clicked(GtkButton *button, gpointer user_data) {
    AppContext *ctx = (AppContext *)user_data;
    // La memoria del ds la liberamos en main.c ahora, solo salimos.
    gtk_main_quit();
}

void on_btn_ir_listados_clicked(GtkButton *button, gpointer user_data) {
    AppContext *ctx = (AppContext *)user_data;
    gtk_stack_set_visible_child_name(ctx->stack_principal, "listados");
}

void on_btn_buscar_clicked(GtkButton *button, gpointer user_data) {
    AppContext *ctx = (AppContext *)user_data;
    gchar *centro = gtk_combo_box_text_get_active_text(ctx->combo_centros);
    if (!centro) return; 

    int num = 0;
    Actividad **lista = datos_obtenerActividadesPorCentro(ctx->ds, centro, &num);
    gtk_list_store_clear(ctx->liststore_actividades);

    if (lista) {
        GtkTreeIter iter;
        for (int i = 0; i < num; i++) {
            gtk_list_store_append(ctx->liststore_actividades, &iter); 
            gtk_list_store_set(ctx->liststore_actividades, &iter,
                0, lista[i]->actividad,
                1, lista[i]->franjaHoraria,
                2, lista[i]->plazasTotales, 
                3, lista[i]->ocupacion, -1); 
        }
        free(lista); 
    }
    g_free(centro); 
}

void on_window_main_destroy(GtkWidget *widget, gpointer user_data) {
    gtk_main_quit();
}


// =========================================================
// ZONA 2: CONSTRUCTOR DE LA INTERFAZ
// =========================================================

int iniciar_interfaz(Dataset *ds) {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;

    // Indicamos que entre en la carpeta ui y lea menu.glade
    if (gtk_builder_add_from_file(builder, "ui/menu.glade", &error) == 0) {
        printf("Error fatal en Glade: %s\n", error->message);
        g_clear_error(&error);
        return 0; // Falla
    }

    // Reservamos el contexto
    AppContext *ctx = malloc(sizeof(AppContext));
    if (!ctx) return 0;
    ctx->ds = ds;

    // Vinculamos widgets
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "ventana_principal"));
    ctx->stack_principal = GTK_STACK(gtk_builder_get_object(builder, "stack_principal"));
    ctx->lbl_estado_carga = GTK_LABEL(gtk_builder_get_object(builder, "lbl_estado_carga"));
    ctx->combo_centros = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "combo_centros"));
    ctx->liststore_actividades = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore_actividades"));

    // Rellenamos el combo de centros (La estantería que hablamos antes)
    if (ctx->combo_centros != NULL && ds->total > 0) {
        int num_centros = 0;
        char **lista_centros = datos_obtenerCentros(ds, &num_centros);
        for (int i = 0; i < num_centros; i++) {
            gtk_combo_box_text_append_text(ctx->combo_centros, lista_centros[i]);
        }
        datos_liberarCentros(lista_centros, num_centros);
        if (num_centros > 0) gtk_combo_box_set_active(GTK_COMBO_BOX(ctx->combo_centros), 0);
        
        // Decimos que todo fue bien y enseñamos el menú
        gtk_stack_set_visible_child_name(ctx->stack_principal, "menu");
        char msj[100];
        snprintf(msj, sizeof(msj), "Estado: %d actividades cargadas", ds->total);
        gtk_label_set_text(ctx->lbl_estado_carga, msj);
    } else {
        gtk_stack_set_visible_child_name(ctx->stack_principal, "error");
    }

    // Conectamos todo
    gtk_builder_connect_signals(builder, ctx); 
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_main_destroy), ctx);

    g_object_unref(builder);
    gtk_widget_show_all(window);
    
    return 1; // Éxito
}
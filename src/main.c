#include <gtk/gtk.h>
#include <stdio.h>
#include "ventana.h"
#include "datos.h"

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    DatosVentana *v;
    int registros;

    gtk_init(&argc, &argv);

    /* cargo la interfaz desde el archivo glade */
    builder = gtk_builder_new_from_file("ui/menu.glade");
    if (builder == NULL) {
        printf("Error: no se pudo abrir el archivo glade\n");
        return 1;
    }

    v = ventana_crear();
    if (v == NULL) {
        printf("Error al reservar memoria\n");
        return 1;
    }

    ventana_inicializar(v, builder);

    // cargo el CSV
    v->ds = datos_crear();
    registros = datos_cargarCSV(v->ds, "data/dataset.csv");

    if (registros <= 0) {
        printf("Error al cargar el dataset\n");
        gtk_label_set_text(v->lbl_estado_carga, "Error al cargar datos");
    } else {
        printf("Dataset cargado: %d registros\n", registros);
        gtk_label_set_text(v->lbl_estado_carga, "Datos cargados correctamente");
        gtk_stack_set_visible_child_name(v->stack_principal, "menu");
    }

    gtk_widget_show_all(GTK_WIDGET(v->ventana_principal));
    gtk_main();

    ventana_liberar(v);
    g_object_unref(builder);
    return 0;
}


#include "interfaz.h"
#include <stdlib.h>
#include <stdio.h>

// 1. CREAMOS LA MACRO (El truco para Windows)
#define EVENTO G_MODULE_EXPORT void


// =========================================================
// ZONA DE FILTRADO (El "portero" de la tabla)
// =========================================================

gboolean filtro_actividades_func(GtkTreeModel *model, GtkTreeIter *iter, gpointer data) {
    AppContext *ctx = (AppContext *)data;
    
    // 1. Obtenemos lo que el usuario ha escrito en el buscador
    const gchar *texto_busqueda = gtk_entry_get_text(GTK_ENTRY(ctx->txt_filtro));
    
    // Si el buscador está vacío, dejamos pasar todas las filas
    if (texto_busqueda == NULL || texto_busqueda[0] == '\0') {
        return TRUE; 
    }

    // 2. Leemos el nombre de la actividad de la fila que estamos evaluando (Columna 0)
    gchar *nombre_actividad;
    gtk_tree_model_get(model, iter, 0, &nombre_actividad, -1);

    // Si por algún error la fila no tiene nombre, la ocultamos
    if (nombre_actividad == NULL) {
        return FALSE;
    }

    // 3. Pasamos ambos textos a minúsculas para compararlos sin problemas de Mayus/Minus
    gchar *busqueda_min = g_utf8_strdown(texto_busqueda, -1);
    gchar *actividad_min = g_utf8_strdown(nombre_actividad, -1);

    // 4. Comprobamos si el texto buscado ESTÁ DENTRO del nombre de la actividad
    gboolean coincide = (g_strstr_len(actividad_min, -1, busqueda_min) != NULL);

    // 5. Liberamos la memoria temporal de los textos
    g_free(busqueda_min);
    g_free(actividad_min);
    g_free(nombre_actividad);

    // 6. El portero da su veredicto: TRUE (se muestra) o FALSE (se oculta)
    return coincide;
}

// =========================================================
// ZONA 1: CALLBACKS (Tus botones y eventos)
// =========================================================

EVENTO on_btn_salir_clicked(GtkButton *button, gpointer user_data) {
    AppContext *ctx = (AppContext *)user_data;
    gtk_main_quit();
}

EVENTO on_btn_ir_listados_clicked(GtkButton *button, gpointer user_data) {
    AppContext *ctx = (AppContext *)user_data;
    gtk_stack_set_visible_child_name(ctx->stack_principal, "listados");
    printf("Usuario ha entrado a la sección de listados.\n");
}

EVENTO on_btn_volver_menu_clicked(GtkButton *button, gpointer user_data) {
    AppContext *ctx = (AppContext *)user_data;
    gtk_stack_set_visible_child_name(ctx->stack_principal, "menu");
}

EVENTO on_btn_buscar_clicked(GtkButton *button, gpointer user_data) {
    // 1. Recuperamos nuestra estructura con todos los punteros
    AppContext *ctx = (AppContext *)user_data;

    // 2. Leemos qué texto ha seleccionado el usuario en el combo box
    gchar *centro = gtk_combo_box_text_get_active_text(ctx->combo_centros);
    if (centro == NULL) {
        return; // Si no hay nada seleccionado, no hacemos nada
    }

    // 3. Llamamos a tu función de datos.c para que busque las coincidencias
    int num = 0;
    Actividad **lista = datos_obtenerActividadesPorCentro(ctx->ds, centro, &num);

    // 4. Limpiamos la tabla (Excel interno) por si había datos de una búsqueda anterior
    gtk_list_store_clear(ctx->liststore_actividades);

    // 5. Si hemos encontrado actividades, las metemos en la tabla una a una
    if (lista != NULL) {
        GtkTreeIter iter; // Un iterador es como el "cursor" que apunta a la fila actual
        for (int i = 0; i < num; i++) {
            // Añadimos una fila vacía
            gtk_list_store_append(ctx->liststore_actividades, &iter); 
            
            // Rellenamos las columnas de esa fila con los datos del puntero
            gtk_list_store_set(ctx->liststore_actividades, &iter,
                0, lista[i]->actividad,
                1, lista[i]->franjaHoraria,
                2, lista[i]->plazasTotales, 
                3, lista[i]->ocupacion, 
                4, "Provisional",
                -1); // IMPORTANTE: Este -1 le indica a GTK que ya no hay más columnas que rellenar
        }
        
        // Liberamos la memoria del array auxiliar de punteros (pero NO las actividades, que siguen en ds)
        free(lista); 
    }

    // 6. Liberamos la memoria del texto que nos dio GTK en el paso 2
    g_free(centro); 
}

EVENTO on_txt_filtro_changed(GtkSearchEntry *entry, gpointer user_data) {
    AppContext *ctx = (AppContext *)user_data;
    
    // Le decimos al filtro: "¡Eh! El texto ha cambiado, vuelve a revisar las filas"
    if (ctx->filtro_actividades != NULL) {
        gtk_tree_model_filter_refilter(ctx->filtro_actividades);
    }
}

EVENTO on_window_main_destroy(GtkWidget *widget, gpointer user_data) {
    gtk_main_quit();
}

EVENTO on_btn_ir_estadisticas_clicked(GtkButton *button, gpointer user_data) {
    printf("Próximamente: Pantalla de Estadísticas\n");
}

EVENTO on_btn_ir_recomendaciones_clicked(GtkButton *button, gpointer user_data) {
    printf("Próximamente: Pantalla de Recomendaciones\n");
}

EVENTO on_btn_ir_graficas_clicked(GtkButton *button, gpointer user_data) {
    printf("Próximamente: Pantalla de Gráficas\n");
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

    // 1. Vinculamos widgets principales
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "ventana_principal"));
    ctx->stack_principal = GTK_STACK(gtk_builder_get_object(builder, "stack_principal"));
    ctx->lbl_estado_carga = GTK_LABEL(gtk_builder_get_object(builder, "lbl_estado_carga"));
    ctx->combo_centros = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "combo_centros"));
    ctx->liststore_actividades = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore_actividades"));

    // 2. Vinculamos los nuevos widgets para el filtro de búsqueda
    ctx->txt_filtro = GTK_SEARCH_ENTRY(gtk_builder_get_object(builder, "txt_filtro"));
    GtkTreeView *tree_actividades = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tree_actividades"));

    // 3. Configuramos el "portero" (Filtro)
    // Creamos el filtro conectándolo al liststore original
    ctx->filtro_actividades = GTK_TREE_MODEL_FILTER(gtk_tree_model_filter_new(GTK_TREE_MODEL(ctx->liststore_actividades), NULL));
    
    // Le asignamos la función que decidirá qué filas se ven
    gtk_tree_model_filter_set_visible_func(ctx->filtro_actividades, filtro_actividades_func, ctx, NULL);
    
    // Conectamos la tabla visual al filtro (y no directamente al liststore)
    gtk_tree_view_set_model(tree_actividades, GTK_TREE_MODEL(ctx->filtro_actividades));
    
    // Buena práctica en GTK: soltar la referencia inicial porque el TreeView ya se encarga de ella
    g_object_unref(ctx->filtro_actividades);

    // 4. Rellenamos el combo de centros
    if (ctx->combo_centros != NULL && ds->total > 0) {
        int num_centros = 0;
        char **lista_centros = datos_obtenerCentros(ds, &num_centros);
        for (int i = 0; i < num_centros; i++) {
            gtk_combo_box_text_append_text(ctx->combo_centros, lista_centros[i]);
        }
        datos_liberarCentros(lista_centros, num_centros);
        
        // Seleccionamos el primer centro por defecto
        if (num_centros > 0) {
            gtk_combo_box_set_active(GTK_COMBO_BOX(ctx->combo_centros), 0);
        }
        
        // Decimos que todo fue bien y enseñamos el menú
        gtk_stack_set_visible_child_name(ctx->stack_principal, "menu");
        char msj[100];
        snprintf(msj, sizeof(msj), "Estado: %d actividades cargadas", ds->total);
        gtk_label_set_text(ctx->lbl_estado_carga, msj);
    } else {
        // Si hay error en los datos, mostramos el panel de error
        gtk_stack_set_visible_child_name(ctx->stack_principal, "error");
    }

    // 5. Conectamos todas las señales (Botones y destrucción de ventana)
    gtk_builder_connect_signals(builder, ctx); 
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_main_destroy), ctx);

    // Liberamos el builder de Glade (ya no lo necesitamos)
    g_object_unref(builder);
    
    // Mostramos la ventana y todo su contenido
    gtk_widget_show_all(window);
    
    return 1; // Éxito
}
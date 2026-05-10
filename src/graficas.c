#include "graficas.h"
#include "ventana.h"
#include "datos.h"
#include <stdio.h>
#include <string.h>

// GTK llama a esta función cada vez que necesita redibujar el area de graficas
void on_area_grafica_draw(GtkWidget *widget, cairo_t *cr, gpointer datos) {
    DatosVentana *v = (DatosVentana *)datos;
    int ancho;
    int alto;

    (void)widget;

    ancho = gtk_widget_get_allocated_width(GTK_WIDGET(v->drawing_grafica));
    alto  = gtk_widget_get_allocated_height(GTK_WIDGET(v->drawing_grafica));

    dibujar_barras_actividades(cr, ancho, alto, v);
}
// Dibuja una barra horizontal por cada actividad del centro seleccionado
void dibujar_barras_actividades(cairo_t *cr, int ancho, int alto, DatosVentana *v) {
    int i;
    char etiqueta[20];
    char pct[8];
    int margen_izq;
    int margen_top;
    int alto_barra;
    int espacio;
    int ancho_max;
    int y;
    int w;
    char *centro;
    // Fondo blanco(el predefinido)
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    if (v == NULL || v->ds == NULL) {
        return;
    }

     // Lee el centro que esta seleccioando en el combo(el mismo que en listados)
    centro = gtk_combo_box_text_get_active_text(v->combo_centros);

    if (centro == NULL) {
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 13.0);
        cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
        cairo_move_to(cr, 20, 40);
        cairo_show_text(cr, "Selecciona un centro en Listados primero");
        return;
    }
// Configuración de la grafica, margenes, tamaño de barra y espacio entre barras.
    margen_izq = 200;
    margen_top = 50;
    alto_barra = 24;
    espacio    = 6;
    ancho_max  = ancho - margen_izq - 60;

    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);

    // Título con el nombre del centro
    cairo_set_font_size(cr, 13.0);
    cairo_set_source_rgb(cr, 0.1, 0.2, 0.5);
    cairo_move_to(cr, margen_izq, 25);
    cairo_show_text(cr, centro);

    cairo_set_font_size(cr, 10.0);

    y = margen_top;

    for (i = 0; i < v->ds->total; i++) {
        //  Únicamente las actividades del centro seleccionado
        if (strcmp(v->ds->actividades[i].centro, centro) != 0) {
            continue;
        }

        strncpy(etiqueta, v->ds->actividades[i].actividad, 19);
        etiqueta[19] = '\0';

        // nombre 
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_move_to(cr, 5, y + alto_barra - 6);
        cairo_show_text(cr, etiqueta);

        // barra horizontal a la ocupacion de la actividad
        w = (int)(v->ds->actividades[i].ocupacion * ancho_max);
        cairo_set_source_rgb(cr, 0.2, 0.4, 0.8);
        cairo_rectangle(cr, margen_izq, y, w, alto_barra);
        cairo_fill(cr);

        // porcentaje 
        sprintf(pct, "%.0f%%", v->ds->actividades[i].ocupacion * 100);
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_move_to(cr, margen_izq + w + 4, y + alto_barra - 6);
        cairo_show_text(cr, pct);

        y += alto_barra + espacio;
    }

    g_free(centro);    // libero el string del combo
    (void)alto;
}

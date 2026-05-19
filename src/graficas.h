#ifndef GRAFICAS_H
#define GRAFICAS_H

#include <gtk/gtk.h>
#include <cairo.h>
#include "ventana.h"

// callback, llama GTK cuando hay que redibujar el area 
void on_area_grafica_draw(GtkWidget *widget, cairo_t *cr, gpointer datos);

// barras ocupacion media por centro 
void dibujar_barras_centros(cairo_t *cr, int ancho, int alto, DatosVentana *v);

// barras de ocupacion media por dia de la semana 
void dibujar_barras_dias(cairo_t *cr, int ancho, int alto, DatosVentana *v);

#endif

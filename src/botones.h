#ifndef BOTONES_H
#define BOTONES_H

#include <gtk/gtk.h>

/* botones del menu principal */
void on_btn_salir_clicked              (GtkButton *btn, gpointer datos);
void on_btn_ir_listados_clicked        (GtkButton *btn, gpointer datos);
void on_btn_ir_estadisticas_clicked    (GtkButton *btn, gpointer datos);
void on_btn_ir_recomendaciones_clicked (GtkButton *btn, gpointer datos);
void on_btn_ir_graficas_clicked        (GtkButton *btn, gpointer datos);

/* panel listados */
gboolean filtro_actividades_func (GtkTreeModel *modelo, GtkTreeIter *fila, gpointer datos);
void on_btn_buscar_clicked        (GtkButton *btn,        gpointer datos);
void on_btn_volver_menu_clicked   (GtkButton *btn,        gpointer datos);
void on_txt_filtro_changed        (GtkSearchEntry *entry, gpointer datos);

#endif

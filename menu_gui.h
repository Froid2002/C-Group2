#ifndef MENU_GUI_H
#define MENU_GUI_H

#include <gtk/gtk.h>

// Function declarations
void show_main_menu_window();
void display_results(const char* results);
void on_search_vibe_clicked(GtkButton *button, gpointer user_data);
void on_search_location_clicked(GtkButton *button, gpointer user_data);
void on_search_name_clicked(GtkButton *button, gpointer user_data);
void on_add_shop_clicked(GtkButton *button, gpointer user_data);
void on_submit_add_shop(GtkButton *button, gpointer user_data);

#endif // MENU_GUI_H 
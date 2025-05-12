#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gtk/gtk.h>

GtkWidget *entry_username, *entry_password, *label_status;
GtkWidget *login_window; // global for login window

// ───────────────────────────────────────────────
// MAIN MENU GUI WINDOW (NO gtk_main() here)
void show_main_menu_window() {
    GtkWidget *menu_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(menu_window), "Main Menu");
    gtk_window_set_default_size(GTK_WINDOW(menu_window), 400, 300);
    g_signal_connect(menu_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(menu_window), grid);

    GtkWidget *label = gtk_label_new("☕ Select an option:");
    GtkWidget *button_vibe = gtk_button_new_with_label("Search by Vibe");
    GtkWidget *button_location = gtk_button_new_with_label("Search by Location");
    GtkWidget *button_name = gtk_button_new_with_label("Search by Name");
    GtkWidget *button_add = gtk_button_new_with_label("Add a New Coffee Shop");
    GtkWidget *button_exit = gtk_button_new_with_label("Exit");

    gtk_grid_attach(GTK_GRID(grid), label,           0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_vibe,     0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_location, 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_name,     0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_add,      0, 4, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_exit,     0, 5, 2, 1);

    g_signal_connect(button_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(menu_window);
}

// ───────────────────────────────────────────────
// LOGIN SYSTEM

int login_gui(const char* username, const char* password) {
    FILE *fp = fopen("users.txt", "r");
    if (!fp) return 0;
    char file_user[100], file_pass[100];
    while (fscanf(fp, "%[^,],%s\n", file_user, file_pass) == 2) {
        if (strcmp(username, file_user) == 0 && strcmp(password, file_pass) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int user_exists_gui(const char* username) {
    FILE *fp = fopen("users.txt", "r");
    if (!fp) return 0;
    char file_user[100], file_pass[100];
    while (fscanf(fp, "%[^,],%s\n", file_user, file_pass) == 2) {
        if (strcmp(username, file_user) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void register_user_gui(const char* username, const char* password) {
    if (user_exists_gui(username)) {
        gtk_label_set_text(GTK_LABEL(label_status), "❌ Username already exists.");
        return;
    }
    FILE *fp = fopen("users.txt", "a");
    if (!fp) {
        gtk_label_set_text(GTK_LABEL(label_status), "❌ Failed to open users.txt");
        return;
    }
    fprintf(fp, "%s,%s\n", username, password);
    fclose(fp);
    gtk_label_set_text(GTK_LABEL(label_status), "✅ Registration successful!");
}

// ───────────────────────────────────────────────
// EVENT HANDLERS

void on_login_clicked(GtkButton *button, gpointer user_data) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(entry_username));
    const char *password = gtk_entry_get_text(GTK_ENTRY(entry_password));

    if (login_gui(username, password)) {
        gtk_label_set_text(GTK_LABEL(label_status), "✅ Login successful!");
        gtk_widget_hide(login_window);   // hide login
        show_main_menu_window();         // show menu in same gtk_main loop
    } else {
        gtk_label_set_text(GTK_LABEL(label_status), "❌ Invalid username or password.");
    }
}

void on_register_clicked(GtkButton *button, gpointer user_data) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(entry_username));
    const char *password = gtk_entry_get_text(GTK_ENTRY(entry_password));
    register_user_gui(username, password);
}

// ───────────────────────────────────────────────
// LOGIN WINDOW GUI

void promptLogin() {
    login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(login_window), "Login/Register");
    gtk_window_set_default_size(GTK_WINDOW(login_window), 300, 200);
    g_signal_connect(login_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 15);
    gtk_container_add(GTK_CONTAINER(login_window), grid);

    GtkWidget *label_user = gtk_label_new("Username:");
    entry_username = gtk_entry_new();

    GtkWidget *label_pass = gtk_label_new("Password:");
    entry_password = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry_password), FALSE);

    GtkWidget *button_login = gtk_button_new_with_label("Login");
    GtkWidget *button_register = gtk_button_new_with_label("Register");
    label_status = gtk_label_new("");

    gtk_grid_attach(GTK_GRID(grid), label_user,     0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_username, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_pass,     0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_password, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_login,   0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_register,1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_status,   0, 3, 2, 1);

    g_signal_connect(button_login, "clicked", G_CALLBACK(on_login_clicked), NULL);
    g_signal_connect(button_register, "clicked", G_CALLBACK(on_register_clicked), NULL);

    gtk_widget_show_all(login_window);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "coffee_shops.h"

// Global variables for menu window
GtkWidget *menu_window;
GtkWidget *entry_search;
GtkWidget *text_view_results;

// Function to display search results in the text view
void display_results(const char* results) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_results));
    gtk_text_buffer_set_text(buffer, results, -1);
}

// Callback for search by vibe button
void on_search_vibe_clicked(GtkButton *button, gpointer user_data) {
    const char *vibe = gtk_entry_get_text(GTK_ENTRY(entry_search));
    if (strlen(vibe) == 0) {
        display_results("Please enter a vibe to search for.");
        return;
    }
    
    // Create a temporary file to capture printf output
    FILE *temp = tmpfile();
    if (!temp) {
        display_results("Error creating temporary file.");
        return;
    }
    
    // Redirect stdout to the temporary file
    FILE *original_stdout = stdout;
    stdout = temp;
    
    // Call the search function
    displayCoffeeShopsByVibe(vibe);
    
    // Restore stdout
    stdout = original_stdout;
    
    // Read the temporary file
    char results[4096] = {0};
    rewind(temp);
    size_t len = fread(results, 1, sizeof(results) - 1, temp);
    results[len] = '\0';
    fclose(temp);
    
    display_results(results);
}

// Callback for search by location button
void on_search_location_clicked(GtkButton *button, gpointer user_data) {
    const char *location = gtk_entry_get_text(GTK_ENTRY(entry_search));
    if (strlen(location) == 0) {
        display_results("Please enter a location to search for.");
        return;
    }
    
    FILE *temp = tmpfile();
    if (!temp) {
        display_results("Error creating temporary file.");
        return;
    }
    
    FILE *original_stdout = stdout;
    stdout = temp;
    
    searchByLocation(location);
    
    stdout = original_stdout;
    
    char results[4096] = {0};
    rewind(temp);
    size_t len = fread(results, 1, sizeof(results) - 1, temp);
    results[len] = '\0';
    fclose(temp);
    
    display_results(results);
}

// Callback for search by name button
void on_search_name_clicked(GtkButton *button, gpointer user_data) {
    const char *name = gtk_entry_get_text(GTK_ENTRY(entry_search));
    if (strlen(name) == 0) {
        display_results("Please enter a name to search for.");
        return;
    }
    
    FILE *temp = tmpfile();
    if (!temp) {
        display_results("Error creating temporary file.");
        return;
    }
    
    FILE *original_stdout = stdout;
    stdout = temp;
    
    searchByName(name);
    
    stdout = original_stdout;
    
    char results[4096] = {0};
    rewind(temp);
    size_t len = fread(results, 1, sizeof(results) - 1, temp);
    results[len] = '\0';
    fclose(temp);
    
    display_results(results);
}

// Callback for add coffee shop button
void on_add_shop_clicked(GtkButton *button, gpointer user_data) {
    // Create a new window for adding a coffee shop
    GtkWidget *add_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(add_window), "Add New Coffee Shop");
    gtk_window_set_default_size(GTK_WINDOW(add_window), 400, 300);
    
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(add_window), grid);
    
    // Create entry fields
    GtkWidget *entry_name = gtk_entry_new();
    GtkWidget *entry_vibe = gtk_entry_new();
    GtkWidget *entry_location = gtk_entry_new();
    
    // Create labels
    GtkWidget *label_name = gtk_label_new("Name:");
    GtkWidget *label_vibe = gtk_label_new("Vibe:");
    GtkWidget *label_location = gtk_label_new("Location:");
    
    // Create submit button
    GtkWidget *button_submit = gtk_button_new_with_label("Add Shop");
    
    // Add widgets to grid
    gtk_grid_attach(GTK_GRID(grid), label_name, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_name, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_vibe, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_vibe, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_location, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_location, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_submit, 0, 3, 2, 1);
    
    // Connect submit button callback
    g_signal_connect(button_submit, "clicked", G_CALLBACK(on_submit_add_shop), 
                    (gpointer)add_window);
    
    gtk_widget_show_all(add_window);
}

// Callback for submit button in add shop window
void on_submit_add_shop(GtkButton *button, gpointer user_data) {
    GtkWidget *add_window = (GtkWidget *)user_data;
    GtkWidget *grid = gtk_bin_get_child(GTK_BIN(add_window));
    
    // Get entry widgets
    GtkWidget *entry_name = gtk_grid_get_child_at(GTK_GRID(grid), 1, 0);
    GtkWidget *entry_vibe = gtk_grid_get_child_at(GTK_GRID(grid), 1, 1);
    GtkWidget *entry_location = gtk_grid_get_child_at(GTK_GRID(grid), 1, 2);
    
    const char *name = gtk_entry_get_text(GTK_ENTRY(entry_name));
    const char *vibe = gtk_entry_get_text(GTK_ENTRY(entry_vibe));
    const char *location = gtk_entry_get_text(GTK_ENTRY(entry_location));
    
    if (strlen(name) == 0 || strlen(vibe) == 0 || strlen(location) == 0) {
        display_results("Error: All fields must be filled.");
        return;
    }
    
    if (coffeeShopExists(name)) {
        display_results("Error: This coffee shop already exists.");
        return;
    }
    
    addCoffeeShop(name, vibe, location);
    saveToFile("coffee_shops.txt");
    display_results("Coffee shop added successfully!");
    
    gtk_widget_destroy(add_window);
}

// Function to show the main menu window
void show_main_menu_window() {
    menu_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(menu_window), "Seoul Coffee Shop Finder");
    gtk_window_set_default_size(GTK_WINDOW(menu_window), 600, 400);
    g_signal_connect(menu_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Create main container
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(menu_window), grid);
    
    // Create search entry
    entry_search = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_search), "Enter search term...");
    
    // Create buttons
    GtkWidget *button_vibe = gtk_button_new_with_label("Search by Vibe");
    GtkWidget *button_location = gtk_button_new_with_label("Search by Location");
    GtkWidget *button_name = gtk_button_new_with_label("Search by Name");
    GtkWidget *button_add = gtk_button_new_with_label("Add New Coffee Shop");
    
    // Create text view for results
    text_view_results = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view_results), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view_results), FALSE);
    
    // Create scrolled window for text view
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view_results);
    
    // Add widgets to grid
    gtk_grid_attach(GTK_GRID(grid), entry_search, 0, 0, 4, 1);
    gtk_grid_attach(GTK_GRID(grid), button_vibe, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_location, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_name, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_add, 3, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 2, 4, 1);
    
    // Connect button signals
    g_signal_connect(button_vibe, "clicked", G_CALLBACK(on_search_vibe_clicked), NULL);
    g_signal_connect(button_location, "clicked", G_CALLBACK(on_search_location_clicked), NULL);
    g_signal_connect(button_name, "clicked", G_CALLBACK(on_search_name_clicked), NULL);
    g_signal_connect(button_add, "clicked", G_CALLBACK(on_add_shop_clicked), NULL);
    
    // Load coffee shops data
    loadFromFile("coffee_shops.txt");
    
    gtk_widget_show_all(menu_window);
} 
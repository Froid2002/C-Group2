#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gtk/gtk.h>
#include "menu_gui.h"
#include "uthash.h"  // For hash table support

void run_coffee_shop_gui();
void promptLogin();


typedef struct CoffeeShop {
    char name[100];
    char vibe[50];
    char location[100];
    struct CoffeeShop* next;
    UT_hash_handle hh;  // For hash table
} CoffeeShop;

CoffeeShop* head = NULL;
CoffeeShop* shopTable = NULL;  // Hash table head

void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void addCoffeeShop(const char* name, const char* vibe, const char* location) {
    CoffeeShop* newShop = (CoffeeShop*)malloc(sizeof(CoffeeShop));
    strncpy(newShop->name, name, sizeof(newShop->name) - 1);
    newShop->name[sizeof(newShop->name) - 1] = '\0';

    strncpy(newShop->vibe, vibe, sizeof(newShop->vibe) - 1);
    newShop->vibe[sizeof(newShop->vibe) - 1] = '\0';

    strncpy(newShop->location, location, sizeof(newShop->location) - 1);
    newShop->location[sizeof(newShop->location) - 1] = '\0';

    newShop->next = NULL;

    if (head == NULL) {
        HASH_ADD_STR(shopTable, name, newShop);  // Add to hash

        head = newShop;
    } else {
        CoffeeShop* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newShop;
    }
}

int coffeeShopExists(const char* name) {
    CoffeeShop* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

void displayCoffeeShopsByVibe(const char* vibe) {
    CoffeeShop* temp = head;
    int found = 0;

    char tempVibe[50];
    char inputVibe[50];
    strncpy(inputVibe, vibe, sizeof(inputVibe));
    inputVibe[sizeof(inputVibe) - 1] = '\0';
    toLowerCase(inputVibe);

    printf("\nFamous coffee shops with a %s vibe in Seoul:\n", vibe);
    while (temp != NULL) {
        strncpy(tempVibe, temp->vibe, sizeof(tempVibe));
        tempVibe[sizeof(tempVibe) - 1] = '\0';
        toLowerCase(tempVibe);

        if (strcmp(tempVibe, inputVibe) == 0) {
            printf("- %s (Location: %s)\n", temp->name, temp->location);
            found = 1;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("No coffee shops found with the '%s' vibe.\n", vibe);
    }
}

void searchByLocation(const char* location) {
    CoffeeShop* temp = head;
    int found = 0;

    char tempLocation[100];
    char inputLocation[100];
    strncpy(inputLocation, location, sizeof(inputLocation));
    inputLocation[sizeof(inputLocation) - 1] = '\0';
    toLowerCase(inputLocation);

    printf("\nCoffee shops in %s:\n", location);
    while (temp != NULL) {
        strncpy(tempLocation, temp->location, sizeof(tempLocation));
        tempLocation[sizeof(tempLocation) - 1] = '\0';
        toLowerCase(tempLocation);

        if (strstr(tempLocation, inputLocation) != NULL) {
            printf("- %s (Vibe: %s)\n", temp->name, temp->vibe);
            found = 1;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("No coffee shops found in '%s'.\n", location);
    }
}

void searchByName(const char* name) {
    CoffeeShop* temp = head;
    int found = 0;

    char tempName[100];
    char inputName[100];
    strncpy(inputName, name, sizeof(inputName));
    inputName[sizeof(inputName) - 1] = '\0';
    toLowerCase(inputName);

    printf("\nSearch results for '%s':\n", name);
    while (temp != NULL) {
        strncpy(tempName, temp->name, sizeof(tempName));
        tempName[sizeof(tempName) - 1] = '\0';
        toLowerCase(tempName);

        if (strstr(tempName, inputName) != NULL) {
            printf("- %s (Vibe: %s, Location: %s)\n", temp->name, temp->vibe, temp->location);
            found = 1;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("No coffee shops found with the name '%s'.\n", name);
    }
}

void saveToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    CoffeeShop* temp = head;
    while (temp != NULL) {
        fprintf(file, "%s,%s,%s\n", temp->name, temp->vibe, temp->location);
        temp = temp->next;
    }

    fclose(file);
    printf("Data saved to %s successfully.\n", filename);
}

void loadFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    char name[100], vibe[50], location[100];
    int count = 0;

    while (fscanf(file, "%[^,],%[^,],%[^\n]\n", name, vibe, location) == 3) {
        addCoffeeShop(name, vibe, location);
        count++;
    }

    fclose(file);

    if (count == 0) {
        printf("No data found in %s.\n", filename);
    } else {
        printf("Data loaded from %s successfully.\n", filename);
    }
}

void freeCoffeeShops() {
    CoffeeShop *current, *tmp;

    HASH_ITER(hh, shopTable, current, tmp) {
        HASH_DEL(shopTable, current);
        free(current);
    }

    CoffeeShop* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void displayMenu() {
    printf("\n--- Seoul Coffee Shop Finder ---\n");
    printf("1. Search by vibe\n");
    printf("2. Search by location\n");
    printf("3. Search by name\n");
    printf("4. Add a new coffee shop\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    promptLogin();          // runs gtk_main() until login window closes
    show_main_menu_window(); // starts new gtk_main() for main menu
    gtk_main();            // start the GTK main loop
    return 0;
}
CoffeeShop* findCoffeeShopByName(const char* name) {
    CoffeeShop* s = NULL;
    HASH_FIND_STR(shopTable, name, s);
    return s;
}
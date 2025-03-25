#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100

typedef struct {
    int id;
    char title[100];
    char author[100];
    int year;
} Book;

Book library[MAX_BOOKS];
int book_count = 0;

GtkWidget *window;
GtkWidget *book_list;

void add_book(const char *title, const char *author, int year) {
    if (book_count >= MAX_BOOKS) {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Library is full! Cannot add more books."
        );
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    Book new_book;
    new_book.id = book_count + 1;
    strncpy(new_book.title, title, 99);
    strncpy(new_book.author, author, 99);
    new_book.year = year;

    library[book_count] = new_book;
    book_count++;
}

void remove_book(int id) {
    for (int i = 0; i < book_count; i++) {
        if (library[i].id == id) {
            for (int j = i; j < book_count - 1; j++) {
                library[j] = library[j + 1];
            }
            book_count--;
            return;
        }
    }
}

void display_all_books() {
    GtkListStore *list_store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(book_list)));
    gtk_list_store_clear(list_store);

    for (int i = 0; i < book_count; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter,
            0, library[i].id,
            1, library[i].title,
            2, library[i].author,
            3, library[i].year,
            -1);
    }
}

void search_books(const char *query) {
    GtkListStore *list_store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(book_list)));
    gtk_list_store_clear(list_store);

    for (int i = 0; i < book_count; i++) {
        if (strstr(library[i].title, query) || strstr(library[i].author, query)) {
            GtkTreeIter iter;
            gtk_list_store_append(list_store, &iter);
            gtk_list_store_set(list_store, &iter,
                0, library[i].id,
                1, library[i].title,
                2, library[i].author,
                3, library[i].year,
                -1);
        }
    }
}

void on_add_book(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Add Book",
        GTK_WINDOW(window),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Cancel", GTK_RESPONSE_CANCEL,
        "OK", GTK_RESPONSE_OK,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    GtkWidget *title_label = gtk_label_new("Title:");
    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 0, 1, 1);

    GtkWidget *title_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), title_entry, 1, 0, 1, 1);

    GtkWidget *author_label = gtk_label_new("Author:");
    gtk_grid_attach(GTK_GRID(grid), author_label, 0, 1, 1, 1);

    GtkWidget *author_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), author_entry, 1, 1, 1, 1);

    GtkWidget *year_label = gtk_label_new("Year:");
    gtk_grid_attach(GTK_GRID(grid), year_label, 0, 2, 1, 1);

    GtkWidget *year_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), year_entry, 1, 2, 1, 1);

    gtk_widget_show_all(dialog);

    int response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_OK) {
        const char *title = gtk_entry_get_text(GTK_ENTRY(title_entry));
        const char *author = gtk_entry_get_text(GTK_ENTRY(author_entry));
        const char *year_str = gtk_entry_get_text(GTK_ENTRY(year_entry));
        int year = atoi(year_str);

        add_book(title, author, year);
        display_all_books();
    }

    gtk_widget_destroy(dialog);
}

void on_remove_book(GtkWidget *widget, gpointer data) {
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(book_list));
    GtkTreeModel *model;
    GtkTreeIter iter;
    int id;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 0, &id, -1);
        remove_book(id);
        display_all_books();
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Please select a book to remove."
        );
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void on_search_book(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Search Books",
        GTK_WINDOW(window),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Cancel", GTK_RESPONSE_CANCEL,
        "OK", GTK_RESPONSE_OK,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    GtkWidget *query_label = gtk_label_new("Search:");
    gtk_grid_attach(GTK_GRID(grid), query_label, 0, 0, 1, 1);

    GtkWidget *query_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), query_entry, 1, 0, 1, 1);

    gtk_widget_show_all(dialog);

    int response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_OK) {
        const char *query = gtk_entry_get_text(GTK_ENTRY(query_entry));
        search_books(query);
    }

    gtk_widget_destroy(dialog);
}

void on_display_all(GtkWidget *widget, gpointer data) {
    display_all_books();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Library Management System");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", gtk_main_quit, NULL);

    GtkWidget * vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create menu bar
    GtkWidget *menubar = gtk_menu_bar_new();

    GtkWidget *file_menu = gtk_menu_new();
    GtkWidget *file_item = gtk_menu_item_new_with_label("File");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);
    gtk_container_add(GTK_CONTAINER(menubar), file_item);

    GtkWidget *add_item = gtk_menu_item_new_with_label("Add Book");
    g_signal_connect(add_item, "activate", G_CALLBACK(on_add_book), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), add_item);

    GtkWidget *remove_item = gtk_menu_item_new_with_label("Remove Book");
    g_signal_connect(remove_item, "activate", G_CALLBACK(on_remove_book), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), remove_item);

    GtkWidget *search_item = gtk_menu_item_new_with_label("Search Books");
    g_signal_connect(search_item, "activate", G_CALLBACK(on_search_book), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), search_item);

    GtkWidget *display_item = gtk_menu_item_new_with_label("Display All");
    g_signal_connect(display_item, "activate", G_CALLBACK(on_display_all), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), display_item);

    GtkWidget *quit_item = gtk_menu_item_new_with_label("Quit");
    g_signal_connect(quit_item, "activate", gtk_main_quit, NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit_item);

    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    // Create book list
    GtkListStore *list_store = gtk_list_store_new(4,
        G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT);

    book_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
    g_object_unref(list_store);

    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(book_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Title", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(book_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Author", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(book_list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Year", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(book_list), column);

    GtkWidget *sw = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(sw), book_list);
    gtk_box_pack_start(GTK_BOX(vbox), sw, TRUE, TRUE, 0);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
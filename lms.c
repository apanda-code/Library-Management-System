#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100

// Structure to store book information
typedef struct {
    int id;
    char title[100];
    char author[100];
    int year;
    int available;
} Book;

Book library[MAX_BOOKS];
int book_count = 0;

// Function prototypes
void add_book();
void search_book();
void display_all_books();
void remove_book();
void save_to_file();
void load_from_file();
void display_menu();

int main() {
    int choice;
    load_from_file();
    
    while (1) {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                add_book();
                break;
            case 2:
                search_book();
                break;
            case 3:
                display_all_books();
                break;
            case 4:
                remove_book();
                break;
            case 5:
                save_to_file();
                printf("Library data saved. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}

void display_menu() {
    printf("\n===== LIBRARY MANAGEMENT SYSTEM =====\n");
    printf("1. Add a new book\n");
    printf("2. Search for a book\n");
    printf("3. Display all books\n");
    printf("4. Remove a book\n");
    printf("5. Exit\n");
    printf("=====================================\n");
}

void add_book() {
    if (book_count >= MAX_BOOKS) {
        printf("Library is full! Cannot add more books.\n");
        return;
    }
    
    Book new_book;
    new_book.id = book_count + 1;
    
    printf("\nEnter book title: ");
    scanf(" %[^\n]", new_book.title);
    
    printf("Enter book author: ");
    scanf(" %[^\n]", new_book.author);
    
    printf("Enter publication year: ");
    scanf("%d", &new_book.year);
    
    new_book.available = 1; // Book is available by default
    
    library[book_count] = new_book;
    book_count++;
    
    printf("Book added successfully!\n");
    save_to_file();
}

void search_book() {
    char search_title[100];
    int found = 0;
    
    printf("\nEnter book title to search: ");
    scanf(" %[^\n]", search_title);
    
    for (int i = 0; i < book_count; i++) {
        if (strstr(library[i].title, search_title) != NULL) {
            printf("\nBook ID: %d", library[i].id);
            printf("\nTitle: %s", library[i].title);
            printf("\nAuthor: %s", library[i].author);
            printf("\nYear: %d", library[i].year);
            printf("\nStatus: %s\n\n", library[i].available ? "Available" : "Not Available");
            found = 1;
        }
    }
    
    if (!found) {
        printf("No books found with title '%s'.\n", search_title);
    }
}

void display_all_books() {
    if (book_count == 0) {
        printf("Library is empty.\n");
        return;
    }
    
    printf("\n===== ALL BOOKS =====\n");
    for (int i = 0; i < book_count; i++) {
        printf("Book ID: %d\n", library[i].id);
        printf("Title: %s\n", library[i].title);
        printf("Author: %s\n", library[i].author);
        printf("Year: %d\n", library[i].year);
        printf("Status: %s\n\n", library[i].available ? "Available" : "Not Available");
    }
}

void remove_book() {
    int id;
    
    printf("Enter book ID to remove: ");
    scanf("%d", &id);
    
    for (int i = 0; i < book_count; i++) {
        if (library[i].id == id) {
            for (int j = i; j < book_count - 1; j++) {
                library[j] = library[j + 1];
            }
            book_count--;
            printf("Book removed successfully!\n");
            save_to_file();
            return;
        }
    }
    
    printf("Book with ID %d not found.\n", id);
}

void save_to_file() {
    FILE *file = fopen("library.dat", "w");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
    
    fwrite(&book_count, sizeof(int), 1, file);
    fwrite(library, sizeof(Book), book_count, file);
    fclose(file);
}

void load_from_file() {
    FILE *file = fopen("library.dat", "r");
    if (file == NULL) {
        // File doesn't exist, start with empty library
        book_count = 0;
        return;
    }
    
    fread(&book_count, sizeof(int), 1, file);
    if (book_count > 0) {
        fread(library, sizeof(Book), book_count, file);
    }
    fclose(file);
}
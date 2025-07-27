#include <stdio.h>
#include <string.h>
#include "user.h"
#include "finance.h"

#define USERFILE "users.txt"

void registerUser() {
    char username[20], password[20];
    FILE *fp = fopen(USERFILE, "a+");
    if (!fp) {
        printf("Unable to open user file.\n");
        return;
    }

    printf("Enter new username: ");
    scanf("%s", username);

    char existing[20], existingPass[20];
    rewind(fp);
    while (fscanf(fp, "%s %s", existing, existingPass) != EOF) {
        if (strcmp(username, existing) == 0) {
            printf("Username already exists.\n");
            fclose(fp);
            return;
        }
    }

    printf("Enter password: ");
    scanf("%s", password);

    fprintf(fp, "%s %s\n", username, password);
    fclose(fp);
    printf("User registered successfully!\n");
}

int login(char currentUser[]) {
    char username[20], password[20];
    FILE *fp = fopen(USERFILE, "r");

    if (!fp) {
        printf("No registered users found.\n");
        return 0;
    }

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    // Admin login
    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        strcpy(currentUser, username);
        fclose(fp);
        printf("Admin login successful.\n");
        return 2;
    }

    // Regular user check
    char tempUser[20], tempPass[20];
    while (fscanf(fp, "%s %s", tempUser, tempPass) != EOF) {
        if (strcmp(tempUser, username) == 0 && strcmp(tempPass, password) == 0) {
            strcpy(currentUser, username);
            fclose(fp);
            printf("Login successful. Welcome, %s!\n", currentUser);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void logout(char currentUser[]) {
    printf("Logging out %s...\n", currentUser);
    strcpy(currentUser, "");
}

void menu(char currentUser[]) {
    int choice;
    while (1) {
        printf("\n---- Main Menu ----\n");
        printf("1. Add Transaction\n");
        printf("2. View Transactions\n");
        printf("3. Monthly Summary\n");
        printf("4. Update Transaction\n");
        printf("5. Delete Transaction\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTransaction(currentUser); break;
            case 2: viewTransactions(currentUser); break;
            case 3: monthlySummary(currentUser); break;
            case 4: updateTransaction(currentUser); break;
            case 5: deleteTransaction(currentUser); break;
            case 6: logout(currentUser); return;
            default: printf("Invalid choice. Try again.\n");
        }
    }
}

void adminMenu(char currentUser[]) {
    int choice;
    while (1) {
        printf("\n---- Admin Menu ----\n");
        printf("1. View All Transactions\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewTransactions("admin"); break; // admin sees all
            case 2: printf("Logging out admin...\n"); return;
            default: printf("Invalid choice.\n");
        }
    }
}
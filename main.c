#include <stdio.h>
#include <string.h>
#include "user.h"
#include "finance.h"

int main() {
    char currentUser[20] = "";

    while (1) {
        printf("\n---- Welcome ----\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf("Choose option: ");
        int option;
        scanf("%d", &option);

        if (option == 1) {
            if (login(currentUser)) {
                if (strcmp(currentUser, "admin") == 0)
                    adminMenu(currentUser);
                else
                    menu(currentUser);
            }
        } else if (option == 2) {
            registerUser();
        } else if (option == 3) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid option.\n");
        }
    }

    return 0;
}
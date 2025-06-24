#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <ctype.h>
#include "finance.h"

#define FILENAME "finance_data.txt"

typedef struct {
    char username[20];
    char date[20];
    int type;           // 1 = income, 2 = expense
    char category[50];
    float amount;
} Transaction;

// Simple date format validation: YYYY-MM-DD
int isValidDate(const char *date) {
    if (strlen(date) != 10) return 0;
    if (date[4] != '-' || date[7] != '-') return 0;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return 0;
    }
    // Basic checks for month/day range
    int year = atoi(date);
    int month = atoi(date + 5);
    int day = atoi(date + 8);
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0; // not perfect, but okay for now
    return 1;
}

void addTransaction(char currentUser[]) {
    FILE *fp = fopen(FILENAME, "a");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    Transaction t;
    char tempCategory[50];
    strcpy(t.username, currentUser);

    while (1) {
        printf("Enter date (YYYY-MM-DD): ");
        scanf("%s", t.date);
        if (isValidDate(t.date)) break;
        printf("Invalid date format! Please enter date as YYYY-MM-DD.\n");
    }

    printf("Enter type:\n1. Income\n2. Expense\nChoice: ");
    scanf("%d", &t.type);
    if (t.type != 1 && t.type != 2) {
        printf("Invalid type. Use 1 for Income, 2 for Expense.\n");
        fclose(fp);
        return;
    }

    if (t.type == 1) {
        printf("\nSelect Income Category:\n");
        printf("+----+---------------------------+\n");
        printf("| No | Income Category           |\n");
        printf("+----+---------------------------+\n");
        printf("| 1  | Salary / Part-time Job    |\n");
        printf("| 2  | Interest / Investment     |\n");
        printf("| 3  | Pocket Money              |\n");
        printf("| 4  | Gift                      |\n");
        printf("| 5  | Other                     |\n");
        printf("+----+---------------------------+\n");
        printf("Enter choice (1-5): ");
        int icat;
        scanf("%d", &icat);
        switch (icat) {
            case 1: strcpy(tempCategory, "Salary/Part-time"); break;
            case 2: strcpy(tempCategory, "Interest/Invest"); break;
            case 3: strcpy(tempCategory, "Pocket Money"); break;
            case 4: strcpy(tempCategory, "Gift"); break;
            case 5: strcpy(tempCategory, "Other"); break;
            default:
                printf("Invalid income category.\n");
                fclose(fp);
                return;
        }
    } else {
        printf("\nSelect Expense Category:\n");
        printf("+-----+--------------------------------------------+\n");
        printf("| No  | Expense Category                           |\n");
        printf("+-----+--------------------------------------------+\n");
        printf("|  1  | Food & Groceries                           |\n");
        printf("|  2  | Transportation (Bus, Fuel, Ride-share)     |\n");
        printf("|  3  | Rent / Hostel Fees                         |\n");
        printf("|  4  | Education (Books, Fees, Supplies)          |\n");
        printf("|  5  | Entertainment (Movies, Subscriptions)      |\n");
        printf("|  6  | Mobile / Internet                          |\n");
        printf("|  7  | Shopping (Clothes, Accessories)            |\n");
        printf("|  8  | Health (Medicines, Checkups)               |\n");
        printf("|  9  | Pocket Expenses (Snacks, Small Spendings)  |\n");
        printf("| 10  | Gifts / Donations                          |\n");
        printf("| 11  | Others                                     |\n");
        printf("+-----+--------------------------------------------+\n");
        printf("Enter choice (1-11): ");
        int ecat;
        scanf("%d", &ecat);
        switch (ecat) {
            case 1: strcpy(tempCategory, "Food"); break;
            case 2: strcpy(tempCategory, "Transport"); break;
            case 3: strcpy(tempCategory, "Rent/Hostel"); break;
            case 4: strcpy(tempCategory, "Education"); break;
            case 5: strcpy(tempCategory, "Entertainment"); break;
            case 6: strcpy(tempCategory, "Mobile/Internet"); break;
            case 7: strcpy(tempCategory, "Shopping"); break;
            case 8: strcpy(tempCategory, "Health"); break;
            case 9: strcpy(tempCategory, "Pocket Exp."); break;
            case 10: strcpy(tempCategory, "Gifts/Donations"); break;
            case 11: strcpy(tempCategory, "Others"); break;
            default:
                printf("Invalid expense category.\n");
                fclose(fp);
                return;
        }
    }

    strcpy(t.category, tempCategory);

    printf("Enter amount: ");
    scanf("%f", &t.amount);

    fprintf(fp, "%s %s %d %s %.2f\n", t.username, t.date, t.type, t.category, t.amount);

    fclose(fp);
    printf("Transaction added successfully.\n");
}

void viewTransactions(char currentUser[]) {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No transactions to display.\n");
        return;
    }

    Transaction t;
    int any = 0;

    char filterMonth[8] = "";
    char filterYear[5] = "";
    int filterByMonthYear = 0;

    if (strcmp(currentUser, "admin") != 0) {
        printf("Do you want to filter transactions by month/year? (y/n): ");
        char ch;
        scanf(" %c", &ch);
        if (ch == 'y' || ch == 'Y') {
            filterByMonthYear = 1;
            printf("Enter year (YYYY): ");
            scanf("%4s", filterYear);
            printf("Enter month (MM): ");
            scanf("%2s", filterMonth);
        }
    }

    printf("\n---- Transaction History ----\n");
    printf("Date       | Type     | Category                 | Amount\n");
    printf("---------------------------------------------------------------\n");

    while (fscanf(fp, "%s %s %d %s %f", t.username, t.date, &t.type, t.category, &t.amount) != EOF) {
        if (strcmp(currentUser, "admin") != 0 && strcmp(t.username, currentUser) != 0)
            continue;

        if (filterByMonthYear) {
            // check year and month in t.date
            if (strncmp(t.date, filterYear, 4) != 0) continue;
            if (strncmp(t.date + 5, filterMonth, 2) != 0) continue;
        }

        any = 1;
        printf("%-10s | %-8s | %-23s | Rs. %.2f\n",
               t.date,
               t.type == 1 ? "Income" : "Expense",
               t.category,
               t.amount);
    }

    if (!any)
        printf("No transactions found for the given filter/user.\n");

    fclose(fp);
}

void monthlySummary(char currentUser[]) {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No data available.\n");
        return;
    }

    char filterMonth[8] = "";
    char filterYear[5] = "";
    int filterByMonthYear = 0;

    if (strcmp(currentUser, "admin") != 0) {
        printf("Do you want to filter summary by month/year? (y/n): ");
        char ch;
        scanf(" %c", &ch);
        if (ch == 'y' || ch == 'Y') {
            filterByMonthYear = 1;
            printf("Enter year (YYYY): ");
            scanf("%4s", filterYear);
            printf("Enter month (MM): ");
            scanf("%2s", filterMonth);
        }
    }

    Transaction t;
    float totalIncome = 0, totalExpense = 0;
    int any = 0;

    while (fscanf(fp, "%s %s %d %s %f", t.username, t.date, &t.type, t.category, &t.amount) != EOF) {
        if (strcmp(currentUser, "admin") != 0 && strcmp(t.username, currentUser) != 0)
            continue;

        if (filterByMonthYear) {
            if (strncmp(t.date, filterYear, 4) != 0) continue;
            if (strncmp(t.date + 5, filterMonth, 2) != 0) continue;
        }

        any = 1;
        if (t.type == 1)
            totalIncome += t.amount;
        else if (t.type == 2)
            totalExpense += t.amount;
    }

    fclose(fp);

    if (!any) {
        printf("No transactions found for the given filter/user.\n");
        return;
    }

    printf("\n---- Monthly Summary ----\n");
    printf("Total Income  : Rs. %.2f\n", totalIncome);
    printf("Total Expenses: Rs. %.2f\n", totalExpense);
    printf("Balance       : Rs. %.2f\n", totalIncome - totalExpense);
}

void updateTransaction(char currentUser[]) {
    FILE *fp = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening file.\n");
        if(fp) fclose(fp);
        if(temp) fclose(temp);
        return;
    }

    char searchDate[20], searchCat[50];
    int found = 0;

    printf("Enter date of transaction to update (YYYY-MM-DD): ");
    scanf("%s", searchDate);
    printf("Enter category of transaction to update: ");
    scanf("%s", searchCat);

    Transaction t;

    while (fscanf(fp, "%s %s %d %s %f", t.username, t.date, &t.type, t.category, &t.amount) != EOF) {
        if (strcmp(t.username, currentUser) == 0 &&
            strcmp(t.date, searchDate) == 0 && strcmp(t.category, searchCat) == 0 && !found) {
            found = 1;

            printf("Transaction found. Enter new data:\n");

            while (1) {
                printf("New Date (YYYY-MM-DD): ");
                scanf("%s", t.date);
                if (isValidDate(t.date)) break;
                printf("Invalid date format! Please enter date as YYYY-MM-DD.\n");
            }

            printf("New Type (1-Income, 2-Expense): ");
            scanf("%d", &t.type);
            if (t.type != 1 && t.type != 2) {
                printf("Invalid type, keeping old value.\n");
            }

            printf("New Category: ");
            scanf("%s", t.category);

            printf("New Amount: ");
            scanf("%f", &t.amount);
        }
        fprintf(temp, "%s %s %d %s %.2f\n", t.username, t.date, t.type, t.category, t.amount);
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("Transaction updated successfully.\n");
    else
        printf("Transaction not found.\n");
}

void deleteTransaction(char currentUser[]) {
    FILE *fp = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening file.\n");
        if(fp) fclose(fp);
        if(temp) fclose(temp);
        return;
    }

    char searchDate[20], searchCat[50];
    int found = 0;

    printf("Enter date of transaction to delete (YYYY-MM-DD): ");
    scanf("%s", searchDate);
    printf("Enter category of transaction to delete: ");
    scanf("%s", searchCat);

    Transaction t;

    while (fscanf(fp, "%s %s %d %s %f", t.username, t.date, &t.type, t.category, &t.amount) != EOF) {
        if (strcmp(t.username, currentUser) == 0 &&
            strcmp(t.date, searchDate) == 0 && strcmp(t.category, searchCat) == 0 && !found) {
            found = 1;
            printf("Deleting transaction.\n");
            continue;
        }
        fprintf(temp, "%s %s %d %s %.2f\n", t.username, t.date, t.type, t.category, t.amount);
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("Transaction deleted successfully.\n");
    else
        printf("Transaction not found.\n");
}
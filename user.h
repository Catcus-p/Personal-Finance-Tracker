#ifndef USER_H
#define USER_H

void registerUser();
int login(char currentUser[]);
void logout(char currentUser[]);
void menu(char currentUser[]);
void adminMenu(char currentUser[]);

#endif
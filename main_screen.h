#ifndef _MAIN_SCREEN_H_
#define _MAIN_SCREEN_H_
#include <ncurses.h>

int main_menu(void);
int print_main(const char **choices);
void print_menu(WINDOW *menu_win,int highlight,const char **chocies);
int menu_selecting(WINDOW *menu_win,int highlight,const char **choiecs);

#endif

#include "main_screen.h"
#include <ncurses.h>
#include <stdbool.h>

int main_menu(void)
{
  const char *choices[] =	{
    "1. Play Game",
    "2. Ranking",
    "3. Recommended Play",
    "4. Exit"
  };
  return print_main(choices);
}

int print_main(const char **choices)
{
  WINDOW *title_win;
  WINDOW *menu_win;
  WINDOW *main_menu_template;

  int MAX_x,MAX_y;
  int highlight = 1;
  int choice;

  getmaxyx(stdscr,MAX_y,MAX_x);

  // main menu box template
  main_menu_template = newwin(MAX_y,MAX_x,0,0);
  box(main_menu_template,0,0);

  if(MAX_y > 30 && MAX_x > 118) {

    // print title logo
    title_win = newwin(16,118,MAX_y/18,MAX_x/2-59);

    mvwprintw(title_win,0,0,"TTTTTTTTTTTTTTTTTTTTTTTEEEEEEEEEEEEEEEEEEEEEETTTTTTTTTTTTTTTTTTTTTTTRRRRRRRRRRRRRRRRR   IIIIIIIIII   SSSSSSSSSSSSSSS ");
    mvwprintw(title_win,1,0,"T:::::::::::::::::::::TE::::::::::::::::::::ET:::::::::::::::::::::TR::::::::::::::::R  I::::::::I SS:::::::::::::::S");
    mvwprintw(title_win,2,0,"T:::::::::::::::::::::TE::::::::::::::::::::ET:::::::::::::::::::::TR::::::RRRRRR:::::R I::::::::IS:::::SSSSSS::::::S");
    mvwprintw(title_win,3,0,"T:::::TT:::::::TT:::::TEE::::::EEEEEEEEE::::ET:::::TT:::::::TT:::::TRR:::::R     R:::::RII::::::IIS:::::S     SSSSSSS");
    mvwprintw(title_win,4,0,"TTTTTT  T:::::T  TTTTTT  E:::::E       EEEEEETTTTTT  T:::::T  TTTTTT  R::::R     R:::::R  I::::I  S:::::S            ");
    mvwprintw(title_win,5,0,"        T:::::T          E:::::E                     T:::::T          R::::R     R:::::R  I::::I  S:::::S            ");
    mvwprintw(title_win,6,0,"        T:::::T          E::::::EEEEEEEEEE           T:::::T          R::::RRRRRR:::::R   I::::I   S::::SSSS         ");
    mvwprintw(title_win,7,0,"        T:::::T          E:::::::::::::::E           T:::::T          R:::::::::::::RR    I::::I    SS::::::SSSSS    ");
    mvwprintw(title_win,8,0,"        T:::::T          E:::::::::::::::E           T:::::T          R::::RRRRRR:::::R   I::::I      SSS::::::::SS  ");
    mvwprintw(title_win,9,0,"        T:::::T          E::::::EEEEEEEEEE           T:::::T          R::::R     R:::::R  I::::I         SSSSSS::::S ");
    mvwprintw(title_win,10,0,"        T:::::T          E:::::E                     T:::::T          R::::R     R:::::R  I::::I              S:::::S");
    mvwprintw(title_win,11,0,"        T:::::T          E:::::E       EEEEEE        T:::::T          R::::R     R:::::R  I::::I              S:::::S");
    mvwprintw(title_win,12,0,"      TT:::::::TT      EE::::::EEEEEEEE:::::E      TT:::::::TT      RR:::::R     R:::::RII::::::IISSSSSSS     S:::::S");
    mvwprintw(title_win,13,0,"      T:::::::::T      E::::::::::::::::::::E      T:::::::::T      R::::::R     R:::::RI::::::::IS::::::SSSSSS:::::S");
    mvwprintw(title_win,14,0,"      T:::::::::T      E::::::::::::::::::::E      T:::::::::T      R::::::R     R:::::RI::::::::IS:::::::::::::::SS ");
    mvwprintw(title_win,15,0,"      TTTTTTTTTTT      EEEEEEEEEEEEEEEEEEEEEE      TTTTTTTTTTT      RRRRRRRR     RRRRRRRIIIIIIIIII SSSSSSSSSSSSSSS   ");
  }
  else if(MAX_y > 20 && MAX_x > 60) {

    title_win = newwin(8,60,MAX_y/18,MAX_x/2-30);

    wprintw(title_win,"$$$$$$$$\\ $$$$$$$$\\ $$$$$$$$\\ $$$$$$$\\  $$$$$$\\  $$$$$$\\\n");
    wprintw(title_win,"\\__$$  __|$$  _____|\\__$$  __|$$  __$$\\ \\_$$  _|$$  __$$\\\n");
    wprintw(title_win,"   $$ |   $$ |         $$ |   $$ |  $$ |  $$ |  $$ /  \\__|\n");
    wprintw(title_win,"   $$ |   $$$$$\\       $$ |   $$$$$$$  |  $$ |  \\$$$$$$\\\n");
    wprintw(title_win,"   $$ |   $$  __|      $$ |   $$  __$$<   $$ |   \\____$$\\\n");
    wprintw(title_win,"   $$ |   $$ |         $$ |   $$ |  $$ |  $$ |  $$\\   $$ |\n");
    wprintw(title_win,"   $$ |   $$$$$$$$\\    $$ |   $$ |  $$ |$$$$$$\\ \\$$$$$$  |\n");
    wprintw(title_win,"   \\__|   \\________|   \\__|   \\__|  \\__|\\______| \\______/");
  } else {
    title_win = newwin(4,20,0,0);
    wprintw(title_win,"1. play\n");
    wprintw(title_win,"2. rank\n");
    wprintw(title_win,"3. recommend play\n");
    wprintw(title_win,"4. exit");
    return wgetch(title_win) - '0';
  }


  // print menu selections
  menu_win = newwin(8,30,(MAX_y*2)/3,(MAX_x-30)/2);
  keypad(menu_win, true);

  wrefresh(main_menu_template);
  wrefresh(title_win);
  print_menu(menu_win,highlight,choices, 4);

  choice = menu_selecting(menu_win,highlight,choices,4);
  wrefresh(menu_win);

  return choice;
}

void print_menu(WINDOW *menu_win,int highlight,const char **choices, int n_choices)
{
  int i, x, y;
  x = 2;
  y = 2;
  box(menu_win,0,0);
  for(i = 0;i < n_choices;++i) {
    if(highlight == i + 1) {
      wattron(menu_win,A_REVERSE);
      mvwprintw(menu_win,y,x,"%s",choices[i]);
      wattroff(menu_win,A_REVERSE);
    } else
      mvwprintw(menu_win,y,x,"%s",choices[i]);
    ++y;
  }
  wrefresh(menu_win);
}

int menu_selecting(WINDOW *menu_win,int highlight,const char **choices, int n_choices)
{
  int c;
  int choice = 0;

  while(true)
  {
    c = wgetch(menu_win);
    switch(c)
    {
      case KEY_UP:
        if(highlight == 1)
          highlight = n_choices;
        else
          --highlight;
        break;
      case KEY_DOWN:
        if(highlight == n_choices)
          highlight = 1;
        else
          ++highlight;
        break;
      case 10:
        choice = highlight;
        break;
    }
    print_menu(menu_win,highlight,choices, 4);
    if(choice != 0)
      return choice;
  }
}

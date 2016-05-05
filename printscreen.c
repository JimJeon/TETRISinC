#include<ncurses.h>

#define TRUE 1
#define FALSE 0

#define n_choices 4


void print_main(const char **choices);
void print_menu(WINDOW *menu_win,int highlight,const char **chocies);
int menu_selecting(WINDOW *menu_win,int highlight,const char **choiecs);

void main_menu(void)
{/*{{{*/
	// User's main menu selection
	int user_input;
	const char *choices[] =	{
							"1. Play Game",
							"2. Show Ranking",
							"3. Option",
							"4. Exit"
						};

	print_main(choices);
	
	user_input = getch();
	
	switch(user_input)
	{
	case 1: //print_player_select();
			break;
	case 2: break;
	case 3: break;
	case 4: return;
	default: break;
	}
	return;
}/*}}}*/

void print_main(const char **choices)
{/*{{{*/
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

	// print title logo
	title_win = newwin(6,45,MAX_y/6,(MAX_x-45)/2);
	mvwprintw(title_win,0,0,"████████╗███████╗████████╗██████╗ ██╗███████╗ \n");  
	mvwprintw(title_win,1,0,"╚══██╔══╝██╔════╝╚══██╔══╝██╔══██╗██║██╔════╝ \n");  
	mvwprintw(title_win,2,0,"   ██║   █████╗     ██║   ██████╔╝██║███████╗ \n");
	mvwprintw(title_win,3,0,"   ██║   ██╔══╝     ██║   ██╔══██╗██║╚════██║ \n");
	mvwprintw(title_win,4,0,"   ██║   ███████╗   ██║   ██║  ██║██║███████║ \n");  
	mvwprintw(title_win,5,0,"   ╚═╝   ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚══════╝ \n");  

	// print menu selections
	menu_win = newwin(8,30,(MAX_y*2)/3,(MAX_x-30)/2);
	keypad(menu_win, TRUE);

	wrefresh(main_menu_template);
	wrefresh(title_win);
	print_menu(menu_win,highlight,choices);

	choice = menu_selecting(menu_win,highlight,choices);
	wrefresh(menu_win);

	

	return;
}/*}}}*/

void print_menu(WINDOW *menu_win,int highlight,const char **choices)
{/*{{{*/
	int x, y;
	
	x = 2;
	y = 2;
	box(menu_win,0,0);
	for(int i = 0;i < n_choices;++i)
	{
		if(highlight == i + 1)
		{
			wattron(menu_win,A_REVERSE);
			mvwprintw(menu_win,y,x,"%s",choices[i]);
			wattroff(menu_win,A_REVERSE);
		}
		else
			mvwprintw(menu_win,y,x,"%s",choices[i]);
		++y;
	}
	wrefresh(menu_win);
}/*}}}*/

int menu_selecting(WINDOW *menu_win,int highlight,const char **choices)
{/*{{{*/
	int c;
	int choice = 0;

	while(TRUE)
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
		print_menu(menu_win,highlight,choices);
		if(choice != 0)
			return choice;
	}
}/*}}}*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<ncurses.h>
#include<locale.h>

/*
#define PLAY_BOARD_WIDTH
#define PLAY_BOARD_HEIGHT

#define DELAY
#define RAND

#define LEFT
#define RIGHT
#define UP
#define DOWN
#define SPACE
#define ENTER
#define ESC
*/


void main_menu(void);
void change_setting(void);
void show_ranking(void);

int main(void){
	setlocale(LC_ALL, "ko_KR.utf8");
	// A arrow key input
	int user_input = 0;
	// Ranking File pointer
	FILE* pRank = fopen("Ranking.dat","w");
	// Scores for the game
	int game_score = 0;
	

	srand(time(NULL));

	// Start a new window
	initscr();
	clear();
	noecho();
	cbreak();

	// Main menu
	main_menu();


/*
	switch(user_choice){
		case 1: player_select();
				cin << user_choice;
				switch(user_choice){
					case 1: one_player();
							break;
					case 2: two_player();
							break;
				}
				break;

		case 2: change_setting();
				break;

		case 3: show_ranking();
				break;

		case 4: return 0; // End game

		default: break;
	}
*/

	// Close a window
	endwin();

	return 0;
}

void play_game(int players,int level){
	int currentBlock, nextBlock;
	

	nextBlock = rand()%7;
	
	

	
	

}


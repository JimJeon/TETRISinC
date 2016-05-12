﻿#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
    curs_set(0);
	keypad(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_BLACK, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);
    init_pair(6, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(7, COLOR_BLACK, COLOR_GREEN);

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(main_menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
	for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	blockRotate=0;
	blockY=-3;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
    DrawBox(9,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(16,WIDTH+10);
	printw("SCORE");
	DrawBox(17,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
    int i = 0;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
    case ' ':
        while( CheckToMove(field, nextBlock[0], blockRotate, blockY+(++i), blockX) );
        blockY += i - 1;
        break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}

void PrintScore(int score){
	move(18,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(COLOR_PAIR(nextBlock[1]+1));
				printw(" ");
				attroff(COLOR_PAIR(nextBlock[1]+1));
			}
			else printw(" ");
		}
	}
	for( i = 0; i < 4; i++ ){
		move(10+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[2]][0][i][j] == 1 ){
				attron(COLOR_PAIR(nextBlock[2]+1));
				printw(" ");
				attroff(COLOR_PAIR(nextBlock[2]+1));
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(COLOR_PAIR(blockID+1));
				printw("%c",tile);
				attroff(COLOR_PAIR(blockID+1));
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
    int i, j;

    for(i = 0; i < 4; ++i)
        for(j = 0; j < 4; ++j)
            if( block[currentBlock][blockRotate][i][j] ) {
                if( blockY + i >= HEIGHT )                      return 0;
                if( !(0 <= blockX + j && blockX + j < WIDTH) )  return 0;
                if( f[blockY + i][blockX + j] )         return 0;
            }
    return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
    DrawField();
    DrawBlock(blockY, blockX, currentBlock, blockRotate, ' ');
    DrawShadow(blockY, blockX, currentBlock, blockRotate);
}

void BlockDown(int sig){
	// user code
    if(CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX))
        DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, ++blockY, blockX);
    else {
        if(blockY == -2) gameOver = 1;

        AddBlockToField(field,nextBlock[0],blockRotate, blockY, blockX);
        score += DeleteLine(field);

        nextBlock[0] = nextBlock[1];
        nextBlock[1] = nextBlock[2];
        nextBlock[2] = rand()%7;

        DrawField();
        DrawNextBlock(nextBlock);
        PrintScore(score);

        blockRotate = 0;
        blockY = -3;
        blockX = WIDTH/2 - 2;
    }
    timed_out = 0;
}

void AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
    int i, j;
    for(i = 0;i < 4; ++i)
        for(j = 0;j < 4; ++j)
        if(block[currentBlock][blockRotate][i][j])
            if(f[blockY+i+1][blockX+j]||blockY+i+1 >= HEIGHT) score+=10;
    for(i = 0;i < 4; ++i)
        for(j = 0;j < 4; ++j)
            if(block[currentBlock][blockRotate][i][j])
                f[blockY+i][blockX+j] = 1;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code
    int i, j, h, r, count = 0;
    int line_flag = 0;
    for(i = 0; i < HEIGHT; ++i) {
        for(j = 0; j < WIDTH; ++j) {
            if(!f[i][j]) {
                line_flag = 0;
                break;
            }
            else line_flag = 1;
        }
        if(line_flag) {
            count++;
            for(h = i; h > 0; --h)
                for(r = 0; r < WIDTH; ++r)
                    f[h][r] = f[h-1][r];
        }
    }
    return count*count*100;
}

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
    int i = 0;
    while( CheckToMove(field, blockID, blockRotate, y+(++i), x) );
    DrawBlock(y+i-1, x, blockID, blockRotate, '/');
}

void createRankList(){
	// user code
}

void rank(){
	// user code
}

void writeRankFile(){
	// user code
}

void newRank(int score){
	// user code
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}

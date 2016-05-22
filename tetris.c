#include "tetris.h"

static struct sigaction act, oact;

int main(){
  rank_t *tmp_node = head_node;
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

  createRankList();

  while(!exit){
    clear();
    switch(main_menu()){
      case MENU_PLAY: play(); break;
      case MENU_RANK: rank(); break;
      case MENU_EXIT: exit=1; break;
      default: break;
    }
  }

  endwin();
  system("clear");

  while(tmp_node!=NULL) {
    head_node = tmp_node->link;
    free(tmp_node);
    tmp_node = head_node;
  }
  return 0;
}

void InitTetris(){
  int i,j;

  for(j=0;j<HEIGHT;j++)
    for(i=0;i<WIDTH;i++)
      field[j][i]=0;

  for(i = 0;i < BLOCK_NUM; ++i)
    nextBlock[i] = rand() % 7;
  blockRotate=0;
  blockY=-3;
  blockX=WIDTH/2-2;
  score=0;	
  gameOver=0;
  timed_out=0;

  DrawOutline();
  DrawField();
  DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
  DrawShadow(blockY, blockX, nextBlock[0], blockRotate);
  DrawNextBlock(nextBlock);
  PrintScore(score);

  recRoot = calloc(1, sizeof(RecNode));
  recRoot->lv = -1;
  recRoot->score = 0;
  recRoot->f = calloc(HEIGHT * WIDTH, sizeof(char));
  for(i = 0; i < HEIGHT; ++i)
    for(j = 0;j < WIDTH; ++j)
      recRoot->f[i][j] = field[i][j];
  construct_tree(recRoot);
  recommend(recRoot);
  DrawRecommend(recommendY,recommendX,nextBlock[0],recommendR);
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
      newRank(score);

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
  DrawRecommend(recommendY, recommendX, currentBlock, recommendR);
}

void BlockDown(int sig){
  // user code
  int i = 0, j = 0;
  if(CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX))
    DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, ++blockY, blockX);
  else {
    if(blockY == -2) gameOver = 1;

    score += AddBlockToField(field,nextBlock[0],blockRotate, blockY, blockX);
    score += DeleteLine(field);

    for(i = 0;i < BLOCK_NUM - 1; ++i)
      nextBlock[i] = nextBlock[i+1];
    nextBlock[i] = rand() % 7;

    DrawField();
    DrawNextBlock(nextBlock);
    PrintScore(score);

    blockRotate = 0;
    blockY = -3;
    blockX = WIDTH/2 - 2;

    for(i = 0;i < HEIGHT; ++i)
      for(j = 0;j < WIDTH; ++j)
        recRoot->f[i][j] = field[i][j];
    recommend(recRoot);
  }
  timed_out = 0;
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
  // user code
  int i, j, point = 0;
  for(i = 0;i < 4; ++i)
    for(j = 0;j < 4; ++j)
      if(block[currentBlock][blockRotate][i][j])
        if(f[blockY+i+1][blockX+j]||blockY+i+1 >= HEIGHT) point+=10;
  for(i = 0;i < 4; ++i)
    for(j = 0;j < 4; ++j)
      if(block[currentBlock][blockRotate][i][j])
        f[blockY+i][blockX+j] = 1;
  return point;
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
  int i = 0;
  int score = 0;
  int number = 0;
  char name[NAMELEN] = {0, };

  FILE *fp = fopen("rank.txt", "r");

  if(fp == NULL) {
    fp = fopen("rank.txt", "w");
    fclose(fp);
    return;
  }
  fscanf(fp,"%d\n", &number);

  for(i = 0;i < number; ++i) {
    fscanf(fp, "%[^' '] %d\n", name, &score);
    insert_node(name, score);
  }
  fclose(fp);
}

void rank(){
  // user code
  bool flag = true;
  char search_name[NAMELEN];
  char user_input = 0;
  int X = -1, Y = -1, i = 0, target = 0;
  rank_t* tmp_node = head_node;
  rank_t* del_node = NULL;
  printw("1. list ranks from X to Y\n");
  printw("2. list ranks by a specific name\n");
  printw("3. delete a specific rank\n");
  user_input = wgetch(stdscr);
  while(user_input != '1' && user_input != '2' && user_input != '3') {
    printw("wrong input! input 1 or 2 or 3\n");
    user_input = wgetch(stdscr);
  }

  switch(user_input) {
    case '1':
      tmp_node = head_node;
      echo();
      printw("X = ");
      scanw("%d",&X);
      printw("Y = ");
      scanw("%d",&Y);
      noecho();

      if(X==-1) X = 1;
      if(Y==-1) Y = node_num;
      if(Y < X) {
        printw("search failure: no rank in the list\n");
        break;
      }
      if(node_num == 0) {
        printw("search failure: no rankings\n");
        break;
      }
      if(node_num < X) {
        printw("search failure: bound error\n");
        break;
      }

      printw("       name       |   score   \n");
      printw("------------------------------\n");
      for(i = 1;i <= node_num; ++i) {
        if(X<=i&&i<=Y)
          printw(" %-17s| %-10d\n",tmp_node->name, tmp_node->score); 
        tmp_node = tmp_node->link;
      }
      break;
    case '2':
      tmp_node = head_node;
      printw("input the name: ");
      echo();
      scanw("%s", search_name);
      noecho();

      printw("       name       |   score   \n");
      printw("------------------------------\n");
      for(flag = true, tmp_node = head_node ;
          tmp_node != NULL ; tmp_node = tmp_node->link) {
        if(strcmp(tmp_node->name, search_name)) continue;
        printw(" %-17s| %-10d\n", tmp_node->name, tmp_node->score);
        flag = false;
      }
      if(flag)
        printw("\nsearch failure: no name in the list\n");
      break;
    case '3':
      printw("input the rank: ");
      echo();
      scanw("%d", &target);
      noecho();
      if(!(1<=target&&target<=node_num)) {
        printw("\nsearch failure: no rank in the list\n");
        break;
      } else {
        tmp_node = head_node;
        if(target == 1) {
          del_node = head_node;
          head_node = del_node->link;
        } else {
          for(i = 1;i < target - 1; ++i)
            tmp_node = tmp_node->link;
          del_node = tmp_node->link;
          tmp_node->link = del_node->link;
        }
        free(del_node);
        node_num--;
        printw("\nresult: the rank deleted\n");
      }
      writeRankFile();
      break;
  }
  wgetch(stdscr);
}

void writeRankFile(){
  // user code
  int i = 0;
  rank_t *tmp_node = head_node;
  FILE *fp = fopen("rank.txt", "w");
  fprintf(fp, "%d\n", node_num);

  while(tmp_node != NULL) {
    fprintf(fp, "%s %d\n", tmp_node->name, tmp_node->score);
    tmp_node = tmp_node->link;
  }
  fclose(fp);
}

void newRank(int score){
  // user code
  char name[NAMELEN];
  clear();
  move(0,0);
  printw("insert your name: ");
  echo();
  scanw("%s", name);
  noecho();

  insert_node(name, score);
  writeRankFile();
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
  // user code
  DrawBlock(y,x,blockID,blockRotate,'R');
}

int recommend(RecNode *root){
  int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
  // user code
  int x = 0, y = 0, rotation = 0;
  int i = 0, j = 0, child = 0;
  int idx = 0;

  if(root->lv == BLOCK_NUM - 1) return 0;

  // initialize child field
  for(child = 0;child < CHILDREN_MAX; ++child)
    for(i = 0;i < HEIGHT; ++i)
      for(j = 0;j < WIDTH; ++j)
        root->c[child]->f[i][j] = root->f[i][j];

  // calculating child node value
  for(rotation = 0; rotation < NUM_OF_ROTATE; ++rotation) {
    for(x = -1; x < WIDTH - 2; ++x) {
      idx = (WIDTH-1)* rotation + x + 1;
      y = 0;
      while( CheckToMove(root->c[idx]->f, nextBlock[root->c[idx]->lv], rotation, ++y, x) );
      root->c[idx]->score += AddBlockToField(root->c[idx]->f, nextBlock[root->c[idx]->lv], rotation, --y, x);
      root->c[idx]->score += recommend(root->c[idx]);
      if(max < root->c[idx]->score) {
        max = root->c[idx]->score;
        if(root->lv == -1) {
          recommendX = x;
          recommendY = y;
          recommendR = rotation;
        }
      }
    }
  }

  return max;
}

void recommendedPlay(){
  // user code
}

void insert_node(char* name, int score) {
  rank_t *pre_node = NULL;
  rank_t *cur_node = head_node;
  rank_t *new_node = malloc(sizeof(*new_node));

  if(new_node == NULL) {
    fprintf(stderr, "ERROR: memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  strcpy(new_node->name, name);
  new_node->score = score;
  new_node->link = NULL;

  while(cur_node!=NULL) {
    if(cur_node->score < score) break;

    pre_node = cur_node;
    cur_node = cur_node->link;
  }
  if(pre_node == NULL) {
    head_node = new_node;
    new_node->link = cur_node;
  } else {
    new_node->link = pre_node->link;
    pre_node->link = new_node;
  }

  node_num++;
}

void construct_tree(RecNode *root) {
  int i = 0, j = 0;
  if(root->lv == BLOCK_NUM - 1) return;
  for(i = 0;i < CHILDREN_MAX; ++i) {
    root->c[i] = calloc(1,sizeof(RecNode));
    root->c[i]->lv = root->lv+1;
    root->c[i]->score = 0;
    root->c[i]->f = calloc(HEIGHT*WIDTH, sizeof(char));
    for(j = 0;j < CHILDREN_MAX; ++j)
      root->c[i]->c[j] = NULL;
    construct_tree(root->c[i]);
  }
}

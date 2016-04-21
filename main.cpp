#include <ncurses.h>
#include "Apple1.h"

int main() {
  initscr();
  //curs_set(0);
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);
  wresize(stdscr, 24, 40);
  scrollok(stdscr, TRUE); 
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  attron(COLOR_PAIR(1));
  refresh();
  
  Apple1 apple1;
  apple1.reset();
  apple1.run();

  nocbreak();
  nodelay(stdscr, FALSE);
  endwin();

  return 0;
}

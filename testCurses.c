#include <curses.h>
#include <string.h>
#include <strings.h>

int main()
{
    initscr();

    printw("Hello world!");

    refresh();

    int c = getch();

    move(10, 10);
    refresh();
    printw("%d", c);

    getch();

    endwin();

    return 0;
}

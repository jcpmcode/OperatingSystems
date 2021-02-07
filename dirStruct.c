/*
 * This program displays the names of all files in the current directory.
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

int leeChar()
{
    int chars[5];
    int ch, i = 0;
    nodelay(stdscr, TRUE);
    while ((ch = getch()) == ERR)
        ; /* Espera activa */
    ungetch(ch);
    while ((ch = getch()) != ERR)
    {
        chars[i++] = ch;
    }
    /* convierte a numero con todo lo leido */
    int res = 0;
    for (int j = 0; j < i; j++)
    {
        res <<= 8;
        res |= chars[j];
    }
    return res;
}

int main(void)
{
    int i = 0;
    int c = 0;
    int count = 0;

    FILE *file;

    initscr();
    raw();
    noecho();
    cbreak();

    struct s_dir
    {
        int type;
        char *name;
        long int size;
    } entry[1000];

    // read directory
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            entry[count].type = dir->d_type;
            entry[count].name = dir->d_name;
            if (dir->d_type == DT_REG) /* If the entry is a regular file */
            {
                file = fopen(dir->d_name, "rb");
                if (file != NULL)
                {
                    fseek(file, 0L, SEEK_END);
                    entry[count].size = ftell(file);
                    fclose(file);
                }
            }
            count++;
        }
    }

    // display list
    do
    {
        for (int j = 0; j < count; j++)
        {
            if (j == i)
            {
                attron(A_REVERSE);
            }
            mvprintw(5 + j, 5, "%ld\t%s", entry[j].size, entry[j].name);
            attroff(A_REVERSE);
        }
        move(5 + i, 5);
        refresh();
        c = leeChar();
        switch (c)
        {
        case 0x1B5B41:
            i = (i > 0) ? i - 1 : count - 1;
            break;
        case 0x1B5B42:
            i = (i < count - 1) ? i + 1 : 0;
            break;
        default:
            // Nothing
            break;
        }
        move(1, 5);
        printw("Estoy en %d: Lei %d", i, c);
        move(3, 5);
        printw("SIZE\tNAME");
        move(4, 5);
        printw("=====================================");
    } while (c != 'q');

    endwin();
    closedir(d);

    return 0;
}
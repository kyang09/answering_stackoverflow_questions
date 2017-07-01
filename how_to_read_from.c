#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

void print_status(WINDOW *win, const char *str) {
    init_pair(1, COLOR_RED, COLOR_WHITE);
    clear();
    wattron(win, COLOR_PAIR(1));
    mvwhline (win, 0, 0, ' ', COLS);
    mvwaddnstr (win, 0, 0, str, COLS);
    wattroff(win, COLOR_PAIR(1));
    wrefresh(win);
}

void print_line (const char *str, int *i) {
    init_pair(2, COLOR_RED, COLOR_BLACK);
    if(*i > LINES - 2) {
        *i = 1;
        wclear(stdscr);
    }   
    attron(COLOR_PAIR(2));
    mvwprintw(stdscr, (*i)++, 0, str);
    attroff(COLOR_PAIR(2));
    refresh();
}

int main() {
    WINDOW *menu_win;
    int i = 1, q = 1;
    char buf[1024];
    initscr();

    clear();
    noecho();
    halfdelay(5);
    start_color();
    nonl();
    keypad(stdscr, TRUE);
    intrflush(stdscr, FALSE);

    menu_win = newwin(LINES, COLS, 0, 0); 
    keypad(menu_win, TRUE);

    bool on_a_tty = isatty(fileno(stdin));

    FILE *inp = fopen("/dev/stdin", "r");
    freopen("/dev/tty", "r", stdin);
    print_status(menu_win, "Use arrow keys to go up and down");
    while(q) {
        int c;
        switch((c=wgetch(menu_win))) {
            case KEY_UP:
                print_status(menu_win, "Moved up");
                break;
            case KEY_DOWN:
                print_status(menu_win, "Moved down");
                break;
            case 'q':
                q = 0;
                break;
            default:;
                if(!on_a_tty){
                    while (fgets(buf, 1024, inp) != NULL) {
                        print_line(buf, &i);
                    }
                }
        }
    }
    endwin();
    return 0;
}

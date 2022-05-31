#include <iostream>
#include <vector>
#include <fstream>
#include <sqlite3.h>
#include <ncurses.h>
#include <menu.h>

#include "rparser.h"
#include "cparser.h"

WINDOW* feedWindow;
WINDOW* articleWindow;
MENU* feedMenu;
MENU* articleMenu;
ITEM** feedItems;
ITEM** articleItems;

int feedChoices, articleChoices;

std::vector<feed> myFeeds;

void loadFeedMenu() {
    feedChoices = myFeeds.size();

    feedItems = (ITEM**)calloc(feedChoices + 1, sizeof(ITEM*));                  
                                                                             
    for(int i = 0; i < feedChoices; i++) {
        char* temp = const_cast<char*>(myFeeds.at(i).title.c_str());

        feedItems[i] = new_item(temp, "");
        set_item_userptr(feedItems[i], &myFeeds.at(i));
    }

    feedItems[feedChoices] = (ITEM*) NULL;
                                                                             
    feedMenu = new_menu((ITEM**) feedItems);
    set_menu_win(feedMenu, feedWindow);
    set_menu_sub(feedMenu, derwin(feedWindow, 0, 0, 1, 1));
    set_menu_mark(feedMenu, "");
    post_menu(feedMenu);

    wrefresh(feedWindow);
}

void loadArticleMenu(feed mFeed) {
    wclear(articleWindow);
    box(articleWindow, 0, 0);

    articleChoices = mFeed.articles.size();
    articleItems = (ITEM**)calloc(articleChoices + 1, sizeof(ITEM *));                  

    for(int i = 0; i < articleChoices; i++) {
        char* temp = const_cast<char*>(mFeed.articles.at(i).title.c_str());
        articleItems[i] = new_item(temp, "");
    }

    articleItems[articleChoices] = (ITEM*) NULL;

    articleMenu = new_menu((ITEM**) articleItems);
    set_menu_win(articleMenu, articleWindow);
    set_menu_sub(articleMenu, derwin(articleWindow, 0, 0, 1, 1));
    set_menu_mark(articleMenu, "");
    post_menu(articleMenu);

    wrefresh(articleWindow);
}

void killMenu(MENU* menu, ITEM** items, int choices) {
    for(int i = 0; i < choices; i++) {
        free_item(items[i]);
    }

    free_menu(menu);
}

void cleanUp() {
    killMenu(feedMenu, feedItems, feedChoices);
    killMenu(articleMenu, articleItems, articleChoices);

    endwin();
}

void initialSetup() {
    setlocale(LC_CTYPE, "");

	initscr();
	cbreak();
	noecho();
    curs_set(0);
	keypad(stdscr, TRUE);

    std::string info = "virss v0.1";
	mvaddstr(LINES - 2, 0, "'q' to Exit");
    mvaddstr(LINES - 2, COLS - (info.length()), const_cast<char*>(info.c_str()));
   
    refresh();

    feedWindow = newwin((LINES - 4), (COLS / 2), 0, 0);
    keypad(feedWindow, TRUE);
    box(feedWindow, 0, 0);

    wrefresh(feedWindow);

    articleWindow = newwin((LINES - 4), (COLS / 2), 0, (COLS / 2));
    keypad(articleWindow, TRUE);
    box(articleWindow, 0, 0);

    wrefresh(articleWindow);

    myFeeds = readFeeds();
    loadFeedMenu();
    loadArticleMenu(myFeeds.at(0));
}

int main() {
	initialSetup();

	int c;
    bool curWin = true;

	while((c = getch()) != 'q') {
        bool reloadFeedWin = false;

        switch(c) {
            case 'j':
                if(curWin) {
		            menu_driver(feedMenu, REQ_DOWN_ITEM);
                    reloadFeedWin = true;
                } else {
                    menu_driver(articleMenu, REQ_DOWN_ITEM);
                }
				break;
			case 'k':
                if(curWin) {
                    menu_driver(feedMenu, REQ_UP_ITEM);
                    reloadFeedWin = true;
                } else {
                    menu_driver(articleMenu, REQ_UP_ITEM);
                }
				break;
            case 'h':
                curWin = true;
                break;
            case 'l':
                curWin = false;
                break;
		}

        if(reloadFeedWin) {
            ITEM* cur = current_item(feedMenu);
            feed* p = (feed*)item_userptr(cur);
            loadArticleMenu(*p);

            wrefresh(feedWindow);
        } else {
            wrefresh(articleWindow);
        }
	}
    
    cleanUp();

    return 0;
}

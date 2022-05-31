#include <iostream>
#include <vector>
#include <fstream>
#include <sqlite3.h>
#include <ncurses.h>
#include <menu.h>

#include "rparser.h"
#include "cparser.h"

WINDOW* feedWindow, articleWindow;
MENU* feedMenu, articleMenu;
ITEM** feedItems, articleItems;

int feedChoices, articleChoices;

void loadFeedMenu(std::vector<feed> mFeeds) {
    feedChoices = mFeeds.size();

    feedItems = (ITEM **)calloc(feedChoices + 1, sizeof(ITEM *));                  
                                                                             
    for(int i = 0; i < feedChoices; i++) {
        char* temp = const_cast<char*>(mFeeds.at(i).title.c_str());
        feedItems[i] = new_item(temp, "");
    }

    feedItems[feedChoices] = (ITEM*) NULL;
                                                                             
    feedMenu = new_menu((ITEM**) menuItems);
    set_menu_win(feedMenu, feedWindow);
    set_menu_format(feedMenu, (LINES - 4), 1);
    set_menu_mark(feedMenu, "");
    post_menu(feedMenu);

    refresh();
}

void loadArticlesMenu(feed mFeed) {
    articleChoices = mFeed.articles.size();

    articleItems = (ITEM **)calloc(articleChoices + 1, sizeof(ITEM *));                  

    for(int i = 0; i < articleChoices; i++) {
        char* temp = const_cast<char*>(mFeed.articles.at(i).title.c_str());
        articleItems[i] = new_item(temp, "");
    }

    articlesItem{articleChoices} = (ITEM*) NULL;

    articleMenu = new_menu((ITEM**) articleItems);
    set_menu_win(articleMenu, articleWindow);
    set_menu_format(articleMenu, (LINES - 4), 1);
    set_menu_mark(articleMenu, "");
    post_menu(articleMenu);

    refresh();
}

void killMenu(MENU* menu, ITEM** items, int choices) {
    for(int i = 0; i < choices; i++) {
        free_item(items[i]);
    }

    free_menu(menu);
}

void cleanUp() {
    killMenu(feedMenu, feedItems, feedChoices);
    killMenu(articleMenu, artuckeItems, articleChoices);

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

    feedWindow = newwin((LINES - 4), (COLS / 2), 0, (COLS / 2));
    articleWindow = newwin((LINES - 4), (COLS / 2), 0, (COLS / 2));

    std::vector<feed> myFeeds = readFeeds();
    loadFeedMenu(myFeeds);

    refresh();
}

int main() {
	initialSetup();

	int c;

    // IMPLEMENT LOADING FEED ON MENU CHANGE

	while((c = getch()) != 'q') {
        switch(c) {
            case 'j':
		        menu_driver(feedMenu, REQ_DOWN_ITEM);
				break;
			case 'k':
				menu_driver(feedMenu, REQ_UP_ITEM);
				break;
		}
	}
    
    cleanUp();

    return 0;
}

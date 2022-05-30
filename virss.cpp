#include <iostream>
#include <vector>
#include <fstream>
#include <sqlite3.h>
#include <ncurses.h>
#include <menu.h>

#include "rparser.h"
#include "cparser.h"

MENU* feedMenu, articleMenu;
ITEM** feedItems, articleItems;

int feedChoices, articleChoices;

void loadFeedMenu(std::vector<feed> mFeeds) {
    int feedChoices = mFeeds.size();

    feedItems = (ITEM **)calloc(feedChoices + 1, sizeof(ITEM *));                  
                                                                             
    for(int i = 0; i < feedChoices; i++) {
        char* temp = const_cast<char*>(mFeeds.at(i).title.c_str());
        feedItems[i] = new_item(temp, "");
    }
                                                                             
    feedItems[feedChoices] = (ITEM *)NULL;
                                                                             
    feedMenu = new_menu((ITEM **)menuItems);
    set_menu_format(feedMenu, LINES-4, 1);
    set_menu_mark(feedMenu, "");
    post_menu(feedMenu);
}

void loadArticlesMenu(feed mFeed) {
    articleItems = mFeed.articles.size();


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

int main() {
    setlocale(LC_CTYPE, "");

	int c;				

	initscr();
	cbreak();
	noecho();
    curs_set(0);
	keypad(stdscr, TRUE);
	
    std::vector<feed> myFeeds = readFeeds();

    // TODO: CREATE MENU FOR FEEDS SORTED BY NAME
    // WHEN MENU SELECTED LOAD FEED

	n_choices = myFeed.articles.size();

	my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));

	for(int i = 0; i < n_choices; i++) {
        char* temp = const_cast<char*>(myFeed.articles.at(i).title.c_str());
	    my_items[i] = new_item(temp, "");
    }

	my_items[n_choices] = (ITEM *)NULL;

	my_menu = new_menu((ITEM **)my_items);
    set_menu_format(my_menu, LINES-4, 1);
    set_menu_mark(my_menu, "");
	post_menu(my_menu);

    std::string info = "virss v0.1";

	mvaddstr(LINES - 2, 0, "'q' to Exit");
    mvaddstr(LINES - 2, COLS - (info.length()), const_cast<char*>(info.c_str()));

	refresh();

	while((c = getch()) != 'q')
	{   switch(c)
	    {	case 'j':
		        menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case 'k':
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
		}
	}	
    
    cleanUp();
	endwin();
}

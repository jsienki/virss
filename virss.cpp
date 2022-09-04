#include <iostream>
#include <vector>
#include <fstream>
#include <ncurses.h>
#include <menu.h>
#include <cstdio>
#include <cstdlib>

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
feed currentFeed;
rssObject currentObject;

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
    set_menu_sub(feedMenu, derwin(feedWindow, (LINES - 6), ((COLS / 2) - 2), 1, 1));
    set_menu_format(feedMenu, (LINES -6), 1);
    set_menu_mark(feedMenu, "");
    post_menu(feedMenu);

    wrefresh(feedWindow);
}

void loadArticleMenu() {
    wclear(articleWindow);
    box(articleWindow, 0, 0);

    articleChoices = currentFeed.articles.size();
    articleItems = (ITEM**)calloc(articleChoices + 1, sizeof(ITEM *));                  

    for(int i = 0; i < articleChoices; i++) {
        char* temp = const_cast<char*>(currentFeed.articles.at(i).title.c_str());
        articleItems[i] = new_item(temp, "");
        set_item_userptr(articleItems[i], &currentFeed.articles.at(i));
    }

    articleItems[articleChoices] = (ITEM*) NULL;

    articleMenu = new_menu((ITEM**) articleItems);
    set_menu_win(articleMenu, articleWindow);
    set_menu_sub(articleMenu, derwin(articleWindow, (LINES-6), ((COLS/2) - 2), 1, 1));
    set_menu_format(articleMenu, (LINES -6), 1);
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
    currentFeed = myFeeds.at(0);
    loadFeedMenu();
    loadArticleMenu();
}

int main() {
	initialSetup();

	int c;
    bool curWin = true;

	while((c = getch()) != 'q') {
        bool reloadFeedWin = false;
        bool openUrl = false;

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
            case 'o':
                openUrl = true;
                break;
		}

        if(reloadFeedWin) {
            ITEM* cur = current_item(feedMenu);
            feed* p = (feed*)item_userptr(cur);
            currentFeed = *p;
            loadArticleMenu();

            wrefresh(feedWindow);
        } else {
            wrefresh(articleWindow);

            if(openUrl) {
                ITEM* cur = current_item(articleMenu);
                rssObject* p = (rssObject*)item_userptr(cur);
                currentObject = *p;
                
                std::string run;
                //std::string browser = "xdg-open ";
                std::string browser = "open ";
                run += browser;
                run += p -> link;
                run += " </dev/null &>/dev/null &";
                char* temp = const_cast<char*>(run.c_str());
                system(temp);
            }
        }
	}
    
    cleanUp();

    return 0;
}

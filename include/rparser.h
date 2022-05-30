#include <string>
#include <vector>
#include <sqlite3.h>

struct rssObject {
    std::string title;
    std::string link;
    std::string desc;
    std::string pubdate;
    std::string guid;
    bool read;
};

struct feed {
    std::string title;
    std::string subtitle;
    std::string category;
    std::vector<rssObject> articles;
};

std::vector<feed> readStored(sqlite3* db);
std::vector<feed> readFeeds();
feed newFeed(std::string url);
void updateFeed(std::string url, feed& stored);
void updateDatabase(std::vector<feed>& feeds, sqlite3* db);
void sortFeed(std::vector<feed>& feeds, char method);

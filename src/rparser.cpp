#include <string>
#include <vector>
#include <fstream>
#include <sqlite3.h>
#include <curl/curl.h>
#include <tinyxml2.h>

#include "rparser.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

std::vector<feed> readStored(sqlite3* db) {
    std::vector<feed> output;

    return output;
}

std::vector<feed> readFeeds() {
    std::vector<feed> output;
    std::string line;
    std::ifstream myfile;
    myfile.open("urls");

    while(getline(myfile, line)) {
        output.push_back(newFeed(line)); 
    }

    return output;
}

feed newFeed(std::string url) {
    feed output;
    output.title = "TEST_TITLE HELLO";
    output.subtitle = "TEST_DESC abcsd";
    output.category = "TEST_CATEGORY news";
    
    CURL* curl;
    FILE* fp;
    CURLcode res;
    char outfilename[255] = "feed.xml";
    curl = curl_easy_init();

    if (curl) {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, const_cast<char*>(url.c_str()));
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);
    }

    tinyxml2::XMLDocument doc;
    doc.LoadFile(outfilename);
    tinyxml2::XMLElement* pRootElement = doc.RootElement();

    if (pRootElement != NULL) {
        tinyxml2::XMLElement* pChannel = pRootElement -> FirstChildElement("channel");

        if (pChannel != NULL) {
            tinyxml2::XMLElement* pItem = pChannel -> FirstChildElement("item");

            tinyxml2::XMLElement* pFeedTitle = pChannel -> FirstChildElement("title");

            if (pFeedTitle != NULL) {
                output.title = pFeedTitle -> GetText();
            }

            while(pItem) {
                rssObject newItem;
                newItem.link = "TEST_LINK google.com";
                newItem.desc = "TEST_DESC abc";
                newItem.pubdate = "TEST_DATE 23-10-2022 13:00";
                newItem.guid = "TEST_GUID 213940";

                tinyxml2::XMLElement* pTitle = pItem -> FirstChildElement("title");

                if (pTitle != NULL) {
                    newItem.title = pTitle -> GetText();
                }

                output.articles.push_back(newItem);

                pItem = pItem -> NextSiblingElement("item");
            }
        }
    }

    return output;
}

void updateFeed(std::string url, feed& stored) {

}

void updateDatabase(std::vector<feed>& feeds, sqlite3* db) {

}

void sortFeed(std::vector<feed>& feeds, std::string method) {

}

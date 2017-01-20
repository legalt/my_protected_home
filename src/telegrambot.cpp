#include <curl/curl.h>
#include <stdio.h>
#include <algorithm>    
#include <boost/format.hpp>

#include "../include/telegrambot.h"


TelegramBot::TelegramBot ( std::string bot_id, std::string chat_id ) {
  this->id = bot_id;
  this->chat_id = chat_id;
}

bool TelegramBot::sendMessage ( std::string message ) {          
    if ( !message.size() ) {
      return false;
    }

    replace(message.begin(), message.end(), ' ', '+');
    CURL *curl = curl_easy_init();

    if ( curl == NULL ) { 
      return false;
    }
    
    std::string url = boost::str(boost::format("api.telegram.org/bot%1%/sendMessage?chat_id=%2%&text=%3%") % this->id % this->chat_id % message);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());                
    CURLcode result = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if ( result != CURLE_OK ) {
      printf("curl_easy_perform(): %s\n", curl_easy_strerror(result));
      return false;
    }    
    
    return true;
}


bool TelegramBot::sendPhoto ( std::string imgPath, std::string caption ) {          
    // set up the header
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;

    curl_formadd(&formpost, &lastptr,
       CURLFORM_COPYNAME, "photo",       
       CURLFORM_FILE, imgPath.c_str(),
       CURLFORM_END);

    curl_formadd(&formpost, &lastptr,
       CURLFORM_COPYNAME, "chat_id",
       CURLFORM_COPYCONTENTS, this->chat_id.c_str(),
       CURLFORM_END);

    if ( caption.size() ) {
        curl_formadd(&formpost, &lastptr,
           CURLFORM_COPYNAME, "caption",
           CURLFORM_COPYCONTENTS, caption.c_str(),
           CURLFORM_END);
    }    

    CURL *curl = curl_easy_init();   

    if ( curl == NULL ) {
      return false;
    }
    
    struct curl_slist *headerlist = NULL;
    static const char buf[] = "Expect:";

    headerlist = curl_slist_append(headerlist, buf);        
    curl_easy_setopt(curl, CURLOPT_URL, boost::str(boost::format("https://api.telegram.org/bot%1%/sendPhoto") % this->id).c_str());    
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
    
    CURLcode result = curl_easy_perform(curl);
    curl_formfree(formpost);
    curl_easy_cleanup(curl);        
        
    if ( result != CURLE_OK ) {
      printf("curl_easy_perform(): %s\n", curl_easy_strerror(result));
      return false;
    }

    return true;
}
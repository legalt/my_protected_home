#ifndef __TELEGRAMBOT_H__
#define __TELEGRAMBOT_H__

#include <iostream>
#include <vector>

using namespace std;

class TelegramBot {
	private:
  		string id;
  		string chat_id;
  		vector<string> chat_list;
	
	public:
    	TelegramBot ( string bot_id, string chat_id );
    	bool sendMessage ( string message );    	
    	bool sendPhoto ( string imgPath, string caption = "" );
};

#endif
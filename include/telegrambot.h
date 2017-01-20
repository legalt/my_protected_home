#ifndef __TELEGRAMBOT_H__
#define __TELEGRAMBOT_H__

#include <iostream>
#include <vector>

class TelegramBot {
	private:
  		std::string id;
  		std::string chat_id;
  		std::vector<std::string> chat_list;
	
	public:
    	TelegramBot ( std::string bot_id, std::string chat_id );
    	bool sendMessage ( std::string message );    	
    	bool sendPhoto ( std::string imgPath, std::string caption = "" );
};

#endif
#ifndef __SAFEHOUSE_H__
#define __SAFEHOUSE_H__

#include <iostream>
#include "telegrambot.h"

class SafeHouse {
	private:
    	TelegramBot *m_tb;
    	int m_last_notify_time = 0;

	public:
    	SafeHouse ( std::string, std::string );
    	static int count_allowed_camers();
    	int detect_face ( const int& );
    	void send_notification ( int, std::string );
};

#endif
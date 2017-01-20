#ifndef __SAFEHOUSE_H__
#define __SAFEHOUSE_H__

#include <iostream>
#include "telegrambot.h"

using namespace std;

class SafeHouse {
	private:
    	TelegramBot *m_tb;
    	int m_last_notify_time = 0;

	public:
    	SafeHouse ( string, string );
    	static int count_allowed_camers();
    	int detect_face ( const int& );
    	void send_notification ( int, string );
};

#endif
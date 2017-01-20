#include <iostream>
#include <pthread.h>
#include "../include/safehouse.h"

const std::string TELEGRAM_BOT_ID = "ID";
const std::string TELEGRAM_CHAT_ID = "ID";

void *run_spy ( void * data ) {
    int cam_id = *(int *)data;

    SafeHouse sf(TELEGRAM_BOT_ID, TELEGRAM_CHAT_ID);    
    sf.detect_face(cam_id);	

    return data;
}


int main() {  
    const int COUNT_ALLOWED_CAMERS = SafeHouse::count_allowed_camers();

    int * id = new int[COUNT_ALLOWED_CAMERS];    
    pthread_t * thread = new pthread_t[COUNT_ALLOWED_CAMERS];    
			
    if ( !COUNT_ALLOWED_CAMERS  ) {		
        std::cerr << "Not found camers" << std::endl;
       	return 0;
    }	
    
    for ( int i = 0; i < COUNT_ALLOWED_CAMERS; i++ ) { 
        id[i] = i;

        if (pthread_create(&thread[i], NULL, run_spy, &id[i]) != 0 ) {
            std::cerr << "Creating the thread" << std::endl;
        }
    }

    for ( int i = 0; i < COUNT_ALLOWED_CAMERS; i++ ) {
        std::cout << i << std::endl;
        pthread_join(thread[i], NULL);
    }
  
    return 0;
}

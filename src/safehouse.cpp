#include <ctime>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>

#include "../include/safehouse.h"
#include "../include/telegrambot.h"

using namespace cv;
using namespace std;

#define FACE_CASCADE_FILE "data/haarcascade_frontalface_default.xml"
#define LOWER_BODY_CASCADE_FILE "data/haarcascade_frontalface_default.xml"
#define FULL_BODY_CASCADE_FILE "data/haarcascade_frontalface_default.xml"

SafeHouse::SafeHouse( string id, string chat_id ) {
  m_tb = new TelegramBot(id, chat_id); 
}

int SafeHouse::count_allowed_camers () {
    VideoCapture temp_camera;
    int maxTested = 100, i;
    bool res;

    for ( i = 0; i < maxTested; i++ ) {
        VideoCapture temp_camera(i);
        res = (!temp_camera.isOpened());
        temp_camera.release();

        if ( res ) {
        	return i;
        }
    }

    return maxTested;
}

int SafeHouse::detect_face ( const int &cam_id ) {
    Mat frame;
    Mat frame_gray;    
    vector<Rect> faces, lower_bodies, full_bodies;
    CascadeClassifier face_cascade, lower_body_cascade, full_body_cascade;    
    int offset_time_sec = 10;
    bool hasObject = false;

    if ( !face_cascade.load(FACE_CASCADE_FILE) ) {
        cerr << "Error loading face cascade" << endl;
        return 1;
    }

    if ( !lower_body_cascade.load(LOWER_BODY_CASCADE_FILE) ) {
        cerr << "Error loading lower body cascade" << endl;
        return 1;
    }

    if ( !full_body_cascade.load(FULL_BODY_CASCADE_FILE) ) {
        cerr << "Error loading full body cascade" << endl;
        return 1;
    }

    VideoCapture cap(cam_id);
    
    if ( !cap.isOpened() ) {
        cerr << "Capture Device ID " << cam_id << "cannot be opened." << endl;
        return 1;
    }

    for ( ;; ) {
        cap.read(frame);
        cvtColor(frame, frame_gray, CV_BGR2GRAY);
        equalizeHist(frame_gray, frame_gray);

        face_cascade.detectMultiScale(frame_gray, faces, 2, 3, 0 | CASCADE_SCALE_IMAGE, Size(50, 50));
        lower_body_cascade.detectMultiScale(frame_gray, lower_bodies, 2, 3, 0 | CASCADE_SCALE_IMAGE, Size(50, 50));
        full_body_cascade.detectMultiScale(frame_gray, full_bodies, 2, 3, 0 | CASCADE_SCALE_IMAGE, Size(50, 50));
        
        if ( faces.size() ) {                      
            rectangle(frame, faces[0], Scalar(0, 0, 255), 2, 8, 0);

	    	hasObject = true;	    		             
        }

        if ( lower_bodies.size() ) {
            rectangle(frame, lower_bodies[0], Scalar(0, 0, 255), 2, 8, 0);

            hasObject = true;                            
        }

        if ( full_bodies.size() ) {
            rectangle(frame, full_bodies[0], Scalar(0, 0, 255), 2, 8, 0);

            hasObject = true;                            
        }

        if ( hasObject ) {
            if ( m_last_notify_time == 0 || (m_last_notify_time + offset_time_sec) < time(0) ) {
                m_last_notify_time = time(0);
                
                imwrite("face.png", frame);
                SafeHouse::send_notification(cam_id, "face.png");
            }
        }

        if ( waitKey(30) == 27 ) {
            break;
        }
    }

    return 0;
}

void SafeHouse::send_notification ( int cam_id, string img_name ) {
    char message[25];
    char cwd[1024];
    char imgPathValid[1024];
    sprintf(message, "Found person in camera %d", cam_id);    

    getcwd(cwd, sizeof(cwd));
    sprintf(imgPathValid, "%s/%s", cwd, img_name.c_str());    
    
    m_tb->sendPhoto(string(imgPathValid), string(message));
}
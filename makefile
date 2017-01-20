CFLAGS = -Wall -pedantic -std=c++11 `pkg-config --cflags opencv`

LIBS = -lpthread -lstdc++ -lcurl `pkg-config --libs opencv`

OBJS = src/main.cpp src/safehouse.cpp src/telegrambot.cpp

TARGET = bin/main

clear:
	rm -f bin/main
	
build: $(OBJS)
	gcc -o $(TARGET) $(OBJS) $(CFLAGS) $(LIBS)

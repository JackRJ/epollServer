OBJS = Util.o DayListUser.o ThreadPool.o HttpData.o main.o 
PP = g++
CFLAGS = -Wall
PTHREAD = -pthread
MYSQL = `mysql_config --cflags --libs`

LIBS = $(shell mysql_config --libs)
CFLAGS = $(shell mysql_config --cflags)

main: $(OBJS)
	$(PP) $(PTHREAD) $(OBJS) -o $@ $(LIBS)

DayListUser.o: mysql/DayListUser.cpp mysql/DayListUser.h
	$(PP) -o $@ $(CFLAGS) $< $(LIBS)

Util.o: Util.cpp Util.h
	$(PP) -c $(CFLAGS) $< -o $@

ThreadPool.o: ThreadPool.cpp ThreadPool.h
	$(PP) -c $(PTHREAD) $(CFLAGS) $< -o $@

HttpData.o: HttpData.cpp HttpData.h 
	$(PP) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o

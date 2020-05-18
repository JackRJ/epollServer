OBJS = Util.o ThreadPool.o HttpData.o main.o 
PP = g++
CFLAGS = -Wall
PTHREAD = -pthread
MYSQL = `mysql_config --cflags --libs`

main: $(OBJS)
	$(PP) $(PTHREAD) $(OBJS) -o $@

#DayListUser.o: mysql/DayListUser.cpp mysql/DayListUser.h
#	$(PP) -c $(MYSQL) $< -o $@

Util.o: Util.cpp Util.h
	$(PP) -c $(CFLAGS) $< -o $@

ThreadPool.o: ThreadPool.cpp ThreadPool.h
	$(PP) -c $(PTHREAD) $(CFLAGS) $< -o $@

HttpData.o: HttpData.cpp HttpData.h 
	$(PP) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o

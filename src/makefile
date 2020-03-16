OBJS = Util.o ThreadPool.o main.o
PP = g++
CFLAGS = -Wall
PTHREAD = -pthread

main: $(OBJS)
	$(PP) $(PTHREAD) $(OBJS) -o $@

Util.o: Util.cpp Util.h
	$(PP) -c $(CFLAGS) $< -o $@

ThreadPool.o: ThreadPool.cpp ThreadPool.h
	$(PP) -c $(PTHREAD) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o

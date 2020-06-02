#OBJS = Util.o ThreadPool.o HttpData.o main.o 
#PP = g++
#WALL = -Wall
#PTHREAD = -pthread
#MYSQL = `mysql_config --cflags --libs`

#LIBS = $(shell mysql_config --libs)
#CFLAGS = $(shell mysql_config --cflags)

#main: $(OBJS)
#	$(PP) $(PTHREAD) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

#DayListUser.o: mysql/DayListUser.cpp mysql/DayListUser.h
	#$(PP) -o $@ $(CFLAGS) $< $(LIBS)

#Util.o: Util.cpp Util.h
#	$(PP) -c $(CFLAGS) $(WALL) $< -o $@ $(LIBS)

#ThreadPool.o: ThreadPool.cpp ThreadPool.h
#	$(PP) -c $(PTHREAD) $(WALL) $< -o $@

#HttpData.o: HttpData.cpp HttpData.h 
#	$(PP) -c $(WALL) $< -o $@

#clean:
#	rm -rf *.o

all : $(OBJ_DIR)/main.o $(OBJ_DIR)/Util.o $(OBJ_DIR)/ThreadPool.o $(OBJ_DIR)/HttpData.o

$(OBJ_DIR)/main.o : main.cpp
	$(PP) -c $^ -o $@
$(OBJ_DIR)/Util.o : Util.cpp Util.h
	$(PP) -c $(CFLAGS) $< -o $@ $(LIBS)
$(OBJ_DIR)/ThreadPool.o: ThreadPool.cpp ThreadPool.h
	$(PP) -c $(PTHREAD) $< -o $@
$(OBJ_DIR)/HttpData.o: HttpData.cpp HttpData.h 
	$(PP) -c $(PTHREAD) $(CFLAGS) $< -o $@

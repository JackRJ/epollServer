all : $(OBJ_DIR)/UrlTranslation.o $(OBJ_DIR)/FileUtil.o $(OBJ_DIR)/LogFile.o $(OBJ_DIR)/LogStream.o \
$(OBJ_DIR)/AsyncLogging.o $(OBJ_DIR)/CountDownLatch.o $(OBJ_DIR)/Thread.o $(OBJ_DIR)/Logging.o

$(OBJ_DIR)/LogFile.o: LogFile.cpp LogFile.h nonCopyable.h MutexLock.h
	$(PP) -c $(PTHREAD) $< -o $@

$(OBJ_DIR)/FileUtil.o: FileUtil.cpp FileUtil.h nonCopyable.h
	$(PP) -c $< -o $@

$(OBJ_DIR)/UrlTranslation.o: UrlTranslation.cpp UrlTranslation.h
	$(PP) -c $< $(LIBS) -o $@ $(CFLAGS)

$(OBJ_DIR)/LogStream.o: LogStream.cpp LogStream.h nonCopyable.h
	$(PP) -c $< -o $@

$(OBJ_DIR)/AsyncLogging.o: AsyncLogging.cpp AsyncLogging.h nonCopyable.h
	$(PP) -c $< -o $@

$(OBJ_DIR)/Thread.o: Thread.cpp Thread.h CurrentThread.h nonCopyable.h
	$(PP) -c $(PTHREAD) $< -o $@

$(OBJ_DIR)/CountDownLatch.o: CountDownLatch.cpp CountDownLatch.h Condition.h nonCopyable.h
	$(PP) -c $(PTHREAD) $< -o $@

$(OBJ_DIR)/Logging.o: Logging.cpp Logging.h
	$(PP) -c $(PTHREAD) $< -o $@
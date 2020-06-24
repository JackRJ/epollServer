all : $(OBJ_DIR)/UrlTranslation.o $(OBJ_DIR)/FileUtil.o $(OBJ_DIR)/LogFile.o

$(OBJ_DIR)/LogFile.o: LogFile.cpp LogFile.h nonCopyable.h MutexLock.h
	$(PP) -c $(PTHREAD) $< -o $@

$(OBJ_DIR)/FileUtil.o: FileUtil.cpp FileUtil.h nonCopyable.h
	$(PP) -c $< -o $@

$(OBJ_DIR)/UrlTranslation.o: UrlTranslation.cpp UrlTranslation.h
	$(PP) -c $< $(LIBS) -o $@ $(CFLAGS)
CC = g++

CFLAGS = -O2 -W -Wall -pedantic -g  -ansi

LIBS = -lSDL
INCLUDES = -I./include -I. -I./test

HEADERS_DIR = include
SOURCES_DIR = src
OBJECTS_DIR = obj
TEST_DIR    = test
BIN_DIR	    = bin
PLUGIN_DIR  = plugins

FILES = data.cpp wave.cpp chunkfactory.cpp generic.cpp meta.cpp sound.cpp \
		lgmk.cpp format.cpp 

TEST_FILES = 

MAIN_FILE = main.cpp

HEADERS = $(addsuffix .h, $(addprefix $(HEADERS_DIR)/, $(basename $(FILES))))
SOURCES = $(addprefix $(SOURCES_DIR)/, $(FILES))
OBJECTS = $(addsuffix .o, $(addprefix $(OBJECTS_DIR)/, $(basename $(FILES))))

TEST_SOURCES = $(addprefix $(TEST_DIR)/, $(TEST_FILES))
TEST_OBJECTS = $(addsuffix .o, $(addprefix $(OBJECTS_DIR)/, $(basename $(TEST_FILES))))

MAIN_SOURCE = $(addprefix $(SOURCES_DIR)/, $(MAIN_FILE))
MAIN_OBJECT = $(addprefix $(OBJECTS_DIR)/, $(addsuffix .o, $(basename $(MAIN_FILE))))

.PHONY: all

all:
	@mkdir -p bin obj 
	@make $(BIN_DIR)/editorLc

$(OBJECTS_DIR)/%.o: $(SOURCES_DIR)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	
#$(OBJECTS_DIR)/%.o: $(TEST_DIR)/%.cpp $(HEADERS)
#	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	
$(MAIN_OBJECT):  $(MAIN_SOURCE) $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

#$(TEST_DIR)/test: $(OBJECTS) $(TEST_OBJECTS)
#	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJECTS) $(TEST_OBJECTS) $(LIBS)

$(BIN_DIR)/editorLc: $(OBJECTS) $(MAIN_OBJECT)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJECTS) $(MAIN_OBJECT) $(LIBS)

clean:
	@find . -name '*~' -exec rm -rf {} \;
	@rm -rf $(OBJECTS_DIR)/*.o
	@rm -rf $(BIN_DIR) $(OBJECTS_DIR)

dist-clean: clean
	@rm -rf bin obj 

TARGET = coviwar
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
SRC_DIR = src
OBJ_DIR = opt
CXX = g++
CXXFLAGS = -std=c++17 -O2 -w -MMD
LDFLAGS = $(shell sdl2-config --cflags --libs) -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm
ifeq ($(shell uname -s),Darwin)
	LDFLAGS += -I/usr/local/include
endif
DEPENDS = $(OBJS:.o=.d)

.SUFFIXES: .cpp .o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< $(LDFLAGS) -o $@

clean:
	-rm -rf $(OBJ_DIR) $(TARGET) 
	-rm *bmp
-include $(DEPENDS)

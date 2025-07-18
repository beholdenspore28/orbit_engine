BUILD_DIR = build

GCC = gcc -fanalyzer
CLANG = clang -fsanitize=address,undefined
CC = clang

CFLAGS_DEBUG = -g3 
CFLAGS_RELEASE = -O3 -flto

CFLAGS = -Wall \
				 -Wextra \
				 -Wpedantic \
				 -std=c11 \
				 $(CFLAGS_DEBUG)

LIBS := -lm -lopenal -lalut -lX11

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
INC = -Isrc -Idep -Idep/glad/include

GAME = $(BUILD_DIR)/game
GLAD = $(BUILD_DIR)/glad.o
GLX = $(BUILD_DIR)/glx.o

all: $(BUILD_DIR) $(OBJ) $(GAME)
	./build/game

$(GAME): $(OBJ) $(GLAD) $(GLX)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(GLAD):
	$(CC) $(CFLAGS) -c dep/glad/src/gl.c -o $(GLAD) -Idep/glad/include

$(GLX):
	$(CC) $(CFLAGS) -c dep/glad/src/glx.c -o $(GLX) -Idep/glad/include




OPTIMIZE ?= 0



CXX = g++
CXXFLAGS = -std=c++23 \
-Wall \
-Wextra \
-Wno-write-strings \
-Wno-old-style-cast \
-Wno-sign-conversion \
-Iinclude \
-g
CXXFLAGS_LIB := -std=c++23 \
-Wall \
-Wextra \
-Wno-write-strings \
-Wno-old-style-cast \
-Wno-sign-conversion \
-Iinclude \
-O3
CXXLIBS = 

CXX_WIN = x86_64-w64-mingw32-g++
CXXFLAGS_WIN = -std=c++23 \
-Wall \
-Wextra \
-Wno-write-strings \
-Wno-old-style-cast \
-Wno-sign-conversion \
-Iinclude \
-g
CXXFLAGS_LIB_WIN := -std=c++23 \
-Wall \
-Wextra \
-Wno-write-strings \
-Wno-old-style-cast \
-Wno-sign-conversion \
-Iinclude \
-O3
CXXLIBS_WIN = -static

#source files
SRC := $(shell find src -maxdepth 1 -type f \( -name '*.c' -o -name '*.cpp' \))
#source files of library
SRC_LIB := $(shell find src -maxdepth 20 -mindepth 2 -type f \( -name '*.c' -o -name '*.cpp' \))

#objects to compile for program
OBJ_COMP := $(patsubst src/%.cpp,obj/linux/%.o,$(filter %.cpp,$(SRC))) $(patsubst src/%.c,obj/linux/%.o,$(filter %.c,$(SRC)))
OBJ_COMP_WIN := $(patsubst src/%.cpp,obj/windows/%.o,$(filter %.cpp,$(SRC))) $(patsubst src/%.c,obj/windows/%.o,$(filter %.c,$(SRC)))

#objects to compile for library
OBJ_LIB := $(patsubst src/%.cpp,obj/linux/%.o,$(filter %.cpp,$(SRC_LIB))) $(patsubst src/%.c,obj/linux/%.o,$(filter %.c,$(SRC_LIB)))
OBJ_LIB_WIN := $(patsubst src/%.cpp,obj/windows/%.o,$(filter %.cpp,$(SRC_LIB))) $(patsubst src/%.c,obj/windows/%.o,$(filter %.c,$(SRC_LIB)))

#all objects that needs to be linked together to make final executable
OBJ = $(shell find obj/linux -type f)
OBJ_WIN = $(shell find obj/windows -type f)

#final executable targets
TARGET := build/main
TARGET_WIN := build/main.exe

#debug or no
DEBUG ?= 0
ARGS ?= 

RED    := \033[31m
GREEN  := \033[32m
YELLOW := \033[33m
BLUE   := \033[34m
PURPLE := \033[35m
CYAN   := \033[36m
RESET  := \033[0m

$(info SRC: $(SRC))
$(info SRC_LIB: $(SRC_LIB))
$(info OBJ_COMP: $(OBJ_COMP))
$(info OBJ_COMP_WIN: $(OBJ_COMP_WIN))
$(info OBJ_LIB: $(OBJ_LIB))
$(info OBJ_LIB_WIN: $(OBJ_LIB_WIN));
$(info )

.PHONY: build include libs obj shaders src $(TARGET) $(OBJ_COMP) $(TARGET_WIN) $(OBJ_COMP_WIN)

linux: $(OBJ_LIB)
	@echo -e "$(PURPLE)Compiling program for linux$(RESET)"
	@for obj in $(OBJ_COMP); do \
		src=$$(echo $$obj | sed 's|obj/linux/|src/|' | sed 's|\.o$$||'); \
		if [ -f "$${src}.cpp" ]; then \
			src="$${src}.cpp"; \
		elif [ -f "$${src}.c" ]; then \
			src="$${src}.c"; \
		else \
			echo -e "$(RED)Error: No matching source for $$obj$(RESET)"; \
			exit 1; \
		fi; \
		echo -e "$(CYAN)Compiling $$src -> $$obj$(RESET)"; \
		mkdir -p $$(dirname $$obj); \
		time $(CXX) $(CXXFLAGS) -O$(OPTIMIZE) -c $$src -o $$obj; \
	done

linux_fast: $(OBJ_LIB)
	@echo -e "$(PURPLE)Compiling program for linux$(RESET)"
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o obj/linux/main.o
	$(CXX) $(OBJ) -o $(TARGET) $(CXXLIBS)

$(TARGET): $(OBJ_COMP) $(OBJ_LIB)#linking
	@echo -e "$(PURPLE)Linking program for linux$(RESET)"
	$(CXX) $(OBJ) -o $(TARGET) $(CXXLIBS)
ifeq ($(DEBUG),0)
	strip $(TARGET)
endif

#compiling libs
$(OBJ_LIB):
	@echo -e "$(PURPLE)Compiling libs for linux$(RESET)"
	@for obj in $(OBJ_LIB); do \
		src=$$(echo $$obj | sed 's|obj/linux/|src/|' | sed 's|\.o$$||'); \
		if [ -f "$${src}.cpp" ]; then \
			src="$${src}.cpp"; \
		elif [ -f "$${src}.c" ]; then \
			src="$${src}.c"; \
		else \
			echo -e "$(RED)Error: No matching source for $$obj$(RESET)"; \
			exit 1; \
		fi; \
		echo -e "$(CYAN)Compiling $$src -> $$obj$(RESET)"; \
		mkdir -p $$(dirname $$obj); \
		time $(CXX) $(CXXFLAGS_LIB) -c $$src -o $$obj; \
	done







win: $(OBJ_LIB_WIN)
	@echo -e "$(PURPLE)Compiling program for windows$(RESET)"
	@for obj in $(OBJ_COMP_WIN); do \
		src=$$(echo $$obj | sed 's|obj/windows/|src/|' | sed 's|\.o$$||'); \
		if [ -f "$${src}.cpp" ]; then \
			src="$${src}.cpp"; \
		elif [ -f "$${src}.c" ]; then \
			src="$${src}.c"; \
		else \
			echo -e "$(RED)Error: No matching source for $$obj$(RESET)"; \
			exit 1; \
		fi; \
		echo -e "$(CYAN)Compiling $$src -> $$obj$(RESET)"; \
		mkdir -p $$(dirname $$obj); \
		time $(CXX_WIN) $(CXXFLAGS_WIN) -O$(OPTIMIZE) -c $$src -o $$obj; \
	done

win_fast: $(OBJ_LIB_WIN)
	$(CXX_WIN) $(CXXFLAGS_WIN) -c src/main.cpp -o obj/windows/main.o
	$(CXX_WIN) $(OBJ_WIN) -o $(TARGET_WIN) $(CXXLIBS_WIN)

$(TARGET_WIN): $(OBJ_COMP_WIN) #linking
	@echo -e "$(PURPLE)Linking program for windows$(RESET)"
	$(CXX_WIN) $(OBJ_WIN) -o $(TARGET_WIN) $(CXXLIBS_WIN)
ifeq ($(DEBUG),0)
	strip $(TARGET_WIN)
endif

#compiling libs
$(OBJ_LIB_WIN):
	@echo -e "$(PURPLE)Compiling libs for windows$(RESET)"
	@for obj in $(OBJ_LIB_WIN); do \
		src=$$(echo $$obj | sed 's|obj/windows/|src/|' | sed 's|\.o$$||'); \
		if [ -f "$${src}.cpp" ]; then \
			src="$${src}.cpp"; \
		elif [ -f "$${src}.c" ]; then \
			src="$${src}.c"; \
		else \
			echo -e "$(RED)Error: No matching source for $$obj$(RESET)"; \
			exit 1; \
		fi; \
		echo -e "$(CYAN)Compiling $$src -> $$obj$(RESET)"; \
		mkdir -p $$(dirname $$obj); \
		time $(CXX_WIN) $(CXXFLAGS_LIB_WIN) -c $$src -o $$obj; \
	done


clean:
	rm -f $(TARGET)
	rm -f $(TARGET_WIN)

nuke:
	rm -rf obj/linux/*
	rm -rf obj/windows/*


test: linux $(TARGET) run
fast: linux_fast $(TARGET) run

test_win: win $(TARGET_WIN) run_win
fast_win: win_fast $(TARGET_WIN) run_win

run:
	$(TARGET) $(ARGS)
run_win:
	wine $(TARGET_WIN) $(ARGS)


tempnuke:
	rm test.asm test.o test



























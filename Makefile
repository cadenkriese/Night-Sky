PROJECT = Night_Sky
SRC_FILES = camera.cpp input_helper.cpp location.cpp main.cpp renderable_star.cpp renderer.cpp star.cpp 
H_FILES = camera.h input_helper.h location.h renderable_star.h renderer.h star.h
REZ_FILES = gaiadr3.csv dependencies shaders TemperatureLUT.png
USERNAME = caden_kriese

CXX = g++
CXXFLAGS_DEBUG = -g -O0
CXXFLAGS_WARN = -Wall -Wextra -Wconversion -Wdouble-promotion -Wunreachable-code -Wshadow -Wpedantic
CPPVERSION = -std=c++17

OBJECTS = $(SRC_FILES:.cpp=.o)

ARCHIVE_EXTENSION = zip

GLM_PATH = dependencies/glm

LIBS = -lSDL3

ifeq ($(shell echo "Windows"), "Windows")
	TARGET = $(PROJECT).exe
	DEL = del
	ZIPPER = tar -a -c -f
	ZIP_NAME = $(PROJECT)_$(USERNAME).$(ARCHIVE_EXTENSION)
	Q =
	ISYSTEM = 
	INC_PATH = dependencies/SDL3_Windows/x86_64-w64-mingw32/include/
	LIB_PATH = dependencies/SDL3_Windows/x86_64-w64-mingw32/lib/
	LIBS += -lopengl32
	RPATH =
else
	TARGET = $(PROJECT)
	DEL = rm -f
	ZIPPER = tar -acf
	Q= "

	INC_PATH = "$(shell brew --prefix sdl3)/include"
	LIB_PATH = "$(shell brew --prefix sdl3)/lib"

	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CXXFLAGS += -D LINUX
		RPATH =
		LIBS += -ludev -lX11 -lXcursor -lXrandr -lXi -lGL
		ISYSTEM = 
	endif
	ifeq ($(UNAME_S),Darwin)
		CXXFLAGS += -D OSX
		RPATH = -Wl,-rpath,/Library/Frameworks -Wl,-rpath,$(LIB_PATH)
		ISYSTEM = -isystem $(INC_PATH)
		LIBS += -framework OpenGL
	endif

	ifeq ($(shell tar --version | grep -o "GNU tar"), GNU tar)
		ARCHIVE_EXTENSION = tar.gz
	endif

	ZIP_NAME = $(PROJECT)_$(USERNAME).$(ARCHIVE_EXTENSION)
endif

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(RPATH) -L$(LIB_PATH) $(LIBS)

.cpp.o:
	$(CXX) $(CPPVERSION) $(CXXFLAGS_DEBUG) $(CXXFLAGS_WARN) -o $@ -c $< -I$(INC_PATH) -I$(GLM_PATH)

clean:
	$(DEL) $(TARGET) $(OBJECTS)

depend:
	@sed -i.bak '/^# DEPENDENCIES/,$$d' Makefile
	@$(DEL) sed*
	@echo $(Q)# DEPENDENCIES$(Q) >> Makefile
	@$(CXX) $(CPPVERSION) $(ISYSTEM) -MM $(SRC_FILES) >> Makefile

submission:
	@echo "Creating submission file $(ZIP_NAME) ..."
	@echo "...Zipping source files:   $(SRC_FILES) ..."
	@echo "...Zipping header files:   $(H_FILES) ..."
	@echo "...Zipping resource files: $(REZ_FILES)..."
	@echo "...Zipping Makefile..."
	$(ZIPPER) $(ZIP_NAME) --exclude=".DS_Store" $(SRC_FILES) $(H_FILES) $(REZ_FILES) Makefile
	@echo "...$(ZIP_NAME) done!"

.PHONY: all clean depend submission

# DEPENDENCIES
main.o: main.cpp input_helper.h star.h renderer.h
input_helper.o: input_helper.cpp input_helper.h star.h
star.o: star.cpp star.h
renderer.o: renderer.cpp renderer.h

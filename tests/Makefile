EXE = main

SOURCES = ./src/common/shader.cpp ./src/common/utils.cpp ./src/common/guid.cpp ./src/operators/types.cpp
SOURCES += ./src/layers/layerset.cpp  ./src/layers/layer.cpp
SOURCES += ./src/operators/creator.cpp ./src/operators/representer.cpp
SOURCES += ./src/operators/operation_manager.cpp ./src/operators/operation_set.cpp ./src/operators/local_operations_set.cpp
SOURCES += ./src/operators/base_operator_impl.cpp
SOURCES += ./src/drawing/draw_helper.cpp
SOURCES += ./src/operators/move.cpp ./src/operators/rotate.cpp ./src/operators/scale.cpp
SOURCES += ./main.cpp

OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)

CXXFLAGS += -Wall -Wformat -std=c++2a -I./../libs/stb -I./../src/include -I./../src/common -I./../libs/glad
CXXFLAGS += -I./../include
LIBS =

## Using OpenGL loader: glad
#SOURCES += ../libs/glad/src/glad.c
CXXFLAGS += -I../libs/glad/include -DIMGUI_IMPL_OPENGL_LOADER_GLAD

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib
	LIBS += -lglfw -lglew

	CXXFLAGS += -I/usr/local/include -I/opt/local/include  -DDARWIN
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32

	CXXFLAGS += -I./lib/gl3w/include
	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(findstring MSYS,$(UNAME_S)),MSYS)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32 -lole32 -luuid -lglog -liphlpapi -lws2_32 -lcrypto

	CXXFLAGS += -I./libs/gl3w/include -I/mingw64/include -I./libs/glfw/include -D_WIN32 -DGLOG_NO_ABBREVIATED_SEVERITIES -fpermissive -L/mingw64/lib -I./libs/nativefiledialog/src/include
	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:./../src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:./../src/common/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:./../src/layers/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:./../src/operators/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:./../src/drawing/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

debug: CXXFLAGS += -g -DDEBUG
debug: CFLAGS += -g -DDEBUG
debug: OCFLAGS += -g -DDEBUG
debug: all

clean:
	rm -f $(EXE) $(OBJS)

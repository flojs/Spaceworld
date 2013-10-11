
PLATFORM := $(shell uname)

ifneq (, $(findstring MINGW, $(PLATFORM)))
	GL_LIBS = -lopengl32 -lglu32 -lglut32
	EXT = .exe
	DEFS =-DWIN32 -static-libgcc -static-libstdc++
endif

ifneq (, $(findstring Linux, $(PLATFORM)))
    GL_LIBS = -lGL -lGLU -lglut # -L./. -lGLee
	EXT = 
    DEFS = 
endif

ifneq (, $(findstring Darwin, $(PLATFORM)))
    DEFS   = -DMacOSX -D__APPLE__ -m32 -arch x86_64    
    GL_LIBS = -framework GLUT -framework OpenGL -framework CoreFoundation
	EXT = 
endif

LINK += GLee.o shader.o matrix4f.o matrix3f.o imageloader.o scenery.o cube.o ModelViewer.o objLoader.o obj_parser.o list.o

.PHONY:  clean

all : world$(EXT)

world$(EXT) : main.o $(LINK)
	g++ $(DEFS) -o world main.o $(LINK) $(GL_LIBS)

main.o : main.cpp $(LINK)
	g++ $(DEFS) -c main.cpp
	
ModelViewer.o: ModelViewer.cpp ModelViewer.h objLoader.h
	$(CC) -c ModelViewer.cpp
	
objLoader.o: objLoader.cpp objLoader.h obj_parser.h
	g++ $(DEFS) -c objLoader.cpp

obj_parser.o: obj_parser.cpp obj_parser.h list.h
	g++ $(DEFS) -c obj_parser.cpp

list.o: list.h list.cpp
	g++ $(DEFS) -c list.cpp
	
shader.o : shader.cpp shader.h
	g++ $(DEFS) -c shader.cpp
	
matrix4f.o : matrix4f.cpp matrix4f.h
	g++ $(DEFS) -c matrix4f.cpp

matrix3f.o : matrix3f.cpp matrix3f.h
	g++ $(DEFS) -c matrix3f.cpp
	
GLee.o : GLee.c GLee.h
	gcc -c $(DEFS) GLee.c
	
imageloader.o : imageloader.cpp imageloader.h
	g++ $(DEFS) -c imageloader.cpp
	
scenery.o : scenery.cpp scenery.h
	g++ $(DEFS) -c scenery.cpp
	
cube.o : cube.cpp cube.h
	g++ $(DEFS) -c cube.cpp

clean:
	rm -f *.o world$(EXT)

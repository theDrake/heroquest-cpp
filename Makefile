all: heroquest3d

heroquest3d: src
	g++ src/*.cc -lglut -lGL -lGLU -o heroquest3d

.PHONY: all clean

clean:
	rm -f heroquest3d

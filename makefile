binary = out/mandelbrot

sources =\
src/Main.cpp

includes =\
../opengl/include

libs =\
GL\
glfw3\
GLEW\
dl\
X11\
pthread\


cxx = clang++
cxxflags = -O3 -std=c++17 $(patsubst %, -I %, $(includes)) $(patsubst %, -l %, $(libs))

$(binary): $(sources)
	$(cxx) $(sources) $(cxxflags) -o $@ 

run: $(binary)
	./$(binary)
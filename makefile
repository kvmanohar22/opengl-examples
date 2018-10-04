LINKERS := -ldl -lglfw3 -lm -lX11 -lGL -lGLU -lpthread
INCLUDE_DIRS := include

render_window: render_window.cpp
	@g++ render_window.cpp src/glad.c -I ${INCLUDE_DIRS} ${LINKERS} -o bin/render_window
	@echo "Built rendering window"

render_triangle: render_triangle.cpp
	@g++ render_triangle.cpp src/glad.c -I ${INCLUDE_DIRS} ${LINKERS} -o bin/render_triangle
	@echo "Built rendering single triangle"

render_rectangle: render_rectangle.cpp
	@g++ render_rectangle.cpp src/glad.c -I ${INCLUDE_DIRS} ${LINKERS} -o bin/render_rectangle
	@echo "Built rendering rectangle"

ex1: ex/ex1.cpp
	@g++ ex/ex1.cpp src/glad.c -I ${INCLUDE_DIRS} ${LINKERS} -o bin/ex1
	@echo "Built ex 1"

clean:
	@echo "Cleaning..."
	@rm -rf ./bin/*

all: render_window \
	  render_triangle \
	  render_rectangle \
	  ex1

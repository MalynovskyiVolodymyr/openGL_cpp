simple_rotaion: simple_rotation.cpp
	g++ simple_rotation.cpp `pkg-config --libs glew` -lglut -o simple_rotation

simple_translation: simple_translation.cpp
	g++ simple_translation.cpp `pkg-config --libs glew` -lglut -o simple_translation
	
Matrix4f.o: ./include/Matrix4f.cpp
	g++ ./include/Matrix4f.cpp -c

simple_shader: simple_shaders.cpp
	g++ simple_shaders.cpp `pkg-config --libs glew` -lglut -o simple_shader

simple_triangle: simple_trinagle.cpp
	g++ simple_trinagle.cpp `pkg-config --libs glew` -lglut -o simple_triangle

simple_dot: simple_dot.cpp
	g++ simple_dot.cpp `pkg-config --libs glew` -lglut -o simple_dot

example: example.cpp math_utils.o Vector3f.o
	g++  math_utils.o example.cpp `pkg-config --libs glew` -lglut -o example
	
math_utils.o: ./include/math_utils.cpp
	g++ ./include/math_utils.cpp -c
	
Vector3f.o: ./include/Vector3f.cpp
	g++ ./include/Vector3f.cpp -c

clean:
	rm *.o example

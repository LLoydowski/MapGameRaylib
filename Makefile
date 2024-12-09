compile:
	g++ src/main.cpp -c -o"objs/main.o" -I"inc"
	g++ src/utils.cpp -c -o"objs/utils.o" -I"inc"
	g++ src/UIElement.cpp -c -o"objs/UIElement.o" -I"inc"

link:
	g++ objs/*.o -o"release/main.exe" -Llib -lraylib -lgdi32 -lwinmm -lopengl32

run:
	make compile
	make link
	release/main.exe
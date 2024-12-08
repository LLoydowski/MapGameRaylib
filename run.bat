g++ src/main.cpp -c -o"objs/main.o" -I"inc"


g++ objs/*.o -o"release/main.exe" -Llib -lraylib -lgdi32 -lwinmm -lopengl32
 
"release/main.exe"

pause
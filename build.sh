#!/usr/bin/bash
if g++ -o mandelbrot.out mandelbrot.c include/glad/glad.c -Iinclude -Llib lib/cimgui.so -lglfw3 -O2;  then
    echo 'build successful'
    ./mandelbrot.out
    rm mandelbrot.out
else
    echo 'build failed lol!'
fi

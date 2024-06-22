
#include "./init.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <math.h>

void init(void) {
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 800, 800, 0);
}

void dibujar_mapa() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glFlush();
}

const char* get_carnets(void){
    return "mm19247, ";
}
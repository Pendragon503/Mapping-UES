#include <stdio.h>
#include <stdlib.h>
#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>
#include<GL/freeglut.h>
#include <math.h>
#include "./mapa_prueba.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Mapa con Movimiento y Objetos 3D");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}



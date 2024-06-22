
#include "./init.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <math.h>

/*
 * 
 */
int main(int argc, char** argv) {
    printf("Examen parcial de: %s \n",get_carnets());
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Parcial 3");
    glClearColor(0.8, 0, 0, 1);
    init();
    glutDisplayFunc(dibujar_mapa);
    glutMainLoop();
    printf("=== fin ===");
    return (EXIT_SUCCESS);
}


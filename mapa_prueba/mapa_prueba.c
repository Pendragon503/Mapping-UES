#include <GL/glut.h>
#include <stdio.h>
#include "./mapa_prueba.h"


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define VIEWPORT_WIDTH 600
#define VIEWPORT_HEIGHT 600

int character_x = 100; // Posición inicial del personaje
int character_y = -400; // Posición inicial del personaje

int buildingPositions[6][2] = {
    //Juridicas
    {0, -140},
    //Aulas N
    {0, -25},
    //Usos multiples
    {350, 100},
    //Academica
    {-350, 100},
    //Medicina
    {350, 350},
    //Bunker
    {-200, 350}
};
int buildingSize = 50;

// Variables para el relleno de edificios
int fillBuildings[6] = {0, 0, 0, 0, 0, 0};

void init() {
    glClearColor(1.0, 0.0, 1.0, 0.0); // Cambiar el color de fondo a verde
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // en el init se agregan estas dos lineas
    gluPerspective(45.0, (double) SCREEN_WIDTH / (double) SCREEN_HEIGHT, 1.0, 1000.0); //esta
    glMatrixMode(GL_MODELVIEW); //esta

}

void drawCube(float x, float y, float z, float size) {
    float halfSize = size / 2.0f;
    glBegin(GL_QUADS);

    // Front face
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(x - halfSize, y - halfSize, z + halfSize);
    glVertex3f(x + halfSize, y - halfSize, z + halfSize);
    glVertex3f(x + halfSize, y + halfSize, z + halfSize);
    glVertex3f(x - halfSize, y + halfSize, z + halfSize);

    // Back face
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(x - halfSize, y - halfSize, z - halfSize);
    glVertex3f(x + halfSize, y - halfSize, z - halfSize);
    glVertex3f(x + halfSize, y + halfSize, z - halfSize);
    glVertex3f(x - halfSize, y + halfSize, z - halfSize);

    // Left face
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(x - halfSize, y - halfSize, z - halfSize);
    glVertex3f(x - halfSize, y - halfSize, z + halfSize);
    glVertex3f(x - halfSize, y + halfSize, z + halfSize);
    glVertex3f(x - halfSize, y + halfSize, z - halfSize);

    // Right face
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f(x + halfSize, y - halfSize, z - halfSize);
    glVertex3f(x + halfSize, y - halfSize, z + halfSize);
    glVertex3f(x + halfSize, y + halfSize, z + halfSize);
    glVertex3f(x + halfSize, y + halfSize, z - halfSize);

    // Top face
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(x - halfSize, y + halfSize, z - halfSize);
    glVertex3f(x + halfSize, y + halfSize, z - halfSize);
    glVertex3f(x + halfSize, y + halfSize, z + halfSize);
    glVertex3f(x - halfSize, y + halfSize, z + halfSize);

    // Bottom face
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(x - halfSize, y - halfSize, z - halfSize);
    glVertex3f(x + halfSize, y - halfSize, z - halfSize);
    glVertex3f(x + halfSize, y - halfSize, z + halfSize);
    glVertex3f(x - halfSize, y - halfSize, z + halfSize);

    glEnd();
}

void drawGround() {
    glColor3f(0.0, 1.0, 0.0); // Gris oscuro para el suelo
    glBegin(GL_QUADS);
    glVertex3f(-500, -500, 0);
    glVertex3f(500, -500, 0);
    glVertex3f(500, 500, 0);
    glVertex3f(-500, 500, 0);
    glEnd();
}

void BresenhamLinea(int x1, int y1, int x2, int y2) {
    glColor3f(0.0, 0.0, 1.0);
    //glPointSize(3);
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int pasos;
    if (dy > dx) {
        pasos = 1;
    } else {
        pasos = 0;
    }

    if (pasos) {
        int temp = x1;
        x1 = y1;
        y1 = temp;

        temp = x2;
        x2 = y2;
        y2 = temp;
    }

    if (x1 > x2) {
        int temp = x1;
        x1 = x2;
        x2 = temp;

        temp = y1;
        y1 = y2;
        y2 = temp;
    }

    dx = x2 - x1;
    dy = abs(y2 - y1);
    int error = dx / 2;
    int ypasos = (y1 < y2) ? 1 : -1;
    int y = y1;

    for (int x = x1; x <= x2; x++) {
        if (pasos) {
            glBegin(GL_POINTS);
            glVertex2i(y, x);
            glEnd();
        } else {
            glBegin(GL_POINTS);
            glVertex2i(x, y);
            glEnd();
        }

        error -= dy;

        if (error < 0) {
            y += ypasos;
            error += dx;
        }
    }
}

void puntear(int x, int y) {
    //glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Función para dibujar el círculo utilizando el algoritmo de punto central

void circunferencia(int xcentro, int ycentro, int radio) {
    int x = 0;
    int y = radio;
    int p = 1 - radio;

    // Dibuja los puntos del círculo en los octantes correspondientes
    while (x <= y) {
        puntear(xcentro + x, ycentro + y);
        puntear(xcentro + y, ycentro + x);
        puntear(xcentro + y, ycentro - x);
        puntear(xcentro + x, ycentro - y);
        puntear(xcentro - x, ycentro - y);
        puntear(xcentro - y, ycentro - x);
        puntear(xcentro - y, ycentro + x);
        puntear(xcentro - x, ycentro + y);

        if (p < 0) {
            p += 2 * x + 3;
        } else {
            p += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Configurar el viewport para 600x600 centrado
    glViewport((SCREEN_WIDTH - VIEWPORT_WIDTH) / 2, (SCREEN_HEIGHT - VIEWPORT_HEIGHT) / 2, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0, (double) VIEWPORT_WIDTH / (double) VIEWPORT_HEIGHT, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    // Ajustar la posición de la cámara para una mejor perspectiva
    gluLookAt(character_x, character_y, 500, character_x, character_y, 0, 0, 1, 0);

    // Dibujar el suelo
    drawGround();

    // Dibujar el personaje (una simple esfera)
    glPushMatrix();
    glTranslatef(character_x, character_y, 10); // Posiciona la esfera ligeramente por encima del suelo
    glColor3f(1.0, 1.0, 1.0);
    glutSolidSphere(10, 50, 50); // Dibuja el personaje como una esfera sólida
    glPopMatrix();

    // Dibujar edificios como cubos
    for (int i = 0; i < 6; i++) {
        glPushMatrix();
        glTranslatef(buildingPositions[i][0], buildingPositions[i][1], buildingSize / 2); // Alinea los cubos con el suelo
        glColor3f(0.5, 0.5, 0.5);
        drawCube(0, 0, 0, buildingSize);
        glPopMatrix();
    }

    glDisable(GL_DEPTH_TEST);
    //Calle inicial
    BresenhamLinea(50, -500, 50, -300);
    BresenhamLinea(150, -500, 150, -300);
    //Calle horizontal izquierda
    BresenhamLinea(50, -300, -450, -300);
    BresenhamLinea(50, -200, -450, -200);
    //Calle horizontal derecha
    BresenhamLinea(150, -300, 450, -300);
    BresenhamLinea(150, -200, 450, -200);
    //Calle vertical
    BresenhamLinea(150, -200, 150, 0);
    BresenhamLinea(50, -200, 50, 0);
    //Fuente
    circunferencia(100, 100, 40);
    circunferencia(100, 100, 60);
    //Hacia multiples
    BresenhamLinea(200, 50, 300, 50);
    BresenhamLinea(200, 150, 300, 150);
    //Hacia academica
    BresenhamLinea(0, 50, -300, 50);
    BresenhamLinea(0, 150, -300, 150);

    BresenhamLinea(50, 200, 50, 300);
    BresenhamLinea(150, 200, 150, 300);
    //Hacia medicina
    BresenhamLinea(150, 300, 275, 300);
    BresenhamLinea(150, 400, 275, 400);

    //Hacia el bunker
    BresenhamLinea(-125, 400, 150, 400);
    BresenhamLinea(-125, 300, 50, 300);

    glEnable(GL_DEPTH_TEST);

    glFlush();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': character_y += 10;
            break;
        case 's': character_y -= 10;
            break;
        case 'a': character_x -= 10;
            break;
        case 'd': character_x += 10;
            break;
        case '1': fillBuildings[0] = !fillBuildings[0];
            break;
        case '2': fillBuildings[1] = !fillBuildings[1];
            break;
        case '3': fillBuildings[2] = !fillBuildings[2];
            break;
        case '4': fillBuildings[3] = !fillBuildings[3];
            break;
        case '5': fillBuildings[4] = !fillBuildings[4];
            break;
        case '6': fillBuildings[5] = !fillBuildings[5];
            break;
        case 27: exit(0);
            break; // Tecla Esc
    }
    glutPostRedisplay();
}


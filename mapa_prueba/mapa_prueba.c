#include <GL/glut.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

int character_x = 400; // Posición inicial del personaje
int character_y = 400; // Posición inicial del personaje

int buildingPositions[6][2] = {{100, 100}, {200, 200}, {300, 300}, {400, 400}, {500, 500}, {600, 600}};
int buildingSize = 50;

// Variables para el relleno de edificios
int fillBuildings[6] = {0, 0, 0, 0, 0, 0};

void init() {
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
}

// Algoritmo DDA para dibujar una línea
void drawLineDDA(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    
    float Xinc = dx / (float) steps;
    float Yinc = dy / (float) steps;
    
    float X = x1;
    float Y = y1;
    for (int i = 0; i <= steps; i++) {
        glVertex2i(X, Y);
        X += Xinc;
        Y += Yinc;
    }
}

// Obtener el color del píxel en (x, y)
void getPixelColor(int x, int y, float color[3]) {
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}

// Establecer el color del píxel en (x, y)
void setPixelColor(int x, int y, float color[3]) {
    glColor3fv(color);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

// Algoritmo de relleno por frontera (boundary fill)
void boundaryFill(int x, int y, float fillColor[3], float borderColor[3]) {
    float currentColor[3];
    getPixelColor(x, y, currentColor);

    if ((currentColor[0] != borderColor[0] || currentColor[1] != borderColor[1] || currentColor[2] != borderColor[2]) &&
        (currentColor[0] != fillColor[0] || currentColor[1] != fillColor[1] || currentColor[2] != fillColor[2])) {
        setPixelColor(x, y, fillColor);
        boundaryFill(x + 1, y, fillColor, borderColor);
        boundaryFill(x - 1, y, fillColor, borderColor);
        boundaryFill(x, y + 1, fillColor, borderColor);
        boundaryFill(x, y - 1, fillColor, borderColor);
    }
}

// Función para dibujar un edificio usando DDA
void drawBuilding(int x, int y, int width, int height, int fill) {
    glBegin(GL_LINE_LOOP);
    drawLineDDA(x, y, x + width, y);
    drawLineDDA(x + width, y, x + width, y + height);
    drawLineDDA(x + width, y + height, x, y + height);
    drawLineDDA(x, y + height, x, y);
    glEnd();

    if (fill) {
        float fillColor[3] = {0.0, 1.0, 0.0}; // Color de relleno (verde)
        float borderColor[3] = {0.5, 0.5, 0.5}; // Color del borde (gris)
        boundaryFill(x + 1, y + 1, fillColor, borderColor);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Configurar viewport para seguir al personaje
    int viewport_x = character_x - SCREEN_WIDTH / 2;
    int viewport_y = character_y - SCREEN_HEIGHT / 2;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(viewport_x, viewport_x + SCREEN_WIDTH, viewport_y, viewport_y + SCREEN_HEIGHT);
    glMatrixMode(GL_MODELVIEW);

    // Dibujar el personaje (un simple círculo)
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(character_x, character_y, 0);
    glutSolidSphere(20, 20, 20); // Dibuja el personaje como un círculo
    glPopMatrix();

    // Dibujar edificios usando DDA
    for (int i = 0; i < 6; i++) {
        glColor3f(0.5, 0.5, 0.5);
        drawBuilding(buildingPositions[i][0], buildingPositions[i][1], buildingSize, buildingSize, fillBuildings[i]);
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': character_y += 10; break;
        case 's': character_y -= 10; break;
        case 'a': character_x -= 10; break;
        case 'd': character_x += 10; break;
        case '1': fillBuildings[0] = !fillBuildings[0]; break;
        case '2': fillBuildings[1] = !fillBuildings[1]; break;
        case '3': fillBuildings[2] = !fillBuildings[2]; break;
        case '4': fillBuildings[3] = !fillBuildings[3]; break;
        case '5': fillBuildings[4] = !fillBuildings[4]; break;
        case '6': fillBuildings[5] = !fillBuildings[5]; break;
    }
    glutPostRedisplay();
}


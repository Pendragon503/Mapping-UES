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
    //{0, -140},
    //Aulas N
    //{0, -25},
    //Academica
    //{-350, 100},
    //Medicina
    //{350, 350}
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

void Bresenham3D(float x0, float y0, float z0, float x1, float y1, float z1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int dz = abs(z1 - z0);
    int xs = (x0 < x1) ? 1 : -1;
    int ys = (y0 < y1) ? 1 : -1;
    int zs = (z0 < z1) ? 1 : -1;

    int x = x0;
    int y = y0;
    int z = z0;

    if (dx >= dy && dx >= dz) {        // Driving axis is X-axis
        int p1 = 2 * dy - dx;
        int p2 = 2 * dz - dx;
        while (x != x1) {
            x += xs;
            if (p1 >= 0) {
                y += ys;
                p1 -= 2 * dx;
            }
            if (p2 >= 0) {
                z += zs;
                p2 -= 2 * dx;
            }
            p1 += 2 * dy;
            p2 += 2 * dz;
            glBegin(GL_POINTS);
            glVertex3f(x, y, z);
            glEnd();
        }
    } else if (dy >= dx && dy >= dz) { // Driving axis is Y-axis
        int p1 = 2 * dx - dy;
        int p2 = 2 * dz - dy;
        while (y != y1) {
            y += ys;
            if (p1 >= 0) {
                x += xs;
                p1 -= 2 * dy;
            }
            if (p2 >= 0) {
                z += zs;
                p2 -= 2 * dy;
            }
            p1 += 2 * dx;
            p2 += 2 * dz;
            glBegin(GL_POINTS);
            glVertex3f(x, y, z);
            glEnd();
        }
    } else {                           // Driving axis is Z-axis
        int p1 = 2 * dy - dz;
        int p2 = 2 * dx - dz;
        while (z != z1) {
            z += zs;
            if (p1 >= 0) {
                y += ys;
                p1 -= 2 * dz;
            }
            if (p2 >= 0) {
                x += xs;
                p2 -= 2 * dz;
            }
            p1 += 2 * dy;
            p2 += 2 * dx;
            glBegin(GL_POINTS);
            glVertex3f(x, y, z);
            glEnd();
        }
    }
}

void drawWindowsPoints(float startX, float startY, float startZ, float buildingSize) {
    // Color de las ventanas
    glColor3f(1.0f, 1.0f, 1.0f); // Color blanco para las ventanas

    // Dibujar las ventanas como puntos (ajustar según sea necesario)
    glPointSize(3.0f); // Tamaño de los puntos (ajustar según sea necesario)

    // Calcular la posición de las ventanas
    float windowWidth = buildingSize / 4.0f;
    float windowHeight = buildingSize / 4.0f;
    float windowDepth = buildingSize / 4.0f;

    // Dibujar ventanas en las caras del edificio
    glBegin(GL_POINTS);
    // Cara frontal
    glVertex3f(startX - windowWidth, startY - windowHeight, startZ + windowDepth);
    glVertex3f(startX + windowWidth, startY - windowHeight, startZ + windowDepth);
    glVertex3f(startX - windowWidth, startY + windowHeight, startZ + windowDepth);
    glVertex3f(startX + windowWidth, startY + windowHeight, startZ + windowDepth);

    // Cara posterior
    glVertex3f(startX - windowWidth, startY - windowHeight, startZ - windowDepth);
    glVertex3f(startX + windowWidth, startY - windowHeight, startZ - windowDepth);
    glVertex3f(startX - windowWidth, startY + windowHeight, startZ - windowDepth);
    glVertex3f(startX + windowWidth, startY + windowHeight, startZ - windowDepth);

    // Cara izquierda
    glVertex3f(startX - windowWidth, startY - windowHeight, startZ + windowDepth);
    glVertex3f(startX - windowWidth, startY - windowHeight, startZ - windowDepth);
    glVertex3f(startX - windowWidth, startY + windowHeight, startZ + windowDepth);
    glVertex3f(startX - windowWidth, startY + windowHeight, startZ - windowDepth);

    // Cara derecha
    glVertex3f(startX + windowWidth, startY - windowHeight, startZ + windowDepth);
    glVertex3f(startX + windowWidth, startY - windowHeight, startZ - windowDepth);
    glVertex3f(startX + windowWidth, startY + windowHeight, startZ + windowDepth);
    glVertex3f(startX + windowWidth, startY + windowHeight, startZ - windowDepth);

    glEnd();
}


void drawCube2(float x, float y, float z, float size) {
    float halfSize = size / 2.0f;
    float vertices[8][3] = {
        {x - halfSize, y - halfSize, z - halfSize},
        {x + halfSize, y - halfSize, z - halfSize},
        {x + halfSize, y + halfSize, z - halfSize},
        {x - halfSize, y + halfSize, z - halfSize},
        {x - halfSize, y - halfSize, z + halfSize},
        {x + halfSize, y - halfSize, z + halfSize},
        {x + halfSize, y + halfSize, z + halfSize},
        {x - halfSize, y + halfSize, z + halfSize}
    };

    // Dibujar las 12 aristas del cubo usando Bresenham3D
    Bresenham3D(vertices[0][0], vertices[0][1], vertices[0][2], vertices[1][0], vertices[1][1], vertices[1][2]);
    Bresenham3D(vertices[1][0], vertices[1][1], vertices[1][2], vertices[2][0], vertices[2][1], vertices[2][2]);
    Bresenham3D(vertices[2][0], vertices[2][1], vertices[2][2], vertices[3][0], vertices[3][1], vertices[3][2]);
    Bresenham3D(vertices[3][0], vertices[3][1], vertices[3][2], vertices[0][0], vertices[0][1], vertices[0][2]);

    Bresenham3D(vertices[4][0], vertices[4][1], vertices[4][2], vertices[5][0], vertices[5][1], vertices[5][2]);
    Bresenham3D(vertices[5][0], vertices[5][1], vertices[5][2], vertices[6][0], vertices[6][1], vertices[6][2]);
    Bresenham3D(vertices[6][0], vertices[6][1], vertices[6][2], vertices[7][0], vertices[7][1], vertices[7][2]);
    Bresenham3D(vertices[7][0], vertices[7][1], vertices[7][2], vertices[4][0], vertices[4][1], vertices[4][2]);

    Bresenham3D(vertices[0][0], vertices[0][1], vertices[0][2], vertices[4][0], vertices[4][1], vertices[4][2]);
    Bresenham3D(vertices[1][0], vertices[1][1], vertices[1][2], vertices[5][0], vertices[5][1], vertices[5][2]);
    Bresenham3D(vertices[2][0], vertices[2][1], vertices[2][2], vertices[6][0], vertices[6][1], vertices[6][2]);
    Bresenham3D(vertices[3][0], vertices[3][1], vertices[3][2], vertices[7][0], vertices[7][1], vertices[7][2]);
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

void bunker() {
    float startX1 = -200;
    float startY1 = 350;
    float startX2 = -265; // Segunda posición en X para el segundo edificio
    float startY2 = 350;

    // Dimensiones del edificio
    float buildingWidth = 75;

    float buildingHeight = 100; // Altura del edificio
    float buildingDepth = 50;

    // Color del edificio
    GLfloat buildingColor[] = {0.82f, 0.41f, 0.12f}; // Ocre

    // Color de las ventanas
    GLfloat windowColor[] = {0.0f, 0.0f, 0.55f}; // Azul oscuro

    glPushMatrix();

    // Aplicar rotación para que el edificio esté en diagonal
    glTranslatef(startX1, startY1, 0);
    glRotatef(45, 0, 1, 0);
    glTranslatef(-startX1, -startY1, 0);

    // Dibujar el primer edificio (rectángulo)
    glColor3fv(buildingColor);
    glBegin(GL_QUADS);

    // Front face
    glVertex3f(startX1 - buildingWidth / 2, startY1 - buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX1 + buildingWidth / 2, startY1 - buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX1 + buildingWidth / 2, startY1 + buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX1 - buildingWidth / 2, startY1 + buildingHeight / 2, buildingDepth / 2);

    // Back face
    glVertex3f(startX1 - buildingWidth / 2, startY1 - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX1 + buildingWidth / 2, startY1 - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX1 + buildingWidth / 2, startY1 + buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX1 - buildingWidth / 2, startY1 + buildingHeight / 2, -buildingDepth / 2);

    // Left face
    glVertex3f(startX1 - buildingWidth / 2, startY1 - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX1 - buildingWidth / 2, startY1 - buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX1 - buildingWidth / 2, startY1 + buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX1 - buildingWidth / 2, startY1 + buildingHeight / 2, -buildingDepth / 2);

    // Right face
    glVertex3f(startX1 + buildingWidth / 2, startY1 - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX1 + buildingWidth / 2, startY1 - buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX1 + buildingWidth / 2, startY1 + buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX1 + buildingWidth / 2, startY1 + buildingHeight / 2, -buildingDepth / 2);

    // Top face
    glVertex3f(startX1 - buildingWidth / 2, startY1 + buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX1 + buildingWidth / 2, startY1 + buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX1 + buildingWidth / 2, startY1 + buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX1 - buildingWidth / 2, startY1 + buildingHeight / 2, buildingDepth / 2);

    // Bottom face
    glVertex3f(startX1 - buildingWidth / 2, startY1 - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX1 + buildingWidth / 2, startY1 - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX1 + buildingWidth / 2, startY1 - buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX1 - buildingWidth / 2, startY1 - buildingHeight / 2, buildingDepth / 2);

    glEnd();

    // Dibujar las ventanas (3 ventanas en cada lado)
    float windowWidth = buildingWidth / 2.5f;
    float windowHeight = buildingHeight / 8;
    float windowDepth = 1.0f;

    glColor3fv(windowColor);

    for (int i = 0; i < 3; i++) {
        // Ventanas del lado izquierdo
        glBegin(GL_QUADS);
        glVertex3f(startX1 - buildingWidth / 2 - windowDepth, startY1 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, -buildingDepth / 4);
        glVertex3f(startX1 - buildingWidth / 2 - windowDepth, startY1 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, buildingDepth / 4);
        glVertex3f(startX1 - buildingWidth / 2 - windowDepth, startY1 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, buildingDepth / 4);
        glVertex3f(startX1 - buildingWidth / 2 - windowDepth, startY1 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, -buildingDepth / 4);
        glEnd();

        // Ventanas del lado derecho
        glBegin(GL_QUADS);
        glVertex3f(startX1 + buildingWidth / 2 + windowDepth, startY1 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, -buildingDepth / 4);
        glVertex3f(startX1 + buildingWidth / 2 + windowDepth, startY1 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, buildingDepth / 4);
        glVertex3f(startX1 + buildingWidth / 2 + windowDepth, startY1 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, buildingDepth / 4);
        glVertex3f(startX1 + buildingWidth / 2 + windowDepth, startY1 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, -buildingDepth / 4);
        glEnd();

        // Ventanas de la cara frontal
        glBegin(GL_QUADS);
        glVertex3f(startX1 - buildingWidth / 4, startY1 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, buildingDepth / 2 + windowDepth);
        glVertex3f(startX1 + buildingWidth / 4, startY1 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, buildingDepth / 2 + windowDepth);
        glVertex3f(startX1 + buildingWidth / 4, startY1 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, buildingDepth / 2 + windowDepth);
        glVertex3f(startX1 - buildingWidth / 4, startY1 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, buildingDepth / 2 + windowDepth);
        glEnd();

        // Ventanas de la cara trasera
        glBegin(GL_QUADS);
        glVertex3f(startX1 - buildingWidth / 4, startY1 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, -buildingDepth / 2 - windowDepth);
        glVertex3f(startX1 + buildingWidth / 4, startY1 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, -buildingDepth / 2 - windowDepth);
        glVertex3f(startX1 + buildingWidth / 4, startY1 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, -buildingDepth / 2 - windowDepth);
        glVertex3f(startX1 - buildingWidth / 4, startY1 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, -buildingDepth / 2 - windowDepth);
        glEnd();
    }

    glPopMatrix();

    glPushMatrix();

    // Aplicar rotación para que el edificio esté en diagonal
    glTranslatef(startX2, startY2, 0);
    glRotatef(45, 0, 1, 0);
    glTranslatef(-startX2, -startY2, 0);

    // Dibujar el segundo edificio (rectángulo)
    glColor3fv(buildingColor);
    glBegin(GL_QUADS);

    // Front face
    glVertex3f(startX2 - buildingWidth / 2, startY2 - buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX2 + buildingWidth / 2, startY2 - buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX2 + buildingWidth / 2, startY2 + buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX2 - buildingWidth / 2, startY2 + buildingHeight / 2, buildingDepth / 2);

    // Back face
    glVertex3f(startX2 - buildingWidth / 2, startY2 - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX2 + buildingWidth / 2, startY2 - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX2 + buildingWidth / 2, startY2 + buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX2 - buildingWidth / 2, startY2 + buildingHeight / 2, -buildingDepth / 2);

    // Left face
    glVertex3f(startX2 - buildingWidth / 2, startY2 - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX2 - buildingWidth / 2, startY2 - buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX2 - buildingWidth / 2, startY2 + buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX2 - buildingWidth / 2, startY2 + buildingHeight / 2, -buildingDepth / 2);

    // Right face
    glVertex3f(startX2 + buildingWidth / 2, startY2 - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX2 + buildingWidth / 2, startY2 - buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX2 + buildingWidth / 2, startY2 + buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX2 + buildingWidth / 2, startY2 + buildingHeight / 2, -buildingDepth / 2);

    // Top face
    glVertex3f(startX2 - buildingWidth / 2, startY2 + buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX2 + buildingWidth / 2, startY2 + buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX2 + buildingWidth / 2, startY2 + buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX2 - buildingWidth / 2, startY2 + buildingHeight / 2, buildingDepth / 2);

    // Bottom face
    glVertex3f(startX2 - buildingWidth / 2, startY2 - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX2 + buildingWidth / 2, startY2 - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX2 + buildingWidth / 2, startY2 - buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX2 - buildingWidth / 2, startY2 - buildingHeight / 2, buildingDepth / 2);

    glEnd();

    // Dibujar las ventanas (3 ventanas en cada lado)
    glColor3fv(windowColor);

    for (int i = 0; i < 3; i++) {
        // Ventanas del lado izquierdo
        glBegin(GL_QUADS);
        glVertex3f(startX2 - buildingWidth / 2 - windowDepth, startY2 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, -buildingDepth / 4);
        glVertex3f(startX2 - buildingWidth / 2 - windowDepth, startY2 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, buildingDepth / 4);
        glVertex3f(startX2 - buildingWidth / 2 - windowDepth, startY2 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, buildingDepth / 4);
        glVertex3f(startX2 - buildingWidth / 2 - windowDepth, startY2 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, -buildingDepth / 4);
        glEnd();

        // Ventanas del lado derecho
        glBegin(GL_QUADS);
        glVertex3f(startX2 + buildingWidth / 2 + windowDepth, startY2 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, -buildingDepth / 4);
        glVertex3f(startX2 + buildingWidth / 2 + windowDepth, startY2 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, buildingDepth / 4);
        glVertex3f(startX2 + buildingWidth / 2 + windowDepth, startY2 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, buildingDepth / 4);
        glVertex3f(startX2 + buildingWidth / 2 + windowDepth, startY2 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, -buildingDepth / 4);
        glEnd();

        // Ventanas de la cara frontal
        glBegin(GL_QUADS);
        glVertex3f(startX2 - buildingWidth / 4, startY2 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, buildingDepth / 2 + windowDepth);
        glVertex3f(startX2 + buildingWidth / 4, startY2 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, buildingDepth / 2 + windowDepth);
        glVertex3f(startX2 + buildingWidth / 4, startY2 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, buildingDepth / 2 + windowDepth);
        glVertex3f(startX2 - buildingWidth / 4, startY2 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, buildingDepth / 2 + windowDepth);
        glEnd();

        // Ventanas de la cara trasera
        glBegin(GL_QUADS);
        glVertex3f(startX2 - buildingWidth / 4, startY2 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, -buildingDepth / 2 - windowDepth);
        glVertex3f(startX2 + buildingWidth / 4, startY2 + (i - 1) * (buildingHeight / 3) - windowHeight / 2, -buildingDepth / 2 - windowDepth);
        glVertex3f(startX2 + buildingWidth / 4, startY2 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, -buildingDepth / 2 - windowDepth);
        glVertex3f(startX2 - buildingWidth / 4, startY2 + (i - 1) * (buildingHeight / 3) + windowHeight / 2, -buildingDepth / 2 - windowDepth);
        glEnd();
    }

    glPopMatrix();
}

// Método independiente para dibujar el edificio
void usosMultiples() {
    float startX = 350;
    float startY = 100;

    // Dimensiones del edificio
    float buildingWidth = 100;
    float buildingHeight = 75; // Aumentar la altura del edificio
    float buildingDepth = 150;

    // Color del edificio
    GLfloat buildingColor[] = {0.53f, 0.81f, 0.98f}; // Celeste

    // Color de las ventanas
    GLfloat windowColor[] = {0.0f, 0.0f, 0.55f}; // Azul oscuro

    glPushMatrix();

    // Dibujar el edificio principal
    glColor3fv(buildingColor);
    glBegin(GL_QUADS);

    // Front face
    glVertex3f(startX - buildingWidth / 2, startY - buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX + buildingWidth / 2, startY - buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX + buildingWidth / 2, startY + buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX - buildingWidth / 2, startY + buildingHeight / 2, buildingDepth / 2);

    // Back face
    glVertex3f(startX - buildingWidth / 2, startY - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX + buildingWidth / 2, startY - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX + buildingWidth / 2, startY + buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX - buildingWidth / 2, startY + buildingHeight / 2, -buildingDepth / 2);

    // Left face
    glVertex3f(startX - buildingWidth / 2, startY - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX - buildingWidth / 2, startY - buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX - buildingWidth / 2, startY + buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX - buildingWidth / 2, startY + buildingHeight / 2, -buildingDepth / 2);

    // Right face
    glVertex3f(startX + buildingWidth / 2, startY - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX + buildingWidth / 2, startY - buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX + buildingWidth / 2, startY + buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX + buildingWidth / 2, startY + buildingHeight / 2, -buildingDepth / 2);

    // Top face
    glVertex3f(startX - buildingWidth / 2, startY + buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX + buildingWidth / 2, startY + buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX + buildingWidth / 2, startY + buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX - buildingWidth / 2, startY + buildingHeight / 2, buildingDepth / 2);

    // Bottom face
    glVertex3f(startX - buildingWidth / 2, startY - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX + buildingWidth / 2, startY - buildingHeight / 2, -buildingDepth / 2);
    glVertex3f(startX + buildingWidth / 2, startY - buildingHeight / 2, buildingDepth / 2);
    glVertex3f(startX - buildingWidth / 2, startY - buildingHeight / 2, buildingDepth / 2);

    glEnd();

    // Dibujar las ventanas (6 ventanas)
    float windowWidth = buildingWidth / 2.5f;
    float windowHeight = buildingHeight / 8;
    float windowDepth = 1.0f;

    glColor3fv(windowColor);

    // Ventanas del lado izquierdo y derecho
    for (int i = 0; i < 3; i++) {
        // Izquierda
        glBegin(GL_QUADS);
        glVertex3f(startX - buildingWidth / 2 - windowDepth, startY + (i - 1) * (buildingHeight / 3) - windowHeight / 2, -buildingDepth / 2 + 10);
        glVertex3f(startX - buildingWidth / 2 - windowDepth, startY + (i - 1) * (buildingHeight / 3) - windowHeight / 2, buildingDepth / 2 - 10);
        glVertex3f(startX - buildingWidth / 2 - windowDepth, startY + (i - 1) * (buildingHeight / 3) + windowHeight / 2, buildingDepth / 2 - 10);
        glVertex3f(startX - buildingWidth / 2 - windowDepth, startY + (i - 1) * (buildingHeight / 3) + windowHeight / 2, -buildingDepth / 2 + 10);
        glEnd();

        // Derecha
        glBegin(GL_QUADS);
        glVertex3f(startX + buildingWidth / 2 + windowDepth, startY + (i - 1) * (buildingHeight / 3) - windowHeight / 2, -buildingDepth / 2 + 10);
        glVertex3f(startX + buildingWidth / 2 + windowDepth, startY + (i - 1) * (buildingHeight / 3) - windowHeight / 2, buildingDepth / 2 - 10);
        glVertex3f(startX + buildingWidth / 2 + windowDepth, startY + (i - 1) * (buildingHeight / 3) + windowHeight / 2, buildingDepth / 2 - 10);
        glVertex3f(startX + buildingWidth / 2 + windowDepth, startY + (i - 1) * (buildingHeight / 3) + windowHeight / 2, -buildingDepth / 2 + 10);
        glEnd();
    }

    // Dibujar barras horizontales arriba de las ventanas
    float barHeight = windowHeight / 4;
    for (int i = 0; i < 3; i++) {
        // Izquierda
        glBegin(GL_QUADS);
        glVertex3f(startX - buildingWidth / 2 - windowDepth, startY + (i - 1) * (buildingHeight / 3) + windowHeight / 2, -buildingDepth / 2 + 10);
        glVertex3f(startX - buildingWidth / 2 - windowDepth, startY + (i - 1) * (buildingHeight / 3) + windowHeight / 2 + barHeight, -buildingDepth / 2 + 10);
        glVertex3f(startX - buildingWidth / 2 - windowDepth, startY + (i - 1) * (buildingHeight / 3) + windowHeight / 2 + barHeight, buildingDepth / 2 - 10);
        glVertex3f(startX - buildingWidth / 2 - windowDepth, startY + (i - 1) * (buildingHeight / 3) + windowHeight / 2, buildingDepth / 2 - 10);
        glEnd();

        // Derecha
        glBegin(GL_QUADS);
        glVertex3f(startX + buildingWidth / 2 + windowDepth, startY + (i - 1) * (buildingHeight / 3) + windowHeight / 2, -buildingDepth / 2 + 10);
        glVertex3f(startX + buildingWidth / 2 + windowDepth, startY + (i - 1) * (buildingHeight / 3) + windowHeight / 2 + barHeight, -buildingDepth / 2 + 10);
        glVertex3f(startX + buildingWidth / 2 + windowDepth, startY + (i - 1) * (buildingHeight / 3) + windowHeight / 2 + barHeight, buildingDepth / 2 - 10);
        glVertex3f(startX + buildingWidth / 2 + windowDepth, startY + (i - 1) * (buildingHeight / 3) + windowHeight / 2, buildingDepth / 2 - 10);
        glEnd();
    }
    
    glPopMatrix();
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
    
   // Dibujar el edificio de Ciencias Jurídicas
    glColor3f(0.0, 0.0, 0.5);
    float juridicaSizeX = buildingSize * 1.5;
    drawCube(0, -140, 0, juridicaSizeX);
    
    // Dibujar el edificio de Academica
//    glColor3f(0.5, 0.0, 0.0);
//    float academicaSizeX = buildingSize * 1.5;
//    drawCube(-350, 100, 0, academicaSizeX);


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
    
    bunker();
    usosMultiples();
    
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


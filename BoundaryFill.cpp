#include <iostream>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

using namespace std;

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480);
}

// Boundary fill (4-connected)
void bound_it(int x, int y, float* fillColor, float* bc) {
    float color[3];
    glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, color);

    if ((color[0] != bc[0] || color[1] != bc[1] || color[2] != bc[2]) &&
        (color[0] != fillColor[0] || color[1] != fillColor[1] || color[2] != fillColor[2])) {
        
        glColor3f(fillColor[0], fillColor[1], fillColor[2]);
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
        glFlush();

        bound_it(x + 2, y, fillColor, bc); // Right
        bound_it(x - 2, y, fillColor, bc); // Left
        bound_it(x, y + 2, fillColor, bc); // Up
        bound_it(x, y - 2, fillColor, bc); // Down
    }
}

void mouse(int btn, int state, int x, int y) {
    float bCol[] = {0, 0, 1};   // Boundary color (blue)
    float color[] = {0, 0, 1};  // Fill color (blue)

    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        y = 480 - y; // Convert from screen to OpenGL coordinates
        bound_it(x, y, color, bCol);
    }
}

void world() {
    glLineWidth(3);
    glPointSize(2);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0, 0, 1); // Blue triangle border
    glBegin(GL_LINE_LOOP);
    glVertex2i(150, 100);
    glVertex2i(300, 300);
    glVertex2i(450, 100);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Boundary Fill Inside Triangle");
    init();
    glutDisplayFunc(world);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}

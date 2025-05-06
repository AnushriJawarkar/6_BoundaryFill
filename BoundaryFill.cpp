#include <GL/glut.h>
#include <iostream>
#include <stack>
using namespace std;

int winWidth = 640, winHeight = 480;
float fillColor[3] = {0.0, 1.0, 0.0};   // Fill color (Green)
float borderColor[3] = {0.0, 0.0, 0.0}; // Border color (Black)
float bgColor[3] = {1.0, 1.0, 1.0};     // Background color (White)

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

// Sample polygon (triangle)
Point poly[] = {{100, 100}, {300, 300}, {100, 300}};
int polySize = 3;

void drawPoly() {
    glColor3fv(borderColor);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < polySize; i++)
        glVertex2i(poly[i].x, poly[i].y);
    glEnd();
    glFlush();
}

void getPixelColor(int x, int y, float* color) {
    glReadPixels(x, winHeight - y, 1, 1, GL_RGB, GL_FLOAT, color);
}

void setPixelColor(int x, int y, float* color) {
    glColor3fv(color);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

bool compareColor(float* c1, float* c2) {
    return (c1[0] == c2[0] && c1[1] == c2[1] && c1[2] == c2[2]);
}

void boundaryFill(int x, int y) {
    stack<Point> pixels;
    pixels.push(Point(x, y));
    
    while (!pixels.empty()) {
        Point p = pixels.top();
        pixels.pop();
        
        float color[3];
        getPixelColor(p.x, p.y, color);
        
        if (!compareColor(color, borderColor) && !compareColor(color, fillColor)) {
            setPixelColor(p.x, p.y, fillColor);
            
            pixels.push(Point(p.x + 1, p.y));
            pixels.push(Point(p.x - 1, p.y));
            pixels.push(Point(p.x, p.y + 1));
            pixels.push(Point(p.x, p.y - 1));
        }
    }
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        boundaryFill(x, y);
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'f' || key == 'F') {
        boundaryFill(winWidth/2, winHeight/2);
    } else if (key == 27) { // ESC key
        exit(0);
    }
}

void menuFunc(int option) {
    switch(option) {
        case 1: // Fill with green
            fillColor[0] = 0.0; fillColor[1] = 1.0; fillColor[2] = 0.0;
            break;
        case 2: // Fill with red
            fillColor[0] = 1.0; fillColor[1] = 0.0; fillColor[2] = 0.0;
            break;
        case 3: // Fill with blue
            fillColor[0] = 0.0; fillColor[1] = 0.0; fillColor[2] = 1.0;
            break;
        case 4: // Clear
            glClear(GL_COLOR_BUFFER_BIT);
            drawPoly();
            break;
        case 5: // Exit
            exit(0);
            break;
    }
}

void display() {
    glClearColor(bgColor[0], bgColor[1], bgColor[2], 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    drawPoly();
    glFlush();
}

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, winWidth, 0, winHeight);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Boundary Fill Algorithm");
    
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboard);
    
    // Create menu
    int menu = glutCreateMenu(menuFunc);
    glutAddMenuEntry("Fill Green", 1);
    glutAddMenuEntry("Fill Red", 2);
    glutAddMenuEntry("Fill Blue", 3);
    glutAddMenuEntry("Clear", 4);
    glutAddMenuEntry("Exit", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    glutMainLoop();
    return 0;
}
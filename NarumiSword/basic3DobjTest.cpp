#include<GL/glut.h>
#include<GL/glu.h>

float angleX = 0.0f;
float angleY = 0.0f;

void init(){
    glClearColor(0.6f, 0.6f, 0.6f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
}

void drawBox(float w, float h, float d){
    glPushMatrix();
    glScalef(w, h, d);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    gluLookAt(0.0,0.0,5.0,
              1.0,0.0,0.0,
              0.0,1.0,0.0);

    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(-0.2f, 0.0f, 0.0f);
    drawBox(0.4f, 0.25f, 0.15f);
    glPopMatrix();

    glutSwapBuffers();
}

void specialKey(int key, int x, int y){
    switch(key) {
        case GLUT_KEY_RIGHT: angleY += 5.0f; break;
        case GLUT_KEY_LEFT: angleY -= 5.0f; break;
        case GLUT_KEY_UP: angleX += 5.0f; break;
        case GLUT_KEY_DOWN: angleX -= 5.0f; break;
    }
    glutPostRedisplay();
}

int main(int argc,char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GL_DEPTH);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(50,100);
    glutCreateWindow("Narumi's Wepon");

    init();

    glutDisplayFunc(display);
    glutSpecialFunc(specialKey);

    glutMainLoop();
    return 0;
}

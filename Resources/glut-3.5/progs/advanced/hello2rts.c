
/* Copyright (c) Mark J. Kilgard, 1997.  */

/* This program is freely distributable without licensing fees and is
   provided without guarantee or warrantee expressed or implied.  This
   program is -not- in the public domain. */

/* This code demonstrates use of the OpenGL Real-time Shadowing (RTS)
   routines.  The program renders two objects with two light sources in a
   scene with several other walls and curved surfaces.  Objects cast shadows
   on the walls and curved surfaces as well as each other.  The shadowing
   objects spin.  See the rts.c and  rtshadow.h source code for more details. */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>

#ifdef GLU_VERSION_1_2

#include "rtshadow.h"

/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI 3.14159265
#endif

enum {
  X, Y, Z
};

enum {
  DL_NONE, DL_TORUS, DL_CUBE, DL_DOUBLE_TORUS, DL_SPHERE
};

enum {
  M_TORUS, M_CUBE, M_DOUBLE_TORUS, M_NORMAL_VIEW, M_LIGHT1_VIEW, M_LIGHT2_VIEW,
  M_START_MOTION, M_ROTATING,
  M_TWO_BIT_STENCIL, M_ALL_STENCIL,
  M_RENDER_SILHOUETTE
};

#define OBJECT_1  0x8000
#define OBJECT_2  0x4000

int lightView = M_NORMAL_VIEW;
int rotate1 = 1, rotate2 = 1;

RTSscene *scene;
RTSlight *light;
RTSlight *light2;
RTSobject *object, *object2;

GLfloat eyePos[3] =
{0.0, 0.0, 10.0};
GLfloat lightPos[4] =
{-3.9, 5.0, 1.0, 1.0};
GLfloat lightPos2[4] =
{4.0, 5.0, 0.0, 1.0};
GLfloat objectPos[3] =
{-1.0, 1.0, 0.0};
GLfloat objectPos2[3] =
{2.0, -2.0, 0.0};

GLfloat pink[4] =
{0.75, 0.5, 0.5, 1.0};
GLfloat greeny[4] =
{0.5, 0.75, 0.5, 1.0};

int shape1 = M_TORUS, shape2 = M_CUBE;
int renderSilhouette1, renderSilhouette2;

GLfloat angle1 = 75.0;
GLfloat angle2 = 75.0;
GLfloat viewAngle = 0.0;
int moving, begin;

void
renderBasicObject(int shape)
{
  switch (shape) {
  case M_TORUS:
    glCallList(DL_TORUS);
    break;
  case M_CUBE:
    glCallList(DL_CUBE);
    break;
  case M_DOUBLE_TORUS:
    glCallList(DL_DOUBLE_TORUS);
    glutSolidTorus(0.2, 0.8, 10, 10);
    break;
  }
}

/* ARGSUSED */
void
renderObject(void *data)
{
  glPushMatrix();
  glTranslatef(objectPos[X], objectPos[Y], objectPos[Z]);
  glRotatef(angle1, 1.0, 1.2, 0.0);
  renderBasicObject(shape1);
  glPopMatrix();
}

/* ARGSUSED */
void
renderObject2(void *data)
{
  glPushMatrix();
  glTranslatef(objectPos2[X], objectPos2[Y], objectPos2[Z]);
  glRotatef(-angle2, 1.3, 0.0, 1.0);
  renderBasicObject(shape2);
  glPopMatrix();
}

/* ARGSUSED1 */
void
renderScene(GLenum castingLight, void *sceneData, RTSscene * scene)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0, 1.0, 0.01, 30.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  switch (lightView) {
  case M_NORMAL_VIEW:
    gluLookAt(eyePos[X], eyePos[Y], eyePos[Z],
      objectPos[X], objectPos[Y], objectPos[Z],
      0.0, 1.0, 0.0);
    break;
  case M_LIGHT1_VIEW:
    gluLookAt(lightPos[X], lightPos[Y], lightPos[Z],
      objectPos[X], objectPos[Y], objectPos[Z],
      0.0, 1.0, 0.0);
    break;
  case M_LIGHT2_VIEW:
    gluLookAt(lightPos2[X], lightPos2[Y], lightPos2[Z],
      objectPos[X], objectPos[Y], objectPos[Z],
      0.0, 1.0, 0.0);
    break;
  }
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);

  glEnable(GL_NORMALIZE);
  glPushMatrix();
  glTranslatef(0.0, 8.0, -5.0);
  glScalef(3.0, 3.0, 3.0);
  glCallList(DL_SPHERE);
  glPopMatrix();
  glDisable(GL_NORMALIZE);

  glPushMatrix();
  glTranslatef(-5.0, 0.0, 0.0);
  glCallList(DL_SPHERE);
  glPopMatrix();

  glBegin(GL_QUADS);
  glNormal3f(0.0, 0.0, 1.0);
  glVertex3f(-7.5, -7.5, -7.0);
  glVertex3f(7.5, -7.5, -7.0);
  glVertex3f(7.5, 7.5, -7.0);
  glVertex3f(-7.5, 7.5, -7.0);

  glNormal3f(-1.0, 0.0, 0.0);
  glVertex3f(5.0, -5.0, -5.0);
  glVertex3f(5.0, -5.0, 5.0);
  glVertex3f(5.0, 5.0, 5.0);
  glVertex3f(5.0, 5.0, -5.0);

  glNormal3f(0.0, 1.0, 0.0);
  glVertex3f(-5.0, -5.0, -5.0);
  glVertex3f(-5.0, -5.0, 5.0);
  glVertex3f(5.0, -5.0, 5.0);
  glVertex3f(5.0, -5.0, -5.0);

  glEnd();

  if (castingLight == GL_NONE) {
    /* Rendering that is not affected by lighting should be drawn only once.
       The time to render it is when no light is casting. */
    glDisable(GL_LIGHTING);

    glColor3fv(pink);
    glPushMatrix();
    glTranslatef(lightPos[X], lightPos[Y], lightPos[Z]);
    glutSolidSphere(0.3, 8, 8);
    glPopMatrix();

    glColor3fv(greeny);
    glPushMatrix();
    glTranslatef(lightPos2[X], lightPos2[Y], lightPos2[Z]);
    glutSolidSphere(0.3, 8, 8);
    glPopMatrix();

    glEnable(GL_LIGHTING);
  }
  renderObject(NULL);
  renderObject2(NULL);
}

void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  rtsRenderScene(scene, RTS_USE_SHADOWS);
  if (renderSilhouette1) {
    glColor3f(0.0, 1.0, 0.0);
    rtsRenderSilhouette(scene, light, object);
    glColor3f(0.0, 0.0, 1.0);
    rtsRenderSilhouette(scene, light2, object);
  }
  if (renderSilhouette2) {
    glColor3f(1.0, 0.0, 0.0);
    rtsRenderSilhouette(scene, light, object2);
    glColor3f(1.0, 1.0, 0.0);
    rtsRenderSilhouette(scene, light2, object2);
  }
  glutSwapBuffers();
}

void
idle(void)
{
  if (rotate1) {
    angle1 += 10;
    rtsUpdateObjectShape(object);
  }
  if (rotate2) {
    angle2 += 10;
    rtsUpdateObjectShape(object2);
  }
  glutPostRedisplay();
}

/* ARGSUSED1 */
void
special(int c, int x, int y)
{
  switch (c) {
  case GLUT_KEY_UP:
    lightPos[Y] += 0.5;
    rtsUpdateLightPos(light, lightPos);
    break;
  case GLUT_KEY_DOWN:
    lightPos[Y] -= 0.5;
    rtsUpdateLightPos(light, lightPos);
    break;
  case GLUT_KEY_RIGHT:
    lightPos[X] += 0.5;
    rtsUpdateLightPos(light, lightPos);
    break;
  case GLUT_KEY_LEFT:
    lightPos[X] -= 0.5;
    rtsUpdateLightPos(light, lightPos);
    break;
  case GLUT_KEY_PAGE_UP:
    lightPos[Z] += 0.5;
    rtsUpdateLightPos(light, lightPos);
    break;
  case GLUT_KEY_PAGE_DOWN:
    lightPos[Z] -= 0.5;
    rtsUpdateLightPos(light, lightPos);
    break;
  case GLUT_KEY_HOME:
    angle1 += 15;
    angle2 += 15;
    rtsUpdateObjectShape(object);
    rtsUpdateObjectShape(object2);
    break;
  case GLUT_KEY_END:
    angle1 -= 15;
    angle2 -= 15;
    rtsUpdateObjectShape(object);
    rtsUpdateObjectShape(object2);
    break;
  case GLUT_KEY_F1:
    lightView = !lightView;
    break;
  }
  glutPostRedisplay();
}

/* ARGSUSED1 */
void
keyboard(unsigned char c, int x, int y)
{
  switch (c) {
  case 27:
    exit(0);
    /* NOTREACHED */
    break;
  case ' ':
    if (rotate1 || rotate2) {
      glutIdleFunc(NULL);
      rotate1 = 0;
      rotate2 = 0;
    } else {
      glutIdleFunc(idle);
      rotate1 = 1;
      rotate2 = 1;
    }
    break;
  }
}

void
updateIdleCallback(void)
{
  if (rotate1 || rotate2) {
    glutIdleFunc(idle);
  } else {
    glutIdleFunc(NULL);
  }
}

void
visible(int vis)
{
  if (vis == GLUT_VISIBLE)
    updateIdleCallback();
  else
    glutIdleFunc(NULL);
}

void
menuHandler(int value)
{
  switch (value) {
  case OBJECT_1 | M_TORUS:
  case OBJECT_1 | M_CUBE:
  case OBJECT_1 | M_DOUBLE_TORUS:
    shape1 = value & ~OBJECT_1;
    rtsUpdateObjectShape(object);
    glutPostRedisplay();
    break;
  case OBJECT_2 | M_TORUS:
  case OBJECT_2 | M_CUBE:
  case OBJECT_2 | M_DOUBLE_TORUS:
    shape2 = value & ~OBJECT_2;
    rtsUpdateObjectShape(object2);
    glutPostRedisplay();
    break;
  case M_NORMAL_VIEW:
  case M_LIGHT1_VIEW:
  case M_LIGHT2_VIEW:
    lightView = value;
    glutPostRedisplay();
    break;
  case M_START_MOTION:
    rotate1 = 1;
    rotate2 = 1;
    glutIdleFunc(idle);
    break;
  case OBJECT_1 | M_ROTATING:
    rotate1 = !rotate1;
    updateIdleCallback();
    break;
  case OBJECT_2 | M_ROTATING:
    rotate2 = !rotate2;
    updateIdleCallback();
    break;
  case M_ALL_STENCIL:
    rtsUpdateUsableStencilBits(scene, ~0);
    glutPostRedisplay();
    break;
  case M_TWO_BIT_STENCIL:
    rtsUpdateUsableStencilBits(scene, 0x3);
    glutPostRedisplay();
    break;
  case OBJECT_1 | M_RENDER_SILHOUETTE:
    renderSilhouette1 = !renderSilhouette1;
    glutPostRedisplay();
    break;
  case OBJECT_2 | M_RENDER_SILHOUETTE:
    renderSilhouette2 = !renderSilhouette2;
    glutPostRedisplay();
    break;
  }
}

void
initMenu(void)
{
  glutCreateMenu(menuHandler);

  glutAddMenuEntry("1 Torus", OBJECT_1 | M_TORUS);
  glutAddMenuEntry("1 Cube", OBJECT_1 | M_CUBE);
  glutAddMenuEntry("1 Double torus", OBJECT_1 | M_DOUBLE_TORUS);

  glutAddMenuEntry("2 Torus", OBJECT_2 | M_TORUS);
  glutAddMenuEntry("2 Cube", OBJECT_2 | M_CUBE);
  glutAddMenuEntry("2 Double torus", OBJECT_2 | M_DOUBLE_TORUS);

  glutAddMenuEntry("Normal view", M_NORMAL_VIEW);
  glutAddMenuEntry("View from light 1", M_LIGHT1_VIEW);
  glutAddMenuEntry("View from light 2", M_LIGHT2_VIEW);

  glutAddMenuEntry("Start motion", M_START_MOTION);
  glutAddMenuEntry("1 Toggle rotating", OBJECT_1 | M_ROTATING);
  glutAddMenuEntry("2 Toggle rotating", OBJECT_2 | M_ROTATING);

  glutAddMenuEntry("Use all stencil", M_ALL_STENCIL);
  glutAddMenuEntry("Use only 2 bits stencil", M_TWO_BIT_STENCIL);

  glutAddMenuEntry("1 Toggle silhouette", OBJECT_1 | M_RENDER_SILHOUETTE);
  glutAddMenuEntry("2 Toggle silhouette", OBJECT_2 | M_RENDER_SILHOUETTE);

  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/* ARGSUSED2 */
void
mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    moving = 1;
    begin = x;
  }
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    moving = 0;
  }
}

/* ARGSUSED1 */
void
motion(int x, int y)
{
  if (moving) {
    viewAngle = viewAngle + (x - begin);
    eyePos[X] = sin(viewAngle * M_PI / 180.0) * 10.0;
    eyePos[Z] = cos(viewAngle * M_PI / 180.0) * 10.0;
    begin = x;
    glutPostRedisplay();
  }
}

int
main(int argc, char **argv)
{
  glutInitDisplayString("stencil>=2 rgb double depth samples");
  glutInit(&argc, argv);

  glutCreateWindow("Hello to Real Time Shadows");
  glutDisplayFunc(display);
  glutSpecialFunc(special);
  glutKeyboardFunc(keyboard);
  glutVisibilityFunc(visible);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);

  /* 0xffffffff means "use as much stencil as is available". */
  scene = rtsCreateScene(eyePos, 0xffffffff, renderScene, NULL);

  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, pink);
  light = rtsCreateLight(GL_LIGHT0, lightPos, 1000.0);
  glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, greeny);
  light2 = rtsCreateLight(GL_LIGHT1, lightPos2, 1000.0);

  object = rtsCreateObject(objectPos, 1.0, renderObject, NULL, 100);
  object2 = rtsCreateObject(objectPos2, 1.0, renderObject2, NULL, 100);

  rtsAddLightToScene(scene, light);
  rtsAddObjectToLight(light, object);
  rtsAddObjectToLight(light, object2);

  rtsAddLightToScene(scene, light2);
  rtsAddObjectToLight(light2, object);
  rtsAddObjectToLight(light2, object2);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  initMenu();

  glNewList(DL_TORUS, GL_COMPILE);
  glutSolidTorus(0.2, 0.8, 10, 10);
  glEndList();

  glNewList(DL_CUBE, GL_COMPILE);
  glutSolidCube(1.0);
  glEndList();

  glNewList(DL_DOUBLE_TORUS, GL_COMPILE);
  glCallList(DL_TORUS);
  glRotatef(90.0, 0.0, 1.0, 0.0);
  glCallList(DL_TORUS);
  glRotatef(-90.0, 0.0, 1.0, 0.0);
  glEndList();

  glNewList(DL_SPHERE, GL_COMPILE);
  glutSolidSphere(1.5, 20, 20);
  glEndList();

  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}

#else
int main(int argc, char** argv)
{
  fprintf(stderr, "This program requires the new tesselator API in GLU 1.2.\n");
  fprintf(stderr, "Your GLU library does not support this new interface, sorry.\n");
  return 0;
}
#endif  /* GLU_VERSION_1_2 */

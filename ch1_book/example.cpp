#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
//#include "HiResTimer.h"


/*************************** Macros and constants ***************************/
enum rendermode_t {
  RENDER_REFLECTED,
  RENDER_SHADOW,
  RENDER_NORMAL
};

const GLfloat DEGREES_PER_SECOND = 60.0f;


/******************************** Prototypes ********************************/
void Initialize();
void MouseHandler(int button, int state, int x, int y);
void KeyboardHandler(unsigned char key, int x, int y);
void MainMenuHandler(int option);
void Animate();
void Reshape(int width, int height);
void Display();

void LoadTexture(char *filename, GLuint &texture);
void DrawScene(rendermode_t mode);
void DrawCube();
void DrawSurface();


/********************************* Globals **********************************/
// index for the texture we'll load for the cube
GLuint g_cubeTexture;

// how much to rotate the cube around an axis
GLfloat g_rotationAngle = 0.0;

//CHiResTimer g_timer;


/****************************************************************************
 main()

 Setup GLUT and OpenGL, drop into the event loop
*****************************************************************************/
int main(int argc, char **argv)
{
  // Setup the basic GLUT stuff
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);

  // Create the window
  glutInitWindowSize(512, 512);
  glutInitWindowPosition(400, 350);
  glutCreateWindow("Chapter 1");

  Initialize();

  // Register the event callback functions
  glutDisplayFunc(Display); 
  glutReshapeFunc(Reshape);
  glutMouseFunc(MouseHandler);
  glutKeyboardFunc(KeyboardHandler);
  glutIdleFunc(Animate);

  // At this point, control is relinquished to the GLUT event handler.
  // Control is returned as events occur, via the callback functions.
  glutMainLoop();   
   
  return 0;
} // end main()


/****************************************************************************
 Initialize()

 One time setup, including creating menus, creating a light, setting the
 shading mode and clear color, and loading textures.
*****************************************************************************/
void Initialize()
{
  // set up the only meny
  int mainMenu;

  mainMenu = glutCreateMenu(MainMenuHandler);

  glutSetMenu(mainMenu);
  glutAddMenuEntry("Exit", 0);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  //g_timer.Init();

  // set the background color
  glClearColor(0.0, 0.0, 0.0, 0.0);

  // set the shading model
  glShadeModel(GL_SMOOTH);

  // set up a single white light
  GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0 };

  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  // load the texture for the cube
  LoadTexture("opengl.bmp", g_cubeTexture);

  // make the modelview matrix active, and initialize it
  glMatrixMode(GL_MODELVIEW);
} // end Initialize()


/****************************************************************************
 MouseHandler()
 
 Handle mouse events. For this simple demo, just exit on a left click.
*****************************************************************************/
void MouseHandler(int button, int state, int x, int y)
{
  switch (button)
  {
  case GLUT_LEFT_BUTTON:
    {
      exit(0);
    } break;
  default:
    break;
  }

  // force a screen redraw
  glutPostRedisplay();
} // end MouseHandler()


/****************************************************************************
 KeyboardHandler()

 Keyboard handler. Again, we'll just exit when q is pressed.
*****************************************************************************/
void KeyboardHandler(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'q':  // exit
    {
      exit(0);
    } break;
  default:
    {
    } break;
  }
  glutPostRedisplay();
} // end KeyboardHandler()


/****************************************************************************
 MainMenuHandler()

 Main menu callback.
*****************************************************************************/
void MainMenuHandler(int option)
{
  switch(option)
  {
  case 0:
    {
      exit(0);
    } break;
  default:
    break;
  }
  glutPostRedisplay();
} // end MainMenuHandler()


/****************************************************************************
 Animate()

 Rotate the cube by 4 degrees and force a redisplay.
*****************************************************************************/
void Animate()
{
  glutPostRedisplay();
} // end Animate()


/****************************************************************************
 Reshape()

 Reset the viewport for window changes
*****************************************************************************/
void Reshape(int width, int height)
{
  if (height == 0)
    return;
  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0, width/height, 1.0, 100.0);

  glMatrixMode(GL_MODELVIEW);
} // end Reshape


/****************************************************************************
 Display()

 Clear and redraw the scene.
*****************************************************************************/
void Display()
{
  g_rotationAngle += (DEGREES_PER_SECOND * g_timer.GetElapsedSeconds());

  static int s_frames = 0;

  if (++s_frames > 100)
  {
    cout << g_timer.GetFPS(100) << endl;
    s_frames = 0;
  }

  // set up the view orientation looking at the origin from slightly above
  // and to the left
  glLoadIdentity();
  gluLookAt(0.0, 1.0, 6.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);

  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glRotatef(-g_rotationAngle/8.0, 0.0, 1.0, 0.0);

  // draw the reflected cube first, with fog enable for a more realistic
  // effect
  glEnable(GL_FOG);
  glFogf(GL_FOG_END, 5.0);
  glFogf(GL_FOG_DENSITY, 0.4);
  DrawScene(RENDER_REFLECTED);
  glDisable(GL_FOG);

  // now draw the scene with the shadow
  DrawScene(RENDER_SHADOW);

  // now draw the real cube
  DrawScene(RENDER_NORMAL);

  // draw everything and swap the display buffer
  glFlush();
  glutSwapBuffers();
} // end Display()


/****************************************************************************
 LoadTexture()

 Loads the texture from the specified file and stores it in iTexture. Note
 that we're using the GLAUX library here, which is generally discouraged,
 but in this case spares us having to write a bitmap loading routine.
*****************************************************************************/
void LoadTexture(char *filename, GLuint &texture)
{
  AUX_RGBImageRec *image[1];
  memset(image, 0, sizeof(void *));

  // if the file can be read, load the texture
  if (image[0] = auxDIBImageLoad(filename))
  {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image[0]->sizeX, image[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
  }

  // free memory if we need to
  if (image[0])
  {
    if (image[0]->data)
    {
      free(image[0]->data);
    }
    free(image[0]);
  }
} // end LoadTexture()


/****************************************************************************
 DrawScene()

 Renders the texture mapped cube at the proper location and rotation. If
 isMirrored is true, the reflected version will be drawn.
*****************************************************************************/
void DrawScene(rendermode_t mode)
{
  glPushMatrix();

  switch (mode)
  {
  case RENDER_REFLECTED:
    {
      // since the user can move the view, and we know that the cube is
      // completely reflected in the surface, we can get cheap reflection
      // by merely flipping the cube and light around the xz plane
      GLfloat lightPos[4] = {3.0, -10.0, 3.0, 1.0};
      glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
      glScalef(1.0, -1.0, 1.0);
      DrawCube();
    } break;
  case RENDER_SHADOW:
    {
      // set up a projective shadow
      GLfloat lightPos[4] = {3.0, 10.0, 3.0, 1.0};
      glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

      // since the scene is static and the plane we're projecting the
      // shadow on is y=0, we can use a simplified matrix
      GLfloat shadowMatrix[16] = {
        lightPos[1],  0.0,  0.0,           0.0,
        -lightPos[0], 0.0,  -lightPos[2],  -1.0,
        0.0,          0.0,  lightPos[1],   0.0,
        0.0,          0.0,  0.0,           lightPos[1]
      };

      // set up the stencil buffer so that we only draw the shadow
      // on the reflecting surface
      glEnable(GL_STENCIL_TEST);
      glStencilFunc(GL_ALWAYS, 3, 0xFFFFFFFF);
      glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
      DrawSurface();
      glStencilFunc(GL_LESS, 2, 0xFFFFFFFF);
      glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
      glEnable(GL_POLYGON_OFFSET_FILL);

      // draw the shadow as half-blended black
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_LIGHTING);
      glColor4f(0.0, 0.0, 0.0, 0.5);

      // project the cube through the shadow matrix
      glMultMatrixf(shadowMatrix);
      DrawCube();

      glDisable(GL_BLEND);
      glEnable(GL_LIGHTING);
      glDisable(GL_POLYGON_OFFSET_FILL);
      glDisable(GL_STENCIL_TEST);
    } break;
  case RENDER_NORMAL:
    {
      GLfloat lightPos[4] = {3.0, 10.0, 3.0, 1.0};
      glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
      DrawCube();
    } break;
  default:
    break;
  };

  glPopMatrix();
} // end DrawCube()


/****************************************************************************
 DrawCube()

 Draws the rotating cube
*****************************************************************************/
void DrawCube()
{
  // set the color of the cube's surface
  GLfloat cubeColor[] = { 1.0f, 1.0f, 1.0f, 1.0 };
  glMaterialfv(GL_FRONT, GL_SPECULAR, cubeColor);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cubeColor);
  glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

  glTranslatef(0.0, 2.0, 0.0);
  glRotatef(g_rotationAngle, 1.0, 0.5, 1.0);

  // set up the cube's texture
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, g_cubeTexture);

  // the cube will just be drawn as six quads for the sake of simplicity
  // for each face, we specify the quad's normal (for lighting), then
  // specify the quad's 4 vertices and associated texture coordinates
  glBegin(GL_QUADS);
  // front
  glNormal3f(0.0, 0.0, 1.0);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, 1.0);
  glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, 1.0);
  glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 1.0);
  glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, 1.0);

  // back
  glNormal3f(0.0, 0.0, -1.0);
  glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, -1.0);
  glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
  glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, 1.0, -1.0);
  glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, -1.0);

  // top
  glNormal3f(0.0, 1.0, 0.0);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, 1.0);
  glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 1.0);
  glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, -1.0);
  glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, -1.0);

  // bottom
  glNormal3f(0.0, -1.0, 0.0);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
  glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, -1.0);
  glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, 1.0);
  glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 1.0);

  // left
  glNormal3f(-1.0, 0.0, 0.0);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
  glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0, 1.0);
  glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, 1.0, 1.0);
  glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, -1.0);

  // right
  glNormal3f(1.0, 0.0, 0.0);
  glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 1.0);
  glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, -1.0);
  glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, -1.0);
  glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 1.0);

  glEnd();

  glDisable(GL_TEXTURE_2D);
} // end DrawCube()


/****************************************************************************
 DrawSurface()

 Draws a simple plane to provide a reflection surface.
*****************************************************************************/
void DrawSurface()
{
  // make sure the light is positioned correctly
  GLfloat lightPos[4] = {3.0, 3.0, 3.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  // set up the surface's color
  GLfloat surfaceColor[] = { 0.2f, 0.4f, 0.2f, 0.5 };
  glMaterialfv(GL_FRONT, GL_SPECULAR, surfaceColor);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, surfaceColor);
  glMaterialf(GL_FRONT, GL_SHININESS, 200.0);

  // set up blending so we can see the reflected cube through the
  // surface, and thus create the illusion of reflection
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBegin(GL_QUADS);
  glNormal3f(0.0, 1.0, 0.0);

  // to have lighting effects show up at all, we need to draw the
  // surface as a lot of quads, not just one
  GLfloat x = -5.0, z = -5.0;

  for (GLint i = 0; i < 10; i++, x += 1)
  {
    for (GLint j = 0; j < 10; j++, z += 1)
    {
      // draw the plane slightly offset so the shadow shows up
      glVertex3f(x, -0.1, z);
      glVertex3f(x + 1.0, -0.1, z);
      glVertex3f(x + 1.0, -0.1, z + 1.0);
      glVertex3f(x, -0.1, z + 1.0);
    }
    z = -5.0;
  }

  glEnd();

  glDisable(GL_BLEND);
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <set>
#include <stdlib.h>
#include <vector>
#include <time.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"
#include "modelloader.h"
#include "vec3f.h"
#include "block.h"

using namespace std;

#define TO_RAD 3.1415926535897932384626433832795/180.0
#define BLOCK_NUMBER 1000
#define WIDTH 10
#define HEIGHT 10
#define DEPTH 20


int current=0;
int box[WIDTH][HEIGHT][DEPTH];
vector<Block> _blocks;
Model_OBJ t_model,l_model;
int _axis;
float _angle[3];
GLfloat mat[16];
GLuint _textureId;
GLfloat blue[] = { 0, 0.8, 0.8 };
GLfloat red[] = { 1,0,0 };

//Returns a random float from 0 to < 1
float randomFloat() {
    srand(time(NULL));
	return (float)rand() / ((float)RAND_MAX + 1);
}
//Deletes everything.  This should be called when exiting the program.
void cleanup() {
//	for(unsigned int i = 0; i < _blocks.size(); i++) {
		delete[] &_blocks;
//	}
}
void updateT(float x,float y,float z) {
    Vec3f pos = Vec3f(x,y,z);
	_blocks[current].addPos(pos);
	glutPostRedisplay();
	//glutTimerFunc(25, updateT, 0);
}
void updateR(float angle,char axis) {
    _blocks[current].addAngle(axis,angle);
	if (_blocks[current].getAngle(axis) > 360) {
		_blocks[current].setAngle(axis,0);
	}
	glutPostRedisplay();			//Tell GLUT that the scene has changed
	//glutTimerFunc(25, updateR, 0);	//Tell GLUT to call update again in 25 milliseconds
}
void handleArrow(int key,int x,int y) {
    if (key == GLUT_KEY_LEFT) updateT(-2, 0, 0);
	if (key == GLUT_KEY_DOWN) updateT(0, -2, 0);
	if (key == GLUT_KEY_RIGHT) updateT(2, 0, 0);
	if (key == GLUT_KEY_UP) updateT(0, 2, 0);
}
void handleKeypress(unsigned char key,int x,int y) {
    if (key == 27) { cleanup(); exit(0); }
    if (key == 'z') updateR(10, 'x');
    if (key == 'x') updateR(10, 'y');
    if (key == 'c') updateR(10, 'z');
    if (key == '.') updateT(0,0,-2);
    if (key == '\/') updateT(0,0,2);
    if (key == 'n') {
        current++;
        Vec3f pos = Vec3f(8 * randomFloat() - 4,
                          5,
                          8 * randomFloat() - 4);
        Block block = Block('t',pos,Vec3f(0,0,0),t_model);
        _blocks.push_back(block);
    }
}
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
											 //Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		0,                            //0 for now
		GL_RGB,                       //Format OpenGL uses for image
		image->width, image->height,  //Width and height
		0,                            //The border of the image
		GL_RGB, //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
						  //as unsigned numbers
		image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}
void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	Vec3f pos = Vec3f(8 * randomFloat() - 4,
                          5,
                          8 * randomFloat() - 4);
    Block block = Block('t',pos,Vec3f(0,0,0),t_model);
    if(block.getType()=='t') block.setModel(t_model);
    if(block.getType()=='l') block.setModel(l_model);
    _blocks.push_back(block);
}
void DrawAxes() {
    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(5,0,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,5);

        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,5,0);
    glEnd();
}
void handleResize(int w, int h) {
    	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
								 //Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0,                  //The camera angle
		(double)w / (double)h, //The width-to-height ratio
		1.0,                   //The near z clipping coordinate
		200.0);                //The far z clipping coordinate
}
void setUptexture(char *file) {
    Image* image = loadBMP(file);
	_textureId = loadTexture(image);
	delete image;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
void setUplighting() {
	//lighting
	GLfloat ambientColor[] = { 0.2f,0.2f,0.2f,1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	//Add positioned light
	GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.5, 0.5, 0.5)
														//**(0.0)Positioned at (4, 0, 8)
	GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	//Add directed light
	GLfloat lightColor1[] = { 0.4f, 0.2f, 0.3f, 1.0f }; //Color (0.5, 0.2, 0.2)
														//**(1.0)Coming from the direction (-1, 0.5, 0.5)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
}
void drawRect(char type,float a,float rx,float ry,float rz) {
	glPushMatrix();
	glRotatef(a, rx, ry, rz);
	glBegin(GL_QUADS);
	if(type=='t') glTexCoord2f(0,0);
	glVertex3f(-1, -1, 1);
	if(type=='t') glTexCoord2f(1,0);
	glVertex3f(1, -1, 1);
	if(type=='t') glTexCoord2f(1,1);
	glVertex3f(1, 1, 1);
    if(type=='t') glTexCoord2f(0,1);
	glVertex3f(-1, 1, 1);
	glEnd();
	glPopMatrix();
}
void drawCube(char type,GLfloat &c,float tx,float ty,float tz) {
	glPushMatrix();
	glTranslatef(tx, ty, tz);
	if(type=='c') glColor3fv(&c);
	if(type=='t') {
        glEnable(GL_TEXTURE_2D);
        glColor3f(1,1,1);
	}
	drawRect(type,0, 0, 0, 0);
	drawRect(type,90, 1, 0, 0);
	drawRect(type,-90, 1, 0, 0);
	drawRect(type,180, 1, 0, 0);
	drawRect(type,90, 0, 1, 0);
	drawRect(type,-90, 0, 1, 0);
	glPopMatrix();
}
void drawBlockTexture(Block b) {
    glPushMatrix();
    glTranslatef(b.getPos('x'),
                 b.getPos('y'),
                 b.getPos('z'));
    glRotatef(b.getAngle('x'),1,0,0);
    glRotatef(b.getAngle('y'),0,1,0);
    glRotatef(b.getAngle('z'),0,0,1);
    drawCube('t',*blue,0, 0, 0);
    drawCube('t',*red,2, 0, 0);
    drawCube('t',*blue,2, 2, 0);
    glPopMatrix();
}
void drawBlockModel(Block b) {
    cout<<"push"<<endl;
    glPushMatrix();
    glTranslatef(b.getPos('x'),
                 b.getPos('y'),
                 b.getPos('z'));
    cout<<"translated"<<endl;
    glRotatef(b.getAngle('x'),1,0,0);
    glRotatef(b.getAngle('y'),0,1,0);
    glRotatef(b.getAngle('z'),0,0,1);
    cout<<"rotated"<<endl;
    if(b.getType()=='t') b.setModel(t_model);
    cout<<"model set"<<endl;
    b.getModel().Draw();
//        if(b.getModel()==t_model) cout<<"correct model"<<endl;
    cout<<"pop"<<endl;
    glPopMatrix();
}
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	setUplighting();
	glTranslatef(0,0,-70);
//	setUptexture("D:/_fang/year 3/cg/demotetris/texture/watertexture.bmp");
//	setUptexture("D:/_fang/year 3/cg/demotetris/texture/crate.bmp");
//	setUptexture("D:/_fang/year 3/cg/demotetris/texture/brick.bmp");
	setUptexture("D:/cg_exercise/CGTetris/demotetris/texture/brick.bmp");

	cout<<"current "<<current<<endl;
	cout<<"num block "<<_blocks.size()<<endl;

	for(int i=0; i<_blocks.size(); i++) {
        cout<<"push"<<endl;
        glPushMatrix();
        glTranslatef(_blocks[i].getPos('x'),
                     _blocks[i].getPos('y'),
                     _blocks[i].getPos('z'));
        cout<<"translated"<<endl;
        glRotatef(_blocks[i].getAngle('x'),1,0,0);
        glRotatef(_blocks[i].getAngle('y'),0,1,0);
        glRotatef(_blocks[i].getAngle('z'),0,0,1);
        cout<<"rotated"<<endl;
//        if(_blocks[i].getType()=='t') _blocks[i].setModel(t_model);
//        if(_blocks[i].getType()=='l') _blocks[i].setModel(l_model);
        cout<<"model set"<<endl;
        _blocks[i].getModel().Draw();
    //        if(b.getModel()==t_model) cout<<"correct model"<<endl;
        cout<<"pop"<<endl;
        glPopMatrix();
	}

//	for(int i=0; i<_blocks.size(); i++) {
//        glPushMatrix();
//        glTranslatef(_blocks[i].getPos('x'),
//                     _blocks[i].getPos('y'),
//                     _blocks[i].getPos('z'));
//        glRotatef(_blocks[i].getAngle('x'),1,0,0);
//        glRotatef(_blocks[i].getAngle('y'),0,1,0);
//        glRotatef(_blocks[i].getAngle('z'),0,0,1);
//        drawCube('c',*blue,0, 0, 0);
//        drawCube('c',*red,2, 0, 0);
//        drawCube('c',*blue,2, 2, 0);
//        glPopMatrix();
//	}

	glutSwapBuffers();
}

//Called every TIMER_MS milliseconds
//void update(int value) {
//	advance(_blocks, _octree, (float)TIMER_MS / 1000.0f, _timeUntilUpdate);
//	glutPostRedisplay();
//	glutTimerFunc(TIMER_MS, update, 0);
//}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow("TETRIS ><");
	initRendering();

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleArrow);
	glutReshapeFunc(handleResize);
	t_model.Load("D:/cg_exercise/CGTetris/demotetris/model/t-tetris-m.obj");
	l_model.Load("D:/cg_exercise/CGTetris/demotetris/model/l-tetris-m.obj");
//	t_model.Load("D:/_fang/year 3/cg/demotetris/model/t-tetris-m.obj");
//	l_model.Load("D:/_fang/year 3/cg/demotetris/model/l-tetris-m.obj");
//	glutTimerFunc(TIMER_MS, update, 0);
	glutMainLoop();
	return 0;
}

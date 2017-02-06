<<<<<<< HEAD
#include <windows.h>

#include <gl/gl.h>
#include <gl/glaux.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//添加这3条语句
#pragma comment (lib, "opengl32.lib")  
#pragma comment (lib, "glu32.lib")  
#pragma comment (lib, "glaux.lib")  

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) //这句是不让控制台窗体出现，如果想要出现，去掉即可。

GLuint texture[5];
GLdouble viewer[3]; 
GLdouble dir[3]; 
float    VectorLength;     //由投影中心计算观察点，用于确定投影方向，大小无关，但正负会影响投影结果
float    alpha;   //视点改变方向，绕y轴旋转的角度

#include "3DObject.h"
#include "Rose.h"

C3DObject* objects[20]; //存放物体对象指针
int m_count;//物体对象

CRose  rose;

float sz = 1.0f;
float sx = 0.0f;
float step = 0.0f;

//定义顶点坐标和颜色
GLfloat vertices[][3] = {{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},{1.0,1.0,-1.0},
						{-1.0,1.0,-1.0}, {-1.0,-1.0,1.0},{1.0,-1.0,1.0}, 
                        {1.0,1.0,1.0}, {-1.0,1.0,1.0}};
GLfloat colors[][3] = {	{0.0,0.0,0.0},{1.0,0.0,0.0},{1.0,1.0,0.0}, {0.0,1.0,0.0},
						{0.0,0.0,1.0},{1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}};

GLfloat light_pos[] = { 0.0, 0.0, 1.0, 0.0 };

GLuint LoadGLTexture( const char *filename);

void polygon(int a, int b, int c , int d)
{ 
	if (a==4 || a==1)
	{ 	
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glEnable(GL_TEXTURE_2D); 
		glColor3f(1,1,1);
		glBegin(GL_POLYGON); 
			glTexCoord2f(0,0); glVertex3fv(vertices[a]); 
			glTexCoord2f(1,0); glVertex3fv(vertices[b]); 
			glTexCoord2f(1,1); glVertex3fv(vertices[c]); 
			glTexCoord2f(0,1); glVertex3fv(vertices[d]); 
		glEnd();
		glDisable(GL_TEXTURE_2D);
		}
	else
	{
		glBegin(GL_POLYGON); 
			glColor3fv(colors[a]); 
			glVertex3fv(vertices[a]);  glVertex3fv(vertices[b]); 
			glVertex3fv(vertices[c]);  glVertex3fv(vertices[d]); 
		glEnd();		
	}
}

void colorcube()
{
   polygon(0,3,2,1);   
   polygon(2,3,7,6);  
   polygon(7,3,0,4);
   polygon(1,2,6,5);   
   polygon(4,5,6,7);   
   polygon(5,4,0,1);
}

void DrawGround()//绘制棋盘格地面
{
	int count = 0;
	int size = 64;
	float inter = 4;
	float y=0;

	glPushMatrix();

	glTranslatef(-size*inter/2,0,-size*inter/2);

	glBegin(GL_QUADS); 
	
	for (int i=0; i<size; i++)
	{
		if (i%2==0) count = 1;
		else count = 0;

		for (int j=0; j<size; j++)
		{
			if(count % 2 == 0) glColor3f(1,1,1);
			else glColor3f(0,0,0);
			
			glVertex3f(j*inter,y,i*inter);
			glVertex3f((j+1)*inter,y,i*inter);
			glVertex3f((j+1)*inter,y,(i+1)*inter);
			glVertex3f(j*inter,y,(i+1)*inter);

			count++;
		}
	}
	glEnd();

	glPopMatrix();
}

void RenderScene( )
{ 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    //更新视点位置
	gluLookAt(viewer[0],viewer[1],viewer[2], dir[0], dir[1], dir[2], 0.0, 1.0, 0.0);

	DrawGround();

	glPushMatrix();
	glTranslatef(3,1,0);
	colorcube(); 
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glPopMatrix();	
	glEnable(GL_LIGHT0);  
	
	glTranslatef(-2,1,0);
	objects[0]->Draw();
//	rose.draw();

	glDisable(GL_LIGHTING);
}

void init()
{
	glClearColor(0.0f,162.0f/255.0f,232.0f/255.0f,0.0f);//背景色

	viewer[0] = 0.0f;
	viewer[1] = 2.0f;
	viewer[2] = 5.0f;
	VectorLength = 1;

	dir[0] = 0.0f;
	dir[1] = 2.0f;
	dir[2] = 5.0f - VectorLength;

	alpha = 0.0f;

	rose.ReadData();

	GLuint t = LoadGLTexture("data\\1.bmp");
	texture[0] = t;

	m_count = 0;
	objects[m_count++] = &rose;

	glEnable(GL_DEPTH_TEST);//启动深度测试
}

void CALLBACK draw()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	RenderScene(); 

	glFinish();//强制之前的绘图函数执行
	auxSwapBuffers();
}

void CALLBACK myReshape(GLsizei w, GLsizei h) 
{ 
	if ( 0 >= w || 0 >= h )         return;

	glViewport(0, 0, w, h); // select the full client area

      // select the projection matrix and clear it
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
 /*  if(cx<=cy) 
		glFrustum(-2.0, 2.0, -2.0 * (GLfloat) cy/ (GLfloat) cx,       
                   2.0* (GLfloat) cy / (GLfloat) cx, 2.0, 20.0); 
	else glFrustum( -2.0 * (GLfloat) cx/ (GLfloat) cy,       
                    2.0* (GLfloat) cx / (GLfloat) cy,-2.0, 2.0, 2.0, 20.0);*/
 	 gluPerspective(60.0, (GLfloat)w/h, 0.5, 120.0);
   
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
} 

void CALLBACK Key_DOWN()
{
	step = 0.10f;
}

void CALLBACK Key_UP()
{
	step = -0.10f;
}

void CALLBACK Key_LEFT()
{
	alpha  += .10f;
}

void CALLBACK Key_RIGHT()
{
	alpha -= .10f;
}

void CALLBACK Mouse(AUX_EVENTREC *event)  
{  
	GLint x,y;  
	x=event->data[AUX_MOUSEX];  
	y=event->data[AUX_MOUSEY];  


	draw();
} 

void CALLBACK Idle()
{
	sx = step * sin(alpha);
	sz = step * cos(alpha);
	
	viewer[0] += sx;
	viewer[2] += sz;
	
	dir[0] = viewer[0] - sin(alpha);
	dir[2] = viewer[2] - cos(alpha);


	draw(); //连续绘制
	step = 0;
}

//主函数
void main()
{
	auxInitDisplayMode(AUX_DOUBLE|AUX_RGBA|AUX_DEPTH);
    auxInitPosition(100,100,500,500);
    auxInitWindow("ProjectFive");

    init();

	auxReshapeFunc (myReshape); 
	auxKeyFunc(AUX_LEFT,Key_LEFT);
	auxKeyFunc(AUX_RIGHT,Key_RIGHT);
	auxKeyFunc(AUX_DOWN,Key_DOWN);
	auxKeyFunc(AUX_UP,Key_UP);

    auxMouseFunc(AUX_LEFTBUTTON,AUX_MOUSEDOWN,Mouse);
	auxIdleFunc(Idle);
    auxMainLoop(draw);	
}

GLuint LoadGLTexture( const char *filename )
{
	AUX_RGBImageRec *pImage; // Create Storage Space For The Texture
	GLuint uiTexture = 0;
	
	pImage = auxDIBImageLoadA(filename);

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if ( pImage != NULL && pImage->data != NULL )					// If Texture Image Exists
	{
		glGenTextures(1, &uiTexture);

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, uiTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, pImage->sizeX, pImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                // 也可以注释掉上面三行用下面三行代码
//		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, pImage->sizeX,   pImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		free(pImage->data);											// Free The Texture Image Memory
		free(pImage);												// Free The Image Structure
	}

	return uiTexture;	
=======
#include <windows.h>

#include <gl/gl.h>
#include <gl/glaux.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//添加这3条语句
#pragma comment (lib, "opengl32.lib")  
#pragma comment (lib, "glu32.lib")  
#pragma comment (lib, "glaux.lib")  

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) //这句是不让控制台窗体出现，如果想要出现，去掉即可。

GLuint texture[5];
GLdouble viewer[3]; 
GLdouble dir[3]; 
float    VectorLength;     //由投影中心计算观察点，用于确定投影方向，大小无关，但正负会影响投影结果
float    alpha;   //视点改变方向，绕y轴旋转的角度

#include "3DObject.h"
#include "Rose.h"

C3DObject* objects[20]; //存放物体对象指针
int m_count;//物体对象

CRose  rose;

float sz = 1.0f;
float sx = 0.0f;
float step = 0.0f;

//定义顶点坐标和颜色
GLfloat vertices[][3] = {{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},{1.0,1.0,-1.0},
						{-1.0,1.0,-1.0}, {-1.0,-1.0,1.0},{1.0,-1.0,1.0}, 
                        {1.0,1.0,1.0}, {-1.0,1.0,1.0}};
GLfloat colors[][3] = {	{0.0,0.0,0.0},{1.0,0.0,0.0},{1.0,1.0,0.0}, {0.0,1.0,0.0},
						{0.0,0.0,1.0},{1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}};

GLfloat light_pos[] = { 0.0, 0.0, 1.0, 0.0 };

GLuint LoadGLTexture( const char *filename);

void polygon(int a, int b, int c , int d)
{ 
	if (a==4 || a==1)
	{ 	
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glEnable(GL_TEXTURE_2D); 
		glColor3f(1,1,1);
		glBegin(GL_POLYGON); 
			glTexCoord2f(0,0); glVertex3fv(vertices[a]); 
			glTexCoord2f(1,0); glVertex3fv(vertices[b]); 
			glTexCoord2f(1,1); glVertex3fv(vertices[c]); 
			glTexCoord2f(0,1); glVertex3fv(vertices[d]); 
		glEnd();
		glDisable(GL_TEXTURE_2D);
		}
	else
	{
		glBegin(GL_POLYGON); 
			glColor3fv(colors[a]); 
			glVertex3fv(vertices[a]);  glVertex3fv(vertices[b]); 
			glVertex3fv(vertices[c]);  glVertex3fv(vertices[d]); 
		glEnd();		
	}
}

void colorcube()
{
   polygon(0,3,2,1);   
   polygon(2,3,7,6);  
   polygon(7,3,0,4);
   polygon(1,2,6,5);   
   polygon(4,5,6,7);   
   polygon(5,4,0,1);
}

void DrawGround()//绘制棋盘格地面
{
	int count = 0;
	int size = 64;
	float inter = 4;
	float y=0;

	glPushMatrix();

	glTranslatef(-size*inter/2,0,-size*inter/2);

	glBegin(GL_QUADS); 
	
	for (int i=0; i<size; i++)
	{
		if (i%2==0) count = 1;
		else count = 0;

		for (int j=0; j<size; j++)
		{
			if(count % 2 == 0) glColor3f(1,1,1);
			else glColor3f(0,0,0);
			
			glVertex3f(j*inter,y,i*inter);
			glVertex3f((j+1)*inter,y,i*inter);
			glVertex3f((j+1)*inter,y,(i+1)*inter);
			glVertex3f(j*inter,y,(i+1)*inter);

			count++;
		}
	}
	glEnd();

	glPopMatrix();
}

void RenderScene( )
{ 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    //更新视点位置
	gluLookAt(viewer[0],viewer[1],viewer[2], dir[0], dir[1], dir[2], 0.0, 1.0, 0.0);

	DrawGround();

	glPushMatrix();
	glTranslatef(3,1,0);
	colorcube(); 
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glPopMatrix();	
	glEnable(GL_LIGHT0);  
	
	glTranslatef(-2,1,0);
	objects[0]->Draw();
//	rose.draw();

	glDisable(GL_LIGHTING);
}

void init()
{
	glClearColor(0.0f,162.0f/255.0f,232.0f/255.0f,0.0f);//背景色

	viewer[0] = 0.0f;
	viewer[1] = 2.0f;
	viewer[2] = 5.0f;
	VectorLength = 1;

	dir[0] = 0.0f;
	dir[1] = 2.0f;
	dir[2] = 5.0f - VectorLength;

	alpha = 0.0f;

	rose.ReadData();

	GLuint t = LoadGLTexture("data\\1.bmp");
	texture[0] = t;

	m_count = 0;
	objects[m_count++] = &rose;

	glEnable(GL_DEPTH_TEST);//启动深度测试
}

void CALLBACK draw()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	RenderScene(); 

	glFinish();//强制之前的绘图函数执行
	auxSwapBuffers();
}

void CALLBACK myReshape(GLsizei w, GLsizei h) 
{ 
	if ( 0 >= w || 0 >= h )         return;

	glViewport(0, 0, w, h); // select the full client area

      // select the projection matrix and clear it
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
 /*  if(cx<=cy) 
		glFrustum(-2.0, 2.0, -2.0 * (GLfloat) cy/ (GLfloat) cx,       
                   2.0* (GLfloat) cy / (GLfloat) cx, 2.0, 20.0); 
	else glFrustum( -2.0 * (GLfloat) cx/ (GLfloat) cy,       
                    2.0* (GLfloat) cx / (GLfloat) cy,-2.0, 2.0, 2.0, 20.0);*/
 	 gluPerspective(60.0, (GLfloat)w/h, 0.5, 120.0);
   
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
} 

void CALLBACK Key_DOWN()
{
	step = 0.10f;
}

void CALLBACK Key_UP()
{
	step = -0.10f;
}

void CALLBACK Key_LEFT()
{
	alpha  += .10f;
}

void CALLBACK Key_RIGHT()
{
	alpha -= .10f;
}

void CALLBACK Mouse(AUX_EVENTREC *event)  
{  
	GLint x,y;  
	x=event->data[AUX_MOUSEX];  
	y=event->data[AUX_MOUSEY];  


	draw();
} 

void CALLBACK Idle()
{
	sx = step * sin(alpha);
	sz = step * cos(alpha);
	
	viewer[0] += sx;
	viewer[2] += sz;
	
	dir[0] = viewer[0] - sin(alpha);
	dir[2] = viewer[2] - cos(alpha);


	draw(); //连续绘制
	step = 0;
}

//主函数
void main()
{
	auxInitDisplayMode(AUX_DOUBLE|AUX_RGBA|AUX_DEPTH);
    auxInitPosition(100,100,500,500);
    auxInitWindow("ProjectFive");

    init();

	auxReshapeFunc (myReshape); 
	auxKeyFunc(AUX_LEFT,Key_LEFT);
	auxKeyFunc(AUX_RIGHT,Key_RIGHT);
	auxKeyFunc(AUX_DOWN,Key_DOWN);
	auxKeyFunc(AUX_UP,Key_UP);

    auxMouseFunc(AUX_LEFTBUTTON,AUX_MOUSEDOWN,Mouse);
	auxIdleFunc(Idle);
    auxMainLoop(draw);	
}

GLuint LoadGLTexture( const char *filename )
{
	AUX_RGBImageRec *pImage; // Create Storage Space For The Texture
	GLuint uiTexture = 0;
	
	pImage = auxDIBImageLoadA(filename);

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if ( pImage != NULL && pImage->data != NULL )					// If Texture Image Exists
	{
		glGenTextures(1, &uiTexture);

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, uiTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, pImage->sizeX, pImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                // 也可以注释掉上面三行用下面三行代码
//		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, pImage->sizeX,   pImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		free(pImage->data);											// Free The Texture Image Memory
		free(pImage);												// Free The Image Structure
	}

	return uiTexture;	
>>>>>>> dd82df2a6f75602c93d298985876b7aa6a44ba65
}
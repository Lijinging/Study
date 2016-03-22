#include <GL/glut.h>
#include <iostream>
#include <windows.h>
using namespace std;

float red = 1.0, blue = 1.0, green = 1.0;
const int Width = 800;
const int Height = 500;
int **point = new int*[2];
int max = 0;
int x_c, y_c, x_b, y_b;
bool isBezier = false;

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, Width, 0.0, Height);
}

void destroy()
{
	for (int i = max - 1; i >= 0; i--)
		delete point[i];
	max = 0;
}

void CatmullRom(float t, int max)
{
	if (max < 4)
		return;
	x_c = point[max - 4][0] * (-0.5*t*t*t + t*t - 0.5*t)
		+ point[max - 3][0] * (1.5*t*t*t - 2.5*t*t + 1.0)
		+ point[max - 2][0] * (-1.5*t*t*t + 2.0*t*t + 0.5*t)
		+ point[max - 1][0] * (0.5*t*t*t - 0.5*t*t);

	y_c = point[max - 4][1] * (-0.5*t*t*t + t*t - 0.5*t)
		+ point[max - 3][1] * (1.5*t*t*t - 2.5*t*t + 1.0)
		+ point[max - 2][1] * (-1.5*t*t*t + 2.0*t*t + 0.5*t)
		+ point[max - 1][1] * (0.5*t*t*t - 0.5*t*t);
}

void Bezier(float t, int max)
{
	if (max < 4)
		return;
	int bx[4], by[4];

	bx[0] = point[max - 3][0];
	bx[1] = point[max - 4][0] * -1.0 / 6.0
		+ point[max - 3][0] * 1.0
		+ point[max - 2][0] * 1.0 / 6.0;
	bx[2] = point[max - 3][0] * 1.0 / 6.0
		+ point[max - 2][0] * 1.0
		+ point[max - 1][0] * -1.0 / 6.0;
	bx[3] = point[max - 2][0];

	by[0] = point[max - 3][1];
	by[1] = point[max - 4][1] * -1.0 / 6.0
		+ point[max - 3][1] * 1.0
		+ point[max - 2][1] * 1.0 / 6.0;
	by[2] = point[max - 3][1] * 1.0 / 6.0
		+ point[max - 2][1] * 1.0
		+ point[max - 1][1] * -1.0 / 6.0;
	by[3] = point[max - 2][1];

	x_b = bx[0] * (1 - t)*(1 - t)*(1 - t)
		+ bx[1] * t*(1 - t)*(1 - t) * 3
		+ bx[2] * t*t*(1 - t) * 3
		+ bx[3] * t*t*t;

	y_b = by[0] * (1 - t)*(1 - t)*(1 - t)
		+ by[1] * t*(1 - t)*(1 - t) * 3
		+ by[2] * t*t*(1 - t) * 3
		+ by[3] * t*t*t;
}

void lineSegment(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(red, green, blue);

	glPointSize(6);

	if (max >= 4)
	{
		
		glLineWidth(3);
		float d = 0.04;
		if (!isBezier)
		{
			glColor3f(1.0, 1.0, 0.0);
			for (float t = 0; t <= 1.0; t += d)
			{
				for (int i = 4; i <= max; i++)
				{
					glBegin(GL_LINES);
					{
						CatmullRom(t, i);
						glVertex2i(x_c, y_c);
						CatmullRom(t + d, i);
						glVertex2i(x_c, y_c);

					}
					glEnd();
				}
			}
		}

		else
		{
			glColor3f(0.0, 0.5, 1.0);
			for (float t = 0; t <= 1.0; t += d)
			{
				for (int i = 4; i <= max; i++)
				{
					glBegin(GL_LINES);
					{
						Bezier(t, i);
						glVertex2i(x_b, y_b);
						Bezier(t + d, i);
						glVertex2i(x_b, y_b);

					}
					glEnd();
				}
			}
		}

	}

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	{
		for (int i = 0; i < max; i++)
		{
			if (point[i][0] != 0 && point[i][1] != 0)
			{
				glVertex2i(point[i][0], point[i][1]);
				//max++;
			}
			else
				break;
		}
	}
	glEnd();
	glColor3f(0.0, 0.8, 0.0);
	glLineWidth(1);
	if (max > 1)
	{

		for (int i = 0; i < max - 1; i++)
		{
			glBegin(GL_LINES);
			{
				glVertex2i(point[i][0], point[i][1]);
				glVertex2i(point[i + 1][0], point[i + 1][1]);
			}
			glEnd();
		}

	}
	glFlush();
}


void processNormalKeys(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		//exit(0);
		destroy();
	}
	else if (key == 32)
	{
		isBezier = !isBezier;
	}
}




void processMouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		cout << "(x,y)=(" << x << "," << y << ")" << endl;
		// 		if (max == 8)
		// 		{
		// 			for (int i = 0; i < max; i++)
		// 			{
		// 				point[i][0] = point[i + 1][0];
		// 				point[i][1] = point[i + 1][1];
		// 			}
		// 			point[max-1][0] = x;
		// 			point[max-1][1] = Height-y;
		// 		}
		// 		else
		{
			point[max] = new int[2];
			point[max][0] = x;
			point[max][1] = Height - y;
			max++;
		}
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Test");

	init();
	glutDisplayFunc(lineSegment);
	glutIdleFunc(lineSegment);

	//处理鼠标事件
	glutMouseFunc(processMouse);

	//处理键盘事件
	glutKeyboardFunc(processNormalKeys);
	cout << "按ALT键切换Catmull-ROM(黄色)和Bezier(蓝色),按ESC清除数据" << endl;

	glutMainLoop();

	return 0;
}
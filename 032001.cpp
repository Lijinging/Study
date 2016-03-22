#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;

float red = 1.0, blue = 1.0, green = 1.0;
const int Width = 800;
const int Height = 500;
int x_c, y_c, x_b, y_b;
bool isBezier = false;
vector<int*> pv;

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, Width, 0.0, Height);
}

void destroy()
{
	pv.clear();
}

void CatmullRom(float t, int max)
{
	if (max < 4)
		return;

	x_c = pv[max - 4][0] * (-0.5*t*t*t + t*t - 0.5*t)
		+ pv[max - 3][0] * (1.5*t*t*t - 2.5*t*t + 1.0)
		+ pv[max - 2][0] * (-1.5*t*t*t + 2.0*t*t + 0.5*t)
		+ pv[max - 1][0] * (0.5*t*t*t - 0.5*t*t);

	y_c = pv[max - 4][1] * (-0.5*t*t*t + t*t - 0.5*t)
		+ pv[max - 3][1] * (1.5*t*t*t - 2.5*t*t + 1.0)
		+ pv[max - 2][1] * (-1.5*t*t*t + 2.0*t*t + 0.5*t)
		+ pv[max - 1][1] * (0.5*t*t*t - 0.5*t*t);
}

void Bezier(float t, int max)
{
	if (max < 4)
		return;
	int bx[4], by[4];

	bx[0] = pv[max - 3][0];
	bx[1] = pv[max - 4][0] * -1.0 / 6.0
		+ pv[max - 3][0] * 1.0
		+ pv[max - 2][0] * 1.0 / 6.0;
	bx[2] = pv[max - 3][0] * 1.0 / 6.0
		+ pv[max - 2][0] * 1.0
		+ pv[max - 1][0] * -1.0 / 6.0;
	bx[3] = pv[max - 2][0];

	by[0] = pv[max - 3][1];
	by[1] = pv[max - 4][1] * -1.0 / 6.0
		+ pv[max - 3][1] * 1.0
		+ pv[max - 2][1] * 1.0 / 6.0;
	by[2] = pv[max - 3][1] * 1.0 / 6.0
		+ pv[max - 2][1] * 1.0
		+ pv[max - 1][1] * -1.0 / 6.0;
	by[3] = pv[max - 2][1];

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

	if (pv.size() >= 4)
	{
		
		glLineWidth(3);
		float d = 0.04;
		if (!isBezier)
		{
			glColor3f(1.0, 1.0, 0.0);
			for (float t = 0; t <= 1.0; t += d)
			{
				for (int i = 4; i <= pv.size(); i++)
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
				for (int i = 4; i <= pv.size(); i++)
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
		for (int i = 0; i < pv.size(); i++)
		{
			if (pv[i][0] != 0 && pv[i][1] != 0)
			{
				glVertex2i(pv[i][0], pv[i][1]);
				//pv.size()++;
			}
			else
				break;
		}
	}
	glEnd();
	glColor3f(0.0, 0.8, 0.0);
	glLineWidth(1);
	if (pv.size() > 1)
	{

		for (int i = 0; i < pv.size() - 1; i++)
		{
			glBegin(GL_LINES);
			{
				glVertex2i(pv[i][0], pv[i][1]);
				glVertex2i(pv[i + 1][0], pv[i + 1][1]);
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
		{
			pv.push_back(new int[2]{x, Height - y});
			cout << "pv.size=" << pv.size() << endl;
			cout << "pv:" << pv[pv.size()-1][0] << "," << pv[pv.size()-1][1] << endl;
		}
	}
}

void changeSize(int w, int h)
{
	gluOrtho2D(0.0, w, 0.0, h);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glShadeModel(GL_SMOOTH);


// 	glEnable(GL_BLEND);
// 	glEnable(GL_LINE_SMOOTH);
// 	glEnable(GL_POLYGON_SMOOTH);
// 	glEnable(GL_POINT_SMOOTH);
// 	glTranslatef(3.0f, 0.0f, 0.0f);



	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Test");

	init();
	glutDisplayFunc(lineSegment);
	glutIdleFunc(lineSegment);

//	glutReshapeFunc(changeSize);

	//处理鼠标事件
	glutMouseFunc(processMouse);

	//处理键盘事件
	glutKeyboardFunc(processNormalKeys);
	cout << "按ALT键切换Catmull-ROM(黄色)和Bezier(蓝色),按ESC清除数据" << endl;

	glutMainLoop();

	return 0;
}
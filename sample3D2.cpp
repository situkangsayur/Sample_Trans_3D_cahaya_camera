#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>
#include <GL/glu.h>
#include <GL/gl.h>

void Display(void);
void CreateEnvironment(void);
void bentukGeomeri(void);
void pencahayaan(void);
void kamera(int, int, int);
void keyboard(unsigned char key, int x, int y);
void keyboardArah(int key, int x, int y);
void mouse(int, int, int, int);
void mainMenu(int);
void menuKecepatan(int);
void visibility(int vis);
void idle(void);
void textArahXY(double, double, double, double, char *);
void GiveUsage(char *);

static GLfloat spin = 0.0;

void spinner();

#define TRUE  1
#define FALSE 0
#define PI 3.141592653589793238462643

#define DRAFT  0
#define MEDIUM 1
#define BEST   2

int jarak, vec = -1;

int start = 0;
int kualitasGambar = MEDIUM;
int putaranKamera = TRUE;
int arahKamera = 1;
double rotasiVertikal = 60;
int loncatanBola = TRUE;
double ballspeed = 2;
//ID Bentuk
#define OVALID      1
#define SPHEREID    2
#define BOXID       3
#define PLANEID     4
#define TEXTID      5

int main(int argc, char **argv) {
	int i, j, depth;
	int mainmenu, speedmenu;

	for (i = 1; i < argc; i++) {
		if (strstr(argv[i], "-t") != NULL)
			GiveUsage(argv[0]);
		if (strstr(argv[i], "-q") != NULL) {
			if (i + 1 >= argc)
				GiveUsage(argv[0]);
			kualitasGambar = atoi(argv[i + 1]);
			if (kualitasGambar < DRAFT)
				kualitasGambar = DRAFT;
			if (kualitasGambar == MEDIUM)
				kualitasGambar = MEDIUM;
			if (kualitasGambar > BEST)
				kualitasGambar = BEST;
			i++;
		}
	}

	/* Set things up and go */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Grafika Komputer (^_^)/");
	glutDisplayFunc(Display);
	glutVisibilityFunc(visibility);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardArah);
	glutMouseFunc(mouse);
	CreateEnvironment();

	/* Set up some menus */
	speedmenu = glutCreateMenu(menuKecepatan);
	glutAddMenuEntry("lambat", 1);
	glutAddMenuEntry("sedang", 2);
	glutAddMenuEntry("cepat", 3);
	mainmenu = glutCreateMenu(mainMenu);
	glutAddMenuEntry("play/pause putaran kamera", 1);
	glutAddMenuEntry("play/pause loncatan bola", 2);
	glutAddSubMenu("Ball speed", speedmenu);
	glutAddMenuEntry("keluar/pres \"Q\"", 100);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return (0);
}

/*
 This is where global settings are made, that is,
 things that will not change in time
 */
void CreateEnvironment(void) {
	glEnable(GL_DEPTH_TEST);

	if (kualitasGambar == DRAFT) {
		glShadeModel(GL_FLAT);
	}

	if (kualitasGambar == MEDIUM) {
		glShadeModel(GL_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glShadeModel(GL_SMOOTH);
	}

	if (kualitasGambar == BEST) {
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glShadeModel(GL_SMOOTH);
		glDisable(GL_DITHER); /* Assume RGBA capabilities */
	}

	glLineWidth(2.0);
	glPointSize(4.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glFrontFace(GL_CW);
	//glDisable( GL_CULL_FACE);
	glClearColor(0.0, 0.0, 0.0, 0.5); /* Background colour */
	glEnable(GL_COLOR_MATERIAL);
}

/*
 * prosedur untuk menampilkan gambar,
 */
void Display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	kamera(FALSE, 0, 0);
	pencahayaan();
	bentukGeomeri();
	glPopMatrix();

	glutSwapBuffers();
}

/*prosedur menggambar bentuk geometri
 */
void bentukGeomeri(void) {
	int i;
	//spinner();
	double radius = 0.5;
	static double theta = 0;
	GLfloat mshin1[] = { 5.0 }; /*] sphere */
	GLfloat mspec1[] = { 0.7, 0.2, 0.5, 1.0 };
	GLfloat mdiff1[] = { 0.6, 0.0, 0.6, 1.0 };
	GLfloat mamb1[] = { 0.1, 0.0, 0.1, 1.0 };
	GLfloat mdiff2[] = { 0.8, 1.0, 0.5, 0.7 }; /* Green plane */
	GLfloat mamb2[] = { 0.0, 0.2, 0.0, 1.0 };
	GLfloat mdiff3[] = { 0.5, 0.5, 0.5, 1.0 }; /* Grey boxes */
	GLfloat mamb3[] = { 0.2, 0.2, 0.2, 1.0 };

	/* Create a green ground plane */
	glLoadName(PLANEID);
	if (kualitasGambar > DRAFT)
		glBegin(GL_POLYGON);
	else
		glBegin(GL_LINE_LOOP);
	glColor3f(0.5, 1.0, 0.7);

	if (kualitasGambar > DRAFT) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mdiff2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mamb2);
	}
	glColor3f(0.5, 0.2, 0.7);
	//kotak besar
	glNormal3f(2.5, 1.0, 0.0);
	glVertex3f(2.5, 0.0, 2.0);
	glVertex3f(2.5, 0.0, -2.0);
	glVertex3f(-2.5, 0.0, -2.0);
	glVertex3f(-2.5, 0.0, 2.0);

	glColor3f(0.5, 1.0, 0.2);
	glVertex3f(2.5, -0.4, 2.0);
	glVertex3f(2.5, -0.4, -2.0);
	glVertex3f(-2.5, -0.4, -2.0);
	glVertex3f(-2.5, -0.4, 2.0);

	glVertex3f(2.5, 0.0, 0.0);
	glVertex3f(2.5, -0.4, 0.0);
	glVertex3f(2.5, -0.4, -2.0);
	glVertex3f(2.5, 0.0, -2.0);

	glVertex3f(-2.5, 0.0, 0.0);
	glVertex3f(-2.5, -0.4, 0.0);
	glVertex3f(-2.5, -0.4, -2.0);
	glVertex3f(-2.5, 0.0, -2.0);

	glVertex3f(-2.5, 0.0, 0.0);
	glVertex3f(-2.5, -0.4, 0.0);
	glVertex3f(2.5, -0.4, 0.0);
	glVertex3f(2.5, 0.0, 0.0);

	glVertex3f(-2.5, 0.0, -2.0);
	glVertex3f(-2.5, -0.4, -2.0);
	glVertex3f(2.5, -0.4, -2.0);
	glVertex3f(2.5, 0.0, -2.0);
	//glVertex3f(2.5, 0.0, 0.0);
	glEnd();
	//teko

	glColor3f(0.5, 1.0, 1);

	/* Place a few grey boxes around the place */

	glLoadName(BOXID);
	glColor3f(0.5, 0.5, 0.5);
	if (kualitasGambar > DRAFT) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mdiff3);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mamb3);
	}
	glColor3f(0.5, 0.78, 0.4);
	glPushMatrix();

	glTranslatef(2.4, -0.1, 1.9);
	if (kualitasGambar > DRAFT)
		glutSolidCube(1);
	else
		glutWireCube(1);

	glTranslatef(-4.8, -0.1, 0.0);
	if (kualitasGambar > DRAFT)
		glutSolidCube(1);
	else
		glutWireCube(1);

	glTranslatef(0.1, 0.1, -3.8);
	if (kualitasGambar > DRAFT)
		glutSolidCube(1);
	else
		glutWireCube(1);

	glTranslatef(4.6, 0.0, 0.0);
	if (kualitasGambar > DRAFT)
		glutSolidCube(1);
	else
		glutWireCube(1);
	glPopMatrix();

	/* Create a magenta bouncing ball */
	glLoadName(SPHEREID);
	glColor3f(0.6, 0.8, 0.6);
	if (kualitasGambar > DRAFT) {
		glMaterialfv(GL_FRONT, GL_SHININESS, mshin1);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mspec1);
	}
	glPushMatrix();
	glTranslatef(0.0, radius + 0.9 * (1 + sin(PI * theta / 180)), 0.0);
	glScalef(radius, radius, radius);
	if (loncatanBola)
		theta += ballspeed;
	if (kualitasGambar > DRAFT)
		glutSolidSphere(1.0, 16, 16);
	else
		glutWireSphere(1.0, 32, 32);

	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 2, 0.0);
	glRotatef(spin, 1.0, 1.0, 1.0);

	glScalef(radius * 3, radius * 3, radius * 3);

	glutWireTeapot(1);
	glPopMatrix();

	/* Create a white half-oval loop at one end */
	glLoadName(OVALID);
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0, 0.2, 1.0);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	for (i = 0; i <= 180; i++)
		glVertex3f(2.45 * cos(i * PI / 180), 1.5 * sin(i * PI / 180), -2.0);
	glEnd();

	/* Write some text */
	glLoadName(TEXTID);
	glColor3f(1.0, 0.2, 1.0);
	//glRotatef(spin,1.0,1.0,1.0);
	textArahXY(-2.0, 0.25, -2.0, 0.002, "Kelompok 5: Hendri, Aam, Zaqi");

	//glRotatef(spin*-1,1.0,1.0,1.0);
	textArahXY(-3.0, 2.0, -2.0, 0.004, "Grafika Komputer IF-8 '07");

	glPushMatrix();
	//	textArahXY(2.0, -0.25, 2.0, 0.002, "");
	glRotatef(0.01, 2.0, -0.25, 2.0);
	glPopMatrix();
	glPushMatrix();

	glColor3f(0.9, 0.6, 0.2);

	//	glScalef(radius, radius, radius);
	glScaled(0.5, 0.5, 0.5);
	glTranslatef(0.0, 2.0, 0.0);

	glPopMatrix();

}

void spinner() {
	spin = spin + 3;
	if (spin > 360.0)
		spin = spin - 360.0;
	//glutPostRedisplay();
}

/*
 Set up the lighing environment
 */
void pencahayaan(void) {
	GLfloat globalambient[] = { 0.3, 0.3, 0.3, 1.0 };

	/* The specifications for 3 light sources */
	GLfloat pos0[] = { 1.0, 1.0, 0.0, 0.0 }; /* w = 0 == infinite distance */
	GLfloat dif0[] = { 0.8, 0.8, 0.8, 1.0 };

	GLfloat pos1[] = { 5.0, -5.0, 0.0, 0.0 }; /* Light from below */
	GLfloat dif1[] = { 0.4, 0.4, 0.4, 1.0 }; /* Fainter */

	if (kualitasGambar > DRAFT) {

		/* Set ambient globally, default ambient for light sources is 0 */
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalambient);

		glLightfv(GL_LIGHT0, GL_POSITION, pos0);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif0);

		glLightfv(GL_LIGHT1, GL_POSITION, pos1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, dif1);

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHTING);
	}
}

/*
 Set up the camera
 Optionally creating a small viewport about
 the mouse click point for object selection
 */
void kamera(int pickmode, int x, int y) {
	static double theta = 0;
	GLint viewport[4];

	/* Camera setup */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (pickmode == TRUE) {
		glGetIntegerv(GL_VIEWPORT, viewport); /* Get the viewport bounds */
		gluPickMatrix(x, viewport[3] - y, 3.0, 3.0, viewport);
	}
	gluPerspective(70.0, /* Field of view */
	1.0, /* aspect ratio  */
	0.1, 1000.0); /* near and far  */

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10 * cos(theta * PI / 180) * sin(rotasiVertikal * PI / 180), 3
			* cos(rotasiVertikal * PI / 180), 4 * sin(theta * PI / 180) * sin(
			rotasiVertikal * PI / 180), 0.0, 0.0, 0.0, /* Focus    */
	0.0, 1.0, 0.0); /* Up       */
	if (putaranKamera)
		theta += (arahKamera * 1);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: /* ESC */
	case 'Q':
	case 'q':
		exit(0);
		break;
	case 's':
	case 'S':
		putaranKamera = !putaranKamera;
		break;
	case 'b':
	case 'B':
		loncatanBola = !loncatanBola;
		break;
	}
}

void keyboardArah(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		arahKamera = -1;
		break;
	case GLUT_KEY_RIGHT:
		arahKamera = 1;
		break;
	case GLUT_KEY_UP:
		rotasiVertikal -= 2;
		break;
	case GLUT_KEY_DOWN:
		rotasiVertikal += 2;
		break;
	}
}

/*
 Handle mouse events
 */
void mouse(int button, int state, int x, int y) {
	int i, maxselect = 100, nhits = 0;
	GLuint selectlist[100];

	if (state == GLUT_DOWN) {
		glSelectBuffer(maxselect, selectlist);
		glRenderMode(GL_SELECT);
		glInitNames();
		glPushName(-1);

		glPushMatrix();
		kamera(TRUE, x, y);
		bentukGeomeri();
		glPopMatrix();
		nhits = glRenderMode(GL_RENDER);

		if (button == GLUT_LEFT_BUTTON) {

		} else if (button == GLUT_MIDDLE_BUTTON) {

		}

		if (nhits == -1)
			fprintf(stderr, "error gagal buffering\n");

		if (nhits > 0) {
			fprintf(stderr, "\tPicked %d objects: ", nhits);
			for (i = 0; i < nhits; i++)
				fprintf(stderr, "%d ", selectlist[4 * i + 3]);
			fprintf(stderr, "\n");
		}

	}
}

/*
 Handle the main menu
 */
void mainMenu(int whichone) {
	switch (whichone) {
	case 1:
		putaranKamera = !putaranKamera;
		break;
	case 2:
		loncatanBola = !loncatanBola;
		break;
	case 100:
		exit(0);
		break;
	}
}

/*
 Handle the ball speed sub menu
 */
void menuKecepatan(int whichone) {
	switch (whichone) {
	case 1:
		ballspeed = 2;
		break;
	case 2:
		ballspeed = 5;
		break;
	case 3:
		ballspeed = 10;
		break;
	}
}

/*
 visbility
 */
void visibility(int visible) {

	if (visible == GLUT_VISIBLE) {
		//spinner();
		glutIdleFunc(idle);
	}

	else
		glutIdleFunc(NULL);
}

/*
 What to do on an idle event
 */
void idle(void) {
	spinner();
	glutPostRedisplay();
}

/*
 draw text, x,y,z, dengan tranlasi sebesar scale, dan tex = s
 */
void textArahXY(double x, double y, double z, double scale, char *s) {
	int i;

	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(scale, scale, scale);
	for (i = 0; i < strlen(s); i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, s[i]);
	glPopMatrix();
}

/*
 informasi di terminal
 */
void GiveUsage(char *cmd) {
	fprintf(stderr, "Usage:    %s [-h] [-q n]\n", cmd);
	fprintf(stderr, "          -h   text yg ditamplkan\n");
	fprintf(stderr, "          -q n quality, 0,1,2\n");
	fprintf(stderr, "tombol menu:\n");
	fprintf(stderr, "           q - keluar\n");
	fprintf(stderr, "           s - play/pause putaran kamera\n");
	fprintf(stderr, "           b - play/pause lompatan bola\n");
	fprintf(stderr, "  left arrow - merubah arah kamera ke kiri\n");
	fprintf(stderr, " right arrow - merubah arah kamera ke kanan\n");
	fprintf(stderr, "  down arrow - rotasi kamera ke bawah\n");
	fprintf(stderr, "    up arrow - rotasi kamera ke atas\n");
	exit(-1);
}

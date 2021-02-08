//Hecho por : Isabel Juarez MArtinez

#include <stdio.h>
#include <string.h>
#include <GL/glut.h>

#include "texture.h"
#include "figuras.h"
#include "Camera.h"

//Solo para Visual Studio 2015 o mayor
#if (_MSC_VER >= 1900)
#   pragma comment( lib, "legacy_stdio_definitions.lib" )
#endif

//variables para cámara
CCamera objCamera;	//Create objet Camera
GLfloat g_lookupdown = 0.0f;    // Look Position In The Z-Axis (NEW) 

//variables para texto dibujado
char s[30];
long int font = (long int)GLUT_BITMAP_HELVETICA_18;

CTexture text1; //skybox
CTexture text2; //amarillo
CTexture text3; //cafe
CTexture text4; //pasto

CFiguras fig1; //skybox

//Animacion 

float posW = 10.0;

//Banderas de animación para el teclado


bool banderaAnimacion = false;
bool avanzaW = true;



//función para inicializar estados de gl
void inicializar(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Negro de fondo	

	glEnable(GL_TEXTURE_2D);

	glShadeModel(GL_SMOOTH);

	glEnable(GL_COLOR_MATERIAL);

	glClearDepth(1.0f);									// Configuramos Depth Buffer
	glEnable(GL_DEPTH_TEST);							// Habilitamos Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a realizar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	//Carga de archivos para las texturas
	text1.LoadBMP("imagenes/estre.bmp");
	text1.BuildGLTexture();
	text1.ReleaseImage();

	text2.LoadTGA("imagenes/pasto256.tga");
	text2.BuildGLTexture();
	text2.ReleaseImage();

	text3.LoadBMP("imagenes/cafe.bmp");
	text3.BuildGLTexture();
	text3.ReleaseImage();

	text4.LoadBMP("imagenes/textura.bmp");
	text4.BuildGLTexture();
	text4.ReleaseImage();


	objCamera.Position_Camera(10, 2.5f, 13, 10, 2.5f, 10, 0, 1.0, 0);

}


//Funcion que contiene la letra W con sus respectivas texturas
void wLetra() {

	glBindTexture(GL_TEXTURE_2D, text2.GLindex);
	glBegin(GL_POLYGON);//1
	glTexCoord2d(0.0, 2.0); glVertex3f(3.0, 9.0, -0.5); //1
	glTexCoord2d(3.0, 2.0); glVertex3f(5.0, 9.0, -0.5); //2
	glTexCoord2d(3.0, 0.0); glVertex3f(5.0, 4.0, -0.5); //3
	glTexCoord2d(0.0, 0.0); glVertex3f(3.0, 2.0, -0.5); //4
	glEnd();

	glBindTexture(GL_TEXTURE_2D, text3.GLindex);
	glBegin(GL_QUADS);//2
	glTexCoord2d(0.0, 2.0); glVertex3f(10.0, 9.0, -0.5); //1
	glTexCoord2d(3.0, 2.0); glVertex3f(12.0, 9.0, -0.5); //2
	glTexCoord2d(3.0, 0.0); glVertex3f(5.0, 1.0, -0.5); //3
	glTexCoord2d(0.0, 0.0); glVertex3f(3.0, 1.0, -0.5); //4
	glEnd();


	glBindTexture(GL_TEXTURE_2D, text4.GLindex);
	glBegin(GL_QUADS);//3
	glVertex3f(9.0, 5.0, -0.5); //1
	glVertex3f(11.0, 7.0, -0.5); //2
	glVertex3f(11.0, 1.0, -0.5); //3
	glVertex3f(9.0, 1.0, -0.5); //4
	glEnd();

	glBegin(GL_QUADS);//3
	glVertex3f(17.0, 9.0, -0.5); //1
	glVertex3f(19.0, 9.0, -0.5); //2
	glVertex3f(11.0, 1.0, -0.5); //3
	glVertex3f(9.0, 1.0, -0.5); //4
	glEnd();


}


void dibujar(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpiamos pantalla y Depth Buffer

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();

	glRotatef(g_lookupdown, 1.0f, 0, 0);

	gluLookAt(objCamera.mPos.x, objCamera.mPos.y, objCamera.mPos.z,
		objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,
		objCamera.mUp.x, objCamera.mUp.y, objCamera.mUp.z);


	glPushMatrix(); //Creamos Entorno de Estrellas
	glTranslatef(0, 20, 0);
	fig1.skybox(50.0, 100.0, 50.0, text1.GLindex);
	glPopMatrix();

	//Para que el comando glColor funcione con iluminacion
	glEnable(GL_COLOR_MATERIAL);

	glPushMatrix();
	glTranslatef(4.0, posW, 0.0);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(0.0, 1.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	wLetra();
	glPopMatrix();
	glPopMatrix();


	glFlush();
	glutSwapBuffers();
}

void animacion()
{
	//usando bandera para iniciar o detener animación, límite de distancia y regresa al origen en reversa
	if (banderaAnimacion == true)
	{
		if (avanzaW == true)
		{
			posW -= 0.01;
			if (posW < 0.0)
				avanzaW = false;
		}
		else
		{
			posW += 0.01;
			if (posW >= 10)
			{
				avanzaW = true;
				banderaAnimacion = false;
			}
		}

	}
	glutPostRedisplay();
}

void remodelar(int width, int height)   // Creamos funcion Reshape
{
	if (height == 0)										// Prevenir division entre cero
	{
		height = 1;
	}

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);						// Seleccionamos Projection Matrix
	glLoadIdentity();

	// Tipo de Vista

	glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 170.0);

	glutPostRedisplay();

}

void teclado(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':   //Movimientos de camara
	case 'W':
		objCamera.Move_Camera(CAMERASPEED + 0.2);
		break;

	case 's':
	case 'S':
		objCamera.Move_Camera(-(CAMERASPEED + 0.2));
		break;

	case 'a':
	case 'A':
		objCamera.Strafe_Camera(-(CAMERASPEED + 0.4));
		break;

	case 'd':
	case 'D':
		objCamera.Strafe_Camera(CAMERASPEED + 0.4);
		break;


	case 32: //Inicia la animacion
		banderaAnimacion = !banderaAnimacion;
		break;


	case 27:        // Cuando Esc es presionado...
		exit(0);   // Salimos del programa
		break;
	default:        // Cualquier otra
		break;
	}

	glutPostRedisplay();
}
//giro de camara
void teclasFlechas(int tecla, int x, int y)  // Funcion para manejo de teclas especiales (arrow keys)
{
	switch (tecla) {
	case GLUT_KEY_PAGE_UP:
		objCamera.UpDown_Camera(CAMERASPEED);
		break;

	case GLUT_KEY_PAGE_DOWN:
		objCamera.UpDown_Camera(-CAMERASPEED);
		break;

	case GLUT_KEY_UP:     // Presionamos tecla ARRIBA...
		g_lookupdown -= 1.0f;
		break;

	case GLUT_KEY_DOWN:               // Presionamos tecla ABAJO...
		g_lookupdown += 1.0f;
		break;

	case GLUT_KEY_LEFT:
		objCamera.Rotate_View(-CAMERASPEED);
		break;

	case GLUT_KEY_RIGHT:
		objCamera.Rotate_View(CAMERASPEED);
		break;

	default:
		break;
	}
	glutPostRedisplay();
}


int main(int argc, char** argv)   // Main Function
{
	//inicializa GLUT con el sistema de ventanas pasando los argumentos del main
	glutInit(&argc, argv);

	//Buffer simple para dibujar
	//Colores RGB y alpha
	//Buffer de profundidad
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	//Define ventana de 800 pixeles de ancho por 600 de alto
	glutInitWindowSize(800, 600);

	//Posiciona la ventana de izquierda a derecha 200 pixeles y de arriba a abajo 100
	glutInitWindowPosition(200, 100);

	//Crea y abre la ventana y recibe el nombre que va en su barra de título
	glutCreateWindow("Isabel Juarez Martinez");

	//Llamada a función propia para inicializar estados de opengl
	inicializar();

	//Llamada a la función a ser dibujada y redibujada
	glutDisplayFunc(dibujar);

	glutReshapeFunc(remodelar);

	//Lamada a función que maneja eventos del teclado
	glutKeyboardFunc(teclado);

	glutSpecialFunc(teclasFlechas);

	glutIdleFunc(animacion);

	//Llamada a función que cede el control a GLUT y procesa eventos de ventana, ratón
	glutMainLoop();

	//termina la ejecución devolviendo cero
	return 0;
}

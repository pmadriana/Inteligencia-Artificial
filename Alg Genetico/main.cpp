#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <utility>
//#include <quadTree.h>
#define KEY_ESC 27

using namespace std;
float var_x = 0.0;

struct Point{
    int x,y;
    Point(int a, int b):x(a),y(b){}
};


vector<Point> points;
int num_points;


/*void generate_points(){
    points.assign(num_points,Point(0,0));
    for(size_t i=0;i<num_points;++i){
        points[i].x=rand()%100;
        points[i].y=rand()%100;
    }
}*/

float euclidean(Point u, Point v){
    return sqrt(pow(u.x-v.x,2)+pow(u.y-v.y,2));
}

vector<vector<int>> poblacion;
vector<int> ciudades;
vector<vector<int>> pesos;
vector<int> aptiud_pob;
int tt;



void Crear_Ciudades(){
    for(int j=0; j< num_points; ++j){
    ciudades.push_back(j);
    }
    vector<int> aux;
    for(int i=0; i<k; ++i){
            for(int j=0; j<k; ++j){
                aux.push_back(0);
            }
            pesos.push_back(aux);
            aux.clear();
        }
    for(int i=0; i<k; ++i){
            for(int j=0; j<k; ++j){
                if(j!=i && pesos[i][j]==0 && pesos[j][i]==0){
                int r=(10 + rand() %70);
                pesos[i][j]=r;
                pesos[j][i]=r;
                points[i].x +=r; ///dibujo
                points[i].y +=r; ///dibujo
                }
            }
        }

        for(int i=0; i<k; ++i){
            for(int j=0; j<k; ++j){
                cout<<pesos[i][j]<<", ";
            }
        cout<<endl;
        }
}




void actualizar_peso(){
        aptiud_pob.clear();
        int temp;
    for(int i=0; i<poblacion.size(); ++i){
        temp=0;
        for(int j=1; j<pesos.size(); ++j){
                temp+=pesos[poblacion[i][j]- 65][poblacion[i][j-1]- 65];
        }
        aptiud_pob.push_back(temp);
    }

}


void Crear_Pobl(int k, vector<int> ciudades){
    srand(time(NULL));
    int num;
    vector<int> vali;
    vector<int> aux;
    for(int i=0; i<k; ++i){
    vali=ciudades;
    while (vali.size() !=0){
    num= rand() % (vali.size());
    aux.push_back(vali[num]);
    vali.erase (vali.begin()+num);
    }
    poblacion.push_back(aux);
    aux.clear();
    }
    actualizar_peso();
}

void Imprimir_Ciudades(){
    for(int i=0; i<ciudades.size(); ++i){
    cout <<ciudades[i]<< " ,";
    }
    cout<< endl;
}
void Imprimir_Poblacion(){
    for(int i=0; i<poblacion.size(); ++i){
    cout<< i + 1 << endl;
    for(int j=0; j<ciudades.size(); ++j){
    cout <<poblacion[i][j]<< " ,";
    }
    cout<< "->"<<aptiud_pob[i]<< endl;
    }
    int mini=aptiud_pob[0];
    int mi=0;
    for(int i=1; i<aptiud_pob.size(); ++i){
        if(aptiud_pob[i]<mini){
                mini=aptiud_pob[i];
                mi=i;
        }
    }
    cout<<endl<<endl<<"El mejor de esta poblacion es:"<<endl;
    for(int j=0; j<ciudades.size(); ++j){
    glBegin(GL_LINE_LOOP);
    glColor3d(0.0f,0.0f,0.0f);
    glVertex2d(points[j], points[mi]);
    glEnd();
    cout <<poblacion[mi][j]<< " ,";
    }
    tt=mi;
    cout<<" -> "<< mini<<endl<<endl;

}

vector<char> Mutacion(vector<char>P1){

    int ini = rand() % (ciudades.size());
    int fin;
    do{
            fin = rand() % (ciudades.size());

    }
    while(ini== fin);
    char temp=P1[ini];
    P1[ini]=P1[fin];
    P1[fin]=temp;

    return P1;
}


vector<char> Crossover(vector<int>P1, vector<int>P2){
    int ini = rand() % (ciudades.size());
    int fin;
    do{
        fin = rand() % (ciudades.size());}
    while(ini==fin);
    int m_ini = min(ini,fin);
    int m_fin = max(ini,fin);
    int pos_i = 0;
    vector<char> Child = P1;
    string invalid ="";
    for(int i= m_ini; m_ini <= m_fin; ++i){
    invalid+= P1[i];
    }
    for (unsigned int i = 0; i < m_ini; i++) {
    while (string::npos != invalid.find(P2[pos_i])) {
    pos_i++;
    }
    Child[i] = P2[pos_i];
    pos_i++;
    }
    for (unsigned int i = m_fin+1; i < ciudades.size(); i++) {
    while (string::npos != invalid.find(P2[pos_i])) {
    pos_i++;
    }
    Child[i] = P2[pos_i];
    pos_i++;
    }

     for(int j=0; j<P1.size(); ++j){
    cout <<Child[j]<< " ,";}

    return Child;
}


void actualizar_poblac(){
     int k=poblacion.size();
     //poblacion[(tt)%poblacion.size()]= Mutacion(poblacion[(tt)%poblacion.size()]);
     poblacion[(tt+2)%poblacion.size()]=
    Mutacion(poblacion[(tt+2)%poblacion.size()]);
     poblacion[(tt+3)%poblacion.size()]=
    Mutacion(poblacion[(tt+3)%poblacion.size()]);
     //poblacion.push_back(Crossover(poblacion[tt],poblacion[(tt+1)%poblacion.size()]));
     actualizar_peso();

}




void drawPoints(){
    glPushMatrix();
    glEnable(GL_POINT_SMOOTH);
    glPointSize(40);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
        glColor3d(1.0f,0.0f,0.0f);
        for(size_t i=0;i<num_points;++i)
            glVertex2d(points[i].x,points[i].y);
    glEnd();
    glPopMatrix();
}




void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}
void displayFunction()
{

    glPushMatrix();
    glLineWidth(2.5f);
    glColor3f(0.1f,0.7f,0.1f);


    glColor3f(0.1f,0.1f,0.7f);

    glPopMatrix();
}
//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glOrtho(-20.0f,  120.0f,-20.0f, 120.0f, -1.0f, 1.0f);

        drawPoints();
	    glPushMatrix();
        glBegin(GL_LINES);
            glLineWidth(1.0f);
            glColor3f(0.5f,0.1f,0.8f);
            glVertex2i(-15,0);
            glVertex2i(-15,115);
            glColor3f(0.2f,0.7f,0.2f);
            glVertex2i(-15,0);
            glVertex2i(115,0);
        glEnd();
	    glTranslatef(var_x, 0.0f,0.0f);
        displayFunction();
        glPopMatrix();


	glutSwapBuffers();
	var_x-=1.0;
//	glFlush();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


}

//
//el programa principal
//
int main(int argc, char** argv) {
    srand(time(NULL));
	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("Algoritmo Genetico"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);
    int s;
    cout<<"Ingrese el num de nodos: "<<endl;
    cin>>num_points;
    cout<<"Ingrese poblacion: "<<endl;
    cin>>s;
    Crear_Ciudades(num_points);
    int cont=2;
    cout<<"Poblacion 1:"<<endl,
    Crear_Pobl(s+2,ciudades);
    Imprimir_Poblacion();

    for(int i=cont;i<s+1;++i){
        cout<<"Poblacion "<<i<<" :"<<endl,
        actualizar_poblac();
        Imprimir_Poblacion();
	generate_points();

	glutMainLoop();
	return 0;
}

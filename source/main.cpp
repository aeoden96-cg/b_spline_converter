// Kompajliranje:
// g++ -o SimpleAnim SimpleAnim.cpp util.cpp -lGLEW -lGL -lGLU -lglut -lpthread

#include <iostream>
#include <numeric>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/Shader.h"
#include "main.hpp"
#include "Renderer.hpp"

#ifdef _WIN32
    #include <windows.h>
#endif

#ifndef _WIN32
    #define LINUX_UBUNTU_SEGFAULT
#endif

//#ifdef LINUX_UBUNTU_SEGFAULT
//    #include <pthread.h>
//#endif



[[maybe_unused]] GLuint window;
GLuint sub_width = 500, sub_height = 500;
int WindowHeight;
int WindowWidth;
const double Xmin = 0.0, Xmax = 3.0;
const double Ymin = 0.0, Ymax = 3.0;

glm::mat4 projection;

Shader shader;

bool track = false;

int stupanj= 3;
std::vector<float> T{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 4.0f, 5.0f, 5.0f, 5.0f, 5.0f};

//float t = 2;

std::vector<glm::vec3> kp{
        glm::vec3(-0.5f,-0.5f,0),
        glm::vec3(-0.3f,0.1f,0),
        glm::vec3(0.0f,0.1f,0),
        glm::vec3(0.5f,-0.5f,0),glm::vec3(0.7f,-0.5f,0)
};

std::vector<std::vector<glm::vec3>> P;

std::vector<float> final;

glm::mat4 addView(glm::mat4 model){
    glm::mat4 view = glm::mat4();
    return view * model;
}




/**
 * Trivial.Just apply view matrix.
 * @return
 */
glm::mat4 createMVPBody(){
    return addView(glm::mat4(1.0f));
}

bool approximatelyEqual(float a, float b, float epsilon)
{
    return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}
int ind_track;
int fixed_track;

void glutPassiveMotionFunc(int x, int y ) {

    float xPos = ((float)x)/((float)(WindowWidth-1));
    float yPos = ((float)y)/((float)(WindowHeight-1));
    xPos =2*xPos-1;
    yPos=-2*yPos+1;



    ind_track = -1;
    for(int i = 0; i < kp.size();i++) {
        if (
                approximatelyEqual(kp[i].x, xPos, 10E-2) &&
                approximatelyEqual(kp[i].y, yPos, 10E-2)){
            ind_track = i;
        }
    }

    if (track){
        kp[fixed_track].x = xPos;
        kp[fixed_track].y = yPos;

    }


   // std::cout << ind_track << "\n";


    glutPostRedisplay();

}




void myKeyboardFunc( unsigned char key, int x, int y ){

    if (key == 'm'){
        track = !track;
        fixed_track=ind_track;
    }
}

void mySpecialKeyFunc( int key, int x, int y )
{

}

int main(int argc, char ** argv)
{

	// Sljedeci blok sluzi kao bugfix koji je opisan gore
	#ifdef LINUX_UBUNTU_SEGFAULT
        //int i=pthread_getconcurrency();
	#endif

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_DEBUG);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize((int)sub_width,(int)sub_height);
	glutInitWindowPosition(100,100);
	glutInit(&argc, argv);

	window = glutCreateWindow("SimpleAnim" );
	glutReshapeFunc(resizeWindow);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc( myKeyboardFunc );			// Handles "normal" ascii symbols
    //glutMouseFunc(myMouseFunc);
    glutPassiveMotionFunc(glutPassiveMotionFunc);
	glutSpecialFunc( mySpecialKeyFunc );		// Handles "special" keyboard keys

	glewExperimental = GL_TRUE;
	glewInit();

	init_data();

	// Omogući uporabu Z-spremnika
	glEnable(GL_DEPTH_TEST);




	glutMainLoop();
    return 0;
}



glm::vec3 algo(float t){
    for (auto p: P) p.clear();
    P.clear();
    int j=0;
    for (j = 0 ; t > T[j]; j++ );
    j--;

    //std::cout << "t=" << t << " j=" << j <<  " | " << T[j] << "<"  << t <<  "<" << T[j+1] <<"\n";

    P.push_back(kp);   // P[0] je skup orginalnih tocaka Pi^[0]

    for (int l = 1 ; l <= stupanj ; l++){
        std::vector<glm::vec3> P_l;

        for (int i = l ; i < j ;i++ ){
            glm::vec3 P_i_l =
                    (t - T[i+1])    /(T[i+1+j-l] - T[i+1]) * P[l-1][i]  +
                    (T[i+1+j-l] - t)/(T[i+1+j-l] - T[i+1]) * P[l-1][i-1];

            P_l.push_back(P_i_l);
        }

        P.push_back(P_l);


    }



    //std::cout << P[3].size() << "\n";
    return P[3][j];
}

bool init_data()
{
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

	// Antialijasiranje poligona, ovisno o implementaciji
    //	  glEnable(GL_POLYGON_SMOOTH);
    //	  glEnable(GL_BLEND);
    //    glHint(GL_POLYGON_SMOOTH, GL_DONT_CARE);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);





//    std::cout << P[0][3].x << " " << P[0][3].y << " " << P[0][3].z << "\n";
//
//    std::cout << P[3][0].x << " " << P[3][0].y << " " << P[3][0].z << "\n";



//    std::cout << P[1].size() << "\n";
//    for(auto p : P[1]){
//        std::cout << p.x << " " << p.y << " " << p.z << "\n";
//
//    }







	std::cout << "Going to load programs... " << std::endl << std::flush;


    shader.load_shaders({"BackgroundVertexShader.vert","BackgroundFragmentShader.frag","","",""});


	return true;
}







void myDisplay() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    std::vector<glm::mat4> MVPs;
    MVPs.emplace_back(1);
    std::vector<float> out;

    auto p = algo(2);
    //std::cout << p.x << " " << p.y << " " << p.z << "\n";

    final.push_back(p.x);
    final.push_back(p.y);
    final.push_back(p.z);


    shader.setVec3("col", glm::vec3{(1+1)*0.2f,(1+1)*0.2f,0.5f});
    Renderer::render(shader, std::vector<int>({3}) , final, MVPs);



    for (int i = 0 ; i < 3 ; i++){
        out.clear();
        for (auto p: P[i]){
            out.push_back(p.x);
            out.push_back(p.y);
            out.push_back(p.z);
        }
        shader.setVec3("col", glm::vec3{(i+1)*0.3f,(i+1)*0.2f,0.5f});
        Renderer::render(shader, std::vector<int>({3}) , out, MVPs);
    }






//    MVPs.clear();


    glutSwapBuffers();
    glutPostRedisplay();
}

void resizeWindow(int w, int h)
{
	double scale, center;
	double windowXmin, windowXmax, windowYmin, windowYmax;

	glViewport( 0, 0, w, h );	// View port uses whole window

	w = (w==0) ? 1 : w;
	h = (h==0) ? 1 : h;
    WindowHeight = (h>1) ? h : 2;
    WindowWidth = (w>1) ? w : 2;
	if ( (Xmax-Xmin)/w < (Ymax-Ymin)/h ) {
		scale = ((Ymax-Ymin)/h)/((Xmax-Xmin)/w);
		center = (Xmax+Xmin)/2;
		windowXmin = center - (center-Xmin)*scale;
		windowXmax = center + (Xmax-center)*scale;
		windowYmin = Ymin;
		windowYmax = Ymax;
	}
	else {
		scale = ((Xmax-Xmin)/w)/((Ymax-Ymin)/h);
		center = (Ymax+Ymin)/2;
		windowYmin = center - (center-Ymin)*scale;
		windowYmax = center + (Ymax-center)*scale;
		windowXmin = Xmin;
		windowXmax = Xmax;
	}

    projection = glm::ortho(
            (float) windowXmin,
            (float) windowXmax,
            (float) windowYmin,
            (float) windowYmax,
            -1.0f,
            1.0f);

}




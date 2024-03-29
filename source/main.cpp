// Kompajliranje:
// g++ -o SimpleAnim SimpleAnim.cpp util.cpp -lGLEW -lGL -lGLU -lglut -lpthread

#include <iostream>
#include <numeric>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "util/Shader.h"
#include "main.hpp"
#include "Renderer.hpp"
#include "yaml-cpp/yaml.h"
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

bool converted = false;


std::vector<glm::vec3> colors{
        glm::vec3(0,0,1),
        glm::vec3(0,1,0),
        glm::vec3(1,0,0),
        glm::vec3(0.5,0,1),
        glm::vec3(0.8,0.8,0.1),
        glm::vec3(1,0.5,0.1),

};

int stupanj= 3;
std::vector<float> T{0,0,0,0,0.25,0.5,0.75,1,1,1,1};
std::vector<glm::vec3> kp{
        glm::vec3(-0.5f,-0.5f,0),
        glm::vec3(-0.6f,-0.1f,0),
        glm::vec3(0.0f,0.3f,0),
        glm::vec3(0.4f,0.2f,0),
        glm::vec3(0.5f,-0.2f,0),
        glm::vec3(0.4f,-0.5f,0),
        glm::vec3(0.2f,-0.7f,0)
};
float t =0.85f;



enum spline_type {
    ONE,
    ALL,
    INCLUDE_POLY
};

spline_type st = ONE;

//int ind_track;
//int fixed_track;
bool track = false;
std::vector<int> tracked_indexes;
std::vector<int> fixed_indexes;

std::vector<std::vector<glm::vec3>> P{};

void print_vec(glm::vec3 v){
    std::cout << "("<< v.x << ", " << v.y << ", " << v.z << " )\n";
}


bool approximatelyEqual(float a, float b, float epsilon)
{
    return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}


void glutPassiveMotionFunc(int x, int y ) {

    float xPos = ((float)x)/((float)(WindowWidth-1));
    float yPos = ((float)y)/((float)(WindowHeight-1));
    xPos =2*xPos-1;
    yPos=-2*yPos+1;


    //ind_track = -1;
    tracked_indexes.clear();

    for(int i = 0; i < kp.size();i++) {
        if (
                approximatelyEqual(kp[i].x, xPos, 0.2f) &&
                approximatelyEqual(kp[i].y, yPos, 0.2f)){
//            ind_track = i;
            tracked_indexes.push_back(i);
        }
    }

    if (track){
        for (auto ind : fixed_indexes){
//            kp[fixed_track].x = xPos;
//            kp[fixed_track].y = yPos;
            kp[ind].x = xPos;
            kp[ind].y = yPos;
        }
    }


   //std::cout << tracked_indexes.size() << "\n";


    glutPostRedisplay();

}


glm::vec3 algo(float t,bool info = false){
    for (auto p: P) p.clear();
    P.clear();


    for (int ii = 0; ii< 20 ; ii++) {
        std::vector<glm::vec3> temp;
        for (int jj = 0; jj < 20; jj++) {
            glm::vec3 tt{-5,-5,0};
            temp.push_back(tt);
        }

        P.push_back(temp);
    }

    int j=0;
    for (j = 0 ; t > T[j]; j++ );
    j--;

    if (info)
        std::cout << "t=" << t << " j=" << j <<  " | " << T[j] << "<"  << t <<  "<" << T[j+1] <<"\n";

    P[0]=kp;

    for (int l = 1 ; l <= stupanj ; l++){
        if (info){
            std::cout << std::endl;
            std::cout << "L:" << l << "\n";
        }

        for (int i = j-stupanj+l ; i < j+1 ; i++ ){

            float a = (t - T[i])    /(T[i+stupanj+1-l] - T[i]);
            P[l][i]=a * P[l-1][i]  + (1-a) * P[l-1][i-1];

            if (info){
                //std::cout << "\na:" << a << "\n";
                std::cout << "P" << i << "_[" << l << "]: ";
                print_vec(P[l][i]);
//                std::cout << "i need:\n" ;
//                std::cout << "P" << i << "_[" << l-1 << "] = ";
//                print_vec(P[l-1][i]);
//                std::cout << "P" << i-1 << "_[" << l-1 << "] =";
//                print_vec(P[l-1][i-1]);
            }
        }
        if(info)
            std::cout << std::endl;
    }



    //std::cout << P[3].size() << "\n";
    return P[stupanj][j];
}

void add_knot(float knot = -8){
    float new_knot;
    if (knot > -7){
        new_knot = knot;
    }

    else{
        std::cout << "NEW KNOT: ";
        std::cin >> new_knot;
        std::cout << "\n";
    }


    int l;
    for (int j = 0 ; j < T.size()-1;j++){
        if (T[j]  <= new_knot &&    new_knot < T[j+1]){
            l = j;
            break;
        }
    }


    //std::cout << "l: " << l << "\n";

    T.insert(T.begin() + l+1,new_knot);


    std::vector<glm::vec3> new_kp{};

    for (int j = 0; j < kp.size() + 1 ;j++){
        glm::vec3 q;

        if (j <= l - stupanj ){
            q = kp[j];
        }
        else if(l+1 <=j){
            q = kp[j-1];
        }
        else{
            q = (new_knot-T[j])/(T[j+stupanj+1] - T[j])* kp[j] + (T[j+stupanj+1]-new_knot)/(T[j+stupanj+1] - T[j]) * kp[j-1];
        }


        new_kp.push_back(q);
    }


    kp = new_kp;

    std::cout << "NEW T:[";

    for (auto tt: T){
        std::cout << tt << ", ";
    }
    std::cout << "]\n";



}

void myKeyboardFunc( unsigned char key, int x, int y ){

    if (key == 'b'){
        st = ONE;

    }
    if (key == 'n'){
        st = ALL;

    }
    if (key == 'm'){
        st = INCLUDE_POLY;

    }

    if (key == 'i'){
        algo(t,true);
    }


    if (key == 'a'){
        for (int i = 0; i< T.size(); i ++){
            int num_of = std::count(T.cbegin(),T.cend(),T[i]);
            if (num_of < stupanj){
                for ( int j = 0 ; j< stupanj - num_of + 1 ; j++){
                    add_knot(T[i]);
                }
            }


        }
        converted = true;
    }

    if (key == 's'){
        add_knot();
    }
}

void mySpecialKeyFunc( int key, int x, int y )
{
    if(key == GLUT_KEY_LEFT){
        t -= 0.01f;
    }
    else if(key == GLUT_KEY_RIGHT){
        t += 0.01f;
    }

    if (t <= T[0]) t = 0.001f;
    if (t > T[T.size()-1]) t = T[T.size()-1];

    //std::cout << "new t:" << t << "\n";
}


void mouse(int button, int state, int x, int y)
{


    //std::cout << button  << " " << state << "\n";
    if (button == GLUT_LEFT_BUTTON)
    {

        if (state == GLUT_DOWN){
            track = true;
            fixed_indexes = tracked_indexes;
//            fixed_track=ind_track;
        }
        else{
            track = false;
            fixed_indexes.clear();
//            fixed_track=-1;
        }

    }




}
int main(int argc, char ** argv)
{

    YAML::Node config = YAML::LoadFile("config.yaml");

    std::cout << "Ucitavam:\n";

    if (config["stupanj"]) {
        std::cout << "Stupanj: " << config["stupanj"].as<int>() << "\n";
        stupanj =  config["stupanj"].as<int>();
    }

    if (config["startna tocka"]) {
        std::cout << "Startna tocka: " << config["startna tocka"].as<float>() << "\n";
        t =  config["startna tocka"].as<float>();
    }


    if (config["vektor cvorova"]) {
        T.clear();
        std::cout << "Vektor cvorova: [";
        YAML::Node vekt_c = config["vektor cvorova"];
        for (YAML::iterator it = vekt_c.begin(); it != vekt_c.end(); ++it) {
            const YAML::Node& sensor = *it;
            std::cout << sensor.as<float>() << ", ";

            T.push_back(sensor.as<float>());
        }
        std::cout << "]\n";

    }

    if (config["kontrolni poligon"]) {
        kp.clear();
        std::cout << "Kontrolni poligon: [\n";
        YAML::Node vekt = config["kontrolni poligon"];
        for (YAML::iterator it = vekt.begin(); it != vekt.end(); ++it) {
            const YAML::Node& sensor = *it;
            glm::vec3 temp;
            temp.z=0;

            std::cout << "   " <<sensor.as<float>() << ", ";
            temp.x=sensor.as<float>();
            ++it;
            const YAML::Node& sensor2 = *it;
            std::cout <<sensor2.as<float>() <<"\n";
            temp.y=sensor2.as<float>();

            kp.push_back(temp);

        }
        std::cout << "]\n";

    }


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
    glutMouseFunc(mouse);
    glutMotionFunc(glutPassiveMotionFunc);
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





bool init_data()
{
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

	// Antialijasiranje poligona, ovisno o implementaciji
    //	  glEnable(GL_POLYGON_SMOOTH);
    //	  glEnable(GL_BLEND);
    //    glHint(GL_POLYGON_SMOOTH, GL_DONT_CARE);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	std::cout << "Going to load programs... " << std::endl << std::flush;
    shader.load_shaders({"BackgroundVertexShader.vert","BackgroundFragmentShader.frag","","",""});
	return true;
}


void print_one(){
    std::vector<glm::mat4> MVPs;
    MVPs.emplace_back(1);
    std::vector<float> out;
    auto p = algo(t);

    for (int i = 0 ; i <= stupanj ; i++){
        out.clear();

        for (auto p: P[i]){
            if (p.x < -4) continue;
            out.push_back(p.x);
            out.push_back(p.y);
            out.push_back(p.z);
            if(i == stupanj){
                out.push_back(0);
                out.push_back(-0.6f);
                out.push_back(0);
            }

        }


        shader.setVec3("col", colors[i]);
        Renderer::render(shader, std::vector<int>({3}) , out, MVPs);
    }

}

void print_all(bool include_poly=false){
    std::vector<glm::mat4> MVPs;
    MVPs.emplace_back(1);
    std::vector<float> out;

    if(!converted){
        for(float i = 0.001f;i < 1 ; i+=0.01f){
            auto p = algo(i);
            out.push_back(p.x);
            out.push_back(p.y);
            out.push_back(p.z);
        }

        shader.setVec3("col", colors[1]);
        Renderer::render(shader, std::vector<int>({3}) , out, MVPs);
    }
    else{
        int color_ind = 0;
        for (int j = 0; j < T.size()-1; j++){
            if (T[j] == T[j+1]) continue;

            float limit_l = T[j];
            if (limit_l <=0) limit_l = 0.001f;
            float limit_r = T[j+1];
            out.clear();

            for(float i = limit_l; i < limit_r; i+=0.01f){
                auto p = algo(i);
                out.push_back(p.x);
                out.push_back(p.y);
                out.push_back(p.z);
            }


            shader.setVec3("col", colors[color_ind++ % (colors.size()-1) + 1]);
            Renderer::render(shader, std::vector<int>({3}) , out, MVPs);

        }


    }


    out.clear();

    if(include_poly){
        for(auto p:kp){
            out.push_back(p.x);
            out.push_back(p.y);
            out.push_back(p.z);
        }
        shader.setVec3("col", colors[0]);
        Renderer::render(shader, std::vector<int>({3}) , out, MVPs);

    }



}


void myDisplay() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    if(st == ONE)
        print_one();

    if(st == ALL)
        print_all();

    if(st == INCLUDE_POLY)
        print_all(true);


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




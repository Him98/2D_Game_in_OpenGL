#include "main.h"
#include "timer.h"
#include "ball.h"
#include "ground.h"
#include "pond.h"
#include "magnet.h"
#include "trampoline.h"
#include "slopes.h"
#include "porcupine.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;
GLfloat g_vertex_buffer_data[5000];

/**************************
* Customizable functions *
**************************/

Ball ball1, balls[10];
color_t col[5]={COLOR_DARKRED, COLOR_GREEN, COLOR_RED, COLOR_SBLUE, COLOR_YELLOW};
Ground g1,g2;
Pond p1;
Magnet m1,m2;
Trampoline t1;
Porcupine po1;
Slopes s1[5];
int state_jump = 0,state_pond = 0, state_m1 = 0, state_m2 = 0, state_porcupine = 0;
int score = 0, level = 1, lives = 3;
extern bool drag_pan;
extern double drag_oldx, drag_oldy;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    g1.draw(VP);
    g2.draw(VP);
    for (int i=0;i<10;i++){
        balls[i].draw(VP);
        if(i%2 == 0)
            s1[i/2].draw(VP);
    }
    p1.draw(VP);
    t1.draw(VP);
    if(state_porcupine)
        po1.draw(VP);
    if(state_m1)
        m1.draw(VP);
    else if(state_m2)
        m2.draw(VP);
    ball1.draw(VP);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_SPACE);
    int panA = glfwGetKey(window, GLFW_KEY_A);
    int panD = glfwGetKey(window, GLFW_KEY_D);
    int panS = glfwGetKey(window, GLFW_KEY_S);
    int panW = glfwGetKey(window, GLFW_KEY_W);
    int drag_pan = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

    if(panA){
        if(screen_center_x - 0.05 < -6){
            screen_center_x = 9;
            ball1.position.x = 9;
        }
        else
            screen_center_x -= 0.05;
        reset_screen();
    }
    if(panD){
        if(screen_center_x + 0.05 > 6){
            screen_center_x = -9;
            ball1.position.x = -9;
        }
        else
            screen_center_x += 0.05;
        reset_screen();
    }
    if(panW && screen_center_y + 4/screen_zoom < 4){
        screen_center_y += 0.05;
        reset_screen();
    }
    if(panS && screen_center_y - 4/screen_zoom > -4){
        screen_center_y -= 0.05;
        reset_screen();
    }


    if (left && !state_pond) {
//        if(screen_center_x - 0.05 < -6){
//            screen_center_x = 9;
//            ball1.position.x = 9;
//        }
//        else
//            screen_center_x -= 0.05;
//        reset_screen();
//        if(ball1.position.x - ball1.speed <= -3.5)
//            ball1.position.x = -3.5;
//        else
            ball1.position.x -= ball1.speed;

//        if(state_porcupine && ball1.position.x<=-1.9 && ball1.position.x>=-3.45 && ball1.position.y == -1)
//            ball1.position.x = -4.5;

        if(ball1.position.x>=-1.5 && ball1.position.x<=-1.0 && ball1.position.y==-1){
            state_pond=1;
            ball1.position.y=-1.5;
            ball1.position.x=-1.0;
        }
        else if(ball1.position.x<=1.5 && ball1.position.x>=1.0 && ball1.position.y==-1){
            state_pond=1;
            ball1.position.y=-1.5;
            ball1.position.x=1.0;
        }
    }
    if (left && state_pond) {
        if(ball1.position.x - 0.05 > -1)
            ball1.position.x -= 0.05;
        else
            ball1.position.x = -1.0, ball1.position.y = -1.6;
        ball1.position.y = -1.5 - sqrt(1 - ((ball1.position.x)*(ball1.position.x)));
        if(ball1.position.y >= -1.6 && ball1.position.x == -1.0){
            state_pond = 0;
            ball1.position.y = -1;
            ball1.position.x = -1.6;
        }
    }
    if (right && !state_pond) {
//        if(screen_center_x + 0.05 > 6){
//            screen_center_x = -9;
//            ball1.position.x = -9;
//        }
//        else
//            screen_center_x += 0.05;
//        reset_screen();
//        if(ball1.position.x + ball1.speed >= 3.5)
//            ball1.position.x = 3.5;
//        else
            ball1.position.x += ball1.speed;

//        if(state_porcupine && ball1.position.x<=-1.9 && ball1.position.x>=-3.45 && ball1.position.y == -1)
//            ball1.position.x = -3.5;

        if(ball1.position.x>=-1.5 && ball1.position.x<=-1.0 && ball1.position.y==-1){
            state_pond=1;
            ball1.position.y=-1.5;
            ball1.position.x=-1.0;
        }
        else if(ball1.position.x<=1.5 && ball1.position.x>=1.0 && ball1.position.y==-1){
            state_pond=1;
            ball1.position.y=-1.5;
            ball1.position.x=1.0;
        }
    }
    if (right && state_pond) {
        if(ball1.position.x + 0.05 < 1){
            ball1.position.y = -1.5 - sqrt(1 - ((ball1.position.x)*(ball1.position.x)));
            ball1.position.x += 0.05;
        }
        else{
            ball1.position.y = -1, ball1.position.x = 1.6;
            state_pond = 0;
        }
    }
    if(!state_jump && up && !state_pond){
        ball1.speedy = 0.3;
        state_jump = 1;
    }
    if(!state_jump && up && state_pond){
        ball1.speedy = 0.3;
        state_jump = 1;
    }

    if(drag_pan){
        if(drag_oldx == -1 || drag_oldy == -1) {
            glfwGetCursorPos(window, &drag_oldx, &drag_oldy);
        }
        else{
            int w, h;
            double new_x, new_y;
            glfwGetCursorPos(window, &new_x, &new_y);
            glfwGetWindowSize(window, &w, &h);
            screen_center_x -= 8 * (new_x - drag_oldx) / (w * screen_zoom);
            screen_center_y += 8 * (new_y - drag_oldy) / (h * screen_zoom);
            if(screen_center_x - 6/screen_zoom < -6)
                screen_center_x = -6 + 6/screen_zoom;
            if(screen_center_y - 6/screen_zoom < -6)
                screen_center_y = -6 + 6/screen_zoom;
            if(screen_center_x + 6/screen_zoom > 6)
                screen_center_x = 6 - 6/screen_zoom;
            if(screen_center_y + 6/screen_zoom > 6)
                screen_center_y = 6 - 6/screen_zoom;
            drag_oldx = new_x;
            drag_oldy = new_y;
            reset_screen();
        }
    }
}

void tick_elements() {
    if(state_porcupine)
        po1.tick();
    if(state_porcupine && ball1.position.x != -3.5 && abs(po1.position.x - ball1.position.x)<=0.95 && ball1.position.y == -1){
        ball1.position.x = -3.5;
        lives--;
    }
    for(int i=0;i<10;i++){
        balls[i].tick();
        if(i%2 == 0)
            s1[i/2].tick();
        if(balls[i].position.x<-5 || balls[i].position.y>4){
            int p = (3 + (rand()%8000)*0.001), q = 1 + (rand()%4000)*0.001;
            if(p%4)
                p = p + 2;
            else
                p = p - 2;
            balls[i].position.x = p;
            balls[i].position.y = q;
            balls[i].speed = ((double) rand()/RAND_MAX)*0.06 + 0.01;
            if(i%2 == 0){
                double ang = 30;
                s1[i/2].position.x = p-0.5*sin((ang * M_PI)/180);
                s1[i/2].position.y = q+0.5*cos((ang * M_PI)/180);
                s1[i/2].speed = balls[i].speed;
                s1[i/2].rotation = ang;
            }
        }
    }
    if(state_jump == 1){
        if((ball1.position.x>=1.7 && ball1.position.x<=3.8) && ball1.speedy<0 && ball1.position.y + ball1.speedy <= 0.1){
            ball1.speedy = -1.05*ball1.speedy;
           // ball1.position.y = 0.1;
            state_jump = 1;
            //ball1.speedy = 0;
        }
        else if((ball1.position.x>=1 || ball1.position.x<=-1) && ball1.position.y + ball1.speedy <= -1){
            ball1.position.y = -1;
            if(state_porcupine && ball1.position.x<=-1. && ball1.position.x>=-3.45){
                lives--;
                ball1.position.x=-3.5;
            }
            state_jump = 0;
            ball1.speedy = 0;
        }
        else if((ball1.position.x<=1 || ball1.position.x>=-1) && ball1.speedy<0 && ball1.position.y + ball1.speedy <= -3){
            state_jump = 0;
            state_pond = 1;
            ball1.speedy = 0;
            ball1.position.y = -1.5 - sqrt(1 - ((ball1.position.x)*(ball1.position.x)));
        }
        else
        {
            ball1.position.y += ball1.speedy;
            if(state_m1 && ball1.position.y >= 3 && ball1.position.y<=4){
                if(ball1.position.x - ball1.speedy < -3.5)
                    ball1.position.x = -3.5;
                else
                    ball1.position.x -= ball1.speed;
            }
            else if(state_m2 && ball1.position.y >= 1 && ball1.position.y<=3){
                if(ball1.position.x - ball1.speedy < -3.5)
                    ball1.position.x = -3.5;
                else
                    ball1.position.x -= ball1.speed;
            }
            if(state_pond)
                ball1.speedy -= 0.03;
            else
                ball1.speedy -= 0.01;
        }
    }
    for(int i = 0;i < 10;i++){
        if (ball1.speedy<0 && detect_collision(ball1.bounding_box(), balls[i].bounding_box())) {
            score++;
            if(i%2==0){
                balls[i].position.y = -1000;
                s1[i/2].position.y = -1000;
                ball1.speedy = -ball1.speedy*cos(((120*3.1415)/90) - 180);
                if(ball1.position.x - 0.5 < -3.5)
                    ball1.position.x = -3.5;
                else
                    ball1.position.x -= 0.5;
            }
            else{
                balls[i].position.y = -1000;
                ball1.speedy = -ball1.speedy;
            }
        }
    }
    if(ball1.position.x>=-1.5 && ball1.position.x<=-1.0 && ball1.position.y==-1){
        state_pond=1;
        ball1.position.y=-1.5;
        ball1.position.x=-1.0;
    }
    else if(ball1.position.x<=1.5 && ball1.position.x>=1.0 && ball1.position.y==-1){
        state_pond=1;
        ball1.position.y=-1.5;
        ball1.position.x=1.0;
    }
        if(ball1.position.x>=-1.5 && ball1.position.x<=-1.0 && ball1.position.y==-1){
            state_pond=1;
            ball1.position.y=-1.5;
            ball1.position.x=-1.0;
        }

        else if(ball1.position.x<=1.5 && ball1.position.x>=1.0 && ball1.position.y==-1){
            state_pond=1;
            ball1.position.y=-1.5;
            ball1.position.x=1.0;
        }
        if(state_pond == 1 && ball1.speedy==0){
            if(ball1.position.x < 0)
                ball1.position.x += 0.03;
            else if(ball1.position.x > 0)
                ball1.position.x -= 0.03;
            ball1.position.y = -1.5 - sqrt(1 - ((ball1.position.x)*(ball1.position.x)));
    }
    if(ball1.position.y>-1.5)
        state_pond = 0;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    g1          = Ground(0,-1.5,COLOR_GREEN);
    g2          = Ground(0,-2.5,COLOR_BROWN);
    ball1       = Ball(-3.5, -1, COLOR_RED);
    for (int i=0;i<10;i++){
        float p = 10+((double) rand()/RAND_MAX)*2, q = 1 + (rand()%5000)*0.001;
        if(int(p)%4)
            p = p + 2;
        else
            p = p - 2;
        balls[i] = Ball(p,q,col[rand()%5]);
        balls[i].speed = ((double) rand()/RAND_MAX)*0.06 + 0.01;
        if(i%2 == 0){
            double ang = 30;
            s1[i/2] = Slopes(balls[i].position.x-0.5*sin(M_PI * ang/180),balls[i].position.y+0.5*cos(M_PI * ang/180),COLOR_BROWN);
            s1[i/2].speed = balls[i].speed;
            s1[i/2].rotation = ang;
        }
    }
    p1          = Pond(0,-1.5,COLOR_CYAN);
    t1          = Trampoline(2.75,-0.5,COLOR_BROWN);
    m1          = Magnet(-4,3,COLOR_DARKRED);
    m2          = Magnet(-4,1,COLOR_DARKRED);
    po1         = Porcupine(-2.5,-2,COLOR_ROYALBLUE);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (0, 0, 0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;
    int i=1,j=1;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        char title[100];
        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
//            if(i%200 == 0){
//                state_m1 = 1;
//                state_m2 = 0;
//                state_porcupine = 1;
//            }
//            else if(i%601 == 0){
//                state_m2 = 1;
//                state_m1 = 0;
//                state_porcupine = 1;
//            }
//            else if(i%901 == 0){
//                state_m1 = 0;
//                state_m2 = 0;
//                state_porcupine = 0;
//                i=0;
//            }
            if(lives < 0)
                break;
            if(score>=20 && score < 40){
                if(level == 1)
                    level++;
                state_m1 = 1;
                state_m2 = 0;
                sprintf(title,"Score = %d     ....Level %d...",score,level);
                glfwSetWindowTitle(window, title);
                i=1;
            }
            else if(score<60 && score >= 40){
                if(level == 2)
                    level++;
                if(i%200 == 0){
                      state_m1 = 1;
                      state_m2 = 0;
                      state_porcupine = 1;
                }
                else if(i%601 == 0){
                      state_m2 = 1;
                      state_m1 = 0;
                      //state_porcupine = 1;
                }
                else if(i%901 == 0){
                                state_m1 = 0;
                                state_m2 = 0;
                                state_porcupine = 0;
                                i=0;
                            }
                i++;
                j=1;
            }
            else if(score >= 60){
                if(level == 3)
                    level++;
                if(j%200 == 0){
                      state_m1 = 1;
                      state_m2 = 0;
                      state_porcupine = 1;
                }
                else if(j%601 == 0){
                      state_m2 = 1;
                      state_m1 = 0;
                      state_porcupine = 1;
                }
                else if(j%901 == 0){
                                state_m1 = 0;
                                state_m2 = 0;
                                state_porcupine = 1;
                                j=0;
                            }
                j++;
            }
            sprintf(title,"Score = %d   Lives Left : %d    ....Level %d....",score,lives,level);
            glfwSetWindowTitle(window, title);
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);
            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }
    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width + 1)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height + 1));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}

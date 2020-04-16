////////////////////////////////////////
// Window.cpp
////////////////////////////////////////

#include "Window.h"

////////////////////////////////////////////////////////////////////////////////

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "CSE 291H";
//glm::vec3 cubeMin=glm::vec3(-1,-1,-1);
//glm::vec3 cubeMax=glm::vec3(1, 1, 1);
// Objects to render
Cube * Window::cube;
bool stop = false;
int operate_time = 0;
// Camera Properties
Camera* Cam;
Tetrahedral* tetrahedral[120];
int Tetrahedral_SIZE = 120;
// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;
int fall_len = 0;
// The shader program id
GLuint Window::shaderProgram;
Vertice* Particles[60];
Ground* ground;
//std::vector<Vertice> a = {
//        // Front
//        glm::vec3(-1,-1,1),
//        glm::vec3(1,-1,1),
////        glm::vec3(1,1,1),
//        glm::vec3(-1,1,1),
//
//        // Back
////        glm::vec3(1,-1,-1),
////        glm::vec3(-1,-1,-1),
////        glm::vec3(-1,1,-1),
////        glm::vec3(1,1,-1),
//
//        // Top
////        glm::vec3(-1,1,1),
////        glm::vec3(1,1,1),
////        glm::vec3(1,1,-1),
////        glm::vec3(-1,1,-1),
//
//        // Bottom
//        glm::vec3(-1,-1,-1),
////        glm::vec3(1,-1,-1),
//        glm::vec3(1,-1,1),
//        glm::vec3(-1,-1,1),
//
//        // Left
//        glm::vec3(-1,-1,-1),
//        glm::vec3(-1,-1,1),
//        glm::vec3(-1,1,1),
////        glm::vec3(-1,1,-1),
//
//        // Right
////        glm::vec3(1,-1,1),
////        glm::vec3(1,-1,-1),
////        glm::vec3(1,1,-1),
////        glm::vec3(1,1,1)
//    };

//std::vector<Vertice> a = {
//    // Front
//    glm::vec3(-1,-1,1),
//    glm::vec3(1,-1,1),
//    glm::vec3(-1,1,1),
//
//    glm::vec3(-1,-1,-1),
//    glm::vec3(1,-1,1),
//    glm::vec3(-1,-1,1),
//
//    glm::vec3(-1,-1,-1),
//    glm::vec3(-1,-1,1),
//    glm::vec3(-1,1,1)
//};
float dis = 0.8;
float start_x_1 = -1, start_y_1 = -1, start_z_1 = 1;
std::vector<Vertice> a = {
    glm::vec3(start_x_1, start_y_1, start_z_1),
    glm::vec3(start_x_1 + dis, start_y_1, start_z_1),
    glm::vec3(start_x_1, start_y_1 + dis, start_z_1),
    glm::vec3(start_x_1, start_y_1, start_z_1 - dis),
};
std::vector<Vertice> b = {
    glm::vec3(start_x_1 + dis, start_y_1 + dis, start_z_1),
    glm::vec3(start_x_1 + dis, start_y_1, start_z_1),
    glm::vec3(start_x_1 + dis, start_y_1 + dis, start_z_1 - dis),
    glm::vec3(start_x_1, start_y_1 + dis, start_z_1),
};
std::vector<Vertice> c = {
    glm::vec3(start_x_1 + dis, start_y_1, start_z_1 - dis),
    glm::vec3(start_x_1, start_y_1, start_z_1 - dis),
    glm::vec3(start_x_1 + dis, start_y_1 + dis, start_z_1 - dis),
    glm::vec3(start_x_1 + dis, start_y_1, start_z_1),
};
std::vector<Vertice> d = {
    glm::vec3(start_x_1, start_y_1 + dis, start_z_1 - dis),
    glm::vec3(start_x_1, start_y_1 + dis, start_z_1),
    glm::vec3(start_x_1 + dis, start_y_1 + dis, start_z_1 - dis),
    glm::vec3(start_x_1, start_y_1, start_z_1 - dis),
};
//std::vector<Vertice> e = {
//    glm::vec3(start_x_1, start_y_1, start_z_1 - dis),
//    glm::vec3(start_x_1 + dis, start_y_1 + dis, start_z_1 - dis),
//    glm::vec3(start_x_1, start_y_1 + dis, start_z_1),
//    glm::vec3(start_x_1 + dis, start_y_1, start_z_1),
//};

////////////////////////////////////////////////////////////////////////////////

// Constructors and desctructors 
bool Window::initializeProgram() {

	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}


bool Window::initializeObjects()
{
	// Create a cube
	cube = new Cube();
    ground = new Ground();
	//cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));
    initialize_Particles();
    initialize_Tetrahedral();
	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;
    delete ground;
	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}

////////////////////////////////////////////////////////////////////////////////

// for the Window
GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// set up the camera
	Cam = new Camera();
	Cam->SetAspect(float(width) / float(height));

	// initialize the interaction variables
	LeftDown = RightDown = false;
	MouseX = MouseY = 0;

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	Cam->SetAspect(float(width) / float(height));
}

////////////////////////////////////////////////////////////////////////////////

// update and draw functions
void Window::idleCallback()
{
	// Perform any updates as necessary. 
	Cam->Update();
//    cube->update();
    
    if(stop == false){
    operate_time++;
    for (int i = 0; i < 60; i++) {
        if (Particles[i]->position[1] < -2.5) {
            Particles[i]->velocity *= -0.999;
            Particles[i]->position[1] = -2.5;
//            stop = true;
        }
//        std::cout<<operate_time<<" "<<i<<" the force is "<<to_string(Particles[i]->force)<<"\n the location is "<<to_string(Particles[i]->position)<<std::endl;
        Particles[i]->integrate(0.005);
    }
     for (int i = 0; i < Tetrahedral_SIZE; i++) {
        tetrahedral[i]->update();
//         std::cout<<"tetrahedral "<<i<<" T "<<to_string(tetrahedral[i]->T)<<std::endl;
     }
    }
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render the object.
//    cube->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    ground->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    for(int i = 0;i<Tetrahedral_SIZE;i++)
        tetrahedral[i]->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

////////////////////////////////////////////////////////////////////////////////

// helper to reset the camera
void Window::resetCamera() 
{
	Cam->Reset();
	Cam->SetAspect(float(Window::width) / float(Window::height));
}

////////////////////////////////////////////////////////////////////////////////

// callbacks - for Interaction 
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key) 
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		case GLFW_KEY_R:
			resetCamera();
			break;

		default:
			break;
		}
	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		LeftDown = (action == GLFW_PRESS);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		RightDown = (action == GLFW_PRESS);
	}
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {

	int maxDelta = 100;
	int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

	MouseX = (int)currX;
	MouseY = (int)currY;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if (LeftDown) {
		const float rate = 1.0f;
		Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
		Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
	}
	if (RightDown) {
		const float rate = 0.005f;
		float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
		Cam->SetDistance(dist);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Window::initialize_Particles()
{
    for (int i = 0; i<=2; i++) {
        for (int j = 0; j<=4; j++) {
            for (int k = 0; k<=3; k++) {
                Particles[i*20 + j*4 + k] = new Vertice(glm::vec3(start_x_1 + k * dis, start_y_1 + i * dis, start_z_1 - j * dis));
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////

void Window::initialize_Tetrahedral()
{
    std::vector<int> num;
    for (int i = 0; i<2; i++) {
        for (int j = 0; j<4; j++) {
            for (int k = 0; k<3; k++) {
                int idx = i * 20 + j * 4 + k;
//                a[0].position = Particles[idx]->position;
//                a[1].position = Particles[idx + 1]->position;
//                a[2].position = Particles[idx + 20]->position;
//                a[3].position = Particles[idx + 4]->position;
                num.push_back(idx);
                num.push_back(idx + 1);
                num.push_back(idx + 20);
                num.push_back(idx + 4);
                tetrahedral[i*12 + j*3 + k] = new Tetrahedral(num);
                std::vector<int>().swap(num);
            }
        }
    }
    
    for (int i = 0; i<2; i++) {
        for (int j = 0; j<4; j++) {
            for (int k = 0; k<3; k++) {
                int idx = i * 20 + j * 4 + k;
//                a[0].position = Particles[idx + 21]->position;
//                a[1].position = Particles[idx + 1]->position;
//                a[2].position = Particles[idx + 25]->position;
//                a[3].position = Particles[idx + 20]->position;
                num.push_back(idx + 21);
                num.push_back(idx + 1);
                num.push_back(idx + 25);
                num.push_back(idx + 20);
                tetrahedral[24 + i*12 + j*3 + k] = new Tetrahedral(num);
                std::vector<int>().swap(num);
            }
        }
    }

    for (int i = 0; i<2; i++) {
        for (int j = 0; j<4; j++) {
            for (int k = 0; k<3; k++) {
                int idx = i * 20 + j * 4 + k;
//                a[0].position = Particles[idx + 5]->position;
//                a[1].position = Particles[idx + 4]->position;
//                a[2].position = Particles[idx + 25]->position;
//                a[3].position = Particles[idx + 1]->position;
                num.push_back(idx + 5);
                num.push_back(idx + 4);
                num.push_back(idx + 25);
                num.push_back(idx + 1);
                tetrahedral[48 + i*12 + j*3 + k] = new Tetrahedral(num);
                std::vector<int>().swap(num);
            }
        }
    }

    for (int i = 0; i<2; i++) {
        for (int j = 0; j<4; j++) {
            for (int k = 0; k<3; k++) {
                int idx = i * 20 + j * 4 + k;
//                a[0].position = Particles[idx + 25]->position;
//                a[1].position = Particles[idx + 20]->position;
//                a[2].position = Particles[idx + 24]->position;
//                a[3].position = Particles[idx + 4]->position;
                num.push_back(idx + 24);
                num.push_back(idx + 20);
                num.push_back(idx + 25);
                num.push_back(idx + 4);
                tetrahedral[72 + i*12 + j*3 + k] = new Tetrahedral(num);
                std::vector<int>().swap(num);
            }
        }
    }
    
    
    for (int i = 0; i<2; i++) {
        for (int j = 0; j<4; j++) {
            for (int k = 0; k<3; k++) {
                int idx = i * 20 + j * 4 + k;
                //                a[0].position = Particles[idx + 25]->position;
                //                a[1].position = Particles[idx + 20]->position;
                //                a[2].position = Particles[idx + 24]->position;
                //                a[3].position = Particles[idx + 4]->position;
                num.push_back(idx + 4);
                num.push_back(idx + 20);
                num.push_back(idx + 25);
                num.push_back(idx + 1);
                tetrahedral[96 + i*12 + j*3 + k] = new Tetrahedral(num);
                std::vector<int>().swap(num);
            }
        }
    }
    
//    for (auto a:tetrahedral[0]->positions)
//        std::cout<<" tetrahedral 0 is "<<a[0]<<" "<<a[1]<<" "<<a[2]<<std::endl;
//    for (auto a:tetrahedral[3]->positions)
//        std::cout<<" tetrahedral 4 is "<<a[0]<<" "<<a[1]<<" "<<a[2]<<std::endl;
}

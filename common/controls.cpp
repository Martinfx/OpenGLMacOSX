#include <iostream>
using namespace std;

// Include GLFW
#include <glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

// glm::rotate()
// When using glm::rotate( X, vec3 ), you are multiplying
//      X * glm::rotate( Identity, vec3 )
// This means rotate first, than X

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix()
{
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix()
{
	return ProjectionMatrix;
}

// Initial position : on +Z
glm::vec3 position = glm::vec3( 0,0,5 );
// Initial horizontal angle : toward -Z
float horizontalAngle = .0f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
float zAngle=0.0;
// Initial Field of View
float initialFoV = 45.0f;

float speed = .90f; // 3 units / second
float mouseSpeed = 0.005f;


void computeMatricesFromInputs()
{
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(initialFoV, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix       = glm::lookAt(
                                   glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
                                   glm::vec3(0,0,0), // and looks at the origin
                                   glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                   );

//    ViewMatrix       = glm::mat4(1.0f); // identity
  
    if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
        horizontalAngle += speed;
    }
    // Move backward
    if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
        horizontalAngle -= speed;
    }
    // Strafe right
    if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
        verticalAngle += speed;
    }
    // Strafe left
    if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
        verticalAngle -= speed;
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        zAngle += speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        zAngle -= speed;
    }
    
//    cerr << "ViewMatrix:" << endl << glm::to_string(ViewMatrix) << endl;
    ViewMatrix = glm::rotate(ViewMatrix, horizontalAngle, glm::vec3(1,0,0));
//    cerr << "ViewMatrix:" << endl << glm::to_string(ViewMatrix) << endl;
//    glm::mat4 vr = ViewMatrix * glm::rotate(glm::mat4(1.0), horizontalAngle, glm::vec3(1,0,0));
//    cerr << "V*R:" << endl << glm::to_string(ViewMatrix) << endl;
//    cerr << " -------- " << endl;
    ViewMatrix = glm::rotate(ViewMatrix, verticalAngle, glm::vec3(0,1,0));
    ViewMatrix = glm::rotate(ViewMatrix, zAngle, glm::vec3(0,0,1));
}

void computeMatricesFromInputs_ORG()
{

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// For the next frame, the "last time" will be "now"
    cerr
    << "position=" << glm::to_string( position ) << endl
    << "direction=" << glm::to_string( direction )<< endl
    << "up=" << glm::to_string( up ) << endl
    << "-----" << endl;
    // Camera matrix
//    ViewMatrix       = glm::lookAt(
//                                             glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
//                                             glm::vec3(0,0,0), // and looks at the origin
//                                             glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
//                                             );

	lastTime = currentTime;
}

CamControl::CamControl() {
    glfwSetCursorPos(window, 1024/2, 768/2);
}

void CamControl::computeMatricesFromInputs()
{
    
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();
    
    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    
    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    // Reset mouse position for next frame
    glfwSetCursorPos(window, 1024/2, 768/2);
    
    // Compute new orientation
    horizontalAngle += mouseSpeed * float(1024/2 - xpos );
    verticalAngle   += mouseSpeed * float( 768/2 - ypos );
    
    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
                        cos(verticalAngle) * sin(horizontalAngle),
                        sin(verticalAngle),
                        cos(verticalAngle) * cos(horizontalAngle)
                        );
    direction = glm::vec3(0,0,-1);
    
    // Right vector
    glm::vec3 right = glm::vec3(
                                sin(horizontalAngle - 3.14f/2.0f),
                                0,
                                cos(horizontalAngle - 3.14f/2.0f)
                                );
    
    // Up vector
    glm::vec3 up = glm::cross( right, direction );
    up = vec3(0,1,0);
    
    // Move forward
    if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
        position -= right * deltaTime * speed;
    }
    
    float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.
    
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    position = glm::vec3(4,3,3);
    glm::vec3 lookPos = glm::vec3(0,0,0);// position + direction*5.f;
    ViewMatrix       = glm::lookAt(
                                   position,           // Camera is here
                                   lookPos, //position+direction, // and looks here : at the same position, plus "direction"
                                   up                  // Head is up (set to 0,-1,0 to look upside-down)
                                   );
    // For the next frame, the "last time" will be "now"
    cerr
    << "position=" << glm::to_string( position ) << endl
    << "lookPos=" << glm::to_string( lookPos )<< endl
    << "up=" << glm::to_string( up ) << endl
    << "-----" << endl;
    // Camera matrix
    //    ViewMatrix       = glm::lookAt(
    //                                             glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
    //                                             glm::vec3(0,0,0), // and looks at the origin
    //                                             glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    //                                             );
    
    lastTime = currentTime;
}
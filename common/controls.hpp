#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

class CamControl {
public:
    CamControl();
    
    void computeMatricesFromInputs();
    
    glm::mat4 getViewMatrix() { return mViewMatrix; };
    glm::mat4 getProjectionMatrix() {return mProjectionMatrix; };

    glm::mat4 mViewMatrix;
    glm::mat4 mProjectionMatrix;
};
#endif
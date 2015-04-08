#include <iostream>
using namespace std;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <math.h>

float deg2rad(float deg) { return deg*M_PI/180.; }

void print (glm::mat3 r, string s)
{
	cout << s << endl;
	for (int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
			cout << "  " << r[i][j] ;
		}
		cout << endl;
	}
}

void print (const glm::mat4 &r, string s)
{
	cout << s << endl;
	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++) {
			cout << "  " << r[i][j] ;
		}
		cout << endl;
	}
}

void print(const glm::vec3& v, string s)
{
	cout << s << endl;
	for (int i=0; i<3; i++) {
		cout << "  " << v[i] ;
	}
	cout << endl;
}

int main()
{
 float degree = 45.;
 glm::vec3 zAxis(0.f, 0.f, 1.f);
 glm::mat3 R3 = glm::mat3(glm::rotate (deg2rad(degree),zAxis));
 glm::mat4 R4 = glm::rotate (deg2rad(degree),zAxis);

 cout << "Note that glm matrices are stored in column major order!" << endl;
 cout << "----" << endl;

 print(R3, "R3");
 printf("----\n");
 cout << "The following will show you  column vectors enclosed by parantheses." << endl;
 cout << "R3: " << glm::to_string(R3) << endl;
 printf("----\n");

 print(R4, "R4");
 printf("----\n");

 glm::vec2 p(1,0), q(0,1);
 glm::vec3 u(1,1,1);

 glm::vec3 ru = R3 * u,
           rp = R3 * glm::vec3(p,1),
           rq = R3 * glm::vec3(q,1);

 printf("This is the 1st column of R3.\n");
 print(rp, "rp:");
 printf("----\n");
 printf("This is the 2nd column of R3.\n");
 print(rq, "rq:");
 printf("----\n");
 printf("This is the 45 degrees rotation result of (1,1,1) about the z-axis.\n");
 print(ru, "ru:");
 printf("----\n");

 glm::mat3 H(1.0); // identity matrix
 H[2].x = 1.0; // (0,2)-element of H
 cout << "H: " << glm::to_string(H) << endl;
 H[2] = glm::vec3(2.0f, 3.0f, 1.0f); // 2d translation
 cout << "H: " << glm::to_string(H) << endl;

 glm::mat3 T = H * R3;
 cout << "H*R: " << glm::to_string(T) << endl;
 return 0;
}

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
	Camera() = default;
	Camera(const Camera&) = default;
	Camera(Camera&&) = default;
	
	Camera(glm::vec3 e, glm::vec3 t, glm::vec3 u);
	
	Camera(glm::vec3 eye, float U, float V);
	
	Camera& operator=(const Camera&) = default;
	Camera& operator=(Camera&&) = default;
	
	void LookAt(glm::vec3 e, glm::vec3 t, glm::vec3 u);
	
	const glm::vec3& GetEye() const;
	glm::vec3&& GetForward() const;
	glm::vec3&& GetStrafe() const;
	glm::vec3&& GetUp() const;
	
	float GetU() const;
	float GetV() const;
	
	const glm::mat4& GetView() const;
	//glm::mat4 GetViewInv() const;
	
	void SetUV(float u, float v);
	
	void UpdateView();
	
	void SetEye(const glm::vec3& e);
private:
	/*glm::vec3 eye;
	glm::vec3 vz;
	glm::vec3 vx;
	glm::vec3 vy;*/
	
	glm::vec3 eye;
	float u;
	float v;
	
	glm::mat4 view;
};

#endif //CAMERA_H
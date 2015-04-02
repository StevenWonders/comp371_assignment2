//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 28/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "ThirdPersonCamera.h"
#include "EventManager.h"
#include "World.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>


using namespace glm;


ThirdPersonCamera::ThirdPersonCamera(Model* targetModel)
    : Camera(), mTargetModel(targetModel), mHorizontalAngle(0.0f), mVerticalAngle(0.0f), mRadius(10.0f)

{
    assert(mTargetModel != nullptr);
    CalculateCameraBasis();
	mSpeed = 5.0;
	mouseSpeed = 0.05;
}

ThirdPersonCamera::~ThirdPersonCamera()
{

}

void ThirdPersonCamera::CalculateCameraBasis()
{
	//B = horizontal angle
	//A = vertical angle
	//r = radius

	//x = rcosAcosB
	//y = rsinA
	//z = -rcosAsinB
	float cosA = glm::cos(mVerticalAngle);
	float cosB = glm::cos(mHorizontalAngle);
	float sinA = glm::sin(mVerticalAngle);
	float sinB = glm::sin(mHorizontalAngle);

	float x = mRadius * cosA * cosB;
	float y = mRadius * sinA;
	float z = mRadius * cosA * sinB * -1;

	mPosition = vec3(x,y,z) + mTargetModel->GetPosition();

	// Find Lookat vector
	mLookAt = glm::normalize(mTargetModel->GetPosition() - mPosition);
	std::cout<<"mPos:<" << mPosition.x <<","<<mPosition.y<<","<<mPosition.z<<">"<<std::endl;
	std::cout<<"Lookat:<" << mLookAt.x <<","<<mLookAt.y<<","<<mLookAt.z<<">"<<std::endl;
	// Find right vector
	mRight = glm::cross(mLookAt, glm::vec3(0,1,0));

	// Find the up vector
	mUp = glm::cross(mRight, mLookAt);

	// @TODO
    // Calculate Camera Vectors (LookAt, Up, Right) from Spherical Coordinates
    // Convert from Spherical to Cartesian Coordinates to get the lookAt Vector
	//mPosition = mTargetModel->GetPosition() + vec3(-10.0f, 0.0f, 0.0f); // This is wrong
	//mLookAt = vec3(1.0f, 0.0f, 0.0f); // This is wrong
	//mRight = vec3(0.0f, 1.0f, 0.0f);// This is wrong
	//mUp = vec3(0.0f, 0.0f, 1.0f);// This is wrong

}

void ThirdPersonCamera::Update(float dt)
{
    EventManager::DisableMouseCursor();
    
    // @TODO
    // 1 - Map Mouse motion to Spherical Angles
    // 2 - Clamp vertical angle to [-85, 85] degrees
    // 3 - Wrap Horizontal angle within [-180, 180] degrees
    
	float x_movement = EventManager::GetMouseMotionX();
	float y_movement = EventManager::GetMouseMotionY();

	float newHorizontalAngle = mHorizontalAngle + x_movement*mouseSpeed;
	if (newHorizontalAngle > glm::pi<float>()) {
		newHorizontalAngle = -1*glm::pi<float>();
	}
	else if (newHorizontalAngle < -1*glm::pi<float>()) {
		newHorizontalAngle = glm::pi<float>();
	}
	mHorizontalAngle = newHorizontalAngle;

	float newVerticalAngle = mVerticalAngle - y_movement*mouseSpeed;
	if (newVerticalAngle > 0.05 && newVerticalAngle < 1.48) {
		mVerticalAngle = newVerticalAngle;
	}

	std::cout << "Horizontal angle: " << mHorizontalAngle <<std::endl;
	
	//std::cout << "Horizontal X: " << EventManager::GetMouseMotionX() <<" Vertical Y: " << EventManager::GetMouseMotionY() <<std::endl;
	


	
    
    // Press W to move Forward
    if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W ) == GLFW_PRESS)
    {
		// @TODO - Move Model Forward
		glm::vec3 newPositionModel = mTargetModel->GetPosition() + glm::normalize(vec3(mLookAt.x,0,mLookAt.z))*dt*mSpeed;
		mTargetModel->SetPosition(newPositionModel);

		// Move camera by the same amount
		glm::vec3 newPositionCamera = mPosition + glm::normalize(vec3(mLookAt.x,0,mLookAt.z))*dt*mSpeed;
		mPosition = newPositionCamera;

    }

	// Press S to move Backward
    if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S ) == GLFW_PRESS)
    {
		// @TODO - Move Model Backward
		glm::vec3 newPositionModel = mTargetModel->GetPosition() - glm::normalize(vec3(mLookAt.x,0,mLookAt.z))*dt*mSpeed;
		mTargetModel->SetPosition(newPositionModel);

		// Move camera by the same amount
		glm::vec3 newPositionCamera = mPosition - glm::normalize(vec3(mLookAt.x,0,mLookAt.z))*dt*mSpeed;
		mPosition = newPositionCamera;
    }
    
    // Press A to move left
    if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A ) == GLFW_PRESS)
    {
		// @TODO - Move Model left
		glm::vec3 newPositionModel = mTargetModel->GetPosition() - glm::normalize(vec3(mRight.x,0,mRight.z))*dt*mSpeed;
		mTargetModel->SetPosition(newPositionModel);

		// Move camera by the same amount
		glm::vec3 newPositionCamera = mPosition - glm::normalize(vec3(mRight.x,0,mRight.z))*dt*mSpeed;
		mPosition = newPositionCamera;

    }

	// Press D to move right
    if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D ) == GLFW_PRESS)
    {
		// @TODO - Move Model right
		glm::vec3 newPositionModel = mTargetModel->GetPosition() + glm::normalize(vec3(mRight.x,0,mRight.z))*dt*mSpeed;
		mTargetModel->SetPosition(newPositionModel);

		// Move camera by the same amount
		glm::vec3 newPositionCamera = mPosition + glm::normalize(vec3(mRight.x,0,mRight.z))*dt*mSpeed;
		mPosition = newPositionCamera;
    }

	// Rotate model
	mTargetModel->SetRotation2(glm::vec3(0, 1, 0), mHorizontalAngle*180/glm::pi<float>());

    CalculateCameraBasis();
}

glm::mat4 ThirdPersonCamera::GetViewMatrix() const
{
    return glm::lookAt(mPosition, mPosition + mLookAt, mUp);
}

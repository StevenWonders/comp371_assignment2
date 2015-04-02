//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "Camera.h"
#include <GLM/gtx/transform.hpp>

Camera::Camera()
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewProjectionMatrix() const
{
	glm::mat4 view = GetViewMatrix(); // identity, you need to change this
	glm::mat4 projection = GetProjectionMatrix();
	glm::mat4 viewProjection = projection * view;

	// @TODO 1 - Calculate View Projection Matrix
	//           The projection matrix is hardcoded below
	//           The view matrix is set in the derived camera classes.

	//glm::mat4 viewProjection(1.0f);
	return viewProjection;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
}

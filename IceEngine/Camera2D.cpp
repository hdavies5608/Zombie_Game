#include "Camera2D.h"

namespace IceEngine
{
	Camera2D::Camera2D() : 
		_position(0.0f, 0.0f),
		_cameraMatrix(1.0f),
		_orthoMatrix(1.0f),
		_scale(1.0f),
		_needsMatrixUpdate(true),
		_screenWidth(500),
		_screenHeight(500)
	{
	}


	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight) 
	{
        _screenWidth = screenWidth;
        _screenHeight = screenHeight;
        _orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
    }

	void Camera2D::update()
	{
		if (_needsMatrixUpdate)
		{

			//Camera Translation
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			//Camera Scale
			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			_needsMatrixUpdate = false;
		}
	}

	glm::vec2 Camera2D::convertScreenCoordsToWorldCoords(glm::vec2 screenCoords)
	{
		screenCoords.y = _screenHeight - screenCoords.y;
		// Make it so that 0 is the center
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
		// Scale the coordinates
		screenCoords /= _scale;
		// Translate with the camera position
		screenCoords += _position;
		return screenCoords;
	}

    bool Camera2D::isBoxInView(const glm::vec2 &position, const glm::vec2 &dimensions)
    {
        glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / _scale; 

        // The minimum distance before a collision occurs
        const float MIN_DISTANCE_X = dimensions.x * 0.5f + scaledScreenDimensions.x * 0.5f;
        const float MIN_DISTANCE_Y = dimensions.y * 0.5f + scaledScreenDimensions.y * 0.5f;

        glm::vec2 centerPos = position + dimensions * 0.5f;
        glm::vec2 centerCameraPos = _position;

        // Vector from the input to the camera
        glm::vec2 distVec = centerPos - centerCameraPos;

        // Get the depth of the collision
        float xDepth = MIN_DISTANCE_X - abs(distVec.x);
        float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

        // If either the depths are > 0, then we collided
        if (xDepth > 0 && yDepth > 0) {
            // There was a collision
            return true;
        }
        return false;
    }
}
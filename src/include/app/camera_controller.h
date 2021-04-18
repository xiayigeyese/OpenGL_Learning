#pragma once

#include "camera.h"
#include "input.h"

class CameraController
{
public:
	enum class MoveDirection
	{
		FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN
	};
	
	CameraController()
		:m_camera(nullptr),
		m_input(nullptr),
		m_moveSpeed(2.5f),
		m_mouseSensitivity(0.1f),
		m_wheelScrollKeyId(0),
		m_mouseMoveKeyId(0)
	{}

	CameraController(const CameraController&) = delete;

	CameraController(CameraController&& controller) noexcept
		:m_camera(controller.m_camera),
	     m_input(controller.m_input),
		 m_moveSpeed(controller.m_moveSpeed),
	     m_mouseSensitivity(controller.m_mouseSensitivity),
		 m_wheelScrollKeyId(controller.m_wheelScrollKeyId),
		 m_mouseMoveKeyId(controller.m_mouseMoveKeyId)
	{
		controller.m_camera = nullptr;
		controller.m_input = nullptr;

		// update the callback in Input
		auto mouseMoveCallback = [this](const float xOffset, const float yOffset)->void {return this->mouseMove(xOffset, yOffset); };
		m_input->updateMouseMoveEventHandler(m_mouseMoveKeyId, mouseMoveCallback);
		auto wheelCallback = [this](const float offset)->void {return this->wheelScroll(offset); };
		m_input->updateWheelScrollEventHandler(m_wheelScrollKeyId, wheelCallback);
	}

	CameraController& operator=(const CameraController&) = delete;

	CameraController& operator=(CameraController&& controller) noexcept
	{
		m_camera = controller.m_camera;
		m_input = controller.m_input;
		controller.m_camera = nullptr;
		controller.m_input = nullptr;
		m_moveSpeed = controller.m_moveSpeed;
		m_mouseSensitivity = controller.m_mouseSensitivity;

		// update the callback in Input
		m_mouseMoveKeyId = controller.m_mouseMoveKeyId;
		auto mouseMoveCallback = [this](const float xOffset, const float yOffset)->void {return this->mouseMove(xOffset, yOffset); };
		m_input->updateMouseMoveEventHandler(m_mouseMoveKeyId, mouseMoveCallback);

		m_wheelScrollKeyId = controller.m_wheelScrollKeyId;
		auto wheelCallback = [this](const float offset)->void {return this->wheelScroll(offset); };
		m_input->updateWheelScrollEventHandler(m_wheelScrollKeyId, wheelCallback);
		
		return *this;
	}

	// use for normal
	explicit CameraController(Camera& camera, Input& input)
		: m_camera(&camera),
		  m_input(&input),
		  m_moveSpeed(2.5f),
		  m_mouseSensitivity(0.1f),
		  m_wheelScrollKeyId(0),
		  m_mouseMoveKeyId(0)
	{
		registerWheelScrollCallback();
		registerMouseMoveCallback();
	}

	~CameraController()
	{
		m_camera = nullptr;
		cancelMouseMoveCallback();
		cancelWheelScrollCallback();
		m_input = nullptr;
	}

	void setMoveSpeed(const float moveSpeed)
	{
		m_moveSpeed = moveSpeed;
	}

	void setMouseSensitivity(const float mouseSensitivity)
	{
		m_mouseSensitivity = mouseSensitivity;
	}
	
	void keyBoardMove(const MoveDirection direction, const float duringTime) const
	{
		const float distance = m_moveSpeed * duringTime;
		if (direction == MoveDirection::FORWARD)
		{
			m_camera->moveFront(distance);
		}
		else if (direction == MoveDirection::BACKWARD)
		{
			m_camera->moveBackward(distance);
		}
		else if (direction == MoveDirection::LEFT)
		{
			m_camera->moveLeft(distance);
		}
		else if (direction == MoveDirection::RIGHT)
		{
			m_camera->moveRight(distance);
		}
		else if (direction == MoveDirection::UP)
		{
			m_camera->moveUp(distance);
		}
		else
		{
			m_camera->moveDown(distance);
		}
		m_camera->updateViewAttribute();
	}
	
	void mouseMove(const float xOffset, const float yOffset) const
	{
		float yawDistance = xOffset * m_mouseSensitivity;
		float pitchDistance = yOffset * m_mouseSensitivity;
		m_camera->turnHorizontal(yawDistance);
		m_camera->turnVertical(pitchDistance);
		m_camera->updateViewAttribute();
	}

	void wheelScroll(const float zoomOffset) const
	{
		m_camera->updateZoom(zoomOffset);
	}

	[[nodiscard]] float getMoveSpeed() const
	{
		return m_moveSpeed;
	}

	[[nodiscard]] float getMouseSensitivity() const
	{
		return m_mouseSensitivity;
	}

	// mouse wheel scroll
	
	void registerWheelScrollCallback()
	{
		auto wheelCallback = [this](const float offset)->void {return this->wheelScroll(offset); };
		m_wheelScrollKeyId = m_input->addWheelScrollEventHandler(wheelCallback);
	}

	void cancelWheelScrollCallback()
	{
		if (m_wheelScrollKeyId != 0)
		{
			m_input->deleteWheelScrollEventHandler(m_wheelScrollKeyId);
			m_wheelScrollKeyId = 0;
		}
	}

	// mouse movement
	
	void registerMouseMoveCallback()
	{
		auto mouseMoveCallback = [this](const float xOffset, const float yOffset)->void {return this->mouseMove(xOffset, yOffset); };
		m_mouseMoveKeyId = m_input->addMouseMoveEventHandler(mouseMoveCallback);
	}

	void cancelMouseMoveCallback()
	{
		if (m_mouseMoveKeyId != 0)
		{
			m_input->deleteMouseMoveEventHandler(m_mouseMoveKeyId);
			m_mouseMoveKeyId = 0;
		}
	}
	
private:
	Camera* m_camera;
	Input* m_input;
	float m_moveSpeed, m_mouseSensitivity;
	unsigned int m_wheelScrollKeyId, m_mouseMoveKeyId;
};
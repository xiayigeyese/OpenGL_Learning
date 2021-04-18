#pragma once

#include <functional>
#include <map>

class Input
{
public:
	Input():m_firstMouse(true),m_zoomOffset(0.0f),
			m_lastPosX(0.0f), m_lastPosY(0.0f),m_xOffset(0.0f), m_yOffset(0.0f),
			m_lastFrameTime(0.0f)
	{
		m_mouseMoveHandlers = {};
		m_wheelScrollHandlers = {};
		m_mouseMoveKeyId = 1;
		m_wheelScrollKeyId = 1;
	}

	// mouse Movement

	void updateCursorPos(const float posX, const float posY)
	{
		if (m_firstMouse)
		{
			m_lastPosX = posX;
			m_lastPosY = posY;
			m_firstMouse = false;
		}
		m_xOffset = posX - m_lastPosX;
		m_yOffset = posY - m_lastPosY;
		m_lastPosX = posX;
		m_lastPosY = posY;

		for (auto& f : m_mouseMoveHandlers)
		{
			f.second(m_xOffset, m_yOffset);
		}
	}
	
	unsigned int addMouseMoveEventHandler(std::function<void(float, float)> func)
	{
		m_mouseMoveHandlers[m_mouseMoveKeyId++] = func;
		return m_mouseMoveKeyId - 1;
	}

	void updateMouseMoveEventHandler(const unsigned int keyId, std::function<void(float, float)> func)
	{
		m_mouseMoveHandlers[keyId] = func;
	}

	void deleteMouseMoveEventHandler(const unsigned int keyId)
	{
		m_mouseMoveHandlers.erase(keyId);
	}

	// mouse wheel scroll

	void updateZoomOffset(const float zoomOffset)
	{
		m_zoomOffset = zoomOffset;

		for (auto& f : m_wheelScrollHandlers)
		{
			f.second(m_zoomOffset);
		}
	}
	
	unsigned int addWheelScrollEventHandler(std::function<void(float)> func)
	{
		m_wheelScrollHandlers[m_wheelScrollKeyId++] = func;
		return m_wheelScrollKeyId - 1;
	}

	void updateWheelScrollEventHandler(const unsigned int keyId,  std::function<void(float)> func)
	{
		m_wheelScrollHandlers[keyId] = func;
	}

	void deleteWheelScrollEventHandler(const unsigned int keyId)
	{
		m_wheelScrollHandlers.erase(keyId);
	}

	bool m_firstMouse;
	float m_zoomOffset;
	float m_lastPosX, m_lastPosY, m_xOffset, m_yOffset;
	float m_lastFrameTime;

	// use map for de_attach handler, vector is worse
	int m_mouseMoveKeyId, m_wheelScrollKeyId;
	std::map<unsigned int, std::function<void(float, float)>> m_mouseMoveHandlers;
	std::map<unsigned int, std::function<void(float)>> m_wheelScrollHandlers;
};
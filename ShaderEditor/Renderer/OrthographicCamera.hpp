#pragma once

#include "glm/glm.hpp"

namespace Renderer {
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left = -1.0, float right = 1.0, float bottom = -1.0, float top = 1.0);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPostion(const glm::vec3& position) { 
			m_Position = position;
			RecalculateViewMatrix();
		}

		float GetRotation() { return m_Rotation; }
		void SetRotation(float rotation) { 
			m_Rotation = rotation; 
			RecalculateViewMatrix();
		}
        
        void SetCamera(float left, float right, float bottom, float top);

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};
}

#pragma once
#pragma once
#include <GL/freeglut.h>
#include <cmath>
#define PI 3.141592
template<typename Vector3>
float Length(Vector3&& vec) { return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z); }

template<typename Vector3>
Vector3 Normalize(Vector3&& vec)
{
	float fLength = Length(vec);
	if (fLength == 0) return Vector3{ 0, 0, 0 };
	return Vector3{ vec.x / fLength, vec.y / fLength, vec.z / fLength };
}

template<typename Vector3>
class Camera
{
	Vector3 m_at{ 0.f, 0.f, 0.f };
	Vector3 m_up{ 0.f, 1.f, 0.f };

	float m_distance{ 0 };

	float m_vertical{ float(PI*0.5f) };
	float m_horizontal{ 0 };

	float m_aspect{ 0 };
	float m_near{ 0.f };
	float m_far{ 1.f };
	float m_fovy{ 0 };

public:
	Camera()
		// 카메라 생성 시 최초 1회는 aspect를 자동으로 설정한다.
		: m_aspect{ glutGet(GLUT_WINDOW_WIDTH) / static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)) }
	{
	}
	Camera(float aspect)
		// 카메라 생성 시 최초 1회는 aspect를 자동으로 설정한다.
		: m_aspect{ aspect }
	{
	}

	~Camera() = default;

	void Initialize(const Vector3 & pos, float distance, float zNear, float zFar, float fov)
	{
		m_fovy = fov;
		m_near = zNear;
		m_far = zFar;
		m_distance = distance;
		m_at = pos;
	}

	void SetAspect(float aspect) { m_aspect = aspect; }
	Vector3 GetPosition()	const { return m_at; }

	void SetPosition(const Vector3& at) { m_at = at; LookAt(); }
	//void SetPosition(Vector3&& at)	noexcept { m_at = std::move(at); LookAt(); }
	void Move(const Vector3& v)	noexcept { SetPosition(m_at + v); }
	//void Move(Vector3&& v)		noexcept { SetPosition(m_at + std::move(v)); }

	void SetDistance(const float& d) { m_distance = fmax(d, m_near); }
	float GetDistance() const { return m_distance; }

	void zoom(float per) { m_distance = m_distance * per; }


	void Rotate(float v, float h)
	{
		m_vertical += v;
		if (-PI * 0.5f < m_horizontal + h && m_horizontal + h <0)
			m_horizontal += h;

	}

	void Initialize_radian()
	{
		m_vertical = PI * 0.5f;
		m_horizontal = 0;
	}

	void LookAt() const
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		Vector3 vEye{ eye() };

		gluPerspective(m_fovy, m_aspect, m_near, m_far);

		gluLookAt(
			vEye.x, vEye.y, vEye.z,		//카메라의 위치
			m_at.x, m_at.y, m_at.z,	//(촛점)바라보는곳
			m_up.x, m_up.y, m_up.z		//업벡터
		);
		glMatrixMode(GL_MODELVIEW);
	}

	void LookAtOrtho() const
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		Vector3 vEye{ eye() };

		//gluPerspective(m_fovy, m_aspect, m_near, m_far);
		glOrtho(-500, 500, -500, 500, 5, 6000);

		gluLookAt(
			vEye.x, vEye.y, vEye.z,		//카메라의 위치
			m_at.x, m_at.y, m_at.z,	//바라보는곳
			m_up.x, m_up.y, m_up.z		//업벡터
		);

		glMatrixMode(GL_MODELVIEW);
	}

	Vector3 GetLookVector() const
	{
		return Normalize(Vector3
			{
				cos(m_horizontal) * cos(m_vertical),
				sin(m_horizontal),
				cos(m_horizontal) * sin(m_vertical)
			});
	}

	Vector3 eye() const { return GetPosition() - GetLookVector() * m_distance; }
};

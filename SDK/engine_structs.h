#pragma once
#include "../SDK.h"

/**
 * Name: PlayStation2 - PCSX2
 * Version: 0.0.1
 * Author: NightFyre
*/

#pragma pack(push, 0x01)
namespace PlayStation2
{
	struct Vector2
	{
		float x, y;
		Vector2() : x(0), y(0) {}
		Vector2(float x, float y) : x(x), y(y) {}


		// Addition operator
		Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }

		// Subtraction operator
		Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }

		// Scalar multiplication operator
		Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }

		//	Scalar division operator
		Vector2 operator/(float scalar) const { return Vector2{ x / scalar, y / scalar }; }

		//	float* return operator
		Vector2 operator=(float* other) const { return Vector2{ other[0], other[1] }; }

		// Dot product operator
		float operator*(const Vector2& other) const { return x * other.x + y * other.y; }

		//	
		Vector2& operator*=(const Vector2& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		// Scalar multiplication and assignment operator
		Vector2 operator*=(float scalar) {
			x *= scalar;
			y *= scalar;
			return *this;
		}

		// Length (magnitude) of the vector
		float length() const { return std::sqrt(x * x + y * y); }

		// Normalize the vector
		void normalize()
		{
			float len = length();
			if (len > 0) {
				x /= len;
				y /= len;
			}
		}
	};

	struct Vector3
	{
		float x, y, z;
		Vector3() : x(0), y(0), z(0) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

		// Addition operator
		Vector3 operator+(const Vector3& other) const { return Vector3{ x + other.x, y + other.y, z + other.z }; }

		// Subtraction operator
		Vector3 operator-(const Vector3& other) const { return Vector3{ x - other.x, y - other.y, z - other.z }; }

		// Scalar multiplication operator
		Vector3 operator*(float scalar) const { return Vector3{ x * scalar, y * scalar, z * scalar }; }

		//	Scalar division operator
		Vector3 operator/(float scalar) const { return Vector3{ x / scalar, y / scalar, z / scalar }; }

		// Dot product operator
		float operator*(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }

		// Cross product operator
		Vector3 operator^(const Vector3& other) const { return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x); }

		//	
		Vector3 operator=(float* other) const { return Vector3{ other[0], other[1], other[2] }; }

		//	
		Vector3& operator*=(const Vector3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		// Length (magnitude) of the vector
		float length() const { return std::sqrt(x * x + y * y + z * z); }

		// Normalize the vector
		void normalize()
		{
			float len = length();
			if (len > 0) {
				x /= len;
				y /= len;
				z /= len;
			}
		}
	};

	struct Vector4
	{
		float x, y, z, w;
		Vector4() : x(0), y(0), z(0), w(w) {}
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

		// Addition operator
		Vector4 operator+(const Vector4& other) const { return Vector4(x + other.x, y + other.y, z + other.z, w + other.w); }

		// Subtraction operator
		Vector4 operator-(const Vector4& other) const { return Vector4(x - other.x, y - other.y, z - other.z, w - other.w); }

		// Scalar multiplication operator
		Vector4 operator*(float scalar) const { return Vector4(x * scalar, y * scalar, z * scalar, w * scalar); }

		// Scalar division operator
		Vector4 operator/(float scalar) const { return Vector4{ x / scalar, y / scalar, z / scalar , w / scalar }; }

		// Dot product operator
		float operator*(const Vector4& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }

		//	
		Vector4 operator=(float* other) const { return Vector4{ other[0], other[1], other[2] , other[3] }; }

		//	
		Vector4& operator*=(const Vector4& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}

		// Length (magnitude) of the vector
		float length() const { return std::sqrt(x * x + y * y + z * z + w * w); }

		// Normalize the vector
		void normalize()
		{
			float len = length();
			if (len > 0) {
				x /= len;
				y /= len;
				z /= len;
				w /= len;
			}
		}
	};

	struct Matrix2x3
	{
		Vector2 TopRow, MiddleRow, BottomRow;

		// Matrix addition operator
		Matrix2x3 operator+(const Matrix2x3& other) const { return Matrix2x3{ TopRow + other.TopRow, MiddleRow + other.MiddleRow, BottomRow + other.BottomRow }; }

		// Matrix subtraction operator
		Matrix2x3 operator-(const Matrix2x3& other) const { return Matrix2x3{ TopRow - other.TopRow, MiddleRow - other.MiddleRow, BottomRow - other.BottomRow }; }

		// Matrix-scalar multiplication operator
		Matrix2x3 operator*(float scalar) const { return Matrix2x3{ TopRow * scalar, MiddleRow * scalar, BottomRow * scalar }; }

		bool isZeroMatrix(const Matrix2x3& matrix) {
			if (matrix.TopRow.x == 0 && matrix.TopRow.y == 0 &&
				matrix.MiddleRow.x == 0 && matrix.MiddleRow.y == 0 &&
				matrix.BottomRow.x == 0 && matrix.BottomRow.y == 0) {
				return true;
			}
			return false;
		}
	};

	struct Matrix3x3
	{
		Vector3 TopRow, MiddleRow, BottomRow;

		// Matrix addition operator
		Matrix3x3 operator+(const Matrix3x3& other) const { return Matrix3x3{ TopRow + other.TopRow, MiddleRow + other.MiddleRow, BottomRow + other.BottomRow }; }

		// Matrix subtraction operator
		Matrix3x3 operator-(const Matrix3x3& other) const { return Matrix3x3{ TopRow - other.TopRow, MiddleRow - other.MiddleRow, BottomRow - other.BottomRow }; }

		// Matrix-scalar multiplication operator
		Matrix3x3 operator*(float scalar) const { return Matrix3x3{ TopRow * scalar, MiddleRow * scalar, BottomRow * scalar }; }

		bool isZeroMatrix(const Matrix3x3& matrix) {
			if (matrix.TopRow.x == 0 && matrix.TopRow.y == 0 && matrix.TopRow.z == 0 &&
				matrix.MiddleRow.x == 0 && matrix.MiddleRow.y == 0 && matrix.MiddleRow.z == 0 &&
				matrix.BottomRow.x == 0 && matrix.BottomRow.y == 0 && matrix.BottomRow.z == 0) {
				return true;
			}
			return false;
		}
	};

	struct Matrix4x3
	{
		Vector4 TopRow, MiddleRow, BottomRow;

		// Matrix addition operator
		Matrix4x3 operator+(const Matrix4x3& other) const { return Matrix4x3{ TopRow + other.TopRow, MiddleRow + other.MiddleRow, BottomRow + other.BottomRow }; }

		// Matrix subtraction operator
		Matrix4x3 operator-(const Matrix4x3& other) const { return Matrix4x3{ TopRow - other.TopRow, MiddleRow - other.MiddleRow, BottomRow - other.BottomRow }; }

		// Matrix-scalar multiplication operator
		Matrix4x3 operator*(float scalar) const { return Matrix4x3{ TopRow * scalar, MiddleRow * scalar, BottomRow * scalar }; }

		bool isZeroMatrix(const Matrix4x3& matrix) {
			if (matrix.TopRow.x == 0 && matrix.TopRow.y == 0 && matrix.TopRow.z == 0 && matrix.TopRow.w == 0 &&
				matrix.MiddleRow.x == 0 && matrix.MiddleRow.y == 0 && matrix.MiddleRow.z == 0 && matrix.MiddleRow.w == 0 &&
				matrix.BottomRow.x == 0 && matrix.BottomRow.y == 0 && matrix.BottomRow.z == 0 && matrix.BottomRow.w == 0) {
				return true;
			}
			return false;
		}
	};

	struct ProcessInfo
	{
		DWORD		m_ModulePID{};
		HANDLE		m_ModuleHandle{};
		uintptr_t	m_ModuleBase{};

		HWND		m_GameWindow{};
		LPCSTR		m_WindowTitle;
		LPCSTR		m_ClassName;
		LPCSTR		m_ModulePath;
		int			m_WindowWidth;
		int			m_WindowHeight;
		Vector2		m_WindowSize{};

		void Initialize()
		{
			// Get initial process info
			m_ModulePID		= GetCurrentProcessId();
			m_ModuleHandle	= GetModuleHandle(NULL);
			m_ModuleBase	= (uintptr_t)GetModuleHandle(NULL);

			// Get the path of the module
			char szModulePath[MAX_PATH];
			if (GetModuleFileNameA((HMODULE)m_ModuleHandle, szModulePath, MAX_PATH) == 0)
				CloseHandle(m_ModuleHandle);

			// Get the main window of the current process
			HWND hWnd = GetForegroundWindow();
			if (hWnd == NULL)
				CloseHandle(m_ModuleHandle);

			// Get the window title and class name of the main window
			char szWindowTitle[MAX_PATH];
			char szClassName[MAX_PATH];
			GetWindowTextA(hWnd, szWindowTitle, MAX_PATH);
			GetClassNameA(hWnd, szClassName, MAX_PATH);

			// Get the size of the main window
			RECT rect;
			GetWindowRect(hWnd, &rect);
			int nWidth = rect.right - rect.left;
			int nHeight = rect.bottom - rect.top;

			// Set the fields of the ProcessInfo struct
			m_GameWindow	= hWnd;
			m_WindowTitle	= szWindowTitle;
			m_ClassName		= szClassName;
			m_ModulePath	= szModulePath;
			m_WindowWidth	= nWidth;
			m_WindowHeight	= nHeight;
			m_WindowSize	= Vector2((float)nWidth, (float)nHeight);
		}
	};

    struct PCSX2
    {

        uintptr_t hk_OnLeftDClick = NULL;
        uintptr_t hk_ResetEE = NULL;

        void __cdecl recResetEE(uintptr_t Address)
        {
            typedef void(__cdecl* pFunctionAddress)();
            pFunctionAddress pResetEE = (pFunctionAddress)((Address));
            pResetEE();
        }

    };
}
#pragma pack(pop)
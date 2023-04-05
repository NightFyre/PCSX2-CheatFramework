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

		Vector2 operator+(const Vector2& other) const
		{
			return Vector2{ x + other.x, y + other.y };
		}

		Vector2 operator-(const Vector2& other) const
		{
			return Vector2{ x - other.x, y - other.y };
		}

		Vector2 operator*(float scalar) const
		{
			return Vector2{ x * scalar, y * scalar };
		}

		Vector2 operator/(float scalar) const
		{
			return Vector2{ x / scalar, y / scalar };
		}

		Vector2 operator=(float* other) const
		{
			return Vector2{ other[0], other[1] };
		}

		Vector2& operator*=(const Vector2& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}
    };

    struct Vector3
    {
        float x, y, z;

		Vector3 operator+(const Vector3& other) const
		{
			return Vector3{ x + other.x, y + other.y, z + other.z };
		}

		Vector3 operator-(const Vector3& other) const
		{
			return Vector3{ x - other.x, y - other.y, z - other.z };
		}

		Vector3 operator*(float scalar) const
		{
			return Vector3{ x * scalar, y * scalar, z * scalar };
		}

		Vector3 operator/(float scalar) const
		{
			return Vector3{ x / scalar, y / scalar, z / scalar };
		}

		Vector3 operator=(float* other) const
		{
			return Vector3{ other[0], other[1], other[2] };
		}

		Vector3& operator*=(const Vector3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
    };

    struct Vector4
    {
        float x, y, z, w;

		Vector4 operator+(const Vector4& other) const
		{
			return Vector4{ x + other.x, y + other.y, z + other.z , w + other.w};
		}

		Vector4 operator-(const Vector4& other) const
		{
			return Vector4{ x - other.x, y - other.y, z - other.z , w - other.w };
		}

		Vector4 operator*(float scalar) const
		{
			return Vector4{ x * scalar, y * scalar, z * scalar , w * scalar };
		}

		Vector4 operator/(float scalar) const
		{
			return Vector4{ x / scalar, y / scalar, z / scalar , w / scalar };
		}

		Vector4 operator=(float* other) const
		{
			return Vector4{ other[0], other[1], other[2] , other[3] };
		}

		Vector4& operator*=(const Vector4& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
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

    struct EMU
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
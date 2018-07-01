// MIT License
// 
// Copyright(c) 2018 Jang daemyung
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "platform/window.h"

#include <cassert>
#include <cstdlib>

#include "platform/display.h"
#include "platform/pixmap.h"

namespace plt
{

namespace
{

#ifdef PLATFORM_WIN32


LRESULT CALLBACK wnd_proc (HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	const auto windowPtr = reinterpret_cast<Window*>(GetWindowLongPtr(wnd, GWLP_USERDATA));

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);

	case WM_PAINT:
		windowPtr->render_signal().emit();
		return 0;

	case WM_SIZE:
		windowPtr->resize_signal().emit();
		return 0;

	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE)
		{
			const auto result = PostMessage(wnd, WM_CLOSE, 0, 0);
			assert(result != 0);
		}
		return 0;

	default:
		return DefWindowProc(wnd, msg, wparam, lparam);
	}

	return DefWindowProc(wnd, msg, wparam, lparam);
}

LRESULT CALLBACK pre_wnd_proc (HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_CREATE)
	{
		auto init_params = reinterpret_cast<CREATESTRUCT*>(lparam);
		SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(init_params->lpCreateParams));
		return 0;
	}
	else if (msg == WM_SIZE)
	{
		SetWindowLongPtr(wnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wnd_proc));
		return 0;
	}

	return DefWindowProc(wnd, msg, wparam, lparam);
}

std::string encode_to_utf8 (const std::wstring& source)
{
	auto output = std::string { "{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}" };

	WideCharToMultiByte(CP_UTF8, 0, source.c_str(), -1, output.data(), static_cast<int>(output.size()), NULL, NULL);
	
	return output;
}

std::string generate_class_name ()
{
	auto guid = GUID { };

	const auto result = CoCreateGuid(&guid);
	assert
	(
		S_OK == result && "Fail to create GUID!!!"
	);

	auto class_name = std::wstring 
	{
		L"{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX} "
	};

	const auto written_char_count = StringFromGUID2
	(
		guid, class_name.data(), static_cast<int>(class_name.size())
	);
	assert
	(
		written_char_count == class_name.size() && "Fail to convert from GUID to wstring!!!"
	);

	return encode_to_utf8(class_name);
}

#endif

}

Window::~Window ()
{

#ifdef PLATFORM_WIN32

	ReleaseDC(wnd_, dc_);
	DestroyWindow(wnd_);
	UnregisterClass(class_name_.c_str(), utl::Singleton<Display>::get().get_instance());

#endif

}

void Window::receivce_msgs ()
{
	ShowWindow(wnd_, SW_SHOW);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::draw (Pixmap const& pixmap)
{

#ifdef PLATFORM_WIN32

	SelectObject(pixmap.get_dc(), pixmap.get_bitmap());
	BitBlt(dc_, 0, 0, pixmap.get_size().width, pixmap.get_size().height, pixmap.get_dc(), 0, 0, SRCCOPY);

#endif

}

Window::Window (const std::string& title, const utl::Vec2<uint32_t>& size)
	:
	title_        (title                ),
	size_         (size                 ),
	render_signal_(                     ),
	resize_signal_(                     ),

#ifdef PLATFORM_WIN32

	class_name_   (generate_class_name()),
	atom_         (NULL                 ),
	wnd_          (NULL                 ),
	dc_           (NULL                 )

#endif

{

#ifdef PLATFORM_WIN32

	const auto wnd_class_info = WNDCLASSEX
	{
		sizeof(WNDCLASSEX),                            // cbSize
		CS_HREDRAW | CS_VREDRAW,				       // style
		pre_wnd_proc,								       // lpfnWndProc
		0,										       // cbClsExtra
		0,										       // cbWndExtra
		utl::Singleton<Display>::get().get_instance(),  // hInstance
		NULL,									       // hIcon
		NULL,									       // hCursor
		NULL,									       // hbrBackground
		NULL,									       // lpszMenuName
		class_name_.c_str(),						       // lpszClassName
		NULL									       // hIconSm
	};
	atom_ = RegisterClassEx(&wnd_class_info);
	assert
	(
		NULL != atom_ && "Fail to register WNDCLASSEX!!!"
	);

	auto wnd_rect = RECT
	{
		0, 0,
		static_cast<LONG>(size_.width),
		static_cast<LONG>(size_.height)
	};

	const auto success = AdjustWindowRectEx(&wnd_rect, WS_OVERLAPPEDWINDOW, FALSE, 0);
	assert
	(
		success == TRUE && "Fail to adjust the window rect!!!"
	);

	wnd_ = CreateWindowEx
	(
		0,										       // dwExStyle
		class_name_.c_str(),						   // lpClassName
		title_.c_str(),							       // lpWindowName
		WS_OVERLAPPEDWINDOW,					       // dwStyle
		CW_USEDEFAULT,                                 // X
		CW_USEDEFAULT,			                       // Y
		wnd_rect.right - wnd_rect.left,			       // nWidth
		wnd_rect.bottom - wnd_rect.top,			       // nHeight
		NULL,									       // hWndParent
		NULL,									       // hMenu
		utl::Singleton<Display>::get().get_instance(), // hInstance
		this                                           // lpParam
	);
	assert
	(
		NULL != wnd_ && "Fail to create the window!!!"
	);

	dc_ = GetDC(wnd_);

#endif

}

}
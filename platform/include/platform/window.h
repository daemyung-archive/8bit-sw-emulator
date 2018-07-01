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

#ifndef PLT_WINDOW_H
#define PLT_WINDOW_H

#ifdef PLATFORM_WIN32

#include <windows.h>

#endif

#include <string>
#include <functional>
#include <util/vec.hpp>
#include <util/signal.hpp>
#include <util/singleton-factory.hpp>

namespace plt
{

class Pixmap;

class Window
{
	friend class utl::SingletonFactory<Window>;

	using MsgSignalType = utl::Signal<void (void)>;

public:
	~Window();

	void receivce_msgs();

	void draw (Pixmap const& pixmap);

	inline auto& render_signal() noexcept
	{
		return render_signal_;
	}

	inline auto& resize_signal() noexcept
	{
		return resize_signal_;
	}

	inline const std::string& get_title() const noexcept
	{
		return title_;
	}

	inline const utl::Vec2<uint32_t>& get_size() const noexcept
	{
		return size_;
	}

#ifdef PLATFORM_WIN32

	inline auto get_wnd() const noexcept
	{
		return wnd_;
	}

	inline auto get_dc() const noexcept
	{
		return dc_;
	}

#endif

private:
	Window(const std::string& title, const utl::Vec2<uint32_t>& size);

	std::string            title_;
	utl::Vec2<uint32_t>    size_;
	MsgSignalType          render_signal_;
	MsgSignalType          resize_signal_;

#ifdef PLATFORM_WIN32

	std::string            class_name_;
	ATOM                   atom_;
	HWND                   wnd_;
	HDC                    dc_;

#endif

};

}

#endif
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

#include <string>
#include <functional>
#include <random>
#include <util/singleton.hpp>
#include <util/singleton-factory.hpp>
#include <platform/display.h>
#include <platform/window.h>

class App
{
public:
	App ()
		:
		title_("Platform Demo"),
		size_ { 512, 512      }
	{
		utl::SingletonFactory<plt::Display>::create();
		utl::SingletonFactory<plt::Window>::create(title_, size_);

		utl::Singleton<plt::Window>::get().render_signal().connect(std::bind(&App::render_callback, this));
	}

	~App ()
	{
		utl::Singleton<plt::Window>::get().render_signal().disconnect(std::bind(&App::render_callback, this));

		utl::SingletonFactory<plt::Window>::destroy();
		utl::SingletonFactory<plt::Display>::destroy();
	}

	void run ()
	{
		utl::Singleton<plt::Window>::get().receivce_msgs();
	}

private:
	void render_callback()
	{

#ifdef PLATFORM_WIN32

		HDC dc = GetDC(utl::Singleton<plt::Window>::get().get_wnd());

		std::random_device random_generator;
		
		for (int i = 0; i < 512; ++i)
		{
			const auto pos = utl::Vec2<uint32_t>
			{
				random_generator() % size_.width,
				random_generator() % size_.height
			};

			const auto r = random_generator() % 256;
			const auto g = random_generator() % 256;
			const auto b = random_generator() % 256;

			SetPixel(dc, pos.x, pos.y, RGB(r, g, b));
		}

#endif

	}

private:
	std::string         title_;
	utl::Vec2<uint32_t> size_;

};

int main (int argc, char* argv[])
{
	App().run();

	return 0;
}
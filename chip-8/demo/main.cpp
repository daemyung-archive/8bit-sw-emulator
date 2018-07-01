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

#include <cstdint>
#include <array>
#include <string>
#include <stb/stb_image.h>
#include <stb/stb_image_resize.h>
#include <util/singleton.hpp>
#include <util/singleton-factory.hpp>
#include <platform/display.h>
#include <platform/window.h>
#include <platform/pixmap.h>
#include <chip-8/chip.h>

class Emulator
{
public:
	Emulator ()
		:
		title_     ("Emulator Demo"),
		size_      { 320, 160      },
		pixmap_ptr_(nullptr        ),
		chip_      (               )
	{
		utl::SingletonFactory<plt::Display>::create();
		utl::SingletonFactory<plt::Window>::create(title_, size_);

		pixmap_ptr_ = std::make_unique<plt::Pixmap>(size_);
		
		utl::Singleton<plt::Window>::get().render_signal().connect(std::bind(&Emulator::render_callback, this));
	}

	~Emulator ()
	{
		utl::Singleton<plt::Window>::get().render_signal().disconnect(std::bind(&Emulator::render_callback, this));

		pixmap_ptr_ = nullptr;

		utl::SingletonFactory<plt::Window>::destroy();
		utl::SingletonFactory<plt::Display>::destroy();
	}

	void run ()
	{
		utl::Singleton<plt::Window>::get().receivce_msgs();
	}

private:
	void render_callback ()
	{
		chip_.instruction_cycle();

		auto temp = std::array<utl::Vec3<uint8_t>, chip8::kScreenHeight * chip8::kScreenWidth>();
		for (auto y = 0; y != chip8::kScreenHeight; ++y)
		{
			for (auto x = 0; x != chip8::kScreenWidth; ++x)
			{
				const auto idx   = y * chip8::kScreenWidth + x;
				const auto color = (chip_.get_pixel(idx) == 0) ? 0 : 255;

				temp[idx].r = temp[idx].g = temp[idx].b = color;
			}
		}

#ifdef PLATFORM_WIN32

		auto pixmap = plt::Pixmap(size_);

		stbir_resize_uint8(reinterpret_cast<uint8_t*>(temp.data()),
			               chip8::kScreenWidth,
			               chip8::kScreenHeight,
			               chip8::kScreenWidth * STBI_rgb,
			               reinterpret_cast<uint8_t*>(pixmap.get_dib_ptr()),
			               size_.width,
			               size_.height,
			               size_.width * STBI_rgb,
			               STBI_rgb);

		utl::Singleton<plt::Window>::get().draw(pixmap);

#endif

	}

private:
	std::string                  title_;
	utl::Vec2<uint32_t>          size_;
	std::unique_ptr<plt::Pixmap> pixmap_ptr_;
	chip8::Chip                  chip_;
};

int main(int argc, char* argv[])
{
	Emulator().run();

	return 0;
}
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

#include <random>
#include <iostream>
#include <functional>
#include <util/vec.hpp>
#include <util/signal.hpp>
#include <util/singleton.hpp>
#include <util/singleton-factory.hpp>

class Notifier
{
public:
	Notifier ()
	{
	}

	~Notifier ()
	{
	}

	auto& signal ()
	{
		return signal_;
	}

	void notify ()
	{
		signal_.emit(std::random_device()());
	}

private:
	utl::Signal<void (uint64_t)> signal_;

};

class Receiver
{
public:
	Receiver ()
	{
	}

	~Receiver ()
	{
	}

	void signal_callback(uint64_t hash)
	{
		std::cout << hash << std::endl;
	}
};

int main (int argc, char* argv[])
{
	Notifier notifier;
	Receiver receiver;

	notifier.signal().connect(std::bind(&Receiver::signal_callback, receiver, std::placeholders::_1));

	for (auto i = 0; i != 10; ++i)
	{
		notifier.notify();
	}

	notifier.signal().disconnect(std::bind(&Receiver::signal_callback, receiver, std::placeholders::_1));

	return 0;
}
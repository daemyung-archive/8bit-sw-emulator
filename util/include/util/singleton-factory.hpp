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

#ifndef UTL_SINGLETON_FACTORY_H
#define UTL_SINGLETON_FACTORY_H

#include <cassert>

#include "noncopyable.h"
#include "nonmovable.h"
#include "singleton.hpp"

namespace utl
{

template <typename Class>
class SingletonFactory : private Noncopyable, private Nonmovable
{
public:
	static void create ()
	{
		assert
		(
			nullptr == Singleton<Class>::instance_ptr_ && "Singleton is already created!!!"
		);

		Singleton<Class>::instance_ptr_.reset(new Class());
	}

	template <typename... Args>
	static void create (Args&&... args)
	{
		assert
		(
			nullptr == Singleton<Class>::instance_ptr_ && "Singleton is already created!!!"
		);

		Singleton<Class>::instance_ptr_.reset(new Class(std::forward<Args>(args)...));
	}

	static void destroy ()
	{
		assert
		(
			nullptr != Singleton<Class>::instance_ptr_ && "Singleton is already destroyed!!!"
		);

		Singleton<Class>::instance_ptr_.reset(nullptr);
	}
};

}

#endif
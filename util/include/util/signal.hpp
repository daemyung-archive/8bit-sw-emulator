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

#ifndef UTL_SIGNAL_H
#define UTL_SIGNAL_H

#include <functional>
#include <set>
#include <algorithm>

namespace utl
{

template <typename Signature>
class Signal
{
public:
	template <typename Callback>
	void connect (const Callback& callback)
	{
		callbacks_.insert(callback);
	}

	template <typename Callback>
	void disconnect (const Callback& callback)
	{
		callbacks_.erase(callback);
	}

	void emit () noexcept
	{
		for (auto& callback : callbacks_)
		{
			callback();
		}
	}

	template <typename... Args>
	void emit (Args&&... args) noexcept
	{
		for (auto& callback : callbacks_)
		{
			callback(std::forward<Args>(args)...);
		}
	}

private:
	struct FuncCmp
	{
		bool operator() (const std::function<Signature>& lhs, const std::function<Signature>& rhs) const noexcept
		{
			return lhs.target_type().before(rhs.target_type());
		}
	};

	std::set<std::function<Signature>, FuncCmp> callbacks_;
};

}

#endif
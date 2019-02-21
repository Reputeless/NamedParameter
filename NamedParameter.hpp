//----------------------------------------------------------------------------------------
//
//	Named parameter library for C++
//
//	Copyright (C) 2016-2019 Ryo Suzuki <reputeless@gmail.com>
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files(the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions :
//	
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.
//
//----------------------------------------------------------------------------------------

# pragma once
# include <type_traits>
# include <tuple>
# include <functional>
# include <memory>
# include <utility>

namespace siv
{
	template <class Tag, class Type>
	class NamedParameter
	{
	private:

		Type m_value;

	public:

		constexpr NamedParameter()
			: m_value() {}

		explicit constexpr NamedParameter(Type value)
			: m_value(value) {}

		template <class U, class V = Type, std::enable_if_t<std::is_convertible_v<U, V>>* = nullptr>
		constexpr NamedParameter(const NamedParameter<Tag, U>& other)
			: m_value(other.value()) {}

		template <class... Args, class V = Type, std::enable_if_t<std::is_constructible_v<V, Args...>>* = nullptr>
		constexpr NamedParameter(const NamedParameter<Tag, std::tuple<Args...>>& tuple)
			: m_value(std::make_from_tuple<Type>(tuple.value())) {}

		constexpr const Type* operator ->() const
		{
			return std::addressof(m_value);
		}

		[[nodiscard]] constexpr const Type& operator *() const
		{
			return m_value;
		}

		[[nodiscard]] constexpr const Type& value() const
		{
			return m_value;
		}
	};

	template <class Tag, class Type>
	class NamedParameter<Tag, Type&>
	{
	private:

		Type* m_ref;

	public:

		constexpr NamedParameter() noexcept
			: m_ref(nullptr) {}

		constexpr NamedParameter(Type& value) noexcept
			: m_ref(std::addressof(value)) {}

		constexpr Type* operator ->() const
		{
			return m_ref;
		}

		[[nodiscard]] constexpr Type& operator *() const
		{
			return *m_ref;
		}

		[[nodiscard]] constexpr Type& value() const
		{
			return *m_ref;
		}
	};

	template <class Tag>
	struct NamedParameterHelper
	{
		template <class Type>
		using named_argument_type = NamedParameter<Tag, Type>;

		template <class Type>
		constexpr NamedParameter<Tag, std::decay_t<Type>> operator =(Type&& value) const
		{
			return NamedParameter<Tag, std::decay_t<Type>>(std::forward<Type>(value));
		}

		template <class... Args>
		constexpr NamedParameter<Tag, std::tuple<Args...>> operator ()(const Args&... args) const
		{
			return NamedParameter<Tag, std::tuple<Args...>>(std::make_tuple(args...));
		}

		template <class Type>
		constexpr NamedParameter<Tag, Type&> operator =(std::reference_wrapper<Type> value) const
		{
			return NamedParameter<Tag, Type&>(value.get());
		}

		template <class Type>
		constexpr NamedParameter<Tag, Type&> operator ()(std::reference_wrapper<Type> value) const
		{
			return NamedParameter<Tag, Type&>(value.get());
		}
	};
}

# define SIV_MAKE_NAMED_PARAMETER(name) \
constexpr auto name = siv::NamedParameterHelper<struct name##_tag>{};\
template <class Type> using name##_ = siv::NamedParameter<struct name##_tag, Type>

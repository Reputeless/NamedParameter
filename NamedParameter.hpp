//----------------------------------------------------------------------------------------
//
//	Named parameter library for C++
//
//	Copyright (C) 2016 Ryo Suzuki <reputeless@gmail.com>
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

namespace np
{
	namespace detail
	{
		template <class...> using Void_t = void;

		template <class, class = Void_t<>>
		struct HasOverloadedAddressOf : std::false_type {};

		template <class Type>
		struct HasOverloadedAddressOf<Type, Void_t<decltype(std::declval<Type&>().operator&())>> : std::true_type {};

		template <class Type, std::enable_if_t<!HasOverloadedAddressOf<Type>::value>* = nullptr>
		constexpr Type* AddressOf(Type& ref)
		{
			return &ref;
		}

		template <class Type, std::enable_if_t<HasOverloadedAddressOf<Type>::value>* = nullptr>
		Type* AddressOf(Type& ref)
		{
			return std::addressof(ref);
		}

		template <class Type, class Tuple, std::size_t... I>
		constexpr Type MakeFromTuple_impl(Tuple&& t, std::index_sequence<I...>)
		{
			return Type(std::get<I>(std::forward<Tuple>(t))...);
		}

		template <class Type, class Tuple>
		constexpr Type MakeFromTuple(Tuple&& t)
		{
			return detail::MakeFromTuple_impl<Type>(std::forward<Tuple>(t),
				std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
		}

		template <class Type>
		constexpr Type MakeFromTuple(const std::tuple<>&)
		{
			return Type();
		}
	}

	template <class Tag, class Type>
	struct NamedParameter
	{
	private:

		Type m_value;

	public:

		constexpr NamedParameter()
			: m_value() {}

		explicit constexpr NamedParameter(Type value)
			: m_value(value) {}

		template <class U, class V = Type, std::enable_if_t<std::is_convertible<U, V>::value>* = nullptr>
		constexpr NamedParameter(const NamedParameter<Tag, U>& other)
			: m_value(other.value()) {}

		template <class... Args, class V = Type, std::enable_if_t<std::is_constructible<V, Args...>::value>* = nullptr>
		constexpr NamedParameter(const NamedParameter<Tag, std::tuple<Args...>>& tuple)
			: m_value(detail::MakeFromTuple<Type>(tuple.value())) {}

		constexpr const Type* operator-> () const
		{
			return detail::AddressOf(m_value);
		}

		constexpr const Type& operator* () const
		{
			return m_value;
		}

		constexpr const Type& value() const
		{
			return m_value;
		}
	};

	template <class Tag, class Type>
	struct NamedParameter<Tag, Type&>
	{
	private:

		Type* m_ref;

	public:

		constexpr NamedParameter() noexcept
			: m_ref(nullptr) {}

		constexpr NamedParameter(Type& value) noexcept
			: m_ref(detail::AddressOf(value)) {}

		constexpr Type* operator-> () const
		{
			return m_ref;
		}

		constexpr Type& operator* () const
		{
			return *m_ref;
		}

		constexpr Type& value() const
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
		constexpr NamedParameter<Tag, std::decay_t<Type>> operator= (Type&& value) const
		{
			return NamedParameter<Tag, std::decay_t<Type>>(std::forward<Type>(value));
		}

		template <class... Args>
		constexpr NamedParameter<Tag, std::tuple<Args...>> operator() (const Args&... args) const
		{
			return NamedParameter<Tag, std::tuple<Args...>>(std::make_tuple(args...));
		}

		template <class Type>
		constexpr NamedParameter<Tag, Type&> operator= (std::reference_wrapper<Type> value) const
		{
			return NamedParameter<Tag, Type&>(value.get());
		}

		template <class Type>
		constexpr NamedParameter<Tag, Type&> operator() (std::reference_wrapper<Type> value) const
		{
			return NamedParameter<Tag, Type&>(value.get());
		}
	};
}

# define NP_MAKE_NAMED_PARAMETER(name) \
constexpr auto name = np::NamedParameterHelper<struct name##_tag>{};\
template <class Type> using name##_ = np::NamedParameterHelper<struct name##_tag>::named_argument_type<Type>

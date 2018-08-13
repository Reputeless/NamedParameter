//--------------------------------------------------------------------
//
//	Named parameter library for C++
//
//	Copyright (C) 2016-2018 Ryo Suzuki <reputeless@gmail.com>
//
//	For the license information refer to NamedParameter.hpp.
//
//--------------------------------------------------------------------

# include <iostream>
# include <string>
# include "NamedParameter.hpp"

namespace Arg
{
	SIV_MAKE_NAMED_PARAMETER(x);
	SIV_MAKE_NAMED_PARAMETER(y);
	SIV_MAKE_NAMED_PARAMETER(pos);
	SIV_MAKE_NAMED_PARAMETER(color);
}

struct Vec2
{
	double x, y;
	Vec2() = default;
	constexpr Vec2(double _x, double _y) : x(_x), y(_y) {}
};

struct Color
{
	std::uint32_t r : 8, g : 8, b : 8, a : 8;
	Color() = default;
	constexpr Color(std::uint32_t _r, std::uint32_t _g, std::uint32_t _b, std::uint32_t _a = 255) : r(_r), g(_g), b(_b), a(_a) {}
};

struct Widget
{
	int x = 0;

	constexpr Widget() {}

	explicit constexpr Widget(int _x)
		: x(_x) {}

	Widget(const std::string& s)
		: x(int(s.length()) * 25) {}

	constexpr Widget(int _x, int _y)
		: x(_x + _y) {}
};

void Draw_impl(const Vec2& pos, const Color& color)
{
	std::cout << "-----------\n";
	std::cout << "pos: (" << pos.x << ", " << pos.y << ")\n";
	std::cout << "color: (" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")\n";
}

void Draw(Arg::x_<double> x, Arg::y_<double> y, Arg::color_<Color> color)
{
	Draw_impl(Vec2(x.value(), y.value()), color.value());
}

void Draw(Arg::color_<Color> color, Arg::x_<double> x, Arg::y_<double> y)
{
	Draw_impl(Vec2(x.value(), y.value()), color.value());
}

void Draw(Arg::pos_<Vec2> pos, Arg::color_<Color> color)
{
	Draw_impl(pos.value(), color.value());
}

void Test1(Arg::x_<int> n)
{
	std::cout << n.value() << '\n';
}

void Test2(Arg::x_<int&> n)
{
	std::cout << n.value() << '\n';
	++n.value();
}

void Test3(Arg::x_<const int&> n)
{
	std::cout << n.value() << '\n';
}

void Test4(Arg::x_<Widget> w)
{
	std::cout << w->x << '\n';
}

void Test5(Arg::x_<Widget&> w)
{
	std::cout << w->x << '\n';
	++w->x;
}

void Test6(Arg::x_<const Widget&> w)
{
	std::cout << w->x << '\n';
}

int main()
{
	//
	// example 1
	//
	const Color black(0, 0, 0);
	Draw(Arg::x = 0, Arg::y = 0, Arg::color = black);
	Draw(Arg::x = 11, Arg::y = 111, Arg::color(1, 10, 11));
	Draw(Arg::color(2, 20, 22), Arg::x = 22, Arg::y = 222);
	Draw(Arg::color(3, 30, 33), Arg::x(33), Arg::y(333));
	Draw(Arg::pos = Vec2(4, 44), Arg::color = Color(4, 40, 44));
	Draw(Arg::pos(5, 55), Arg::color(5, 50, 55));
	
	//
	// example 2
	//
	int n = 100;
	const int cn = 100;
	unsigned char un = 100;

	std::cout << "Test1 =\n";
	Test1(Arg::x = 100);
	Test1(Arg::x = n);
	Test1(Arg::x = cn);
	Test1(Arg::x = un);
	Test1(Arg::x = std::ref(n));
	Test1(Arg::x = std::ref(cn));
	Test1(Arg::x = std::ref(un));
	Test1(Arg::x = std::cref(n));
	Test1(Arg::x = std::cref(cn));
	Test1(Arg::x = std::cref(un));

	std::cout << "Test1 ()\n";
	Test1(Arg::x()); // expected: 0
	Test1(Arg::x(100));
	Test1(Arg::x(n));
	Test1(Arg::x(cn));
	Test1(Arg::x(un));
	Test1(Arg::x(std::ref(n)));
	Test1(Arg::x(std::ref(cn)));
	Test1(Arg::x(std::ref(un)));
	Test1(Arg::x(std::cref(n)));
	Test1(Arg::x(std::cref(cn)));
	Test1(Arg::x(std::cref(un)));

	std::cout << "Test2 =\n";
	int m1 = 100;
	Test2(Arg::x = std::ref(m1)); std::cout << m1 << '\n'; // expected: 101

	std::cout << "Test2 ()\n";
	int m2 = 100;
	Test2(Arg::x = std::ref(m2)); std::cout << m2 << '\n'; // expected: 101

	std::cout << "Test3 =\n";
	Test3(Arg::x = std::ref(cn));
	Test3(Arg::x = std::cref(n));
	Test3(Arg::x = std::cref(cn));

	std::cout << "Test3 ()\n";
	Test3(Arg::x(std::ref(cn)));
	Test3(Arg::x(std::cref(n)));
	Test3(Arg::x(std::cref(cn)));

	//
	// example 3
	//
	Widget w(100);
	constexpr Widget cw(100);

	std::cout << "Test4 =\n";
	Test4(Arg::x = w);
	Test4(Arg::x = cw);
	Test4(Arg::x = Widget(100));
	Test4(Arg::x = std::string("abcd"));
	Test4(Arg::x = std::ref(w));
	Test4(Arg::x = std::ref(cw));
	Test4(Arg::x = std::cref(w));
	Test4(Arg::x = std::cref(cw));

	std::cout << "Test4 ()\n";
	Test4(Arg::x()); // expected: 0
	Test4(Arg::x(w));
	Test4(Arg::x(cw));
	Test4(Arg::x(100));
	Test4(Arg::x(Widget(100)));
	Test4(Arg::x(std::string("abcd")));
	Test4(Arg::x(std::ref(w)));
	Test4(Arg::x(std::ref(cw)));
	Test4(Arg::x(std::cref(w)));
	Test4(Arg::x(std::cref(cw)));

	std::cout << "Test5 =\n";
	Widget z1(100);
	Test5(Arg::x = std::ref(z1)); std::cout << z1.x << '\n'; // expected: 101

	std::cout << "Test5 ()\n";
	Widget z2(100);
	Test5(Arg::x(std::ref(z2))); std::cout << z2.x << '\n'; // expected: 101

	std::cout << "Test6 =\n";
	Test6(Arg::x = std::ref(cw));
	Test6(Arg::x = std::cref(w));
	Test6(Arg::x = std::cref(cw));

	std::cout << "Test6 ()\n";
	Test6(Arg::x(std::ref(cw)));
	Test6(Arg::x(std::cref(w)));
	Test6(Arg::x(std::cref(cw)));
}

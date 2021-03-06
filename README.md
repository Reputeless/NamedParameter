# siv::NamedParameter
**siv::NamedParameter** is a simple named parameter interface for modern C++.

## Examples
This prints `month: 12 day: 31` to stdout:
```cpp
namespace Arg
{
	SIV_MAKE_NAMED_PARAMETER(month);
	SIV_MAKE_NAMED_PARAMETER(day);
}

void Date_impl(int month, int day)
{
	std::cout << "month: " << month << " day: " << day << '\n';
}

void Date(Arg::month_<int> m, Arg::day_<int> d)
{
	Date_impl(m.value(), d.value());
}

void Date(Arg::day_<int> d, Arg::month_<int> m)
{
	Date_impl(m.value(), d.value());
}

int main()
{
	Date(Arg::month = 12, Arg::day = 31);
	Date(Arg::day = 31, Arg::month = 12);
}
```

Arguments can also be accessed by `operator*` and `operator->`.
```cpp
namespace Arg
{
	SIV_MAKE_NAMED_PARAMETER(name);
}

void Print(Arg::name_<std::string> name)
{
	std::cout << "name:" << *name << '\n';
	std::cout << "length:" << name->length() << '\n';
}

int main()
{
	Print(Arg::name = "John");
}
```

`operator()` can construct the object from the arguments to the constructor of the element.
```cpp
namespace Arg
{
	SIV_MAKE_NAMED_PARAMETER(name);
}

void Print(Arg::name_<std::string> name)
{
	std::cout << "name:" << *name << '\n';
	std::cout << "length:" << name->length() << '\n';
}

int main()
{
	Print(Arg::name(5, 'A'));
}
```

Right-most parameters can have default arguments.
```cpp
namespace Arg
{
	SIV_MAKE_NAMED_PARAMETER(name);
}

void Print(Arg::name_<std::string> name = (Arg::name = "anonymous"))
{
	std::cout << "name:" << *name << '\n';
	std::cout << "length:" << name->length() << '\n';
}

int main()
{
	Print();
}
```

## License
NamedParameter is distributed under the MIT license.

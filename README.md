# NamedParameter
**NamedParameter** is a simple named parameter interface for modern C++.

## Examples
This prints `month: 12 day: 31` to stdout:
```cpp
namespace Arg
{
	NP_MAKE_NAMED_PARAMETER(month);
	NP_MAKE_NAMED_PARAMETER(day);
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
	NP_MAKE_NAMED_PARAMETER(name);
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

## License
NamedParameter is distributed under the MIT license.

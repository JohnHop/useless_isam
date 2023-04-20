# Part 1

## The dataset

First thing to do is to select a dataset and, in case, adapt it for our purpose.

After thinking a little about it, I endend up with a banal _world cities_ dataset. I downloaded it somewhere on the net (don't remember) and made some adjustments like columns removal and representation edits.

The _cities.csv_ dataset consists of 140.895 rows separated by newline. Each row consists of 9 columns separated by semicolon. In dept

| **Position** |   **Name**   |      **Type**      | **Min/max/length** | **C++ type** |   **C++ size**  |
|:------------:|:------------:|:------------------:|:------------------:|:------------:|:---------------:|
|       1      |  Geoname ID  |  Positive integer  |   max: 12.514.312  |      int     |        4        |
|       2      |  ASCII Name  |       String       |   max length: 69   |   char[70]   |        70       |
|       3      | Country Code | 2 character string |   fixed length: 2  |    char[2]   |        2        |
|       4      | Country Name |       String       |   max length: 38   |   char[39]   |        39       |
|       5      |  Population  |  Positive integer  |   max: 22.315.474  |      int     |        4        |
|       6      |      DEM     |   Signed integer   |   [-9.999, 5.622]  |      int     |        4        |
|       7      |   Timezone   |       String       |   max length: 30   |   char[31]   |        31       |
|       8      |   Latitude   |       String       |    max length: 8   |    char[9]   |        9        |
|       9      |   Longitude  |       String       |    max length: 8   |    char[9]   |        9        |
|     Total    |              |                    |                    |              | **_172 bytes_** |

Instead of storing a positive integer to a `unsigned` variable, we can safely use a `int` type beacause[^1] `INT_MAX = 2.14.748.364` (on my machine) is less than the max possible value for **Geoname ID** and **Population** attributes.

You may think that the total size of a row is 172 bytes but you can't be sure until you check the output of `sizeof()` operator. Let's use [Compiler Explorer](https://godbolt.org/) (using x86-64 gcc 12.2)

```C++
#include <iostream>

struct Record {
  int id;
  char name[70];
  char country_code[2];
  char country_name[39];
  int population;
  int dem;
  char timezone[31];
  char latitude[9];
  char longitude[9];
};

int main()
{
  std::cout << sizeof(Record) << "\n";
}
```

Output:

```
Program returned: 0
Program stdout
176
```

Why `sizeof(Record)` returns 176 instead of 172?<br>
According to Stroustrup[^2]:

> An object doesn’t just need enough storage to hold its representation. In addition, on some machine architectures, the bytes used to hold it must have proper alignment for the hardware to access it efficiently (or in extreme cases to access it at all). For example, a 4-byte **int** often has to be aligned on a word (4-byte) boundary, and sometimes an 8-byte **double** has to be aligned on a word (8-byte) boundary. Of course, this is all very implementation specific, and for most programmers
completely implicit. You can write good C++ code for decades without needing to be explicit about alignment. Where alignment most often becomes visible is in object layouts: sometimes **struct**s contain ‘‘holes’’ to improve alignment (§8.2.1).

The sum of a `struct` (or `class`) attribute members is't always equal to the in-memory size of due to alignment optimizations that are machine-dependent. So you have to check out the previouse code with your compiler.

Also, even though we are using fundamental types, their size are still machine-dependent[^3]. In fact:

> Some of the aspects of C++’s fundamental types, such as the size of an **int**, are implementation-defined.

> Similarly, the **int** type is supposed to be chosen to be the most suitable for holding and manipulating integers on a given computer; it is typically a 4-byte (32-bit) word. It is unwise to assume more. For example, there are machines with 32-bit **char**s.

This is interesting but also quite problematic. You may experience different sizes. If we want to be strict, we should use `int32_t` instead of `int` (including  the `<cstdint>` header) and `char8_t` instead of `char` (using the `-std=c++20` compiler flag). But this problem will not be addressed now. Since most systems are similar to my machine (a typical laptop with Intel cpu) we will assume (wrongly!) that data types and sizes are those point out in the previous table.

## record.h

Now we can discuss some code. Let's take a look at [record.h](https://github.com/JohnHop/useless_isam/blob/main/record.h)

```C++
struct Record {
  int id;
  char name[70];
  char country_code[2];
  char country_name[39];
  int population;
  int dem;
  char timezone[31];
  char latitude[9];
  char longitude[9];

  Record() = default;
  Record(const Record&);
  Record& operator=(const Record&);
};
```

Surely we need a **copy constructor** and an **assignent operator**. Default copy costructor will copy variables and pointers and this would be really bad. In this example, the assignment operator is almost identical to copy constructor because the **struct** does not holds resource like dynamic memory.

```C++
Record::Record(const Record& copy):
  id{copy.id},
  population{copy.population},
  dem{copy.dem}
{
  strcpy(name, copy.name);
  strncpy(country_code, copy.country_code, 2);
  strcpy(country_name, copy.country_name);
  strcpy(timezone, copy.timezone);
  strcpy(latitude, copy.latitude);
  strcpy(longitude, copy.longitude);
}

Record& Record::operator=(const Record& copy) {
  if(this != &copy) { //Protection againts self-assignment
    id = copy.id;
    strcpy(name, copy.name);
    strncpy(country_code, copy.country_code, 2);
    strcpy(country_name, copy.country_name);
    population = copy.population;
    dem = copy.dem;
    strcpy(timezone, copy.timezone);
    strcpy(latitude, copy.latitude);
    strcpy(longitude, copy.longitude);
  }

  return *this;
}
```

No need to define a custom **default constructor**. The one generated by the compiler is fine. But since we defined a custom copy constructor, the generation of default constructor is suppressed. So we need to get it back using `=default`. Remember that you can possibly initialize the **struct** using the **{}** notation.<br>
A summary about default operations generation rules can be found at the [Marius Bancila's Blog](https://mariusbancila.ro/blog/2018/07/26/cpp-special-member-function-rules/).

[^1]: <https://en.cppreference.com/w/cpp/types/climits>
[^2]: from The C++ Programming Language, 4th edition, §6.2.9
[^3]: from The C++ Programming Language, 4th edition, §6.2.8
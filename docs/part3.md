# Part 3

Records stored in _database.bin_ are not sorted by any column. So, it is just an agglomerate of Records with some padding, grouped by pages. Not very useful for our purpose.

We need to to select a proper column as a primary key, let's say **A**, in order to have an **A**-sorted data file. Next step is to generate an index file so it can be used to access to index entry: it is an ordered pair (k<sub>i</sub>, p<sub>i</sub>) where k<sub>i</sub> is the minimal value inside the page p<sub>i</sub>, witch contains a certain number of Records. Since a Record is 176 byte long, and a page holds 23 Records, the key k<sub>i</sub> of page p<sub>i</sub> belonging to the Record with the A-column value is less than any other inside the same page and the next ones.

You can found the code described in this section in [index-gen.cpp](https://github.com/JohnHop/useless_isam/blob/main/index-gen.cpp).

## Getting, sorting and saving data

We need to

1. Fetch all Records from the _database.bin_ to a vector
2. Sort them by a column
3. Write the sorted vector to _database.bin_

Fortunally, we can take advantage of `std::vector` and `std::sort()` provided by the C++ Standard Library.

### load_data()

The function declaration is

```C++
std::vector<Record> load_data(const std::string& filename)
```

It will set up a `std::vector` with all Records loaded from the _database.bin_ and will returns it. Quite simple.

We need to know how many Records are present inside the database. So we will open the file with the `std::ios_base::ate` flag in order to move the file pointer position at the end of file. Then, the `tellg()` method will returns the file byte size, say **db_size**.

Since `PAGE_SIZE = 4096` and `sizeof_record = sizeof(Record)`

- with `records_per_page = PAGE_SIZE / sizeof_record` provide the number of records in a single page
- with `page_count = db_size / PAGE_SIZE` provide get the number of pages
    - `page_count * records_per_page` provide the number of records in the database
- with `remaining_bytes = db_size % PAGE_SIZE` provide the remaing byte of the partial page
    - `remaining_bytes / sizeof_record` provide the number of records in the last page

So the sum of records is

```C++
unsigned int record_count{ (page_count * records_per_page) + (remaining_bytes / sizeof_record) };
```

We will pass this value to `std::vector` constructor.

### About narrowing conversion

We want to avoid as much as possible implicit conversions that can lead to not only loss of data but also values falsification like assigning a -1 value to a unsigned variable by mistake. Compiler can warn you when and where a narrowing conversion occour and we had to take a closer look in those points. Bjarne debate abount implicit type conversion in The C++ Programming Language, ̀§10.5.

```C++
const std::streamoff db_size{file.tellg()};

if(db_size == -1) {
    std::clog << "Error getting file size of " << filename << "\n";
    exit(EXIT_FAILURE);
}
```

`tellg()` returns a std::streampos, aka long long, so the return value will be stored in a same type variable. After checking if no failure accur, we can assume that is a positive value and go on.

```C++
const unsigned int sizeof_record{sizeof(Record)};
```

Compiler know that sizeof(Record) fits inside an unsigned int so it won't issue any narrowing conversion message.

```C++
const unsigned int records_per_page{ PAGE_SIZE / sizeof_record };
```

Almost the same for this.

```C++
const unsigned int page_count( db_size / PAGE_SIZE );
const unsigned remaining_bytes( db_size % PAGE_SIZE );
```

In order to avoid narrowing conversion messages by compiler, we had to initialize the two variables without the {}-initializer notation. Fortunally, we already checked if `db_size` is positive so we can safety do this.

The remainder of the function is quite self-explanatory.

### Moving semantics

You should have noticed that the function is returning a local object. If you don't know about **moving semantics**, you are probably thinking that the function is returning a vector object by value (a copy). Well, if a Record size is 176 byte, and the database has more than 140 thousand of Records, this would be very problematic... Fortunally, C++11 introduced rvalue references and move semantics which let you _move_ an object outside of a function. So, in brief, the vector is not copied but moved and this is almost costless.

Some resources I found useful about move semantics:

- Bjarne Stroustrup. The C++ Programming Language. §3.3.2, §6.4.1, §7.7.2, §17.1, §17.5.2
- [The Cherno](https://www.youtube.com/@TheCherno) on YouTube
    - lvalues and rvalues in C++: <https://youtu.be/fbYknr-HPYE>
    - Move Semantics in C++: <https://youtu.be/ehMg6zvXuMY>
- fredoverflow on Stack Overflow (the best in my opinion): <https://stackoverflow.com/questions/3106110/what-is-move-semantics>

## Sorting and writing data

We can take advantage of the standard library for sorting data with lambda expression[^1]. In the main() function,

```C++
std::vector<Record> database{load_data(DATABASE_FILENAME)};

std::sort(database.begin(), database.end(),
  [](const Record& a, const Record& b) {
    return a.id < b.id;
  }
);
```

Writing data should not be a problematic.

## Generating index file

Same for the database, we will generate a std::vector of index entries e write it to a file _index.bin_.
To represent a pair of (key, page), I decide to use the std::pair from the standard library. This class is a template, so we can save time putting

```C++
using index_entry_t = std::pair<int, int>;
```

in **params.h**. Setting up the index should not be so hard

```C++
std::vector<index_entry_t> index(num_of_pages);

for(int page = 0; page < num_of_pages; page += 1) {
  index[page].first = database[page*records_per_page].id;
  index[page].second = page;
}
```

We take the first Record of each page and build the index entry vector jumping **records_per_page** == 23 Records each time.

Last, we save the vector to a file like we already did before.

[^1]: Bjarne Stroutrup. The C++ Programming Language, ̀§11.4
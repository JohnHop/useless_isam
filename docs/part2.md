# Part 2

We need to take the csv dataset _cities.csv_ as input and generate a binary file. We should call it _databse.bin_. Next, we will use this one with **index-gen** executable in order to generate an index binary file _index.bin_. For now, it's bettere to save to a file all those filenames. Let's call it _**params.h**_:

```C++
#define DATASET_FILENAME "../cities.csv"
#define DATABASE_FILENAME "../database.bin"
#define INDEX_FILENAME "../index.bin"
```

I am using CMake with Visual Studio Code. In doing so, it will be faster to compile and execute the code during the development.

## Organization of database file

We would read all records from _cities.csv_ and write them to _database.bin_ like a simply collection of **Record**s. But we want to take advantage of **paging** to improve performance. This will introduce just a bit of complexity. First, add to _**params.h**_ the line

```C++
#define PAGE_SIZE 4096
```

This is usually the page size in most systems.<br>
So, if sizeof(Record) = 176 bytes, and PAGE_SIZE is 4096, each paga will contains **PAGE_SIZE / sizeof(Record) = 23** records with **PAGE_SIZE % sizeof(Record) = 48** overhead byte remaining. 

## csv2bin

The source _**csv2bin.cpp**_ simply reads each row from _DATASET_FILENAME_ associated input stream, save it in a **Record** variable and append it to _DATABASE_FILENAME_ associated output stream. Each 23 reads we have to write 48 byte to finish a page so

```C++
const unsigned records_per_page{ PAGE_SIZE / sizeof(Record) };  //23
const unsigned overhead_byte_length{ PAGE_SIZE % sizeof(Record) }; //48 byte
const char overhead_data[overhead_byte_length]{0};

size_t count{0};  //Record reads counter

while(!input_file.eof()) {
  std::getline(input_file, row, '\n'); //extracts a row

  /**
   * Save a row/line to Record variable
  */

  output_file.write(reinterpret_cast<char*>(&record), sizeof(Record));
  count += 1;

  if( (count % records_per_page) == 0) {  //each 23 reads
    output_file.write(overhead_data, overhead_byte_length); //finish a page
  }
} //end while-loop
```

And this is the core idea.

### Extracting attributes from a row

Each column is separate by a `;` delimiter and we are storing an entire row to a std::string class type. So let's take advantage from the C++ standard library using `find()`[^1] and `substr()`[^2] methods.

We could do something like this for **int** type

```C++
std::string attr;

next_pos = row.find(delim, last_pos);
attr = row.substr(last_pos, next_pos-last_pos);
column = strtoul(attr.c_str(), nullptr, 10);
last_pos = next_pos + 1;
```

where _column_ is the attribute extracted and converted to the appropriate type. Same for a null-terminated **char array**

```C++
next_pos = row.find(delim, last_pos);
attr = row.substr(last_pos, next_pos-last_pos);
strcpy(column, attr.c_str());
last_pos = next_pos + 1;
```

Instead of repeating these instructions group 9 times inside the `main()`, we could use functions, overloading and template to improve the code quality, maintainability and readability. First, the two versions only differs for the _extracting_ instruction, so

```C++
template<typename T>
void get(const std::string& row, const std::string delim, size_t& last_pos, size_t& next_pos, T& column) {
  std::string attr;

  next_pos = row.find(delim, last_pos);
  attr = row.substr(last_pos, next_pos-last_pos);
  extract(column, attr);  //<-- save attribute to record
  last_pos = next_pos + 1;
}
```

and then we take advantage of function overloading with

```C++
void extract(int& to, const std::string& from) {
  to = strtoul(from.c_str(), nullptr, 10);
}

void extract(char* to, const std::string& from) {
  strcpy(to, from.c_str());
}
```

### Error handling

It can be archieved by catching exceptions or traditional error handling.<br>
First, we can avoid exceptions for the input stream and be sure to enable them for the output stream

```C++
input_file.exceptions(std::ifstream::goodbit);
output_file.exceptions(std::ofstream::failbit);
```

For example, if a stream fails to open, it will be in the bad state. You can check it with

```C++
if(!input_file || !output_file) {
  std::clog << "Error opening " << DATASET_FILENAME << " or " DATABASE_FILENAME << "\n";
  return EXIT_FAILURE;
}
```

We had to check every time if there are reading errors after calling `getline()` with[^3]

```C++
if(input_file.fail()) {
  std::clog << "Error occurred while reading from " << DATASET_FILENAME << "\n";
  return EXIT_FAILURE;
}
```

Finally, write instructions to the output file are near so we can group them inside a **try** block

```C++
try {
  output_file.write(reinterpret_cast<char*>(&record), sizeof(Record));
  count += 1;

  if( (count % records_per_page) == 0) {
    output_file.write(overhead_data, overhead_byte_length);
  }
} catch(std::ofstream::failure& e) {
  std::clog << "Error occurred while writing on " << DATABASE_FILENAME << ": " << e.what() << "\n";
  return EXIT_FAILURE;
}
```

Anyway, this is not the best way to use exceptions. An exception is handled in order to recover from run-time errors and with the RAII technique[^4].

## Results

Well, not very encouraging.<br>
We obtained a file of 25.091.696 byte aganist the original one of 10.395.430 byte: an increment of +141,37%. A bit too much...<br>
We sacrificed a lot of memory space with access speed. This will be discussed later.

Be careful that the last page is partial: this could be a problem when it's time to read it because the size is less than PAGE_SIZE = 4096 byte.

[^1]: <https://en.cppreference.com/w/cpp/string/basic_string/find>
[^2]: <https://en.cppreference.com/w/cpp/string/basic_string/substr>
[^3]: <https://en.cppreference.com/w/cpp/string/basic_string/getline>
[^4]: Bjarne Stroustrup. The C++ Programming Language, 4th edition, §13.1
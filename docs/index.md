# What is useless_isam ?

I was working on a C++ implementation of [db_tutorial](https://github.com/cstack/db_tutorial) made by [cstack](https://github.com/cstack) as a pet project for my portfolio when I started to get hardy time on the B+Tree implementation section. After learning about B-Tree and B+Tree, I was still struggling with indexing and persistence on files. So I decided to switch to something easier and casually bumped into ISAM indexing.

## Why useless?
Because with this project you can't insert new records. So it's a read only database. Just basic search queries and nothing else.<br>
Also, record columns are hard-coded within the source code. So neither CREAT TABLE statement type for column definition.

Quite useless, don't you agree?

## Well, maybe not so much useless...
I like writing code and I am a C++ lover, so this small and simple project is ideal to put some things I learned until now in practice.<br>
Goal, I hope, is to produce a clean and well-written code and not something like _if it compile and works then it's fine, no matter how it is made_.

Some keywords covered by this project are

> `database` `isam` `indexing` `parsing` `lexer` `paging` `file manipulation` `paging` `caching` `cmake` `tokens` `record` `exception handling`

and I hope there will be more in the future.

## Overvew

ISAM is used to fast retrieve data from a database. So I decided to use a binary file for storing the dataset because it allows random access to records. Also, we need a dataset to populate our database. And don't forget that the database must be sorted in order to create an index file and support range selections. Let's put in order...

1. generate a database binary file from a dataset
2. sort the database binary file by a column
3. generate the index binary file

So I decided to archieve these points with three separate executables.

### csv2bin

The program will use a textual dataset to build a _database.bin_ file consists of a collection of fixed length records. Nothing more.<br>
More precisely, records are grouped in pages and, if needed, some padding will be added to the end of each page.

### index-gen

Then the database file will be sorted by a column and rewritten. Based upont it, the program will build a sorted index binary file where each record will represent all records belonging to a single page.<br>
Actually, only single level indexing is supoorted.

### isam

Finally, this executable will load the index and database files for support search queries and range selections.

## References and useful links
Connor Stack. _db_tutorial_<br>
Bjarne Stroustrup. _The C++ Programming Language (4th edition)_<br>
Grust, Torsten (Summer 2013). [_Tree-Structured Indexing: ISAM and B+-trees_](https://web.archive.org/web/20201031195459/https://db.inf.uni-tuebingen.de/staticfiles/teaching/ss13/db2/db2-04-1up.pdf)<br>
Compiler Explorer: https://godbolt.org/
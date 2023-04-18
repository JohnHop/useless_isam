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

> `databases` `isam` `indexing` `parsing` `lexer` `paging` `file manipulation` `paging` `caching` `cmake` `tokens` `record` `statement` 

and I hope there will be more in the future.
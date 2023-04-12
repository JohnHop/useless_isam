# Useless ISAM

This is a simple pet-project about a minimal and easy-understable ISAM database. 

## csv2bin

Convert the dataset file from a _.csv_ format to binary file according to `PAGE_SIZE` value in `params.h`.

## index-gen

Generate an index file from the binary file generated by **csv2bin** executable. Furthermore, records in the binary file are ordered by _id_ to improve performance when using range selections.

## Usage

Executable parameters are hard-coded in params.h.

First, locate the dataset **cities.csv** and execute **csv2bin** on it in order to obtain a binary file **database.bin**

```bash
$ > ./csv2bin
```

After, execute **index-gen** on **database.bin** in order to obtain the **index.bin** file

```bash
$ > ./index-gen
```

Finally, it is possible to execute **isam** to explore the dataset.<br>
Supported queries are single and range selection:

```sql
isam > SELECT [id]
```

or

```sql
isam > SELECT [start_id] - [end_id]
```

## Examples

```
isam > SELECT 40047
40047, Gomishan, Iran. Islamic Rep. of
```

or

```

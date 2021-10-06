# SimpleDB 

I implementated [SimpleDB](http://www.cs.bc.edu/~sciore/simpledb/) by C++.

## Available Query Types

- Basic data retrieval

```
select c1, c2 from t1, t2 where c3 = c4
```

- Nested data retrieval

```
select c1, c2 from ( select c3 from t1 ), t2 where c3 = c4
```

- Data aggregation

```
select count, c1 from t1 group by c1 order by count
```

- Data insertion

```
insert into t1 ( c1, c2 ) values ( v1, v2 )
```

- Create B-tree index

```
create index indexname on student ( c1 )
```

## Setup

You need a compiler which can compile codes written by C++17.

```
$ sudo apt update && sudo apt install -yq libboost-dev
$ git clone https://github.com/google/googletest.git && mkdir googletest/build && cd googletest/build && sudo cmake .. && sudo make install
```

## Run Tests 

```
$ make test
$ ./test/main_test
```

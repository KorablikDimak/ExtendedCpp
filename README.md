[![](https://img.shields.io/badge/GoogleTest-green)](https://github.com/google/googletest)
[![](https://img.shields.io/badge/GoogleBenchmarks-blue)](https://github.com/google/benchmark)
[![](https://img.shields.io/badge/PugiXML-green)](https://github.com/zeux/pugixml)

# ExtendedCpp
A set of libraries that extend standard C++ functionality. The libraries have no dependencies and is written in pure C++.
Contains the following libraries:

## LINQ
### Description
Ports LINQ functionality from C# to C++.
### Methods currently supported:
- `Select` allows you to convert an object of one type to an object of another type
- `SelectMany` allows you to reduce a set of collections into a single collection
- `Where` to select elements from some set by condition
- `Order` sorts the elements of a collection
- `OrderBy` sorts the elements of a collection with selector
- `Reverse` reverse the collection
- `Except` can get the difference of two sequences
- `Intersect` to get the intersection of sequences
- `Distinct` to remove duplicates in a set
- `Union` to join two sequences
- `Aggregate` performs a general aggregation of the elements of the collection depending on the specified expression
- `Count` to get the number of elements (you may use predicate)
- `Sum` to get the sum of values (you may use selector)
- `Min` to find element with the minimum value (you may use selector)
- `Max` to find element with the maximum value (you may use selector)
- `Average` to find the average value of the collection (you may use selector)
- `First` to get first element of collection (you may use predicate)
- `FirstOrDefault` to get first element of collection, return default if not found (you may use predicate)
- `Last`to get last element of collection (you may use predicate)
- `LastOrDefault`to get last element of collection, return default if not found (you may use predicate)
- `IsEmpty` same as stl method empty()
- `Skip` skips a certain number of elements
- `SkipLast` skips a certain number of elements from the end of the collection
- `SkipWhile` skips a chain of elements, starting with the first element, as long as they satisfy a certain condition
- `Take` retrieves a certain number of elements
- `TakeLast` retrieves a certain number of elements from the end of the collection
- `TakeWhile` selects a chain of elements, starting with the first element, as long as they satisfy a certain condition
- `GroupBy` to group data by certain parameters
- `Join` to merge two different types of sets into one
- `GroupJoin` in addition to joining sequences, it also performs grouping
- `Zip` sequentially concatenates the corresponding elements of the current sequence with the second sequence
- `All` checks if all elements match a condition. If all elements match the condition, then true is returned
- `Any` returns true if at least one element of the collection meets a certain condition
- `Contains` returns true if the collection contains a specific element or subcollection
### STL-like methods currently supported:
`size`, `empty`, `begin`, `end`, `cbegin`, `cend`,`rbegin`, `rend`, `crbegin`, `crend`
### How it works:
The main class to work with is [LinqContainer](https://github.com/KorablikDimak/ExtendedCpp/blob/master/include/LINQ/LinqContainer.h) 
that is obtained by the method `LINQ::From(startcollection)`. By calling methods on this class, we perform the necessary operations. 
Methods are usually passed lambda expressions/std::function.
At the end, you can use the methods: `.ToVector() .ToArray() .ToList()` etc. 
(see supported std containers) that return a container stored in the 
[LinqContainer](https://github.com/KorablikDimak/ExtendedCpp/blob/master/include/LINQ/LinqContainer.h). 
### Examples:
```C++
#include <LINQ/LINQ.h>

std::vector names { "Tom", "Alice", "Bob", "Sam", "Tim", "Tomas", "Bill" };
std::vector selectedNames = LINQ::From(names)
    .Where([](const std::string& name){ return name.size() == 3; })
    .ToVector();
// result: { "Tom", "Bob", "Sam", "Tim" }

std::vector numbers { 1, 5, 9, 2, 0, 145, 2 };
std::vector sortedNumbers = LINQ::From(numbers)
    .OrderBy(OrderType::DESC)
    .ToVector();
// result: { 145, 9, 5, 2, 2, 1, 0 }
```
### Supported std containers:
- `vector`
- `array`
- `list`
- `forward_list`
- `stack`
- `queue`
- `deque`
- `priority_queue`
- `set`
- `unordered_set`
- `map`
- `unordered_map`
### Unit tests
All unit tests are in the directory [test](https://github.com/KorablikDimak/ExtendedCpp/tree/master/tests/LINQ).
### Benchmarks
All benchmarks are in the directory [benchmarks](https://github.com/KorablikDimak/ExtendedCpp/tree/master/benchmarks)


## Events


## Json


## Xml


## InfoLog
### Description
A library for custom creation of logs that supports many built-in tools for obtaining detailed information in the log and their subsequent output to the file.
Is an analog of a [InfoLog](https://github.com/KorablikDimak/InfoLog) but for **C++**.
### Logger configuration
[Example and xml documentation](https://github.com/KorablikDimak/ExtendedCpp/blob/master/LogConfig.xml).
The **.xml** file is a convenient way to set the type of output message and its parameters. Builtins can output the method that raised the message.
### Creating a logger
Basic way to get logger with [example xml](https://github.com/KorablikDimak/ExtendedCpp/blob/master/LogConfig.xml):
```C++
#include <InfoLog/InfoLog.h>

std::string xmlFilePath = "LogConfig.xml";
auto factory = InfoLog::LoggerFactory(xmlFilePath);
auto logger = factory.CreateLogger();
logger->Info("my message!");
```
### Unit tests
All unit tests are in the directory [test](https://github.com/KorablikDimak/ExtendedCpp/tree/master/tests/InfoLog).


## Fast install
```CMake
project(MyExec)
find_package(ExtendedCpp REQUIRED)
add_executable(MyExec main.cpp)
target_link_libraries(MyExec PRIVATE 
        ExtendedCpp::LINQ 
        ExtendedCpp::Events 
        ExtendedCpp::Json 
        ExtendedCpp::Xml 
        ExtendedCpp::InfoLog)
```
Assemblies tested using g++ and MSVC (CLang does not fully support C++20, so you can not build all of this)
Testing and benchmarks also require appropriate libraries.
Build CMake options:
-ExtendedCpp_SHARED_LIBS
-BUILD_ALL_TESTS
-LINQ_BUILD_TESTS
-Events_BUILD_TESTS
-Json_BUILD_TESTS
-InfoLog_BUILD_TESTS
-LINQ_BUILD_BENCHMARKS
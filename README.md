[![](https://img.shields.io/badge/GoogleTest-green)](https://github.com/google/googletest)
[![](https://img.shields.io/badge/GoogleBenchmarks-blue)](https://github.com/google/benchmark)
[![](https://img.shields.io/badge/PugiXML-green)](https://github.com/zeux/pugixml)
[![](https://img.shields.io/badge/nlohmannJSON-black)](https://github.com/nlohmann/json)

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
- `RemoveWhere` to remove elements from some set by condition
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
- `IndexAt`
- `BinarySearch`
- `PushBack`
- `PushFront`
- `Insert`
- `Erase`
### STL-like methods currently supported:
`size`, `empty`, `begin`, `end`, `cbegin`, `cend`,`rbegin`, `rend`, `crbegin`, `crend`
```C++
#include <iostream>
#include <LINQ/LINQ.h>

const std::vector numbers { 8, 7, 1, 9, 50, 6, 3 };
const auto linq = LINQ::From(numbers);
for (const auto& element : linq)
    std::cout << element << std::endl;
```
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
### Generator on c++ coroutines
Alternative use of LINQ methods using coroutines. Less memory consumption, more CPU bound.
All methods described above are supported except:
`First`
`FirstOrDefault`
`Last`
`LastOrDefault`
`IsEmpty`
`SkipLast`
`TakeLast`
`Contains`
`IndexAt`
`BinarySearch`
`PushBack`
`PushFront`
`Insert`
`Erase`
### Examples:
```C++
#include <iostream>
#include <LINQ/LINQ.h>

std::vector names { "Tom", "Alice", "Bob", "Sam", "Tim", "Tomas", "Bill" };
std::vector selectedNames = LINQ::Generator(names)
    .Where([](const std::string& name){ return name.size() == 3; })
    .ToVector();
// result: { "Tom", "Bob", "Sam", "Tim" }

std::vector numbers { 1, 5, 9, 2, 0, 145, 2 };
std::vector sortedNumbers = LINQ::Generator(numbers)
    .OrderBy(OrderType::DESC)
    .ToVector();
// result: { 145, 9, 5, 2, 2, 1, 0 }

const std::vector numbers { 8, 7, 1, 9, 50, 6, 3 };
auto linq = LINQ::Generator(numbers);
for (auto& element : linq)
    std::cout << element << std::endl;
```
### Unit tests
All unit tests are in the directory [tests](https://github.com/KorablikDimak/ExtendedCpp/tree/master/tests/LINQ).
### Benchmarks
All benchmarks are in the directory [benchmarks](https://github.com/KorablikDimak/ExtendedCpp/tree/master/benchmarks/LINQ)

---
## Reflection
### Description
Library for implementing dynamic reflection.
Allows you to access constructors, methods, fields, static methods and fields.
Does not require rewriting existing code. To use it, just add two macros.

`META_DECL` in your .h file and `META_IMPL(NameOfYourType, ...members...)`
### Examples:
```C++
// TestStruct.h
#include <Reflection/Reflection.h>

struct TestStruct
{
    int IntField;
    std::string StringField;
    static double StaticField;
    
    TestStruct();
    TestStruct(const std::string& str);
    
    int MethodInt();
    double MethodOverride(double a, double b);
    double MethodOverride(double a, double b, double c);
    static int StaticMethod(int a);
    
    META_DECL;
};

// TestStruct.cpp
META_IMPL(TestStruct,
          FIELD(IntField),
          FIELD(StringField),
          STATIC_FIELD(StaticField),
          CONSTRUCTOR(),
          CONSTRUCTOR(const std::string&),
          METHOD(MethodInt),
          METHOD(MethodOverride, double, double),
          METHOD(MethodOverride, double, double, double),
          STATIC_METHOD(StaticMethod, int));

// Usage

// Get type info
TypeInfo typeInfo = Reflection::GetType<TestStruct>();
// or
std::optional<TypeInfo> typeInfo = Reflection::GetType("TestStruct");
// or
std::optional<TypeInfo> typeInfo = Reflection::GetType(typeid(TestStruct));

// Call constructors
TestStruct testStruct = std::any_cast<TestStruct>(typeInfo.GetConstructors()[0]->Create());
TestStruct* newTestStruct = std::any_cast<TestStruct*>(typeInfo.GetConstructors()[1]->New(std::string));

// Invoke methods
int result1 = std::any_cast<int>
        (typeInfo.GetMethods("MethodInt")[0]->Invoke(&testStruct));
double result2 = std::any_cast<double>
        (typeInfo.GetMethods("MethodOverride")[0]->Invoke(&testStruct, double, double));
double result3 = std::any_cast<double>
        (typeInfo.GetMethods("MethodOverride")[1]->Invoke(&testStruct, double, double, double));

// Get parameter info
std::vector<std::type_index> parameters = typeInfo.GetMethods("MethodOverride")[0]->Parameters(); // size: 2
std::vector<std::type_index> parameters = typeInfo.GetMethods("MethodOverride")[1]->Parameters(); // size: 3

// Access to fields
int* fieldPtr = std::any_cast<int*>(typeInfo.GetField("IntField")->GetField(&testStruct));
*fieldPtr = 5;
std::cout << testStruct.IntField << std::endl; // output: 5

double* staticFieldPtr = std::any_cast<double*>(typeInfo.GetStaticFields("StaticField")->GetField());
```
### Unit tests
All unit tests are in the directory [tests](https://github.com/KorablikDimak/ExtendedCpp/tree/master/tests/Reflection).

---
## Events
### Description
Library of templates for implementing subscription and unsubscription of events.
### Examples:
```C++
#include <Events/Events.h>

auto event = Events::Event<EventType>(); // EventType should be std::shared_ptr<T>&

const std::function functor1 = [](EventType){};
const std::function functor2 = [](EventType){};

const auto handler1 = Events::CreateFunctionHandler(functor1);
const auto handler2 = FUNCTION_HANDLER(functor2);

event += handler1;
event += handler2;

event.Contains(handler1); // true
event.Contains(handler2); // true

event(); // Invoke all handlers

event -= handler2;
event.Contains(handler1); // true
event.Contains(handler2); // false

event -= handler1;
event.Contains(handler1); // false
event.Contains(handler2); // false
```
### Unit tests
All unit tests are in the directory [tests](https://github.com/KorablikDimak/ExtendedCpp/tree/master/tests/Events).

---
## DI
### Description
Template library for implementing dependency container in compile time.
### Examples:
```C++
#include <DI/DI.h>

DI::ServiceProvider serviceProvider;

// class Service : IService
serviceProvider.AddSingleton<IService1, Service1>();
serviceProvider.AddTransient<IService2, Service2>();
serviceProvider.AddScoped<IService3, Service2>();

// get registered service
std::shared_ptr<IService1> service1 = serviceProvider.GetServiceRequired<IService1>();
std::shared_ptr<IService2> service2 = serviceProvider.GetServiceRequired<IService2>();
std::shared_ptr<IService3> service3 = serviceProvider.GetServiceRequired<IService3>();

// constructor of class Target1
// Target1(const std::shared_ptr<IService1>& service1, const std::shared_ptr<IService2>& service2) {...}
Target1 target1 = DI::Register<Target1(IService1, IService2)>::CreateRequired(serviceProvider);

// constructor of class Target2
// Target2(const std::shared_ptr<IService1>& service1, const std::shared_ptr<IService3>& service3) {...}
Target2 target2 = DI::Register<Target2(IService1, IService3)>::CreateRequired(serviceProvider);
```
### Unit tests
All unit tests are in the directory [tests](https://github.com/KorablikDimak/ExtendedCpp/tree/master/tests/DI).

---
## Json
### Description
See [nlohmann json](https://github.com/nlohmann/json). Added [macros](https://github.com/KorablikDimak/ExtendedCpp/blob/master/include/Json/JsonMacros.h) for more convenient serialization and deserialization.
### Unit tests
All unit tests are in the directory [tests](https://github.com/KorablikDimak/ExtendedCpp/tree/master/tests/Json).

---
## Xml
### Description
See [Pugi xml](https://github.com/zeux/pugixml).

---
## InfoLog
### Description
A library for custom creation of logs that supports many built-in tools for obtaining detailed information in the log and their subsequent output to the file.
Is an analog of a [InfoLog](https://github.com/KorablikDimak/InfoLog) but for **C++**.
### Logger configuration
[Example and xml documentation](https://github.com/KorablikDimak/ExtendedCpp/blob/master/LogConfig.xml).
The **.xml** file is a convenient way to set the type of output message and its parameters. Builtins can output the method that raised the message.
### Creating a logger
Basic way to get logger with [example xml](https://github.com/KorablikDimak/ExtendedCpp/blob/master/LogConfig.xml):
### Examples:
```C++
#include <InfoLog/InfoLog.h>

std::string xmlFilePath = "LogConfig.xml";
auto factory = InfoLog::LoggerFactory(xmlFilePath);
auto logger = factory.CreateLogger();
logger->Info("my message!");
```
### Unit tests
All unit tests are in the directory [tests](https://github.com/KorablikDimak/ExtendedCpp/tree/master/tests/InfoLog).

---
## Other libraries
### Description
Other libraries are collected in the catalog [Common](https://github.com/KorablikDimak/ExtendedCpp/tree/master/include/Common)
They include:
- `Matrix` for working with matrices. Overridden addition, multiplication, etc. operators are supported. There are functions for finding the inverse matrix, matrix rank, and searching for the determinant. 
- `Random` to generate numbers in a given range or to generate strings of a given length with random characters.
- `CancellationTokenSourse` creates thread-safe tokens to gracefully stop the thread from outside.
### Examples:
```C++
#include <Common/Common.h>

Matrix matrix1(2, 3);
matrix1.SetRow({ 1, 2, 3 }, 0);
matrix1.SetRow({ 3, 1, -2 }, 1);

Matrix matrix2(3, 2);
matrix2.SetRow({ 1, 2 }, 0);
matrix2.SetRow({ -3, 1 }, 1);
matrix2.SetRow({ 2, 0 }, 2);

const Matrix matrix3 = matrix1 * matrix2;
std::cout << matrix3 << std::endl;
// output:
//  1   4
// -4   7

Matrix matrix4(3, 3);
matrix4.SetRow({ 1, 2, 3 }, 0);
matrix4.SetRow({ 4, 5, 6 }, 1);
matrix4.SetRow({ 7, 8, 9 }, 2);

const double det = matrix4.Det(); // result: 0

const Matrix matrix5(100, 100, []{ return Common::RandomInt(1, 10); }); // generate random matrix

const std::optional<Matrix> invertedMatrix = matrix5.Inverse(); // inverse matrix
// or
const std::optional<Matrix> invertedMatrix = ~matrix5; // inverse matrix
```
### Unit tests
All unit tests are in the directory [tests](https://github.com/KorablikDimak/ExtendedCpp/tree/master/tests/Common).
### Benchmarks
All benchmarks are in the directory [benchmarks](https://github.com/KorablikDimak/ExtendedCpp/tree/master/benchmarks/Common)

---
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
- ExtendedCpp_SHARED_LIBS
- BUILD_ALL_TESTS
- LINQ_BUILD_TESTS
- Events_BUILD_TESTS
- Json_BUILD_TESTS
- InfoLog_BUILD_TESTS
- LINQ_BUILD_BENCHMARKS

The project can be installed using [my vcpkg fork](https://github.com/KorablikDimak/vcpkg).
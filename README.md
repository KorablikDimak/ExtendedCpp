[![](https://img.shields.io/badge/GoogleTest-green)](https://github.com/google/googletest)
[![](https://img.shields.io/badge/GoogleBenchmarks-blue)](https://github.com/google/benchmark)
[![](https://img.shields.io/badge/PugiXML-green)](https://github.com/zeux/pugixml)
[![](https://img.shields.io/badge/nlohmannJSON-black)](https://github.com/nlohmann/json)

# ExtendedCpp
A set of libraries that extend standard C++ functionality. The libraries have no dependencies and is written in pure C++.
Contains the following libraries:

## Documentation
https://korablikdimak.github.io/ExtendedCpp/

## LINQ
### Description:
Ports LINQ functionality from C# to C++.

### STL-like methods:
`size`, `empty`, `begin`, `end`, `cbegin`, `cend`,`rbegin`, `rend`, `crbegin`, `crend`
```C++
#include <iostream>
#include <ExtendedCpp/LINQ.h>

const std::vector numbers { 8, 7, 1, 9, 50, 6, 3 };
const auto linq = ExtendedCpp::LINQ::From(numbers);
for (const auto& element : linq)
    std::cout << element << std::endl;
```
### How it works:
The main class to work with is [LinqContainer](https://korablikdimak.github.io/ExtendedCpp/classExtendedCpp_1_1LINQ_1_1LinqContainer.html) 
that is obtained by the method `ExtendedCpp::LINQ::From(startcollection)`. By calling methods on this class, we perform the necessary operations. 
Methods are usually passed lambda expressions/std::function.
At the end, you can use the methods: `.ToVector() .ToArray() .ToList()` etc. 
(see supported std containers) that return a container stored in the 
[LinqContainer](https://korablikdimak.github.io/ExtendedCpp/classExtendedCpp_1_1LINQ_1_1LinqContainer.html). 
### Examples:
```C++
#include <ExtendedCpp/LINQ.h>

std::vector names { "Tom", "Alice", "Bob", "Sam", "Tim", "Tomas", "Bill" };
std::vector selectedNames = ExtendedCpp::LINQ::From(names)
    .Where([](const std::string& name){ return name.size() == 3; })
    .ToVector();
// result: { "Tom", "Bob", "Sam", "Tim" }

std::vector numbers { 1, 5, 9, 2, 0, 145, 2 };
std::vector sortedNumbers = ExtendedCpp::LINQ::From(numbers)
    .OrderBy(ExtendedCpp::LINQ::OrderType::DESC)
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
See more [here](https://korablikdimak.github.io/ExtendedCpp/classExtendedCpp_1_1LINQ_1_1LinqGenerator.html).
### Examples:
```C++
#include <iostream>
#include <ExtendedCpp/LINQ.h>

std::vector names { "Tom", "Alice", "Bob", "Sam", "Tim", "Tomas", "Bill" };
std::vector selectedNames = ExtendedCpp::LINQ::Generator(names)
    .Where([](const std::string& name){ return name.size() == 3; })
    .ToVector();
// result: { "Tom", "Bob", "Sam", "Tim" }

std::vector numbers { 1, 5, 9, 2, 0, 145, 2 };
std::vector sortedNumbers = ExtendedCpp::LINQ::Generator(numbers)
    .OrderBy(ExtendedCpp::LINQ::OrderType::DESC)
    .ToVector();
// result: { 145, 9, 5, 2, 2, 1, 0 }

const std::vector numbers { 8, 7, 1, 9, 50, 6, 3 };
auto linq = ExtendedCpp::LINQ::Generator(numbers);
for (auto& element : linq)
    std::cout << element << std::endl;
```
### Views
A partial analogue of std::view. Based on iterators. Non-possessing type offering even more efficient use of resources. See more [here](https://korablikdimak.github.io/ExtendedCpp/classExtendedCpp_1_1LINQ_1_1LinqView.html).
### Examples:
```C++
#include <ExtendedCpp/LINQ.h>

const std::vector names { "Tom", "Alice", "Bob", "Sam", "Tim", "Tomas", "Bill" };

const std::vector selectedNames = ExtendedCpp::LINQ::View(names)
        .RemoveWhere([](const std::string& name){ return name.size() == 3; })
        .ToVector();

// result: Alice Tomas Bill
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
Does not require rewriting existing code. To use it, just add macros `META()` in your .h file. For more information see [documentation](https://korablikdimak.github.io/ExtendedCpp/namespaceExtendedCpp_1_1Reflection.html)
### Examples:
```C++
// TestStruct.h
#include <ExtendedCpp/Reflection.h>

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

    META(TestStruct,
         FIELD(IntField),
         FIELD(StringField),
         STATIC_FIELD(StaticField),
         CONSTRUCTOR(),
         CONSTRUCTOR(std::string),
         METHOD(MethodInt),
         METHOD(MethodOverride, double, double),
         METHOD(MethodOverride, double, double, double),
         STATIC_METHOD(StaticMethod, int));
};

// Usage

// Get type info
ExtendedCpp::Reflection::TypeInfo typeInfo = ExtendedCpp::Reflection::GetType<TestStruct>();
// or
std::vector<ExtendedCpp::Reflection::TypeInfo> typeInfo = ExtendedCpp::Reflection::GetType("TestStruct");
// or
std::optional<ExtendedCpp::Reflection::TypeInfo> typeInfo = ExtendedCpp::Reflection::GetType(typeid(TestStruct));

// Call constructors
TestStruct testStruct = std::any_cast<TestStruct>(typeInfo.GetConstructors()[0]->Create());
// or
TestStruct testStruct = typeInfo.GetConstructors()[0]->Create<TestStruct>();

std::shared_ptr<TestStruct> newTestStruct = std::static_pointer_cast<TestStruct>(typeInfo.GetConstructors()[1]->New(std::string));
// or
std::shared_ptr<TestStruct> newTestStruct = typeInfo.GetConstructors()[1]->New<TestStruct>(std::string);

// Invoke methods
int result1 = std::any_cast<int>
        (typeInfo.GetMethods("MethodInt")[0]->Invoke(&testStruct));
double result2 = std::any_cast<double>
        (typeInfo.GetMethods("MethodOverride")[0]->Invoke(&testStruct, double, double));
double result3 = std::any_cast<double>
        (typeInfo.GetMethods("MethodOverride")[1]->Invoke(&testStruct, double, double, double));
// or
int result1 = typeInfo.GetMethods("MethodInt")[0]->Invoke<int>(&testStruct);
double result2 = typeInfo.GetMethods("MethodOverride")[0]->Invoke<double>(&testStruct, double, double);
double result3 = typeInfo.GetMethods("MethodOverride")[1]->Invoke<double>(&testStruct, double, double, double);

// Get parameter info
std::vector<std::type_index> parameters = typeInfo.GetMethods("MethodOverride")[0]->Parameters(); // size: 2
std::vector<std::type_index> parameters = typeInfo.GetMethods("MethodOverride")[1]->Parameters(); // size: 3

// Access to fields
int* fieldPtr = std::any_cast<int*>(typeInfo.GetField("IntField")->GetField(&testStruct));
// or 
int* fieldPtr = typeInfo.GetField("IntField")->GetField<int>(&testStruct);

*fieldPtr = 5;
std::cout << testStruct.IntField << std::endl; // output: 5

double* staticFieldPtr = std::any_cast<double*>(typeInfo.GetStaticFields("StaticField")->GetField());
// or
double* staticFieldPtr = typeInfo.GetStaticFields("StaticField")->GetField<double>();
```
### Unit tests
All unit tests are in the directory [tests](https://github.com/KorablikDimak/ExtendedCpp/tree/master/tests/Reflection).

---
## Events
### Description
Library of templates for implementing subscription and unsubscription of events. For more information see [documentation](https://korablikdimak.github.io/ExtendedCpp/namespaceExtendedCpp_1_1Events.html)
### Examples:
```C++
#include <ExtendedCpp/Events.h>

auto event = ExtendedCpp::Events::Event<EventType>(); // EventType should be const std::shared_ptr<T>&

const std::function functor1 = [](EventType){};
const std::function functor2 = [](EventType){};

const auto handler1 = ExtendedCpp::Events::CreateFunctionHandler(functor1);
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
Template library for implementing dependency container in compile time. For more information see [documentation](https://korablikdimak.github.io/ExtendedCpp/namespaceExtendedCpp_1_1DI.html)
### Examples:
```C++
#include <ExtendedCpp/DI.h>

ExtendedCpp::DI::ServiceProvider serviceProvider;

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
Target1 target1 = ExtendedCpp::DI::Register<Target1(IService1, IService2)>::CreateRequired(serviceProvider);

// constructor of class Target2
// Target2(const std::shared_ptr<IService1>& service1, const std::shared_ptr<IService3>& service3) {...}
Target2 target2 = ExtendedCpp::DI::Register<Target2(IService1, IService3)>::CreateRequired(serviceProvider);

// or with using Reflection

// In declaration of Target1:
META(Target1,
     CONSTRUCTOR(std::shared_ptr<IService1>, std::shared_ptr<IService2>));

// In declaration of Target2:
META(Target2,
     CONSTRUCTOR(std::shared_ptr<IService1>, std::shared_ptr<IService3>));

Target1 target1 = ExtendedCpp::DI::Injector<Target1>::CreateRequired(serviceProvider);
Target2 target2 = ExtendedCpp::DI::Injector<Target2>::CreateRequired(serviceProvider);
```

To use dependency injection, the target class must have at least one public constructor that does not take arguments, 
or all arguments must be `std::shared_ptr<SERVICE_TYPE>`.
To use the reflection approach, you must also add information about at least one such constructor.

### Unit tests
All unit tests are in the directory [tests](https://github.com/KorablikDimak/ExtendedCpp/tree/master/tests/DI).

---
## Json
### Description
See [nlohmann json](https://github.com/nlohmann/json). Added [macros](https://github.com/KorablikDimak/ExtendedCpp/blob/master/include/ExtendedCpp/Json/JsonMacros.h) for more convenient serialization and deserialization.
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

You can add your own senders with interface class [ISender](https://korablikdimak.github.io/ExtendedCpp/classExtendedCpp_1_1InfoLog_1_1ISender.html) with using my reflection library.
See [TestSender.h](https://github.com/KorablikDimak/ExtendedCpp/tree/master/tests/InfoLog/TestSender.h) and [TestSender.cpp](https://github.com/KorablikDimak/ExtendedCpp/tree/master/tests/InfoLog/TestSender.cpp).
### Examples:
```C++
#include <ExtendedCpp/InfoLog.h>

std::string xmlFilePath = "LogConfig.xml";
auto factory = ExtendedCpp::InfoLog::LoggerFactory(xmlFilePath);
auto logger = factory.CreateLogger();
logger->Info("my message!");
```
### Unit tests
All unit tests are in the directory [tests](https://github.com/KorablikDimak/ExtendedCpp/tree/master/tests/InfoLog).

---
## Other libraries
### Description
Include:
- `Matrix.h` for working with matrices. Overridden addition, multiplication, etc. operators are supported. There are functions for finding the inverse matrix, matrix rank, and searching for the determinant. 
- `Random.h` to generate numbers in a given range or to generate strings of a given length with random characters.
- `Cancellation.h` creates thread-safe tokens to gracefully stop the thread from outside.
### Examples:
```C++
#include <ExtendedCpp/Matrix.h>

ExtendedCpp::Matrix<int> matrix1(2, 3);
matrix1.SetRow({ 1, 2, 3 }, 0);
matrix1.SetRow({ 3, 1, -2 }, 1);

ExtendedCpp::Matrix<int> matrix2(3, 2);
matrix2.SetRow({ 1, 2 }, 0);
matrix2.SetRow({ -3, 1 }, 1);
matrix2.SetRow({ 2, 0 }, 2);

const ExtendedCpp::Matrix<int> matrix3 = matrix1 * matrix2;
std::cout << matrix3 << std::endl;
// output:
//  1   4
// -4   7

ExtendedCpp::Matrix<double> matrix4(3, 3);
matrix4.SetRow({ 1, 2, 3 }, 0);
matrix4.SetRow({ 4, 5, 6 }, 1);
matrix4.SetRow({ 7, 8, 9 }, 2);

const double det = matrix4.Det(); // result: 0

const ExtendedCpp::Matrix<double> matrix5(100, 100, []{ return ExtendedCpp::Random(1, 10); }); // generate random matrix

const std::optional<ExtendedCpp::Matrix<double>> invertedMatrix = matrix5.Inverse(); // inverse matrix
// or
const std::optional<ExtendedCpp::Matrix<double>> invertedMatrix = ~matrix5; // inverse matrix
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
        ExtendedCpp::InfoLog
        ExtendedCpp::Common
        ExtendedCpp::DI
        ExtendedCpp::Reflection)
```
Assemblies tested using `GCC`, `MinGW`, `MSVC`, `Clang`, `Apple Clang` with static analizers and sanitizers on `Windows x64`, `Linux x64`, `Arm macOS`, `Arm Linux`.
Testing and benchmarks also require appropriate libraries.
Build CMake options:
- BUILD_SHARED_LIBS (default true)
- BUILD_ALL_TESTS (default false)
- LINQ_BUILD_TESTS (default false)
- Events_BUILD_TESTS (default false)
- Json_BUILD_TESTS (default false)
- InfoLog_BUILD_TESTS (default false)
- Common_BUILD_TESTS (default false)
- Reflection_BUILD_TESTS (default false)
- LINQ_BUILD_BENCHMARKS (default false)
- Common_BUILD_BENCHMARKS (default false)

The project can be installed using [my vcpkg fork](https://github.com/KorablikDimak/vcpkg).

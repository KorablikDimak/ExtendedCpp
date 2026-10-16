#ifndef LINQ_TESTS_H
#define LINQ_TESTS_H

#include <string>
#include <utility>
#include <vector>

class Person
{
public:
    std::string Name;
    unsigned char Age{};

    Person(std::string name, const unsigned char age) : Name(std::move(name)), Age(age) {}
    Person() = default;
};

class Company
{
public:
    std::string Name;
    std::vector<Person> People;

    explicit Company(std::string name) : Name(std::move(name)) {}
    Company() = default;
};

class Employer
{
public:
    std::string Name;
    std::string CompanyName;
    std::string Language;

    Employer(std::string name, std::string  companyName) :
        Name(std::move(name)), CompanyName(std::move(companyName)) {}
    Employer(std::string name, std::string  companyName, std::string language) :
        Name(std::move(name)), CompanyName(std::move(companyName)), Language(std::move(language)) {}
    Employer() = default;
};

class ItCompany
{
public:
    std::string Name;
    std::string Language;
    std::vector<Employer> People;

    ItCompany(std::string name, std::string language) : Name(std::move(name)), Language(std::move(language)) {}
    ItCompany(std::string  name, const std::vector<Employer>& people) : Name(std::move(name)), People(people) {}
    ItCompany() = default;
};

#endif
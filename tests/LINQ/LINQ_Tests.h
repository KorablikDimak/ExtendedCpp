#ifndef LINQ_Tests_H
#define LINQ_Tests_H

#include <string>
#include <vector>

class Person;

class Company
{
public:
    std::string Name;
    std::vector<Person> People;

    explicit Company(const std::string& name)
    {
        Name = name;
    }

    Company() = default;
};

class Person
{
public:
    std::string Name;
    unsigned char Age{};

    Person(const std::string& name, const unsigned char age)
    {
        Name = name;
        Age = age;
    }

    Person() = default;
};

class Employer
{
public:
    std::string Name;
    std::string CompanyName;
    std::string Language;

    Employer(const std::string& name, const std::string& companyName)
    {
        Name = name;
        CompanyName = companyName;
    }

    Employer(const std::string& name, const std::string& companyName, const std::string& language)
    {
        Name = name;
        CompanyName = companyName;
        Language = language;
    }

    Employer() = default;
};

class ItCompany
{
public:
    std::string Name;
    std::string Language;
    std::vector<Employer> People;

    ItCompany(const std::string& name, const std::string& language)
    {
        Name = name;
        Language = language;
    }

    ItCompany(const std::string& name, const std::vector<Employer>& people)
    {
        Name = name;
        People = people;
    }

    ItCompany() = default;
};

#endif
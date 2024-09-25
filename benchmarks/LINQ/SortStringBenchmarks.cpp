#include <string>

#include <benchmark/benchmark.h>

#include <ExtendedCpp/LINQ/Sort.h>
#include <ExtendedCpp/Random.h>

std::vector<std::string> GenerateStrings(const std::size_t count) noexcept
{
    std::vector<std::string> result(count);

    for (std::size_t i = 0; i < count; ++i)
        result[i] = ExtendedCpp::Random::RandomString(20);

    return result;
}

template<typename ...Args>
void QuickSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector strings = std::get<0>(argsTuple);
    for ([[maybe_unused]] auto _ : state)
        ExtendedCpp::LINQ::Sort::QuickSort(strings.data(), 0, strings.size() - 1);
}
BENCHMARK_CAPTURE(QuickSortBenchmark, stringSize20, GenerateStrings(20));
BENCHMARK_CAPTURE(QuickSortBenchmark, stringSize100, GenerateStrings(100));
BENCHMARK_CAPTURE(QuickSortBenchmark, stringSize1000, GenerateStrings(1000));
BENCHMARK_CAPTURE(QuickSortBenchmark, stringSize10000, GenerateStrings(10000));
BENCHMARK_CAPTURE(QuickSortBenchmark, stringSize100000, GenerateStrings(100000));

template<typename ...Args>
void InsertionSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector strings = std::get<0>(argsTuple);
    for ([[maybe_unused]] auto _ : state)
        ExtendedCpp::LINQ::Sort::InsertionSort(strings.data(), 0, strings.size() - 1);
}
BENCHMARK_CAPTURE(InsertionSortBenchmark, stringSize20, GenerateStrings(20));
BENCHMARK_CAPTURE(InsertionSortBenchmark, stringSize100, GenerateStrings(100));
BENCHMARK_CAPTURE(InsertionSortBenchmark, stringSize1000, GenerateStrings(1000));
BENCHMARK_CAPTURE(InsertionSortBenchmark, stringSize10000, GenerateStrings(10000));

template<typename ...Args>
void CombSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector strings = std::get<0>(argsTuple);
    for ([[maybe_unused]] auto _ : state)
        ExtendedCpp::LINQ::Sort::CombSort(strings.data(), 0, strings.size() - 1);
}
BENCHMARK_CAPTURE(CombSortBenchmark, stringSize20, GenerateStrings(20));
BENCHMARK_CAPTURE(CombSortBenchmark, stringSize100, GenerateStrings(100));
BENCHMARK_CAPTURE(CombSortBenchmark, stringSize1000, GenerateStrings(1000));
BENCHMARK_CAPTURE(CombSortBenchmark, stringSize10000, GenerateStrings(10000));
BENCHMARK_CAPTURE(CombSortBenchmark, stringSize100000, GenerateStrings(100000));

template<typename ...Args>
void MergeSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector strings = std::get<0>(argsTuple);
    for ([[maybe_unused]] auto _ : state)
        ExtendedCpp::LINQ::Sort::MergeSort(strings.data(), 0, strings.size() - 1);
}
BENCHMARK_CAPTURE(MergeSortBenchmark, stringSize20, GenerateStrings(20));
BENCHMARK_CAPTURE(MergeSortBenchmark, stringSize100, GenerateStrings(100));
BENCHMARK_CAPTURE(MergeSortBenchmark, stringSize1000, GenerateStrings(1000));
BENCHMARK_CAPTURE(MergeSortBenchmark, stringSize10000, GenerateStrings(10000));
BENCHMARK_CAPTURE(MergeSortBenchmark, stringSize100000, GenerateStrings(100000));

template<typename ...Args>
void TimSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector strings = std::get<0>(argsTuple);
    for ([[maybe_unused]] auto _ : state)
        ExtendedCpp::LINQ::Sort::TimSort(strings.data(), 0, strings.size() - 1);
}
BENCHMARK_CAPTURE(TimSortBenchmark, stringSize20, GenerateStrings(20));
BENCHMARK_CAPTURE(TimSortBenchmark, stringSize100, GenerateStrings(100));
BENCHMARK_CAPTURE(TimSortBenchmark, stringSize1000, GenerateStrings(1000));
BENCHMARK_CAPTURE(TimSortBenchmark, stringSize10000, GenerateStrings(10000));
BENCHMARK_CAPTURE(TimSortBenchmark, stringSize100000, GenerateStrings(100000));

template<typename ...Args>
void StdSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector strings = std::get<0>(argsTuple);
    for ([[maybe_unused]] auto _ : state)
        std::ranges::sort(strings);
}
BENCHMARK_CAPTURE(StdSortBenchmark, stringSize20, GenerateStrings(20));
BENCHMARK_CAPTURE(StdSortBenchmark, stringSize100, GenerateStrings(100));
BENCHMARK_CAPTURE(StdSortBenchmark, stringSize1000, GenerateStrings(1000));
BENCHMARK_CAPTURE(StdSortBenchmark, stringSize10000, GenerateStrings(10000));
BENCHMARK_CAPTURE(StdSortBenchmark, stringSize100000, GenerateStrings(100000));
#include <ctime>

#include <benchmark/benchmark.h>

#include <LINQ/Sort.h>

std::vector<int> GenerateInts(const std::size_t count) noexcept
{
    std::vector<int> result(count);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (std::size_t i = 0; i < count; ++i)
        result[i] = rand() * rand();

    return std::move(result);
}

template<typename ...Args>
void QuickSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for (auto _ : state)
        LINQ::Sort::QuickSort(numbers.data(), 0, numbers.size() - 1);
}
BENCHMARK_CAPTURE(QuickSortBenchmark, intSize20, GenerateInts(20));
BENCHMARK_CAPTURE(QuickSortBenchmark, intSize100, GenerateInts(100));
BENCHMARK_CAPTURE(QuickSortBenchmark, intSize1000, GenerateInts(1000));
BENCHMARK_CAPTURE(QuickSortBenchmark, intSize10000, GenerateInts(10000));
BENCHMARK_CAPTURE(QuickSortBenchmark, intSize50000, GenerateInts(50000));

template<typename ...Args>
void SelectionSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for (auto _ : state)
        LINQ::Sort::SelectionSort(numbers.data(), 0, numbers.size() - 1);
}
BENCHMARK_CAPTURE(SelectionSortBenchmark, intSize20, GenerateInts(20));
BENCHMARK_CAPTURE(SelectionSortBenchmark, intSize100, GenerateInts(100));
BENCHMARK_CAPTURE(SelectionSortBenchmark, intSize1000, GenerateInts(1000));
BENCHMARK_CAPTURE(SelectionSortBenchmark, intSize10000, GenerateInts(10000));
BENCHMARK_CAPTURE(SelectionSortBenchmark, intSize100000, GenerateInts(100000));

template<typename ...Args>
void InsertionSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for (auto _ : state)
        LINQ::Sort::InsertionSort(numbers.data(), 0, numbers.size() - 1);
}
BENCHMARK_CAPTURE(InsertionSortBenchmark, intSize20, GenerateInts(20));
BENCHMARK_CAPTURE(InsertionSortBenchmark, intSize100, GenerateInts(100));
BENCHMARK_CAPTURE(InsertionSortBenchmark, intSize1000, GenerateInts(1000));
BENCHMARK_CAPTURE(InsertionSortBenchmark, intSize10000, GenerateInts(10000));
BENCHMARK_CAPTURE(InsertionSortBenchmark, intSize100000, GenerateInts(100000));

template<typename ...Args>
void BucketSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for (auto _ : state)
        LINQ::Sort::BucketSort(numbers.data(), 0, numbers.size() - 1);
}
BENCHMARK_CAPTURE(BucketSortBenchmark, intSize20, GenerateInts(20));
BENCHMARK_CAPTURE(BucketSortBenchmark, intSize100, GenerateInts(100));
BENCHMARK_CAPTURE(BucketSortBenchmark, intSize1000, GenerateInts(1000));
BENCHMARK_CAPTURE(BucketSortBenchmark, intSize10000, GenerateInts(10000));
BENCHMARK_CAPTURE(BucketSortBenchmark, intSize100000, GenerateInts(100000));
BENCHMARK_CAPTURE(BucketSortBenchmark, intSize1000000, GenerateInts(1000000));

template<typename ...Args>
void CombSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for (auto _ : state)
        LINQ::Sort::CombSort(numbers.data(), 0, numbers.size() - 1);
}
BENCHMARK_CAPTURE(CombSortBenchmark, intSize20, GenerateInts(20));
BENCHMARK_CAPTURE(CombSortBenchmark, intSize100, GenerateInts(100));
BENCHMARK_CAPTURE(CombSortBenchmark, intSize1000, GenerateInts(1000));
BENCHMARK_CAPTURE(CombSortBenchmark, intSize10000, GenerateInts(10000));
BENCHMARK_CAPTURE(CombSortBenchmark, intSize100000, GenerateInts(100000));
BENCHMARK_CAPTURE(CombSortBenchmark, intSize1000000, GenerateInts(1000000));
BENCHMARK_CAPTURE(CombSortBenchmark, intSize10000000, GenerateInts(10000000));

template<typename ...Args>
void MergeSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for (auto _ : state)
        LINQ::Sort::CombSort(numbers.data(), 0, numbers.size() - 1);
}
BENCHMARK_CAPTURE(MergeSortBenchmark, intSize20, GenerateInts(20));
BENCHMARK_CAPTURE(MergeSortBenchmark, intSize100, GenerateInts(100));
BENCHMARK_CAPTURE(MergeSortBenchmark, intSize1000, GenerateInts(1000));
BENCHMARK_CAPTURE(MergeSortBenchmark, intSize10000, GenerateInts(10000));
BENCHMARK_CAPTURE(MergeSortBenchmark, intSize100000, GenerateInts(100000));
BENCHMARK_CAPTURE(MergeSortBenchmark, intSize1000000, GenerateInts(1000000));
BENCHMARK_CAPTURE(MergeSortBenchmark, intSize10000000, GenerateInts(10000000));

template<typename ...Args>
void TimSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for (auto _ : state)
        LINQ::Sort::TimSort(numbers.data(), 0, numbers.size() - 1);
}
BENCHMARK_CAPTURE(TimSortBenchmark, intSize20, GenerateInts(20));
BENCHMARK_CAPTURE(TimSortBenchmark, intSize100, GenerateInts(100));
BENCHMARK_CAPTURE(TimSortBenchmark, intSize1000, GenerateInts(1000));
BENCHMARK_CAPTURE(TimSortBenchmark, intSize10000, GenerateInts(10000));
BENCHMARK_CAPTURE(TimSortBenchmark, intSize100000, GenerateInts(100000));
BENCHMARK_CAPTURE(TimSortBenchmark, intSize1000000, GenerateInts(1000000));
BENCHMARK_CAPTURE(TimSortBenchmark, intSize10000000, GenerateInts(10000000));

template<typename ...Args>
void StdSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for (auto _ : state)
        std::ranges::sort(numbers);
}
BENCHMARK_CAPTURE(StdSortBenchmark, intSize20, GenerateInts(20));
BENCHMARK_CAPTURE(StdSortBenchmark, intSize100, GenerateInts(100));
BENCHMARK_CAPTURE(StdSortBenchmark, intSize1000, GenerateInts(1000));
BENCHMARK_CAPTURE(StdSortBenchmark, intSize10000, GenerateInts(10000));
BENCHMARK_CAPTURE(StdSortBenchmark, intSize100000, GenerateInts(100000));
BENCHMARK_CAPTURE(StdSortBenchmark, intSize1000000, GenerateInts(1000000));
BENCHMARK_CAPTURE(StdSortBenchmark, intSize10000000, GenerateInts(10000000));
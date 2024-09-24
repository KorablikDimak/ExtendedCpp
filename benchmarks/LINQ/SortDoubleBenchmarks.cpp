#include <benchmark/benchmark.h>

#include <ExtendedCpp/LINQ/Sort.h>
#include <ExtendedCpp/Random.h>

std::vector<double> GenerateDoubles(const std::size_t count) noexcept
{
    std::vector<double> result(count);

    for (std::size_t i = 0; i < count; ++i)
        result[i] = static_cast<double>(ExtendedCpp::Random::RandomInt(0, 100000));

    return std::move(result);
}

template<typename ...Args>
void QuickSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for ([[maybe_unused]] auto _ : state)
        ExtendedCpp::LINQ::Sort::QuickSort(numbers.data(), 0, numbers.size() - 1);
}
BENCHMARK_CAPTURE(QuickSortBenchmark, doubleSize20, GenerateDoubles(20));
BENCHMARK_CAPTURE(QuickSortBenchmark, doubleSize100, GenerateDoubles(100));
BENCHMARK_CAPTURE(QuickSortBenchmark, doubleSize1000, GenerateDoubles(1000));
BENCHMARK_CAPTURE(QuickSortBenchmark, doubleSize10000, GenerateDoubles(10000));
BENCHMARK_CAPTURE(QuickSortBenchmark, doubleSize100000, GenerateDoubles(100000));
BENCHMARK_CAPTURE(QuickSortBenchmark, doubleSize1000000, GenerateDoubles(1000000));

template<typename ...Args>
void SelectionSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for ([[maybe_unused]] auto _ : state)
        ExtendedCpp::LINQ::Sort::SelectionSort(numbers.data(), 0, numbers.size() - 1);
}
BENCHMARK_CAPTURE(SelectionSortBenchmark, doubleSize20, GenerateDoubles(20));
BENCHMARK_CAPTURE(SelectionSortBenchmark, doubleSize100, GenerateDoubles(100));
BENCHMARK_CAPTURE(SelectionSortBenchmark, doubleSize1000, GenerateDoubles(1000));
BENCHMARK_CAPTURE(SelectionSortBenchmark, doubleSize10000, GenerateDoubles(10000));
BENCHMARK_CAPTURE(SelectionSortBenchmark, doubleSize100000, GenerateDoubles(100000));

template<typename ...Args>
void InsertionSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for ([[maybe_unused]] auto _ : state)
        ExtendedCpp::LINQ::Sort::InsertionSort(numbers.data(), 0, numbers.size() - 1);
}
BENCHMARK_CAPTURE(InsertionSortBenchmark, doubleSize20, GenerateDoubles(20));
BENCHMARK_CAPTURE(InsertionSortBenchmark, doubleSize100, GenerateDoubles(100));
BENCHMARK_CAPTURE(InsertionSortBenchmark, doubleSize1000, GenerateDoubles(1000));
BENCHMARK_CAPTURE(InsertionSortBenchmark, doubleSize10000, GenerateDoubles(10000));
BENCHMARK_CAPTURE(InsertionSortBenchmark, doubleSize100000, GenerateDoubles(100000));

template<typename ...Args>
void BucketSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for ([[maybe_unused]] auto _ : state)
        ExtendedCpp::LINQ::Sort::BucketSort(numbers.data(), 0, numbers.size() - 1);
}
BENCHMARK_CAPTURE(BucketSortBenchmark, doubleSize20, GenerateDoubles(20));
BENCHMARK_CAPTURE(BucketSortBenchmark, doubleSize100, GenerateDoubles(100));
BENCHMARK_CAPTURE(BucketSortBenchmark, doubleSize1000, GenerateDoubles(1000));
BENCHMARK_CAPTURE(BucketSortBenchmark, doubleSize10000, GenerateDoubles(10000));
BENCHMARK_CAPTURE(BucketSortBenchmark, doubleSize100000, GenerateDoubles(100000));
BENCHMARK_CAPTURE(BucketSortBenchmark, doubleSize1000000, GenerateDoubles(1000000));

template<typename ...Args>
void CombSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for ([[maybe_unused]] auto _ : state)
        ExtendedCpp::LINQ::Sort::CombSort(numbers.data(), 0, numbers.size() - 1);
}
BENCHMARK_CAPTURE(CombSortBenchmark, doubleSize20, GenerateDoubles(20));
BENCHMARK_CAPTURE(CombSortBenchmark, doubleSize100, GenerateDoubles(100));
BENCHMARK_CAPTURE(CombSortBenchmark, doubleSize1000, GenerateDoubles(1000));
BENCHMARK_CAPTURE(CombSortBenchmark, doubleSize10000, GenerateDoubles(10000));
BENCHMARK_CAPTURE(CombSortBenchmark, doubleSize100000, GenerateDoubles(100000));
BENCHMARK_CAPTURE(CombSortBenchmark, doubleSize1000000, GenerateDoubles(1000000));
BENCHMARK_CAPTURE(CombSortBenchmark, doubleSize10000000, GenerateDoubles(10000000));

template<typename ...Args>
void MergeSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for ([[maybe_unused]] auto _ : state)
        ExtendedCpp::LINQ::Sort::CombSort(numbers.data(), 0, numbers.size() - 1);
}
BENCHMARK_CAPTURE(MergeSortBenchmark, doubleSize20, GenerateDoubles(20));
BENCHMARK_CAPTURE(MergeSortBenchmark, doubleSize100, GenerateDoubles(100));
BENCHMARK_CAPTURE(MergeSortBenchmark, doubleSize1000, GenerateDoubles(1000));
BENCHMARK_CAPTURE(MergeSortBenchmark, doubleSize10000, GenerateDoubles(10000));
BENCHMARK_CAPTURE(MergeSortBenchmark, doubleSize100000, GenerateDoubles(100000));
BENCHMARK_CAPTURE(MergeSortBenchmark, doubleSize1000000, GenerateDoubles(1000000));
BENCHMARK_CAPTURE(MergeSortBenchmark, doubleSize10000000, GenerateDoubles(10000000));

template<typename ...Args>
void TimSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for ([[maybe_unused]] auto _ : state)
        ExtendedCpp::LINQ::Sort::TimSort(numbers.data(), 0, numbers.size() - 1);
}
BENCHMARK_CAPTURE(TimSortBenchmark, doubleSize20, GenerateDoubles(20));
BENCHMARK_CAPTURE(TimSortBenchmark, doubleSize100, GenerateDoubles(100));
BENCHMARK_CAPTURE(TimSortBenchmark, doubleSize1000, GenerateDoubles(1000));
BENCHMARK_CAPTURE(TimSortBenchmark, doubleSize10000, GenerateDoubles(10000));
BENCHMARK_CAPTURE(TimSortBenchmark, doubleSize100000, GenerateDoubles(100000));
BENCHMARK_CAPTURE(TimSortBenchmark, doubleSize1000000, GenerateDoubles(1000000));
BENCHMARK_CAPTURE(TimSortBenchmark, doubleSize10000000, GenerateDoubles(10000000));

template<typename ...Args>
void StdSortBenchmark(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    std::vector numbers = std::get<0>(argsTuple);
    for ([[maybe_unused]] auto _ : state)
        std::ranges::sort(numbers);
}
BENCHMARK_CAPTURE(StdSortBenchmark, doubleSize20, GenerateDoubles(20));
BENCHMARK_CAPTURE(StdSortBenchmark, doubleSize100, GenerateDoubles(100));
BENCHMARK_CAPTURE(StdSortBenchmark, doubleSize1000, GenerateDoubles(1000));
BENCHMARK_CAPTURE(StdSortBenchmark, doubleSize10000, GenerateDoubles(10000));
BENCHMARK_CAPTURE(StdSortBenchmark, doubleSize100000, GenerateDoubles(100000));
BENCHMARK_CAPTURE(StdSortBenchmark, doubleSize1000000, GenerateDoubles(1000000));
BENCHMARK_CAPTURE(StdSortBenchmark, doubleSize10000000, GenerateDoubles(10000000));
#include <ctime>

#include <benchmark/benchmark.h>

#include <Common/Matrix.h>

std::vector<std::vector<double>> GenerateDoubles(const std::size_t count) noexcept
{
    std::vector<std::vector<double>> result(count);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (std::size_t i = 0; i < count; ++i)
        for (std::size_t j = 0; j < count; ++j)
        {
            result[i].resize(count);
            result[i][j] = static_cast<double>(rand() * rand());
        }

    return std::move(result);
}

std::vector<std::vector<std::int32_t>> GenerateInts(const std::size_t count) noexcept
{
    std::vector<std::vector<std::int32_t>> result(count);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (std::size_t i = 0; i < count; ++i)
        for (std::size_t j = 0; j < count; ++j)
        {
            result[i].resize(count);
            result[i][j] = static_cast<std::int32_t>(rand());
        }

    return std::move(result);
}

template<typename ...Args>
void MultiplyBenchmarkDouble(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    const Common::MatrixF64 matrix1(std::move(std::get<0>(argsTuple)));
    const Common::MatrixF64 matrix2(std::move(std::get<1>(argsTuple)));

    for (auto _ : state)
        const Common::MatrixF64 result = (matrix1 * matrix2).value();
}
BENCHMARK_CAPTURE(MultiplyBenchmarkDouble, matrixDoubleSize20, GenerateDoubles(20), GenerateDoubles(20));
BENCHMARK_CAPTURE(MultiplyBenchmarkDouble, matrixDoubleSize100, GenerateDoubles(100), GenerateDoubles(100));
BENCHMARK_CAPTURE(MultiplyBenchmarkDouble, matrixDoubleSize1000, GenerateDoubles(1000), GenerateDoubles(1000));
BENCHMARK_CAPTURE(MultiplyBenchmarkDouble, matrixDoubleSize2000, GenerateDoubles(2000), GenerateDoubles(2000));

template<typename ...Args>
void MultiplyBenchmarkInts(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    const Common::MatrixI32 matrix1(std::move(std::get<0>(argsTuple)));
    const Common::MatrixI32 matrix2(std::move(std::get<1>(argsTuple)));

    for (auto _ : state)
        const Common::MatrixI32 result = (matrix1 * matrix2).value();
}
BENCHMARK_CAPTURE(MultiplyBenchmarkInts, matrixIntsSize20, GenerateInts(20), GenerateInts(20));
BENCHMARK_CAPTURE(MultiplyBenchmarkInts, matrixIntsSize100, GenerateInts(100), GenerateInts(100));
BENCHMARK_CAPTURE(MultiplyBenchmarkInts, matrixIntsSize1000, GenerateInts(1000), GenerateInts(1000));
BENCHMARK_CAPTURE(MultiplyBenchmarkInts, matrixIntsSize2000, GenerateInts(2000), GenerateInts(2000));

template<typename ...Args>
void InverseBenchmarkDouble(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    const Common::MatrixF64 matrix(std::move(std::get<0>(argsTuple)));

    for (auto _ : state)
        const auto result = matrix.Det();
}
BENCHMARK_CAPTURE(InverseBenchmarkDouble, matrixDoubleSize20, GenerateDoubles(10));

template<typename ...Args>
void InverseBenchmarkInts(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    const Common::MatrixI32 matrix(std::move(std::get<0>(argsTuple)));

    for (auto _ : state)
        const auto result = matrix.Det();
}
BENCHMARK_CAPTURE(InverseBenchmarkInts, matrixIntsSize20, GenerateInts(10));
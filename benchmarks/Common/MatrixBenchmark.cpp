#include <benchmark/benchmark.h>

#include <ExtendedCpp/Matrix.h>
#include <ExtendedCpp/Random.h>

ExtendedCpp::MatrixF64 GenerateDoubles(const std::size_t size) noexcept
{
    return { ExtendedCpp::MatrixF64(size, size, []{ return ExtendedCpp::Random::RandomInt(1, 10); }) };
}

ExtendedCpp::MatrixI32 GenerateInts(const std::size_t size) noexcept
{
    return { ExtendedCpp::MatrixI32(size, size, []{ return ExtendedCpp::Random::RandomInt(1, 10); }) };
}

template<typename ...Args>
void MultiplyBenchmarkDouble(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    const ExtendedCpp::MatrixF64 matrix1(std::move(std::get<0>(argsTuple)));
    const ExtendedCpp::MatrixF64 matrix2(std::move(std::get<1>(argsTuple)));

    for ([[maybe_unused]] auto _ : state)
        const ExtendedCpp::MatrixF64 result = (matrix1.Multiply(matrix2, false)).value();
}
BENCHMARK_CAPTURE(MultiplyBenchmarkDouble, matrixDoubleSize20, GenerateDoubles(20), GenerateDoubles(20));
BENCHMARK_CAPTURE(MultiplyBenchmarkDouble, matrixDoubleSize100, GenerateDoubles(100), GenerateDoubles(100));
BENCHMARK_CAPTURE(MultiplyBenchmarkDouble, matrixDoubleSize1000, GenerateDoubles(1000), GenerateDoubles(1000));
BENCHMARK_CAPTURE(MultiplyBenchmarkDouble, matrixDoubleSize2000, GenerateDoubles(2000), GenerateDoubles(2000));

template<typename ...Args>
void MultiplyBenchmarkInts(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    const ExtendedCpp::MatrixI32 matrix1(std::move(std::get<0>(argsTuple)));
    const ExtendedCpp::MatrixI32 matrix2(std::move(std::get<1>(argsTuple)));

    for ([[maybe_unused]] auto _ : state)
        const ExtendedCpp::MatrixI32 result = (matrix1.Multiply(matrix2, false)).value();
}
BENCHMARK_CAPTURE(MultiplyBenchmarkInts, matrixIntsSize20, GenerateInts(20), GenerateInts(20));
BENCHMARK_CAPTURE(MultiplyBenchmarkInts, matrixIntsSize100, GenerateInts(100), GenerateInts(100));
BENCHMARK_CAPTURE(MultiplyBenchmarkInts, matrixIntsSize1000, GenerateInts(1000), GenerateInts(1000));
BENCHMARK_CAPTURE(MultiplyBenchmarkInts, matrixIntsSize2000, GenerateInts(2000), GenerateInts(2000));

template<typename ...Args>
void InverseBenchmarkDouble(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    const ExtendedCpp::MatrixF64 matrix(std::move(std::get<0>(argsTuple)));

    for ([[maybe_unused]] auto _ : state)
        const auto result = matrix.Inverse().value();
}
BENCHMARK_CAPTURE(InverseBenchmarkDouble, matrixDoubleSize20, GenerateDoubles(20));
BENCHMARK_CAPTURE(InverseBenchmarkDouble, matrixDoubleSize100, GenerateDoubles(100));
BENCHMARK_CAPTURE(InverseBenchmarkDouble, matrixDoubleSize1000, GenerateDoubles(1000));
BENCHMARK_CAPTURE(InverseBenchmarkDouble, matrixDoubleSize2000, GenerateDoubles(2000));

template<typename ...Args>
void InverseBenchmarkInts(benchmark::State& state, Args&&... args)
{
    auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
    const ExtendedCpp::MatrixI32 matrix(std::move(std::get<0>(argsTuple)));

    for ([[maybe_unused]] auto _ : state)
        const auto result = matrix.Inverse().value();
}
BENCHMARK_CAPTURE(InverseBenchmarkInts, matrixIntSize20, GenerateInts(20));
BENCHMARK_CAPTURE(InverseBenchmarkInts, matrixIntSize100, GenerateInts(100));
BENCHMARK_CAPTURE(InverseBenchmarkInts, matrixIntSize1000, GenerateInts(1000));
BENCHMARK_CAPTURE(InverseBenchmarkInts, matrixIntSize2000, GenerateInts(2000));
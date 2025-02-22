#include <gtest/gtest.h>

#include <ExtendedCpp/Matrix.h>
#include <ExtendedCpp/Random.h>

typedef ExtendedCpp::MatrixF64 Matrix;

TEST(MatrixTests, SumMatrixTest)
{
    // Average
    Matrix matrix1(3, 3);
    matrix1.SetRow({ 1, 2, 3 }, 0);
    matrix1.SetRow({ 4, 5, 6 }, 1);
    matrix1.SetRow({ 7, 8, 9 }, 2);

    Matrix matrix2(3, 3);
    matrix2.SetRow({ 7, 8, 9 }, 0);
    matrix2.SetRow({ 4, 5, 6 }, 1);
    matrix2.SetRow({ 1, 2, 3 }, 2);

    // Act
    const Matrix matrix3 = matrix1 + matrix2;

    // Assert
    Matrix matrix4(3, 3);
    matrix4.SetRow({ 8, 10, 12 }, 0);
    matrix4.SetRow({ 8, 10, 12 }, 1);
    matrix4.SetRow({ 8, 10, 12 }, 2);

    ASSERT_TRUE(matrix3 == matrix4);
}

TEST(MatrixTests, DetTest)
{
    // Average
    Matrix matrix(3, 3);
    matrix.SetRow({ 1, 2, 3 }, 0);
    matrix.SetRow({ 4, 5, 6 }, 1);
    matrix.SetRow({ 7, 8, 9 }, 2);

    // Act
    const double det = matrix.Det().value();

    // Assert
    ASSERT_TRUE(det < 0.0000001);
}

TEST(MatrixTests, DetBigTest)
{
    // Average
    const Matrix matrix(1000, 1000, []{ return ExtendedCpp::Random::RandomInt(1, 10); });

    // Act
    const auto det = matrix.Det();

    // Assert
    ASSERT_TRUE(det.has_value() && det.value() != 0);
}

TEST(MatrixTests, MultiplyMatrixTest)
{
    // Average
    Matrix matrix1(2, 3);
    matrix1.SetRow({ 1, 2, 3 }, 0);
    matrix1.SetRow({ 3, 1, -2 }, 1);

    Matrix matrix2(3, 2);
    matrix2.SetRow({ 1, 2 }, 0);
    matrix2.SetRow({ -3, 1 }, 1);
    matrix2.SetRow({ 2, 0 }, 2);

    // Act
    const Matrix matrix3 = matrix1 * matrix2;

    // Assert
    Matrix matrix4(2, 2);
    matrix4.SetRow({ 1, 4 }, 0);
    matrix4.SetRow({ -4, 7 }, 1);

    ASSERT_TRUE(matrix3 == matrix4);
}

TEST(MatrixTests, BigMatrixTest)
{
    // Average
    const Matrix matrix1 = E_MATRIX_F64(500, 500);
    const Matrix matrix2 = E_MATRIX_F64(500, 500);

    // Act
    const Matrix matrix3 = matrix1 * matrix2;

    // Assert
    ASSERT_EQ(matrix3.RowCount(), 500);
    ASSERT_EQ(matrix3.ColumnCount(), 500);
}

TEST(MatrixTests, MultiplyPTest)
{
    // Average
    const ExtendedCpp::MatrixI64 matrix1(500, 500, []{ return ExtendedCpp::Random::RandomInt(1, 10); });
    const ExtendedCpp::MatrixI64 matrix2(500, 500, []{ return ExtendedCpp::Random::RandomInt(1, 10); });

    // Act
    const ExtendedCpp::MatrixI64 matrix3 = matrix1.Multiply(matrix2, true);
    const ExtendedCpp::MatrixI64 matrix4 = matrix1.Multiply(matrix2, false);

    // Assert
    ASSERT_TRUE(matrix3 == matrix4);
}

TEST(MatrixTests, MacrosZeroTest)
{
    // Average
    // Act
    const ExtendedCpp::MatrixF64 matrixF64 = ZERO_MATRIX_F64(10, 10);
    const ExtendedCpp::MatrixF32 matrixF32 = ZERO_MATRIX_F32(10, 10);

    const ExtendedCpp::MatrixI64 matrixI64 = ZERO_MATRIX_I64(10, 10);
    const ExtendedCpp::MatrixI32 matrixI32 = ZERO_MATRIX_I32(10, 10);
    const ExtendedCpp::MatrixI16 matrixI16 = ZERO_MATRIX_I16(10, 10);
    const ExtendedCpp::MatrixI8 matrixI8 = ZERO_MATRIX_I8(10, 10);

    const ExtendedCpp::MatrixU64 matrixU64 = ZERO_MATRIX_U64(10, 10);
    const ExtendedCpp::MatrixU32 matrixU32 = ZERO_MATRIX_U32(10, 10);
    const ExtendedCpp::MatrixU16 matrixU16 = ZERO_MATRIX_U16(10, 10);
    const ExtendedCpp::MatrixU8 matrixU8 = ZERO_MATRIX_U8(10, 10);

    // Assert
    for (std::size_t i = 0; i < 10; ++i)
        for (std::size_t j = 0; j < 10; ++j)
        {
            ASSERT_EQ(matrixF64[i][j], 0);
            ASSERT_EQ(matrixF32[i][j], 0);
            ASSERT_EQ(matrixI64[i][j], 0);
            ASSERT_EQ(matrixI32[i][j], 0);
            ASSERT_EQ(matrixI8[i][j], 0);
            ASSERT_EQ(matrixU64[i][j], 0);
            ASSERT_EQ(matrixU32[i][j], 0);
            ASSERT_EQ(matrixU16[i][j], 0);
            ASSERT_EQ(matrixU8[i][j], 0);
        }
}

TEST(MatrixTests, MacrosETest)
{
    // Average
    // Act
    const ExtendedCpp::MatrixF64 matrixF64 = E_MATRIX_F64(10, 10);
    const ExtendedCpp::MatrixF32 matrixF32 = E_MATRIX_F32(10, 10);

    const ExtendedCpp::MatrixI64 matrixI64 = E_MATRIX_I64(10, 10);
    const ExtendedCpp::MatrixI32 matrixI32 = E_MATRIX_I32(10, 10);
    const ExtendedCpp::MatrixI16 matrixI16 = E_MATRIX_I16(10, 10);
    const ExtendedCpp::MatrixI8 matrixI8 = E_MATRIX_I8(10, 10);

    const ExtendedCpp::MatrixU64 matrixU64 = E_MATRIX_U64(10, 10);
    const ExtendedCpp::MatrixU32 matrixU32 = E_MATRIX_U32(10, 10);
    const ExtendedCpp::MatrixU16 matrixU16 = E_MATRIX_U16(10, 10);
    const ExtendedCpp::MatrixU8 matrixU8 = E_MATRIX_U8(10, 10);

    // Assert
    for (std::size_t i = 0; i < 10; ++i)
    {
        ASSERT_EQ(matrixF64[i][i], 1);
        ASSERT_EQ(matrixF32[i][i], 1);
        ASSERT_EQ(matrixI64[i][i], 1);
        ASSERT_EQ(matrixI32[i][i], 1);
        ASSERT_EQ(matrixI8[i][i], 1);
        ASSERT_EQ(matrixU64[i][i], 1);
        ASSERT_EQ(matrixU32[i][i], 1);
        ASSERT_EQ(matrixU16[i][i], 1);
        ASSERT_EQ(matrixU8[i][i], 1);
    }
}
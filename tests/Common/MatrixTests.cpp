#include <gtest/gtest.h>

#include <Common/Matrix.h>

typedef Common::MatrixF64 Matrix;

TEST(MatrixTests, SumMatrixTest)
{
    // Average
    Matrix matrix1(3, 3);
    matrix1[0] = { 1, 2, 3 };
    matrix1[1] = { 4, 5, 6 };
    matrix1[2] = { 7, 8, 9 };

    Matrix matrix2(3, 3);
    matrix2[0] = { 7, 8, 9 };
    matrix2[1] = { 4, 5, 6 };
    matrix2[2] = { 1, 2, 3 };

    // Act
    const Matrix matrix3 = (matrix1 + matrix2).value();

    // Assert
    Matrix matrix4(3, 3);
    matrix4[0] = { 8, 10, 12 };
    matrix4[1] = { 8, 10, 12 };
    matrix4[2] = { 8, 10, 12 };

    ASSERT_TRUE(matrix3 == matrix4);
}

TEST(MatrixTests, DetTest)
{
    // Average
    Matrix matrix(3, 3);
    matrix[0] = { 1, 2, 3 };
    matrix[1] = { 4, 5, 6 };
    matrix[2] = { 7, 8, 9 };

    // Act
    const double det = matrix.Det().value();

    // Assert
    ASSERT_EQ(det, 0);
}

TEST(MatrixTests, MultiplyMatrix)
{
    // Average
    Matrix matrix1(2, 3);
    matrix1[0] = { 1, 2, 3 };
    matrix1[1] = { 3, 1, -2 };

    Matrix matrix2(3, 2);
    matrix2[0] = { 1, 2 };
    matrix2[1] = { -3, 1 };
    matrix2[2] = { 2, 0 };

    // Act
    const Matrix matrix3 = (matrix1 * matrix2).value();

    // Assert
    Matrix matrix4(2, 2);
    matrix4[0] = { 1, 4 };
    matrix4[1] = { -4, 7 };

    ASSERT_TRUE(matrix3 == matrix4);
}

TEST(MatrixTests, BigMatrix)
{
    // Average
    const Matrix matrix1 = E_MATRIX_F64(100, 100);
    const Matrix matrix2 = E_MATRIX_F64(100, 100);

    // Act
    const Matrix matrix3 = (matrix1 * matrix2).value();

    // Assert
    ASSERT_TRUE(matrix3.RowCount() == 100);
    ASSERT_TRUE(matrix3.ColumnCount() == 100);
}

TEST(MatrixTests, MacrosZeroTest)
{
    // Average
    // Act
    const Common::MatrixF64 matrixF64 = ZERO_MATRIX_F64(10, 10);
    const Common::MatrixF32 matrixF32 = ZERO_MATRIX_F32(10, 10);

    const Common::MatrixI64 matrixI64 = ZERO_MATRIX_I64(10, 10);
    const Common::MatrixI32 matrixI32 = ZERO_MATRIX_I32(10, 10);
    const Common::MatrixI16 matrixI16 = ZERO_MATRIX_I16(10, 10);
    const Common::MatrixI8 matrixI8 = ZERO_MATRIX_I8(10, 10);

    const Common::MatrixU64 matrixU64 = ZERO_MATRIX_U64(10, 10);
    const Common::MatrixU32 matrixU32 = ZERO_MATRIX_U32(10, 10);
    const Common::MatrixU16 matrixU16 = ZERO_MATRIX_U16(10, 10);
    const Common::MatrixU8 matrixU8 = ZERO_MATRIX_U8(10, 10);

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
    const Common::MatrixF64 matrixF64 = E_MATRIX_F64(10, 10);
    const Common::MatrixF32 matrixF32 = E_MATRIX_F32(10, 10);

    const Common::MatrixI64 matrixI64 = E_MATRIX_I64(10, 10);
    const Common::MatrixI32 matrixI32 = E_MATRIX_I32(10, 10);
    const Common::MatrixI16 matrixI16 = E_MATRIX_I16(10, 10);
    const Common::MatrixI8 matrixI8 = E_MATRIX_I8(10, 10);

    const Common::MatrixU64 matrixU64 = E_MATRIX_U64(10, 10);
    const Common::MatrixU32 matrixU32 = E_MATRIX_U32(10, 10);
    const Common::MatrixU16 matrixU16 = E_MATRIX_U16(10, 10);
    const Common::MatrixU8 matrixU8 = E_MATRIX_U8(10, 10);

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
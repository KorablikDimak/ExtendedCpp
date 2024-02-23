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
    const Matrix matrix3 = matrix1 + matrix2;

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
    const double det = matrix.Det();

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
    const Matrix matrix3 = matrix1 * matrix2;

    // Assert
    Matrix matrix4(2, 2);
    matrix4[0] = { 1, 4 };
    matrix4[1] = { -4, 7 };

    ASSERT_TRUE(matrix3 == matrix4);
}
#ifndef Common_Matrix_H
#define Common_Matrix_H

#include <vector>
#include <stdexcept>
#include <cmath>
#include <cstdint>

namespace Common
{
    template<typename T>
    class Matrix final
    {
    private:
        std::vector<std::vector<T>> _table;
        std::size_t _rowCount;
        std::size_t _columnCount;

    public:
        Matrix(const std::size_t rowCount, const std::size_t columnCount)
        {
            if (rowCount == 0 || columnCount == 0)
                throw std::invalid_argument("rowCount and columnCount must be positive integer");

            _rowCount = rowCount;
            _columnCount = columnCount;

            _table.resize(_rowCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                _table[i].resize(_columnCount);
        }

        Matrix(const Matrix& matrix) noexcept
        {
            _rowCount = matrix._rowCount;
            _columnCount = matrix._columnCount;

            _table.resize(_rowCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                _table[i] = matrix[i];
        }

        Matrix(Matrix&& matrix) noexcept
        {
            _rowCount = matrix._rowCount;
            _columnCount = matrix._columnCount;

            _table.resize(_rowCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                _table[i] = std::move(matrix[i]);
        }

        explicit Matrix(const std::vector<std::vector<T>>& matrix)
        {
            if (matrix.empty() || matrix[0].empty())
                throw std::invalid_argument("matrix must not be empty");

            _rowCount = matrix.size();
            _columnCount = matrix[0].size();

            _table.resize(_rowCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
            {
                _table[i].resize(_columnCount);
                for (std::size_t j = 0; j < _columnCount; ++j)
                {
                    if (matrix[i].size() != _columnCount)
                        throw std::invalid_argument("matrix must be a rectangle");
                    _table[i][j] = matrix[i][j];
                }
            }
        }

        explicit Matrix(std::vector<std::vector<T>>&& matrix)
        {
            if (matrix.empty() || matrix[0].empty())
                throw std::invalid_argument("matrix must not be empty");

            _rowCount = matrix.size();
            _columnCount = matrix[0].size();

            for (std::size_t i = 0; i < _rowCount; ++i)
                if (matrix[i].size() != _columnCount)
                    throw std::invalid_argument("matrix must be a rectangle");

            _table = matrix;
        }

        ~Matrix() noexcept = default;

        Matrix& operator=(const Matrix& matrix) noexcept
        {
            _rowCount = matrix._rowCount;
            _columnCount = matrix._columnCount;

            _table.resize(_rowCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                _table[i] = matrix[i];

            return *this;
        }

        Matrix& operator=(Matrix&& matrix) noexcept
        {
            _rowCount = matrix._rowCount;
            _columnCount = matrix._columnCount;

            _table.resize(_rowCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                _table[i] = std::move(matrix[i]);

            return *this;
        }

        Matrix& operator=(const std::vector<std::vector<T>>& matrix)
        {
            if (matrix.empty() || matrix[0].empty())
                throw std::invalid_argument("matrix must not be empty");

            _rowCount = matrix.size();
            _columnCount = matrix[0].size();

            _table.resize(_rowCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
            {
                _table[i].resize(_columnCount);
                for (std::size_t j = 0; j < _columnCount; ++j)
                {
                    if (matrix[i].size() != _columnCount)
                        throw std::invalid_argument("matrix must be a rectangle");
                    _table[i][j] = matrix[i][j];
                }
            }
            return *this;
        }

        Matrix& operator=(std::vector<std::vector<T>>&& matrix)
        {
            if (matrix.empty() || matrix[0].empty())
                throw std::invalid_argument("matrix must not be empty");

            _rowCount = matrix.size();
            _columnCount = matrix[0].size();

            for (std::size_t i = 0; i < _rowCount; ++i)
                if (matrix[i].size() != _columnCount)
                    throw std::invalid_argument("matrix must be a rectangle");

            _table = matrix;
            return *this;
        }

        bool operator==(const Matrix& matrix) const noexcept
        {
            if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
                return false;

            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    if (_table[i][j] != matrix[i][j]) return false;

            return true;
        }

        bool operator!=(const Matrix& matrix) const noexcept
        {
            if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
                return true;

            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    if (_table[i][j] == matrix[i][j]) return false;

            return true;
        }

        Matrix operator+(const Matrix& matrix) const
        {
            if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
                throw std::invalid_argument("Size of matrix must be equal");

            Matrix result(_rowCount, _columnCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    result[i][j] = _table[i][j] + matrix[i][j];

            return std::move(result);
        }

        Matrix operator-(const Matrix& matrix) const
        {
            if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
                throw std::invalid_argument("Size of matrix must be equal");

            Matrix result(_rowCount, _columnCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    result[i][j] = _table[i][j] - matrix[i][j];

            return std::move(result);
        }

        Matrix operator*(const Matrix& matrix) const
        {
            if (_columnCount != matrix._rowCount)
                throw std::invalid_argument("Column size of left matrix must be equal to row size of right matrix");

            Matrix result(_rowCount, matrix._columnCount);

            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < matrix._columnCount; ++j)
                {
                    T c{};
                    for (std::size_t k = 0; k < _columnCount; ++k)
                        c += _table[i][k] * matrix[k][j];

                    result[i][j] = c;
                }

            return std::move(result);
        }

        Matrix operator*(const T& alpha) const noexcept
        {
            Matrix result(_rowCount, _columnCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    result[i][j] = _table[i][j] * alpha;

            return std::move(result);
        }

        Matrix operator*(T&& alpha) const noexcept
        {
            Matrix result(_rowCount, _columnCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    result[i][j] = _table[i][j] * alpha;

            return std::move(result);
        }

        const std::vector<T>& operator[](std::size_t rowNumber) const noexcept
        {
            return _table[rowNumber];
        }

        std::vector<T>&& operator[](std::size_t rowNumber) noexcept
        {
            return std::move(_table[rowNumber]);
        }

        Matrix Transpose() const noexcept
        {
            Matrix result(_columnCount, _rowCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    result[j][i] = _table[i][j];

            return std::move(result);
        }

        void EraseRow(std::size_t rowNumber)
        {
            if (_rowCount == 1)
                throw std::invalid_argument("You cannot erase a row from a matrix that has only one row");

            _table.erase(_table.cbegin() + rowNumber);
            --_rowCount;
        }

        void EraseColumn(std::size_t columnNumber)
        {
            if (_columnCount == 1)
                throw std::invalid_argument("You cannot erase a column from a matrix that has only one column");

            for (std::size_t i = 0; i < _rowCount; ++i)
                _table[i].erase(_table[i].cbegin() + columnNumber);
            --_columnCount;
        }

        T AlgebraicComplement(const std::size_t i, const std::size_t j) const
        {
            Matrix minorMatrix(*this);
            minorMatrix.EraseRow(i);
            minorMatrix.EraseColumn(j);
            return std::pow(-1, i + j + 2) * minorMatrix.Det();
        }

        T Det() const
        {
            if (_rowCount != _columnCount)
                throw std::range_error("The matrix must be square");

            if (_rowCount == 1)
                return _table[0][0];

            T result{};
            for (std::size_t i = 0; i < _rowCount; ++i)
                result += _table[i][0] * AlgebraicComplement(i, 0);

            return std::move(result);
        }

        Matrix Inverse() const
        {
            const T det = Det();
            if (det == 0)
                throw std::logic_error("Det must not be zero");
            const T k = 1 / det;

            Matrix inverseMatrix(_rowCount, _columnCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                {
                    T a = AlgebraicComplement(i, j);
                    inverseMatrix[j][i] = a * k;
                }

            return std::move(inverseMatrix);
        }

        Matrix operator~() const
        {
            return Inverse();
        }

        Matrix operator-() const noexcept
        {
            Matrix copy = Matrix(*this);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    copy[i][j] = -copy[i][j];
            return std::move(copy);
        }

        void SwapRow(std::size_t row1Number, std::size_t row2Number) noexcept
        {
            std::swap(_table[row1Number], _table[row2Number]);
        }

        void SwapColumn(std::size_t column1Number, std::size_t column2Number) noexcept
        {
            for (std::size_t i = 0; i < _rowCount; ++i)
                std::swap(_table[i][column1Number], _table[i][column2Number]);
        }

        [[nodiscard]]
        std::size_t Rank() const noexcept
        {
            std::size_t rank = _columnCount;
            Matrix copy(*this);

            for (std::size_t rowIndex = 0; rowIndex < rank; ++rowIndex)
            {
                if (copy[rowIndex][rowIndex])
                {
                    for (std::size_t columnIndex = 0; columnIndex < _rowCount; ++columnIndex)
                        if (columnIndex != rowIndex)
                        {
                            T multiplier = copy[columnIndex][rowIndex] / copy[rowIndex][rowIndex];
                            for (std::size_t i = 0; i < rank; ++i)
                                copy[columnIndex][i] -= multiplier * copy[rowIndex][i];
                        }
                }
                else
                {
                    bool reduce = true;

                    for (std::size_t i = rowIndex + 1; i < _rowCount; ++i)
                        if (copy[i][rowIndex])
                        {
                            copy.SwapRow(rowIndex, i);
                            reduce = false;
                            break;
                        }

                    if (reduce)
                    {
                        --rank;
                        for (int i = 0; i < _rowCount; ++i)
                            copy[i][rowIndex] = copy[i][rank];
                    }
                    --rowIndex;
                }
            }

            return rank;
        }

        [[nodiscard]]
        std::size_t RowCount() const noexcept
        {
            return _rowCount;
        }

        [[nodiscard]]
        std::size_t ColumnCount() const noexcept
        {
            return _columnCount;
        }

        std::vector<T> GetRow(std::size_t rowNumber) const noexcept
        {
            return _table[rowNumber];
        }

        void SetRow(const std::vector<T>& newRow, std::size_t rowNumber)
        {
            if (newRow.size() != _columnCount)
                throw std::invalid_argument("Incorrect newRow size");

            _table[rowNumber] = newRow;
        }

        void SetRow(std::vector<T>&& newRow, std::size_t rowNumber)
        {
            if (newRow.size() != _columnCount)
                throw std::invalid_argument("Incorrect newRow size");

            _table[rowNumber] = newRow;
        }

        std::vector<T> GetColumn(std::size_t columnNumber) const noexcept
        {
            std::vector<T> column(_rowCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                column[i] = _table[i][columnNumber];
            return column;
        }

        void SetColumn(const std::vector<T>& newColumn, std::size_t columnNumber)
        {
            if (newColumn.size() != _rowCount)
                throw std::invalid_argument("Incorrect newColumn size");

            for (std::size_t i = 0; i < _rowCount; ++i)
                _table[i][columnNumber] = newColumn[i];
        }

        void SetColumn(std::vector<T>&& newColumn, std::size_t columnNumber)
        {
            if (newColumn.size() != _rowCount)
                throw std::invalid_argument("Incorrect newColumn size");

            for (std::size_t i = 0; i < _rowCount; ++i)
                _table[i][columnNumber] = std::move(newColumn[i]);
        }

        T GetElement(std::size_t i, std::size_t j) const noexcept
        {
            return _table[i][j];
        }

        const T& GetElementRef(std::size_t i, std::size_t j) const noexcept
        {
            return _table[i][j];
        }

        T&& GetElementRef(std::size_t i, std::size_t j) noexcept
        {
            return std::move(_table[i][j]);
        }

        void SetElement(const T& newValue, std::size_t i, std::size_t j) noexcept
        {
            _table[i][j] = newValue;
        }

        void SetElement(T&& newValue, std::size_t i, std::size_t j) noexcept
        {
            _table[i][j] = std::move(newValue);
        }

        void Resize(std::size_t rowCount, std::size_t columnCount)
        {
            if (rowCount == 0 || columnCount == 0)
                throw std::invalid_argument("Matrix must not be empty");

            for (std::size_t i = 0; i < _rowCount; ++i)
                _table[i].resize(columnCount);
            _table.resize(rowCount);

            _rowCount = rowCount;
            _columnCount = columnCount;
        }

        [[nodiscard]]
        std::string ToString() const noexcept
        {
            std::string matrixString;

            for (std::size_t i = 0; i < _rowCount; ++i)
            {
                for (std::size_t j = 0; j < _columnCount; ++j)
                    matrixString += std::to_string(_table[i][j]) + "\t";
                matrixString += "\n";
            }

            return std::move(matrixString);
        }
    };

    typedef Matrix<std::double_t> MatrixF64;
    typedef Matrix<std::float_t> MatrixF32;

    typedef Matrix<std::int64_t> MatrixI64;
    typedef Matrix<std::int32_t> MatrixI32;
    typedef Matrix<std::int16_t> MatrixI16;
    typedef Matrix<std::int8_t> MatrixI8;

    typedef Matrix<std::uint64_t> MatrixU64;
    typedef Matrix<std::uint32_t> MatrixU32;
    typedef Matrix<std::uint16_t> MatrixU16;
    typedef Matrix<std::uint8_t> MatrixU8;
}

#define ZERO_MATRIX_F64(rowCount, columnCount) \
Common::MatrixF64(rowCount, columnCount)

#define ZERO_MATRIX_F32(rowCount, columnCount) \
Common::MatrixF32(rowCount, columnCount)

#define ZERO_MATRIX_I64(rowCount, columnCount) \
Common::MatrixI64(rowCount, columnCount)

#define ZERO_MATRIX_I32(rowCount, columnCount) \
Common::MatrixI32(rowCount, columnCount)

#define ZERO_MATRIX_I16(rowCount, columnCount) \
Common::MatrixI16(rowCount, columnCount)

#define ZERO_MATRIX_I8(rowCount, columnCount) \
Common::MatrixI8(rowCount, columnCount)

#define ZERO_MATRIX_U64(rowCount, columnCount) \
Common::MatrixU64(rowCount, columnCount)

#define ZERO_MATRIX_U32(rowCount, columnCount) \
Common::MatrixU32(rowCount, columnCount)

#define ZERO_MATRIX_U16(rowCount, columnCount) \
Common::MatrixU16(rowCount, columnCount)

#define ZERO_MATRIX_U8(rowCount, columnCount) \
Common::MatrixU8(rowCount, columnCount)

#define E_MATRIX_F64(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixF64 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < rowCount && j < columnCount; ++i, ++j) \
        matrix[i][j] = 1; \
    return matrix; \
}() \

#define E_MATRIX_F32(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixF32 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < rowCount && j < columnCount; ++i, ++j) \
        matrix[i][j] = 1; \
    return matrix; \
}() \

#define E_MATRIX_I64(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixI64 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < rowCount && j < columnCount; ++i, ++j) \
        matrix[i][j] = 1; \
    return matrix; \
}() \

#define E_MATRIX_I32(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixI32 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < rowCount && j < columnCount; ++i, ++j) \
        matrix[i][j] = 1; \
    return matrix; \
}() \

#define E_MATRIX_I16(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixI16 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < rowCount && j < columnCount; ++i, ++j) \
        matrix[i][j] = 1; \
    return matrix; \
}() \

#define E_MATRIX_I8(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixI8 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < rowCount && j < columnCount; ++i, ++j) \
        matrix[i][j] = 1; \
    return matrix; \
}() \

#define E_MATRIX_U64(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixU64 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < rowCount && j < columnCount; ++i, ++j) \
        matrix[i][j] = 1; \
    return matrix; \
}() \

#define E_MATRIX_U32(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixU32 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < rowCount && j < columnCount; ++i, ++j) \
        matrix[i][j] = 1; \
    return matrix; \
}() \

#define E_MATRIX_U16(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixU16 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < rowCount && j < columnCount; ++i, ++j) \
        matrix[i][j] = 1; \
    return matrix; \
}() \

#define E_MATRIX_U8(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixU8 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < rowCount && j < columnCount; ++i, ++j) \
        matrix[i][j] = 1; \
    return matrix; \
}() \

#endif
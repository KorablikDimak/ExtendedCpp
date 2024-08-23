#ifndef Common_Matrix_H
#define Common_Matrix_H

#include <vector>
#include <stdexcept>
#include <cmath>
#include <cstdint>
#include <optional>

namespace Common
{
    template<typename T>
    class Matrix final
    {
    private:
        std::vector<T> _table;
        std::size_t _rowCount;
        std::size_t _columnCount;

    public:
        Matrix(const std::size_t rowCount, const std::size_t columnCount) noexcept
        {
            _rowCount = rowCount;
            _columnCount = columnCount;
            _table.resize(_rowCount * _columnCount);
        }

        Matrix(const Matrix& matrix) noexcept
        {
            _rowCount = matrix._rowCount;
            _columnCount = matrix._columnCount;
            _table = matrix._table;
        }

        Matrix(Matrix&& matrix) noexcept
        {
            _rowCount = matrix._rowCount;
            _columnCount = matrix._columnCount;
            _table = std::move(matrix._table);
        }

        explicit Matrix(const std::vector<std::vector<T>>& matrix) noexcept
        {
            std::size_t maxColumnColumn = 0;
            for (std::size_t i = 0; i < matrix.size(); ++i)
                if (maxColumnColumn < matrix[i].size())
                    maxColumnColumn = matrix[i].size();

            _rowCount = matrix.size();
            _columnCount = maxColumnColumn;
            _table.resize(_rowCount * _columnCount);

            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < matrix[i].size(); ++j)
                    _table[i * _columnCount + j] = matrix[i][j];
        }

        explicit Matrix(std::vector<std::vector<T>>&& matrix) noexcept
        {
            std::size_t maxColumnColumn = 0;
            for (std::size_t i = 0; i < matrix.size(); ++i)
                if (maxColumnColumn < matrix[i].size())
                    maxColumnColumn = matrix[i].size();

            _rowCount = matrix.size();
            _columnCount = maxColumnColumn;
            _table.resize(_rowCount * _columnCount);

            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < matrix[i].size(); ++j)
                    _table[i * _columnCount + j] = std::move(matrix[i][j]);
        }

        ~Matrix() = default;

        Matrix& operator=(const Matrix& matrix) noexcept = default;

        Matrix& operator=(Matrix&& matrix) noexcept
        {
            _rowCount = matrix._rowCount;
            _columnCount = matrix._columnCount;
            _table = std::move(matrix._table);
            return *this;
        }

        Matrix& operator=(const std::vector<std::vector<T>>& matrix) noexcept
        {
            std::size_t maxColumnColumn = 0;
            for (std::size_t i = 0; i < matrix.size(); ++i)
                if (maxColumnColumn < matrix[i].size())
                    maxColumnColumn = matrix[i].size();

            _rowCount = matrix.size();
            _columnCount = maxColumnColumn;
            _table.resize(_rowCount * _columnCount);

            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < matrix[i].size(); ++j)
                    _table[i * _columnCount + j] = matrix[i][j];

            return *this;
        }

        Matrix& operator=(std::vector<std::vector<T>>&& matrix) noexcept
        {
            std::size_t maxColumnColumn = 0;
            for (std::size_t i = 0; i < matrix.size(); ++i)
                if (maxColumnColumn < matrix[i].size())
                    maxColumnColumn = matrix[i].size();

            _rowCount = matrix.size();
            _columnCount = maxColumnColumn;
            _table.resize(_rowCount * _columnCount);

            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < matrix[i].size(); ++j)
                    _table[i * _columnCount + j] = std::move(matrix[i][j]);

            return *this;
        }

        bool operator==(const Matrix& matrix) const noexcept
        {
            if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
                return false;

            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    if (_table[i * _columnCount + j] != matrix._table[i * _columnCount + j]) return false;

            return true;
        }

        bool operator!=(const Matrix& matrix) const noexcept
        {
            if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
                return true;

            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    if (_table[i * _columnCount + j] == matrix._table[i * _columnCount + j]) return false;

            return true;
        }

        std::optional<Matrix> operator+(const Matrix& matrix) const noexcept
        {
            if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
                return std::nullopt;

            Matrix result(_rowCount, _columnCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    result._table[i * _columnCount + j] = _table[i * _columnCount + j] + matrix._table[i * _columnCount + j];

            return std::move(result);
        }

        std::optional<Matrix> operator-(const Matrix& matrix) const noexcept
        {
            if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
                return std::nullopt;

            Matrix result(_rowCount, _columnCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    result._table[i * _columnCount + j] = _table[i * _columnCount + j] - matrix._table[i * _columnCount + j];

            return std::move(result);
        }

        std::optional<Matrix> operator*(const Matrix& matrix) const noexcept
        {
            if (_columnCount != matrix._rowCount)
                return std::nullopt;

            Matrix result(_rowCount, matrix._columnCount);
            Matrix transpose = matrix.Transpose();

            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < matrix._columnCount; ++j)
                {
                    T c{};
                    for (std::size_t k = 0; k < _columnCount; ++k)
                        c += _table[i * _columnCount + k] * transpose._table[j * transpose._columnCount + k];
                    result._table[i * matrix._columnCount + j] = c;
                }

            return std::move(result);
        }

        Matrix operator*(const T& alpha) const noexcept
        {
            Matrix result(_rowCount, _columnCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    result._table[i * _columnCount + j] = _table[i * _columnCount + j] * alpha;

            return std::move(result);
        }

        Matrix operator*(T&& alpha) const noexcept
        {
            Matrix result(_rowCount, _columnCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    result._table[i * _columnCount + j] = _table[i * _columnCount + j] * std::move(alpha);

            return std::move(result);
        }

        std::vector<T> operator[](const std::size_t rowNumber) const noexcept
        {
            std::vector<T> row(_columnCount);

            for (std::size_t i = 0; i < _columnCount; ++i)
                row[i] = _table[rowNumber * _columnCount + i];

            return std::move(row);
        }

        Matrix Transpose() const noexcept
        {
            Matrix result(_columnCount, _rowCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    result._table[j * _rowCount + i] = _table[i * _columnCount + j];

            return std::move(result);
        }

        void EraseRow(const std::size_t rowNumber) noexcept
        {
            if (_rowCount == 0) return;
            _table.erase(_table.begin() + rowNumber * _columnCount, _table.begin() + rowNumber * _columnCount + _rowCount);
            --_rowCount;
        }

        void EraseColumn(const std::size_t columnNumber) noexcept
        {
            if (_columnCount == 0) return;
            for (std::size_t i = 0; i < _rowCount; ++i)
                _table.erase(_table.begin() + i * _columnCount + columnNumber);
            --_columnCount;
        }

        std::optional<T> Det() const noexcept
        {
            if (_rowCount != _columnCount || _rowCount == 0 || _columnCount == 0)
                return std::nullopt;
            return DetHelper();
        }

        std::optional<Matrix> Inverse() const noexcept
        {
            if (_rowCount != _columnCount || _rowCount == 0 || _columnCount == 0)
                return std::nullopt;
            const T det = DetHelper();

            if (det == 0)
                return std::nullopt;
            const T k = 1 / det;

            Matrix inverseMatrix(_rowCount, _columnCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                {
                    T a = AlgebraicComplement(i, j);
                    inverseMatrix._table[j * _rowCount + i] = a * k;
                }

            return std::move(inverseMatrix);
        }

        std::optional<Matrix> operator~() const noexcept
        {
            return Inverse();
        }

        Matrix operator-() const noexcept
        {
            Matrix copy = Matrix(*this);
            for (std::size_t i = 0; i < _rowCount * _columnCount; ++i)
                copy._table[i] = -copy._table[i];
            return std::move(copy);
        }

        void SwapRow(const std::size_t row1Number, const std::size_t row2Number) noexcept
        {
            for (std::size_t i = 0; i < _columnCount; ++i)
                std::swap(_table[row1Number * _columnCount + i], _table[row2Number * _columnCount + i]);
        }

        void SwapColumn(const std::size_t column1Number, const std::size_t column2Number) noexcept
        {
            for (std::size_t i = 0; i < _rowCount; ++i)
                std::swap(_table[i * _columnCount + column1Number], _table[i * _columnCount + column2Number]);
        }

        [[nodiscard]]
        std::size_t Rank() const noexcept
        {
            if (_rowCount == 0 || _columnCount == 0)
                return 0;

            std::size_t rank = _columnCount;
            Matrix copy(*this);

            for (std::size_t rowIndex = 0; rowIndex < rank; ++rowIndex)
            {
                if (copy._table[rowIndex * _columnCount + rowIndex])
                {
                    for (std::size_t columnIndex = 0; columnIndex < _rowCount; ++columnIndex)
                        if (columnIndex != rowIndex)
                        {
                            T multiplier = copy._table[columnIndex  * _columnCount + rowIndex] / copy._table[rowIndex * _columnCount + rowIndex];
                            for (std::size_t i = 0; i < rank; ++i)
                                copy._table[columnIndex * _columnCount + i] -= multiplier * copy._table[rowIndex * _columnCount + i];
                        }
                }
                else
                {
                    bool reduce = true;

                    for (std::size_t i = rowIndex + 1; i < _rowCount; ++i)
                        if (copy._table[i * _columnCount + rowIndex])
                        {
                            copy.SwapRow(rowIndex, i);
                            reduce = false;
                            break;
                        }

                    if (reduce)
                    {
                        --rank;
                        for (int i = 0; i < _rowCount; ++i)
                            copy._table[i * _columnCount + rowIndex] = copy._table[i * _columnCount + rank];
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

        std::vector<T> GetRow(const std::size_t rowNumber) const noexcept
        {
            std::vector<T> result(_columnCount);
            for (std::size_t i = 0; i < _columnCount; ++i)
                result[i] = _table[rowNumber * _columnCount + i];
            return _table[rowNumber];
        }

        void SetRow(const std::vector<T>& newRow, const std::size_t rowNumber) noexcept
        {
            if (newRow.size() != _columnCount)
                return;

            for (std::size_t i = 0; i < _columnCount; ++i)
                _table[rowNumber * _columnCount + i] = newRow[i];
        }

        void SetRow(std::vector<T>&& newRow, const std::size_t rowNumber) noexcept
        {
            if (newRow.size() != _columnCount)
                return;

            for (std::size_t i = 0; i < _columnCount; ++i)
                _table[rowNumber * _columnCount + i] = std::move(newRow[i]);
        }

        std::vector<T> GetColumn(const std::size_t columnNumber) const noexcept
        {
            std::vector<T> column(_rowCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                column[i] = _table[i * _columnCount + columnNumber];
            return column;
        }

        void SetColumn(const std::vector<T>& newColumn, const std::size_t columnNumber) noexcept
        {
            if (newColumn.size() != _rowCount)
                return;

            for (std::size_t i = 0; i < _columnCount; ++i)
                _table[i * _columnCount + columnNumber] = newColumn[i];
        }

        void SetColumn(std::vector<T>&& newColumn, const std::size_t columnNumber) noexcept
        {
            if (newColumn.size() != _rowCount)
                return;

            for (std::size_t i = 0; i < _columnCount; ++i)
                _table[i * _columnCount + columnNumber] = std::move(newColumn[i]);
        }

        T GetElement(const std::size_t i, const std::size_t j) const noexcept
        {
            return _table[i * _columnCount + j];
        }

        const T& GetElementRef(const std::size_t i, const std::size_t j) const noexcept
        {
            return _table[i * _columnCount + j];
        }

        T&& GetElementRef(const std::size_t i, const std::size_t j) noexcept
        {
            return std::move(_table[i * _columnCount + j]);
        }

        void SetElement(const T& newValue, const std::size_t i, const std::size_t j) noexcept
        {
            _table[i * _columnCount + j] = newValue;
        }

        void SetElement(T&& newValue, const std::size_t i, const std::size_t j) noexcept
        {
            _table[i * _columnCount + j] = std::move(newValue);
        }

        void Resize(const std::size_t rowCount, const std::size_t columnCount) noexcept
        {
            std::vector<T> newTable(rowCount * columnCount);
            for (std::size_t i = 0; i < _rowCount && i < rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount && j < columnCount; ++j)
                    newTable[i * columnCount + j] = std::move(_table[i * _columnCount + j]);

            _table = std::move(newTable);
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
                    matrixString += std::to_string(_table[i * _columnCount + j]) + "\t";
                matrixString += "\n";
            }

            return std::move(matrixString);
        }

    private:
        T AlgebraicComplement(const std::size_t i, const std::size_t j) const noexcept
        {
            Matrix minorMatrix(*this);
            minorMatrix.EraseRow(i);
            minorMatrix.EraseColumn(j);

            if ((i + j) % 2 == 0)
                return minorMatrix.DetHelper();
            else
                return -minorMatrix.DetHelper();
        }

        T DetHelper() const noexcept
        {
            if (_rowCount == 1)
                return _table[0];

            if (_rowCount == 2)
                return _table[0] * _table[3] - _table[1] * _table[2];

            if (_rowCount == 3)
                return _table[0] * _table[4] * _table[8] +
                    _table[6] * _table[1] * _table[5] +
                    _table[2] * _table[3] * _table[7] -
                    _table[2] * _table[4] * _table[6] -
                    _table[0] * _table[5] * _table[7] -
                    _table[1] * _table[3] * _table[8];

            T result{};
            for (std::size_t i = 0; i < _rowCount; ++i)
                result += _table[i * _columnCount] * AlgebraicComplement(i, 0);

            return std::move(result);
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
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}() \

#define E_MATRIX_F32(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixF32 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}() \

#define E_MATRIX_I64(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixI64 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}() \

#define E_MATRIX_I32(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixI32 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}() \

#define E_MATRIX_I16(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixI16 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}() \

#define E_MATRIX_I8(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixI8 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}() \

#define E_MATRIX_U64(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixU64 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}() \

#define E_MATRIX_U32(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixU32 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}() \

#define E_MATRIX_U16(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixU16 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}() \

#define E_MATRIX_U8(rowCount, columnCount) \
[]() \
{ \
    Common::MatrixU8 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}() \

#endif
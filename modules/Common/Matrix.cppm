module;

#include <vector>
#include <stdexcept>
#include <cmath>
#include <cstdint>
#include <optional>
#include <functional>
#include <future>
#include <array>
#include <string>

export module ExtendedCpp.Common.Matrix;

import ExtendedCpp.Common.Concepts;

export namespace ExtendedCpp::Common
{
    template<typename T>
    class Matrix final
    {
    private:
        std::vector<T> _table{};
        std::size_t _rowCount{};
        std::size_t _columnCount{};

    public:
        Matrix(const std::size_t rowCount, const std::size_t columnCount) noexcept
        requires std::is_default_constructible_v<T>
        {
            _rowCount = rowCount;
            _columnCount = columnCount;
            _table.resize(_rowCount * _columnCount);
        }

        Matrix(const std::size_t rowCount, const std::size_t columnCount, std::function<T()> init) noexcept
        requires std::is_default_constructible_v<T>
        {
            _rowCount = rowCount;
            _columnCount = columnCount;
            _table.resize(_rowCount * _columnCount);

            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    _table[i * _columnCount + j] = init();
        }

        Matrix(const std::size_t rowCount, const std::size_t columnCount, std::function<T(std::size_t, std::size_t)> init) noexcept
        requires std::is_default_constructible_v<T>
        {
            _rowCount = rowCount;
            _columnCount = columnCount;
            _table.resize(_rowCount * _columnCount);

            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    _table[i * _columnCount + j] = init(i, j);
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
        requires std::is_default_constructible_v<T>
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
        requires std::is_default_constructible_v<T>
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
        requires std::is_default_constructible_v<T>
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
        requires std::is_default_constructible_v<T>
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
        requires Concepts::Equatable<T>
        {
            if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
                return false;

            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    if (_table[i * _columnCount + j] != matrix._table[i * _columnCount + j]) return false;

            return true;
        }

        bool operator!=(const Matrix& matrix) const noexcept
        requires Concepts::Equatable<T>
        {
            if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
                return true;

            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    if (_table[i * _columnCount + j] == matrix._table[i * _columnCount + j]) return false;

            return true;
        }

        std::optional<Matrix> Sum(const Matrix& matrix) const noexcept
        requires Concepts::Summarize<T>
        {
            if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
                return std::nullopt;

            Matrix result(_rowCount, _columnCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    result._table[i * _columnCount + j] =
                            _table[i * _columnCount + j] + matrix._table[i * _columnCount + j];

            return std::move(result);
        }

        std::optional<Matrix> operator+(const Matrix& matrix) const noexcept
        {
            return Sum(matrix);
        }

        std::optional<Matrix> Substitute(const Matrix& matrix) const noexcept
        requires Concepts::Substitute<T>
        {
            if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
                return std::nullopt;

            Matrix result(_rowCount, _columnCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    result._table[i * _columnCount + j] =
                            _table[i * _columnCount + j] - matrix._table[i * _columnCount + j];

            return std::move(result);
        }

        std::optional<Matrix> operator-(const Matrix& matrix) const noexcept
        {
            return Substitute(matrix);
        }

        std::optional<Matrix> Multiply(const Matrix& matrix, const bool asParallel = true) const noexcept
        requires Concepts::Multiply<T>
        {
            if (_columnCount != matrix._rowCount)
                return std::nullopt;

            if (asParallel)
            {
                Matrix result = StrassenMultiplyParallel(matrix);
                result.Resize(_rowCount, matrix._columnCount);
                return std::move(result);
            }
            else
            {
                Matrix result = StrassenMultiply(matrix);
                result.Resize(_rowCount, matrix._columnCount);
                return std::move(result);
            }
        }

        std::optional<Matrix> operator*(const Matrix& matrix) const noexcept
        {
            return Multiply(matrix, true);
        }

        Matrix Multiply(const T& alpha) const noexcept
        requires Concepts::Multiply<T>
        {
            Matrix result(_rowCount, _columnCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    result._table[i * _columnCount + j] = _table[i * _columnCount + j] * alpha;

            return std::move(result);
        }

        Matrix operator*(const T&& alpha) const noexcept
        {
            return Multiply(alpha);
        }

        Matrix Multiply(T&& alpha) const noexcept
        requires Concepts::Multiply<T>
        {
            Matrix result(_rowCount, _columnCount);
            for (std::size_t i = 0; i < _rowCount; ++i)
                for (std::size_t j = 0; j < _columnCount; ++j)
                    result._table[i * _columnCount + j] = _table[i * _columnCount + j] * std::move(alpha);

            return std::move(result);
        }

        Matrix operator*(T&& alpha) const noexcept
        {
            return Multiply(alpha);
        }

        std::vector<T> operator[](const std::size_t rowNumber) const noexcept
        requires std::is_default_constructible_v<T>
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
            for (std::size_t i = 0, j = _rowCount - 1; i < _rowCount; ++i, --j)
                _table.erase(_table.begin() + j * _columnCount + columnNumber);
            --_columnCount;
        }

        std::optional<T> Det() const noexcept
        {
            if (_rowCount != _columnCount || _rowCount == 0 || _columnCount == 0)
                return std::nullopt;

            const auto decomposeResult = LUPDecompose();
            if (!decomposeResult.has_value())
                return std::nullopt;

            const auto [A, P] = decomposeResult.value();
            return A.LUPDet(P);
        }

        std::optional<Matrix> Inverse() const noexcept
        {
            if (_rowCount != _columnCount || _rowCount == 0 || _columnCount == 0)
                return std::nullopt;

            const auto decomposeResult = LUPDecompose();
            if (!decomposeResult.has_value())
                return std::nullopt;

            const auto [A, P] = decomposeResult.value();
            return A.LUPInvert(P);
        }

        std::optional<Matrix> operator~() const noexcept
        {
            return Inverse();
        }

        Matrix operator-() const noexcept
        requires Concepts::Negative<T>
        {
            Matrix copy = Matrix(*this);
            for (std::size_t i = 0; i < _rowCount * _columnCount; ++i)
                copy._table[i] = -copy._table[i];
            return std::move(copy);
        }

        void SwapRows(const std::size_t row1Number, const std::size_t row2Number) noexcept
        {
            for (std::size_t i = 0; i < _columnCount; ++i)
                std::swap(_table[row1Number * _columnCount + i], _table[row2Number * _columnCount + i]);
        }

        void SwapColumns(const std::size_t column1Number, const std::size_t column2Number) noexcept
        {
            for (std::size_t i = 0; i < _rowCount; ++i)
                std::swap(_table[i * _columnCount + column1Number], _table[i * _columnCount + column2Number]);
        }

        [[nodiscard]]
        std::size_t Rank() const noexcept
        requires Concepts::Divisible<T> && Concepts::Multiply<T> && Concepts::Substitute<T>
        {
            if (_rowCount == 0 || _columnCount == 0)
                return 0;

            std::size_t rank = _columnCount;
            Matrix copy(*this);

            for (std::size_t rowIndex = 0; rowIndex < rank; ++rowIndex)
            {
                if (std::abs(copy._table[rowIndex * _columnCount + rowIndex]) < std::numeric_limits<std::double_t>::min())
                {
                    for (std::size_t columnIndex = 0; columnIndex < _rowCount; ++columnIndex)
                        if (columnIndex != rowIndex)
                        {
                            T multiplier = copy._table[columnIndex  * _columnCount + rowIndex] /
                                           copy._table[rowIndex * _columnCount + rowIndex];
                            for (std::size_t i = 0; i < rank; ++i)
                                copy._table[columnIndex * _columnCount + i] -=
                                        multiplier * copy._table[rowIndex * _columnCount + i];
                        }
                }
                else
                {
                    bool reduce = true;

                    for (std::size_t i = rowIndex + 1; i < _rowCount; ++i)
                        if (copy._table[i * _columnCount + rowIndex])
                        {
                            copy.SwapRows(rowIndex, i);
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

        [[nodiscard]]
        std::vector<T> GetRow(const std::size_t rowNumber) const noexcept
        requires std::is_default_constructible_v<T>
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

        [[nodiscard]]
        std::vector<T> GetColumn(const std::size_t columnNumber) const noexcept
        requires std::is_default_constructible_v<T>
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
        requires std::is_default_constructible_v<T>
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
        [[nodiscard]]
        std::optional<std::pair<Matrix, std::vector<std::size_t>>> LUPDecompose() const noexcept
        requires std::is_default_constructible_v<T> && Concepts::Comparable<T> &&
                 Concepts::Divisible<T> && Concepts::Substitute<T> && Concepts::Multiply<T>
        {
            std::vector<std::size_t> P(_rowCount + 1);
            Matrix A(*this);

            std::size_t i, j, k, imax;
            T maxA, absA;

            for (i = 0; i <= _rowCount; ++i)
                P[i] = i;

            for (i = 0; i < _rowCount; ++i)
            {
                maxA = 0.0;
                imax = i;

                for (k = i; k < _rowCount; ++k)
                    if ((absA = std::abs(A._table[k * A._columnCount + i])) > maxA)
                    {
                        maxA = absA;
                        imax = k;
                    }

                if (std::abs(maxA) < std::numeric_limits<std::double_t>::min())
                    return std::nullopt;

                if (imax != i)
                {
                    j = P[i];
                    P[i] = P[imax];
                    P[imax] = j;

                    A.SwapRows(i, imax);
                    ++P[_rowCount];
                }

                for (j = i + 1; j < _rowCount; ++j)
                {
                    A._table[j * A._columnCount + i] /= A._table[i * A._columnCount + i];

                    for (k = i + 1; k < _rowCount; ++k)
                        A._table[j * A._columnCount + k] -=
                                A._table[j * A._columnCount + i] * A._table[i * A._columnCount + k];
                }
            }

            return std::make_pair(std::move(A), std::move(P));
        }

        T LUPDet(const std::vector<std::size_t>& P) const noexcept
        requires Concepts::Multiply<T> && Concepts::Negative<T>
        {
            T det = _table[0];

            for (std::size_t i = 1; i < _rowCount; ++i)
                det *= _table[i * _columnCount + i];

            return ((long long) P[_rowCount] - (long long) _rowCount) % 2 == 0 ? det : -det;
        }

        Matrix LUPInvert(const std::vector<std::size_t>& P) const noexcept
        requires Concepts::ConstructableFromNumber<T> && Concepts::Substitute<T> &&
                 Concepts::Multiply<T> && Concepts::Divisible<T>
        {
            Matrix IA(_rowCount, _columnCount);

            for (std::size_t j = 0; j < _rowCount; ++j)
            {
                for (std::size_t i = 0; i < _rowCount; ++i)
                {
                    IA._table[i * IA._columnCount + j] = P[i] == j ? T(1) : T(0);

                    for (std::size_t k = 0; k < i; k++)
                        IA._table[i * IA._columnCount + j] -=
                                _table[i * _columnCount + k] * IA._table[k * IA._columnCount + j];
                }

                for (long long i = _rowCount - 1; i >= 0; --i)
                {
                    for (std::size_t k = i + 1; k < _rowCount; ++k)
                        IA._table[i * IA._columnCount + j] -=
                                _table[i * _columnCount + k] * IA._table[k * IA._columnCount + j];

                    IA._table[i * IA._columnCount + j] /= _table[i * _columnCount + i];
                }
            }

            return std::move(IA);
        }

        [[deprecated]]
        Matrix Gauss() const noexcept
        requires Concepts::Divisible<T> && Concepts::Multiply<T> && Concepts::Substitute<T>
        {
            Matrix copy(*this);

            for (std::size_t i = 0; i < _rowCount - 1; ++i)
                for (std::size_t row = i + 1; row < _rowCount; ++row)
                {
                    if (std::abs(copy._table[i * _columnCount + i]) < std::numeric_limits<std::double_t>::min())
                        continue;
                    T k = copy._table[row * _columnCount + i] / copy._table[i * _columnCount + i];
                    for (std::size_t j = 0; j < _columnCount; ++j)
                        copy._table[row * _columnCount + j] -= k * copy._table[i * _columnCount + j];
                    copy._table[row * _columnCount + i] = 0;
                }

            for (long long i = _rowCount - 1; i > 0; --i)
                for (long long row = i - 1; row > -1; --row)
                {
                    if (std::abs(copy._table[i * _columnCount + i]) < std::numeric_limits<std::double_t>::min())
                        continue;
                    T k = copy._table[row * _columnCount + i] / copy._table[i * _columnCount + i];
                    for (std::size_t j = 0; j < _columnCount; ++j)
                        copy._table[row * _columnCount + j] -= k * copy._table[i * _columnCount + j];
                    copy._table[row * _columnCount + i] = 0;
                }

            return std::move(copy);
        }

        [[deprecated]]
        [[nodiscard]]
        std::vector<T> LUPSolve(const std::vector<std::size_t>& P, const std::vector<T>& B) const noexcept
        requires Concepts::Substitute<T> && Concepts::Divisible<T> &&
                 Concepts::Multiply<T> && std::is_default_constructible_v<T>
        {
            std::vector<T> X(_rowCount);

            for (std::size_t i = 0; i < _rowCount; ++i)
            {
                X[i] = B[P[i]];

                for (std::size_t k = 0; k < i; ++k)
                    X[i] -= _table[i * _columnCount + k] * X[k];
            }

            for (long long i = _rowCount - 1; i >= 0; --i)
            {
                for (std::size_t k = i + 1; k < _rowCount; ++k)
                    X[i] -= _table[i * _columnCount + k] * X[k];

                X[i] /= _table[i * _columnCount + i];
            }

            return std::move(X);
        }

        [[nodiscard]]
        std::size_t NewDimension(std::size_t value) const noexcept
        {
            std::size_t result = 1;
            while (result < value)
                result <<= 1;
            return result;
        }

        std::array<Matrix, 4> SplitMatrix() const noexcept
        {
            const std::size_t size = _rowCount / 2;

            Matrix matrix1(size, size);
            Matrix matrix2(size, size);
            Matrix matrix3(size, size);
            Matrix matrix4(size, size);

            for (std::size_t i = 0; i < size; ++i)
                for (std::size_t j = 0; j < size; ++j)
                    matrix1._table[i * matrix1._columnCount + j] = _table[i * _columnCount + j];

            for (std::size_t i = 0; i < size; ++i)
                for (std::size_t j = size; j < _rowCount; ++j)
                    matrix2._table[i * matrix3._columnCount + j - size] = _table[i * _columnCount + j];

            for (std::size_t i = size; i < _rowCount; ++i)
                for (std::size_t j = 0; j < size; ++j)
                    matrix3._table[(i - size) * matrix2._columnCount + j] = _table[i * _columnCount + j];

            for (std::size_t i = size; i < _rowCount; ++i)
                for (std::size_t j = size; j < _rowCount; ++j)
                    matrix4._table[(i - size) * matrix4._columnCount + j - size] = _table[i * _columnCount + j];

            return { matrix1, matrix2, matrix3, matrix4 };
        }

        static Matrix CollectMatrix(std::array<Matrix, 4> parts) noexcept
        {
            const std::size_t size = parts[0]._rowCount;

            Matrix matrix(size * 2, size * 2);

            for (std::size_t i = 0; i < size; ++i)
                for (std::size_t j = 0; j < size; ++j)
                {
                    matrix._table[i * matrix._columnCount + j] = parts[0]._table[i * size + j];
                    matrix._table[i * matrix._columnCount + j + size] = parts[1]._table[i * size + j];
                    matrix._table[(i + size) * matrix._columnCount + j] = parts[2]._table[i * size + j];
                    matrix._table[(i + size) * matrix._columnCount + j + size] = parts[3]._table[i * size + j];
                }

            return std::move(matrix);
        }

        Matrix StrassenMultiply(const Matrix& matrix) const noexcept
        {
            std::size_t newDimension = NewDimension(std::max({_rowCount, _columnCount, matrix._columnCount}));

            if (newDimension <= 64)
                return MultiplyTranspose(matrix);

            Matrix left(*this);
            left.Resize(newDimension, newDimension);
            std::array<Matrix, 4> A = left.SplitMatrix();

            Matrix right(matrix);
            right.Resize(newDimension, newDimension);
            std::array<Matrix, 4> B = right.SplitMatrix();

            Matrix a11 = std::move(A[0]);
            Matrix a12 = std::move(A[1]);
            Matrix a21 = std::move(A[2]);
            Matrix a22 = std::move(A[3]);

            Matrix b11 = std::move(B[0]);
            Matrix b12 = std::move(B[1]);
            Matrix b21 = std::move(B[2]);
            Matrix b22 = std::move(B[3]);

            Matrix p1 = ((a11 + a22).value() * (b11 + b22).value()).value();
            Matrix p2 = ((a21 + a22).value() * b11).value();
            Matrix p3 = (a11 * (b12 - b22).value()).value();
            Matrix p4 = (a22 * (b21 - b11).value()).value();
            Matrix p5 = ((a11 + a12).value() * b22).value();
            Matrix p6 = ((a21 - a11).value() * (b11 + b12).value()).value();
            Matrix p7 = ((a12 - a22).value() * (b21 + b22).value()).value();

            Matrix c11 = ((p1 + p4).value() + (p7 - p5).value()).value();
            Matrix c12 = (p3 + p5).value();
            Matrix c21 = (p2 + p4).value();
            Matrix c22 = ((p1 - p2).value() + (p3 + p6).value()).value();

            return CollectMatrix({ c11, c12, c21, c22 });
        }

        Matrix StrassenMultiplyParallel(const Matrix& matrix) const noexcept
        {
            std::size_t newDimension = NewDimension(std::max({_rowCount, _columnCount, matrix._columnCount}));

            if (newDimension <= 64)
                return MultiplyTranspose(matrix);

            std::future<std::array<Matrix, 4>> taskLeft = std::async(std::launch::async, [this, newDimension]
            {
                Matrix left(*this);
                left.Resize(newDimension, newDimension);
                return left.SplitMatrix();
            });

            std::future<std::array<Matrix, 4>> taskRight = std::async(std::launch::async, [this, &matrix, newDimension]
            {
                Matrix right(matrix);
                right.Resize(newDimension, newDimension);
                return right.SplitMatrix();
            });

            std::array<Matrix, 4> A = taskLeft.get();
            std::array<Matrix, 4> B = taskRight.get();

            Matrix a11 = std::move(A[0]);
            Matrix a12 = std::move(A[1]);
            Matrix a21 = std::move(A[2]);
            Matrix a22 = std::move(A[3]);

            Matrix b11 = std::move(B[0]);
            Matrix b12 = std::move(B[1]);
            Matrix b21 = std::move(B[2]);
            Matrix b22 = std::move(B[3]);

            std::future<Matrix> taskP1 = std::async(std::launch::async, [&a11, &a22, &b11, &b22]
            { return ((a11 + a22).value() * (b11 + b22).value()).value(); });

            std::future<Matrix> taskP2 = std::async(std::launch::async, [&a21, &a22, &b11]
            { return ((a21 + a22).value() * b11).value(); });

            std::future<Matrix> taskP3 = std::async(std::launch::async, [&a11, &b12, &b22]
            { return (a11 * (b12 - b22).value()).value(); });

            std::future<Matrix> taskP4 = std::async(std::launch::async, [&a22, &b21, &b11]
            { return (a22 * (b21 - b11).value()).value(); });

            std::future<Matrix> taskP5 = std::async(std::launch::async, [&a11, &a12, &b22]
            { return ((a11 + a12).value() * b22).value(); });

            std::future<Matrix> taskP6 = std::async(std::launch::async, [&a21, &a11, &b11, &b12]
            { return  ((a21 - a11).value() * (b11 + b12).value()).value(); });

            std::future<Matrix> taskP7 = std::async(std::launch::async, [&a12, &a22, &b21, &b22]
            { return ((a12 - a22).value() * (b21 + b22).value()).value(); });

            Matrix p1 = taskP1.get();
            Matrix p2 = taskP2.get();
            Matrix p3 = taskP3.get();
            Matrix p4 = taskP4.get();
            Matrix p5 = taskP5.get();
            Matrix p6 = taskP6.get();
            Matrix p7 = taskP7.get();

            std::future<Matrix> taskC11 = std::async(std::launch::async, [&p1, &p4, &p7, &p5]
            { return ((p1 + p4).value() + (p7 - p5).value()).value(); });

            std::future<Matrix> taskC12 = std::async(std::launch::async, [&p3, &p5]
            { return (p3 + p5).value(); });

            std::future<Matrix> taskC21 = std::async(std::launch::async, [&p2, &p4]
            { return (p2 + p4).value(); });

            std::future<Matrix> taskC22 = std::async(std::launch::async, [&p1, &p2, &p3, &p6]
            { return ((p1 - p2).value() + (p3 + p6).value()).value(); });

            Matrix c11 = taskC11.get();
            Matrix c12 = taskC12.get();
            Matrix c21 = taskC21.get();
            Matrix c22 = taskC22.get();

            return CollectMatrix({ c11, c12, c21, c22 });
        }

        Matrix MultiplyTranspose(const Matrix& matrix) const noexcept
        requires std::is_default_constructible_v<T> && Concepts::Multiply<T> && Concepts::Summarize<T>
        {
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
    };

    template<typename T>
    std::ostream& operator<< (std::ostream& stream, const Matrix<T>& matrix)
    {
        stream << matrix.ToString();
        return stream;
    }

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
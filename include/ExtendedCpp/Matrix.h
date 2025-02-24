#ifndef Common_Matrix_H
#define Common_Matrix_H

#include <vector>
#include <stdexcept>
#include <cmath>
#include <cstdint>
#include <optional>
#include <functional>
#include <future>
#include <array>
#include <algorithm>
#include <string>
#include <format>

#include <ExtendedCpp/Concepts.h>

/// @brief Namespace for extended C++ utilities
namespace ExtendedCpp
{
	/// @brief A class representing a mathematical matrix
	/// @tparam T The type of elements stored in the matrix
	template<typename T>
	class Matrix final
	{
	private:
		std::vector<T> _table{}; ///< The underlying storage for matrix elements
		std::size_t _rowCount{}; ///< The number of rows in the matrix
		std::size_t _columnCount{}; ///< The number of columns in the matrix

	public:
		/// @brief Constructs a matrix with the specified number of rows and columns
		/// @param rowCount The number of rows
		/// @param columnCount The number of columns
		Matrix(const std::size_t rowCount, const std::size_t columnCount) 
		noexcept(std::is_nothrow_default_constructible_v<T>)
		requires std::is_default_constructible_v<T>
		{
			_rowCount = rowCount;
			_columnCount = columnCount;
			_table.resize(_rowCount * _columnCount);
		}

		/// @brief Constructs a matrix with the specified number of rows and columns, initializing elements using a callable
		/// @tparam TInit The type of the callable used for initialization
		/// @param rowCount The number of rows
		/// @param columnCount The number of columns
		/// @param init The callable used to initialize elements 
		template<std::invocable TInit>
		Matrix(const std::size_t rowCount, const std::size_t columnCount, TInit&& init)
		noexcept(std::is_nothrow_invocable_v<TInit>)
		{
			_rowCount = rowCount;
			_columnCount = columnCount;
			_table.reserve(_rowCount * _columnCount);

			for (std::size_t i = 0; i < _rowCount; ++i)
				for (std::size_t j = 0; j < _columnCount; ++j)
					_table.push_back(init());
		}

		/// @brief Constructs a matrix with the specified number of rows and columns, initializing elements using a callable with indices
		/// @tparam TInit The type of the callable used for initialization
		/// @param rowCount The number of rows
		/// @param columnCount The number of columns
		/// @param init The callable used to initialize elements, taking row and column indices as parameters
		template<typename TInit>
		requires std::invocable<TInit, std::size_t, std::size_t>
		Matrix(const std::size_t rowCount, const std::size_t columnCount, TInit&& init)
		noexcept(std::is_nothrow_invocable_v<TInit, std::size_t, std::size_t>)
		{
			_rowCount = rowCount;
			_columnCount = columnCount;
			_table.reserve(_rowCount * _columnCount);

			for (std::size_t i = 0; i < _rowCount; ++i)
				for (std::size_t j = 0; j < _columnCount; ++j)
					_table.push_back(init(i, j));
		}

		/// @brief Copy constructor
		/// @param matrix The matrix to copy from
		Matrix(const Matrix& matrix) 
		noexcept(std::is_nothrow_copy_assignable_v<std::vector<T>>)
		{
			_rowCount = matrix._rowCount;
			_columnCount = matrix._columnCount;
			_table = matrix._table;
		}

		/// @brief Move constructor
		/// @param matrix The matrix to move from
		Matrix(Matrix&& matrix) 
		noexcept(std::is_nothrow_move_assignable_v<std::vector<T>>)
		{
			_rowCount = matrix._rowCount;
			_columnCount = matrix._columnCount;
			_table = std::move(matrix._table);
		}

		/// @brief Constructs a matrix from a 2D vector
		/// @param matrix The 2D vector to copy from
		explicit Matrix(const std::vector<std::vector<T>>& matrix) 
		noexcept(std::is_nothrow_default_constructible_v<T> && std::is_nothrow_copy_assignable_v<T>)
		requires std::is_default_constructible_v<T> && std::is_copy_assignable_v<T>
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

		/// @brief Constructs a matrix from a 2D vector, moving elements
		/// @param matrix The 2D vector to move from
		explicit Matrix(std::vector<std::vector<T>>&& matrix) 
		noexcept(std::is_nothrow_default_constructible_v<T> && std::is_nothrow_move_assignable_v<T>)
		requires std::is_default_constructible_v<T> && std::is_move_assignable_v<T>
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

		/// @brief Default destructor
		~Matrix() = default;

		/// @brief Copy assignment operator
		/// @param matrix The matrix to copy from
		/// @return Reference to the current matrix
		Matrix& operator=(const Matrix& matrix) 
		noexcept(std::is_nothrow_copy_assignable_v<std::vector<T>>) = default;

		/// @brief Move assignment operator
		/// @param matrix The matrix to move from
		/// @return Reference to the current matrix
		Matrix& operator=(Matrix&& matrix) 
		noexcept(std::is_nothrow_move_assignable_v<std::vector<T>>)
		{
			_rowCount = matrix._rowCount;
			_columnCount = matrix._columnCount;
			_table = std::move(matrix._table);
			return *this;
		}

		/// @brief Copy assignment operator from a 2D vector
		/// @param matrix The 2D vector to copy from
		/// @return Reference to the current matrix 
		Matrix& operator=(const std::vector<std::vector<T>>& matrix) 
		noexcept(std::is_nothrow_default_constructible_v<T> && std::is_nothrow_copy_assignable_v<T>)
		requires std::is_default_constructible_v<T> && std::is_copy_assignable_v<T>
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

		/// @brief Move assignment operator from a 2D vector
		/// @param matrix The 2D vector to move from
		/// @return Reference to the current matrix
		Matrix& operator=(std::vector<std::vector<T>>&& matrix) 
		noexcept(std::is_nothrow_default_constructible_v<T> && std::is_nothrow_move_assignable_v<T>)
		requires std::is_default_constructible_v<T> && std::is_move_assignable_v<T>
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

		/// @brief Equality operator
		/// @param matrix The matrix to compare with
		/// @return True if the matrices are equal, false otherwise
		bool operator==(const Matrix& matrix) const noexcept
		requires Concepts::Equatable<T>
		{
			if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
				return false;

			for (std::size_t i = 0; i < _rowCount; ++i)
				for (std::size_t j = 0; j < _columnCount; ++j)
					if (_table[i * _columnCount + j] != matrix._table[i * _columnCount + j]) 
						return false;

			return true;
		}

		/// @brief Inequality operator
		/// @param matrix The matrix to compare with
		/// @return True if the matrices are not equal, false otherwise
		bool operator!=(const Matrix& matrix) const noexcept
		requires Concepts::Equatable<T>
		{
			if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
				return true;

			for (std::size_t i = 0; i < _rowCount; ++i)
				for (std::size_t j = 0; j < _columnCount; ++j)
					if (_table[i * _columnCount + j] == matrix._table[i * _columnCount + j]) 
						return false;

			return true;
		}

		/// @brief Safely sums two matrices
		/// @param matrix The matrix to add
		/// @return An optional containing the result if the matrices have the same size, std::nullopt otherwise
		std::optional<Matrix> SafeSum(const Matrix& matrix) const
		noexcept(std::is_nothrow_copy_constructible_v<Matrix> && std::is_nothrow_copy_assignable_v<T>)
		requires Concepts::Summarize<T> && std::is_copy_assignable_v<T>
		{
			if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
				return std::nullopt;

			Matrix result(*this);
			for (std::size_t i = 0; i < _rowCount; ++i)
				for (std::size_t j = 0; j < _columnCount; ++j)
					result._table[i * _columnCount + j] += matrix._table[i * _columnCount + j];

			return result;
		}

		/// @brief Sums two matrices
		/// @param matrix The matrix to add
		/// @return The result of the sum
		/// @throws std::invalid_argument if the matrices have different sizes
		Matrix Sum(const Matrix& matrix) const
		requires Concepts::Summarize<T> && std::is_copy_assignable_v<T>
		{
			if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
				throw std::invalid_argument("Left and right matrix have different size.");

			Matrix result(*this);
			for (std::size_t i = 0; i < _rowCount; ++i)
				for (std::size_t j = 0; j < _columnCount; ++j)
					result._table[i * _columnCount + j] += matrix._table[i * _columnCount + j];

			return result;
		}

		/// @brief Addition operator
		/// @param matrix The matrix to add
		/// @return The result of the addition
		Matrix operator+(const Matrix& matrix) const
		{
			return Sum(matrix);
		}

		/// @brief Safely subtracts two matrices
		/// @param matrix The matrix to subtract
		/// @return An optional containing the result if the matrices have the same size, std::nullopt otherwise
		std::optional<Matrix> SafeSubstitute(const Matrix& matrix) const 
		noexcept(std::is_nothrow_copy_constructible_v<Matrix> && std::is_nothrow_copy_assignable_v<T>)
		requires Concepts::Substitute<T> && std::is_copy_assignable_v<T>
		{
			if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
				return std::nullopt;

			Matrix result(*this);
			for (std::size_t i = 0; i < _rowCount; ++i)
				for (std::size_t j = 0; j < _columnCount; ++j)
					result._table[i * _columnCount + j] -= matrix._table[i * _columnCount + j];

			return result;
		}

		/// @brief Subtracts two matrices
		/// @param matrix The matrix to subtract
		/// @return The result of the subtraction
		/// @throws std::invalid_argument if the matrices have different sizes
		Matrix Substitute(const Matrix& matrix) const
		requires Concepts::Substitute<T> && std::is_copy_assignable_v<T>
		{
			if (_rowCount != matrix._rowCount || _columnCount != matrix._columnCount)
				throw std::invalid_argument("Left and right matrix have different size.");

			Matrix result(*this);
			for (std::size_t i = 0; i < _rowCount; ++i)
				for (std::size_t j = 0; j < _columnCount; ++j)
					result._table[i * _columnCount + j] -= matrix._table[i * _columnCount + j];

			return result;
		}

		/// @brief Subtraction operator
		/// @param matrix The matrix to subtract
		/// @return The result of the subtraction
		Matrix operator-(const Matrix& matrix) const
		{
			return Substitute(matrix);
		}

		/// @brief Safely multiplies two matrices
		/// @param matrix The matrix to multiply with
		/// @param asParallel Whether to perform the multiplication in parallel
		/// @return An optional containing the result if the matrices can be multiplied, std::nullopt otherwise
		std::optional<Matrix> SafeMultiply(const Matrix& matrix, const bool asParallel = true) const noexcept
		{
			if (_columnCount != matrix._rowCount)
				return std::nullopt;

			if (asParallel)
			{
				Matrix result = StrassenMultiplyParallel(matrix);
				result.Resize(_rowCount, matrix._columnCount);
				return result;
			}
			else
			{
				Matrix result = StrassenMultiply(matrix);
				result.Resize(_rowCount, matrix._columnCount);
				return result;
			}
		}

		/// @brief Multiplies two matrices
		/// @param matrix The matrix to multiply with
		/// @param asParallel Whether to perform the multiplication in parallel
		/// @return The result of the multiplication
		/// @throws std::invalid_argument if the matrices cannot be multiplied
		Matrix Multiply(const Matrix& matrix, const bool asParallel = true) const
		{
			if (_columnCount != matrix._rowCount)
				throw std::invalid_argument("Column count of left matrix and row count of right matrix must be equal.");
			
			if (asParallel)
			{
				Matrix result = StrassenMultiplyParallel(matrix);
				result.Resize(_rowCount, matrix._columnCount);
				return result;
			}
			else
			{
				Matrix result = StrassenMultiply(matrix);
				result.Resize(_rowCount, matrix._columnCount);
				return result;
			}
		}

		/// @brief Multiplication operator
		/// @param matrix The matrix to multiply with
		/// @return The result of the multiplication
		Matrix operator*(const Matrix& matrix) const
		{
			return Multiply(matrix, true);
		}

		/// @brief Multiplies the matrix by a scalar
		/// @param alpha The scalar to multiply with
		/// @return The result of the multiplication
		Matrix Multiply(T&& alpha) const 
		noexcept(std::is_nothrow_copy_constructible_v<Matrix> && std::is_nothrow_copy_assignable_v<T>)
		requires Concepts::Multiply<T> && std::is_copy_assignable_v<T>
		{
			Matrix result(*this);
			for (std::size_t i = 0; i < _rowCount; ++i)
				for (std::size_t j = 0; j < _columnCount; ++j)
					result._table[i * _columnCount + j] *= std::forward<T>(alpha);

			return result;
		}

		/// @brief Multiplication operator with a scalar
		/// @param alpha The scalar to multiply with
		/// @return The result of the multiplication
		Matrix operator*(T&& alpha) const 
		noexcept(std::is_nothrow_invocable_v<decltype(&Matrix::Multiply), T&&>)
		{
			return Multiply(std::forward<T>(alpha));
		}

		/// @brief Multiplies the matrix by a scalar
		/// @param alpha The scalar to multiply with
		/// @return The result of the multiplication
		Matrix Multiply(T alpha) const 
		noexcept(std::is_nothrow_copy_constructible_v<Matrix> && std::is_nothrow_copy_assignable_v<T>)
		requires Concepts::Multiply<T> && std::is_copy_assignable_v<T>
		{
			Matrix result(*this);
			for (std::size_t i = 0; i < _rowCount; ++i)
				for (std::size_t j = 0; j < _columnCount; ++j)
					result._table[i * _columnCount + j] *= alpha;

			return result;
		}

        /// @brief Multiplies the matrix by a scalar
        /// @param alpha The scalar to multiply with
        /// @return The result of the multiplication
        Matrix operator*(T alpha) const
        noexcept(std::is_nothrow_invocable_v<decltype(&Matrix::Multiply), T>)
        {
			return Multiply(alpha);
        }

        /// @brief Accesses a specific row of the matrix
        /// @param rowNumber The index of the row to access
        /// @return A vector containing the elements of the specified row
        /// @throws std::out_of_range if the row number is out of range
        std::vector<T> operator[](const std::size_t rowNumber) const
        {
			if (rowNumber >= _rowCount)
				throw std::out_of_range(std::format("Row number {} > matrix rows which is {}.", rowNumber, _rowCount));

			return std::vector<T>(_table.begin() + (rowNumber * _columnCount),
				_table.begin() + (rowNumber * _columnCount + _columnCount));
        }

        /// @brief Transposes the matrix
        /// @return The transposed matrix
        Matrix Transpose() const 
        noexcept(std::is_nothrow_constructible_v<Matrix, std::size_t, std::size_t> && std::is_nothrow_copy_assignable_v<T>)
        requires std::is_copy_assignable_v<T>
        {
			Matrix result(_columnCount, _rowCount);
			for (std::size_t i = 0; i < _rowCount; ++i)
				for (std::size_t j = 0; j < _columnCount; ++j)
					result._table[j * _rowCount + i] = _table[i * _columnCount + j];

			return result;
        }

        /// @brief Erases a row from the matrix without bounds checking
        /// @param rowNumber The index of the row to erase
        void EraseRowUnchecked(const std::size_t rowNumber) noexcept
        {
			_table.erase(_table.begin() + rowNumber * _columnCount, _table.begin() + rowNumber * _columnCount + _rowCount);
			--_rowCount;
        }

        /// @brief Erases a row from the matrix with bounds checking
        /// @param rowNumber The index of the row to erase
        /// @throws std::out_of_range if the row number is out of range
        void EraseRow(const std::size_t rowNumber)
        {
			if (rowNumber >= _rowCount)
				throw std::out_of_range(std::format("Row number {} > matrix rows which is {}.", rowNumber, _rowCount));
			if (_rowCount == 0) 
				return;

			_table.erase(_table.begin() + rowNumber * _columnCount, _table.begin() + rowNumber * _columnCount + _rowCount);
			--_rowCount;
        }

        /// @brief Erases a column from the matrix without bounds checking
        /// @param columnNumber The index of the column to erase
        void EraseColumnUnchecked(const std::size_t columnNumber) noexcept
        {
			for (std::size_t i = 0, j = _rowCount - 1; i < _rowCount; ++i, --j)
				_table.erase(_table.begin() + j * _columnCount + columnNumber);
			--_columnCount;
        }

        /// @brief Erases a column from the matrix with bounds checking
        /// @param columnNumber The index of the column to erase
        /// @throws std::out_of_range if the column number is out of range
        void EraseColumn(const std::size_t columnNumber)
        {
			if (columnNumber >= _columnCount)
				throw std::out_of_range(std::format("Column number {} > matrix columns which is {}.", columnNumber, _columnCount));
			if (_columnCount == 0) 
				return;

			for (std::size_t i = 0, j = _rowCount - 1; i < _rowCount; ++i, --j)
				_table.erase(_table.begin() + j * _columnCount + columnNumber);
			--_columnCount;
        }

        /// @brief Calculates the determinant of the matrix
        /// @return An optional containing the determinant if the matrix is square, std::nullopt otherwise
        [[nodiscard]]
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

        /// @brief Calculates the inverse of the matrix safely
        /// @return An optional containing the inverse matrix if the matrix is square, std::nullopt otherwise
        [[nodiscard]]
        std::optional<Matrix> SafeInverse() const noexcept
        {
			if (_rowCount != _columnCount || _rowCount == 0 || _columnCount == 0)
				return std::nullopt;

			const auto decomposeResult = LUPDecompose();
			if (!decomposeResult.has_value())
				return std::nullopt;

			const auto [A, P] = decomposeResult.value();
			return A.LUPInvert(P);
        }

        /// @brief Calculates the inverse of the matrix
        /// @return The inverse matrix
        /// @throws std::domain_error if the matrix is not square or the inverse cannot be calculated
        [[nodiscard]]
        Matrix Inverse() const
        {
			if (_rowCount != _columnCount || _rowCount == 0 || _columnCount == 0)
				throw std::domain_error("Finding the determinant is only possible for a square matrix");

			const auto decomposeResult = LUPDecompose();
			if (!decomposeResult.has_value())
				throw std::domain_error("Inverse matrix cannot be calculated.");

			const auto [A, P] = decomposeResult.value();
			return A.LUPInvert(P);
        }

        /// @brief Calculates the inverse of the matrix using the bitwise NOT operator
        /// @return The inverse matrix
        Matrix operator~() const
        {
			return Inverse();
        }

        /// @brief Negates the matrix
        /// @return The negated matrix
        Matrix operator-() const 
        noexcept(std::is_nothrow_copy_constructible_v<Matrix> && std::is_nothrow_copy_assignable_v<T>)
        requires Concepts::Negative<T> && std::is_copy_assignable_v<T>
        {
			Matrix copy(*this);
			for (std::size_t i = 0; i < _rowCount * _columnCount; ++i)
				copy._table[i] = -copy._table[i];
			return copy;
        }

        /// @brief Swaps two rows of the matrix without bounds checking
        /// @param row1Number The index of the first row
        /// @param row2Number The index of the second row
        void SwapRowsUnchecked(const std::size_t row1Number, const std::size_t row2Number) noexcept
        {
			for (std::size_t i = 0; i < _columnCount; ++i)
				std::swap(_table[row1Number * _columnCount + i], _table[row2Number * _columnCount + i]);
        }

        /// @brief Swaps two rows of the matrix with bounds checking
        /// @param row1Number The index of the first row
        /// @param row2Number The index of the second row
        /// @throws std::out_of_range if either row index is out of range
        void SwapRows(const std::size_t row1Number, const std::size_t row2Number)
        {
			if (row1Number == row2Number)
				return;
			if (row1Number >= _rowCount || row2Number >= _rowCount)
				throw std::out_of_range("row1Number and row2Number must be less then matrix row count.");

			for (std::size_t i = 0; i < _columnCount; ++i)
				std::swap(_table[row1Number * _columnCount + i], _table[row2Number * _columnCount + i]);
        }

        /// @brief Swaps two columns of the matrix without bounds checking
        /// @param column1Number The index of the first column
        /// @param column2Number The index of the second column
        void SwapColumnsUnchecked(const std::size_t column1Number, const std::size_t column2Number) noexcept
        {
			for (std::size_t i = 0; i < _rowCount; ++i)
				std::swap(_table[i * _columnCount + column1Number], _table[i * _columnCount + column2Number]);
        }

        /// @brief Swaps two columns of the matrix with bounds checking
        /// @param column1Number The index of the first column
        /// @param column2Number The index of the second column
        /// @throws std::out_of_range if either column index is out of range
        void SwapColumns(const std::size_t column1Number, const std::size_t column2Number)
        {
			if (column1Number == column2Number)
				return;
			if (column1Number >= _columnCount || column2Number >= _columnCount)
				throw std::out_of_range("column1Number and column2Number must be less then matrix column count.");

			for (std::size_t i = 0; i < _rowCount; ++i)
				std::swap(_table[i * _columnCount + column1Number], _table[i * _columnCount + column2Number]);
        }

        /// @brief Calculates the rank of the matrix
        /// @return The rank of the matrix
        [[nodiscard]]
        std::size_t Rank() const noexcept(std::is_nothrow_copy_constructible_v<Matrix> && std::is_nothrow_copy_assignable_v<T>)
        requires Concepts::Divisible<T> && Concepts::Multiply<T> && Concepts::Substitute<T> && std::is_copy_assignable_v<T>
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
							T multiplier = copy._table[columnIndex * _columnCount + rowIndex] /
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

        /// @brief Returns the number of rows in the matrix
        /// @return The number of rows
        [[nodiscard]]
        std::size_t RowCount() const noexcept
        {
			return _rowCount;
        }

        /// @brief Returns the number of columns in the matrix
        /// @return The number of columns
        [[nodiscard]]
        std::size_t ColumnCount() const noexcept
        {
			return _columnCount;
        }

        /// @brief Returns a specific row of the matrix without bounds checking
        /// @param rowNumber The index of the row to access
        /// @return A vector containing the elements of the specified row
        [[nodiscard]]
        std::vector<T> GetRowUnchecked(const std::size_t rowNumber) const noexcept
        {
			return std::vector<T>(_table.begin() + (rowNumber * _columnCount),
				_table.begin() + (rowNumber * _columnCount + _columnCount));
        }

        /// @brief Returns a specific row of the matrix with bounds checking
        /// @param rowNumber The index of the row to access
        /// @return A vector containing the elements of the specified row
        /// @throws std::out_of_range if the row number is out of range
        [[nodiscard]]
        std::vector<T> GetRow(const std::size_t rowNumber) const
        {
			if (rowNumber >= _rowCount)
				throw std::out_of_range(std::format("Row number {} > matrix rows which is {}.", rowNumber, _rowCount));

			return std::vector<T>(_table.begin() + (rowNumber * _columnCount),
				_table.begin() + (rowNumber * _columnCount + _columnCount));
        }

        /// @brief Sets a specific row of the matrix with a new row
        /// @param newRow The new row to set
        /// @param rowNumber The index of the row to set
        /// @throws std::out_of_range if the row number is out of range
        void SetRow(const std::vector<T>& newRow, const std::size_t rowNumber) 
        noexcept(std::is_nothrow_copy_assignable_v<T>)
        requires std::is_copy_assignable_v<T>
        {
			if (newRow.size() < _columnCount || rowNumber >= _rowCount)
				return;

			for (std::size_t i = 0; i < _columnCount; ++i)
				_table[rowNumber * _columnCount + i] = newRow[i];
        }

        /// @brief Sets a specific row of the matrix with a new row, moving elements
        /// @param newRow The new row to set
        /// @param rowNumber The index of the row to set
        /// @throws std::out_of_range if the row number is out of range
        void SetRow(std::vector<T>&& newRow, const std::size_t rowNumber) 
        noexcept(std::is_nothrow_move_assignable_v<T>)
        requires std::is_move_assignable_v<T>
        {
			if (newRow.size() < _columnCount || rowNumber >= _rowCount)
				return;

			for (std::size_t i = 0; i < _columnCount; ++i)
				_table[rowNumber * _columnCount + i] = std::move(newRow[i]);
        }

        /// @brief Returns a specific column of the matrix without bounds checking
        /// @param columnNumber The index of the column to access
        /// @return A vector containing the elements of the specified column
        [[nodiscard]]
        std::vector<T> GetColumnUnchecked(const std::size_t columnNumber) const noexcept
        {
			std::vector<T> column;
			column.reserve(_rowCount);
			for (std::size_t i = 0; i < _rowCount; ++i)
				column.push_back(_table[i * _columnCount + columnNumber]);
			return column;
        }

        /// @brief Returns a specific column of the matrix with bounds checking
        /// @param columnNumber The index of the column to access
        /// @return A vector containing the elements of the specified column
        /// @throws std::out_of_range if the column number is out of range
        [[nodiscard]]
        std::vector<T> GetColumn(const std::size_t columnNumber) const
        {
			if (columnNumber >= _columnCount)
				throw std::out_of_range(std::format("Column number {} > matrix columns which is {}.", columnNumber, _columnCount));

			std::vector<T> column;
			column.reserve(_rowCount);
			for (std::size_t i = 0; i < _rowCount; ++i)
				column.push_back(_table[i * _columnCount + columnNumber]);
			return column;
        }

        /// @brief Sets a specific column of the matrix with a new column
        /// @param newColumn The new column to set
        /// @param columnNumber The index of the column to set
        /// @throws std::out_of_range if the column number is out of range
        void SetColumn(const std::vector<T>& newColumn, const std::size_t columnNumber) 
        noexcept(std::is_nothrow_copy_assignable_v<T>)
        requires std::is_copy_assignable_v<T>
        {
			if (newColumn.size() < _rowCount || columnNumber >= _columnCount)
				return;

			for (std::size_t i = 0; i < _rowCount; ++i)
				_table[i * _columnCount + columnNumber] = newColumn[i];
        }

        /// @brief Sets a specific column of the matrix with a new column, moving elements
        /// @param newColumn The new column to set
        /// @param columnNumber The index of the column to set
        /// @throws std::out_of_range if the column number is out of range
        void SetColumn(std::vector<T>&& newColumn, const std::size_t columnNumber) 
        noexcept(std::is_nothrow_move_assignable_v<T>)
        requires std::is_move_assignable_v<T>
        {
			if (newColumn.size() < _rowCount || columnNumber >= _columnCount)
				return;

			for (std::size_t i = 0; i < _rowCount; ++i)
				_table[i * _columnCount + columnNumber] = std::move(newColumn[i]);
        }

        /// @brief Returns a specific element of the matrix without bounds checking
        /// @param i The row index of the element
        /// @param j The column index of the element
        /// @return The element at the specified position
        T GetElementUnchecked(const std::size_t i, const std::size_t j) const noexcept
        {
			return _table[i * _columnCount + j];
        }

        /// @brief Returns a specific element of the matrix with bounds checking
        /// @param i The row index of the element
        /// @param j The column index of the element
        /// @return The element at the specified position
        /// @throws std::out_of_range if the row or column index is out of range
        T GetElement(const std::size_t i, const std::size_t j) const
        {
			if (i >= _rowCount || j >= _columnCount)
				throw std::out_of_range(
					std::format("i which is {} must be less then matrix row count which is {}, ", i, _rowCount) +
					std::format("j which is {} must be less then matrix column count which is {}", j, _columnCount));
			return _table[i * _columnCount + j];
        }

        /// @brief Returns a reference to a specific element of the matrix without bounds checking
        /// @param i The row index of the element
        /// @param j The column index of the element
        /// @return A reference to the element at the specified position
        const T& GetElementRefUnchecked(const std::size_t i, const std::size_t j) const noexcept
        {
			return _table[i * _columnCount + j];
        }

        /// @brief Returns a reference to a specific element of the matrix with bounds checking
        /// @param i The row index of the element
        /// @param j The column index of the element
        /// @return A reference to the element at the specified position
        /// @throws std::out_of_range if the row or column index is out of range
        const T& GetElementRef(const std::size_t i, const std::size_t j) const
        {
			if (i >= _rowCount || j >= _columnCount)
				throw std::out_of_range(
					std::format("i which is {} must be less then matrix row count which is {}, ", i, _rowCount) +
					std::format("j which is {} must be less then matrix column count which is {}", j, _columnCount));
			return _table[i * _columnCount + j];
        }

        /// @brief Returns a reference to a specific element of the matrix without bounds checking
        /// @param i The row index of the element
        /// @param j The column index of the element
        /// @return A reference to the element at the specified position
        T& GetElementRefUnchecked(const std::size_t i, const std::size_t j) noexcept
        {
			return _table[i * _columnCount + j];
        }

        /// @brief Returns a reference to a specific element of the matrix with bounds checking
        /// @param i The row index of the element
        /// @param j The column index of the element
        /// @return A reference to the element at the specified position
        /// @throws std::out_of_range if the row or column index is out of range
        T& GetElementRef(const std::size_t i, const std::size_t j)
        {
			if (i >= _rowCount || j >= _columnCount)
				throw std::out_of_range(
					std::format("i which is {} must be less then matrix row count which is {}, ", i, _rowCount) +
					std::format("j which is {} must be less then matrix column count which is {}", j, _columnCount));
			return _table[i * _columnCount + j];
        }

        /// @brief Sets a specific element of the matrix with a new value
        /// @param newValue The new value to set
        /// @param i The row index of the element
        /// @param j The column index of the element
        /// @throws std::out_of_range if the row or column index is out of range
        void SetElement(const T& newValue, const std::size_t i, const std::size_t j)
        noexcept(std::is_nothrow_copy_assignable_v<T>)
        requires std::is_copy_assignable_v<T>
        {
			if (i >= _rowCount || j >= _columnCount)
				return;
			_table[i * _columnCount + j] = newValue;
        }

        /// @brief Sets a specific element of the matrix with a new value, moving the value
        /// @param newValue The new value to set
        /// @param i The row index of the element
        /// @param j The column index of the element
        /// @throws std::out_of_range if the row or column index is out of range
        void SetElement(T&& newValue, const std::size_t i, const std::size_t j)
        noexcept(std::is_nothrow_move_assignable_v<T>)
        requires std::is_move_assignable_v<T>
        {
			if (i >= _rowCount || j >= _columnCount)
				return;
			_table[i * _columnCount + j] = std::move(newValue);
        }

        /// @brief Resizes the matrix to the specified number of rows and columns
        /// @param rowCount The new number of rows
        /// @param columnCount The new number of columns
        void Resize(const std::size_t rowCount, const std::size_t columnCount) 
        noexcept(std::is_nothrow_default_constructible_v<T> && std::is_nothrow_move_assignable_v<T>)
        requires std::is_default_constructible_v<T> && std::is_move_assignable_v<T>
        {
			std::vector<T> newTable(rowCount * columnCount);
			for (std::size_t i = 0; i < _rowCount && i < rowCount; ++i)
				for (std::size_t j = 0; j < _columnCount && j < columnCount; ++j)
					newTable[i * columnCount + j] = std::move(_table[i * _columnCount + j]);

			_table = std::move(newTable);
			_rowCount = rowCount;
			_columnCount = columnCount;
        }

        /// @brief Solves the system of linear equations using LUP decomposition
        /// @param P The permutation vector
        /// @param B The right-hand side vector
        /// @return The solution vector
        std::vector<T> LUPSolve(const std::vector<std::size_t>& P, const std::vector<T>& B) const 
        noexcept (std::is_nothrow_default_constructible_v<T> && std::is_nothrow_copy_assignable_v<T>)
        requires std::is_default_constructible_v<T> && std::is_copy_assignable_v<T> &&
        Concepts::Substitute<T> && Concepts::Divisible<T> && Concepts::Multiply<T>
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

			return X;
        }

        /// @brief Converts the matrix to a string representation
        /// @return A string representation of the matrix
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

			return matrixString;
        }

	private:
		std::optional<std::pair<Matrix, std::vector<std::size_t>>> LUPDecompose() const 
		noexcept(std::is_nothrow_default_constructible_v<T>)
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
				maxA = static_cast<T>(0);
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

					A.SwapRowsUnchecked(i, imax);
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

		T LUPDet(const std::vector<std::size_t>& P) const 
		noexcept(std::is_nothrow_copy_assignable_v<T>)
		requires Concepts::Multiply<T> && Concepts::Negative<T> && std::is_copy_assignable_v<T>
		{
			T det = _table[0];

			for (std::size_t i = 1; i < _rowCount; ++i)
				det *= _table[i * _columnCount + i];

			return ((long long) P[_rowCount] - (long long) _rowCount) % 2 == 0 ? det : -det;
		}

		Matrix LUPInvert(const std::vector<std::size_t>& P) const 
		noexcept(std::is_nothrow_constructible_v<Matrix, std::size_t, std::size_t> && std::is_nothrow_copy_assignable_v<T>)
		requires std::is_copy_assignable_v<T> && Concepts::Substitute<T> && Concepts::Multiply<T> && Concepts::Divisible<T>
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

			return IA;
		}

		[[deprecated]]
		Matrix Gauss() const 
		noexcept(std::is_nothrow_copy_constructible_v<Matrix> && std::is_nothrow_copy_assignable_v<T>)
		requires std::is_copy_assignable_v<T> && Concepts::Divisible<T> && Concepts::Multiply<T> && Concepts::Substitute<T>
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

			return copy;
		}

		[[nodiscard]]
		std::size_t NewDimension(std::size_t value) const noexcept
		{
			std::size_t result = 1;
			while (result < value)
				result <<= 1;
			return result;
		}

		std::array<Matrix, 4> SplitMatrix() const 
		noexcept(std::is_nothrow_constructible_v<Matrix, std::size_t, std::size_t> && std::is_nothrow_copy_assignable_v<T>)
		requires std::is_copy_assignable_v<T>
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

			return { std::move(matrix1), std::move(matrix2), std::move(matrix3), std::move(matrix4) };
		}

		std::array<Matrix, 4> SplitMatrixParallel() const 
		noexcept(std::is_nothrow_constructible_v<Matrix, std::size_t, std::size_t> && std::is_nothrow_copy_assignable_v<T>)
		requires std::is_copy_assignable_v<T>
		{
			auto task1 = std::async(std::launch::async, [this]()
			{
				const std::size_t size = _rowCount / 2;
				Matrix matrix1(size, size);
				for (std::size_t i = 0; i < size; ++i)
					for (std::size_t j = 0; j < size; ++j)
						matrix1._table[i * matrix1._columnCount + j] = _table[i * _columnCount + j];
				return matrix1;
			});

			auto task2 = std::async(std::launch::async, [this]()
			{
				const std::size_t size = _rowCount / 2;
				Matrix matrix2(size, size);
				for (std::size_t i = 0; i < size; ++i)
					for (std::size_t j = size; j < _rowCount; ++j)
						matrix2._table[i * matrix2._columnCount + j - size] = _table[i * _columnCount + j];
				return matrix2;
			});

			auto task3 = std::async(std::launch::async, [this]()
			{
				const std::size_t size = _rowCount / 2;
				Matrix matrix3(size, size);
				for (std::size_t i = size; i < _rowCount; ++i)
					for (std::size_t j = 0; j < size; ++j)
						matrix3._table[(i - size) * matrix3._columnCount + j] = _table[i * _columnCount + j];
				return matrix3;
			});

			auto task4 = std::async(std::launch::async, [this]()
			{
				const std::size_t size = _rowCount / 2;
				Matrix matrix4(size, size);
				for (std::size_t i = size; i < _rowCount; ++i)
					for (std::size_t j = size; j < _rowCount; ++j)
						matrix4._table[(i - size) * matrix4._columnCount + j - size] = _table[i * _columnCount + j];
				return matrix4;
			});

			return { std::move(task1.get()), std::move(task2.get()), std::move(task3.get()), std::move(task4.get()) };
		}

		static Matrix CollectMatrix(std::array<Matrix, 4> parts) 
		noexcept(std::is_nothrow_constructible_v<Matrix, std::size_t, std::size_t> && std::is_nothrow_copy_assignable_v<T>)
		requires std::is_copy_assignable_v<T>
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

			return matrix;
		}

		Matrix StrassenMultiply(const Matrix& matrix) const 
		noexcept(std::is_nothrow_copy_constructible_v<Matrix> && std::is_nothrow_move_assignable_v<Matrix>)
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

			Matrix p1 = (a11 + a22) * (b11 + b22);
			Matrix p2 = ((a21 + a22) * b11);
			Matrix p3 = a11 * (b12 - b22);
			Matrix p4 = a22 * (b21 - b11);
			Matrix p5 = (a11 + a12) * b22;
			Matrix p6 = (a21 - a11) * (b11 + b12);
			Matrix p7 = (a12 - a22) * (b21 + b22);

			Matrix c11 = p1 + p4 + p7 - p5;
			Matrix c12 = p3 + p5;
			Matrix c21 = p2 + p4;
			Matrix c22 = p1 - p2 + p3 + p6;

			return CollectMatrix({ std::move(c11), std::move(c12), std::move(c21), std::move(c22) });
		}

		Matrix StrassenMultiplyParallel(const Matrix& matrix) const 
		noexcept(std::is_nothrow_copy_constructible_v<Matrix> && std::is_nothrow_move_assignable_v<Matrix>)
		{
			std::size_t newDimension = NewDimension(std::max({_rowCount, _columnCount, matrix._columnCount}));

			if (newDimension <= 64)
				return MultiplyTranspose(matrix);

			std::future<std::array<Matrix, 4>> taskLeft = std::async(std::launch::async, [this, newDimension]
			{
				Matrix left(*this);
				left.Resize(newDimension, newDimension);
				return left.SplitMatrixParallel();
			});

			std::future<std::array<Matrix, 4>> taskRight = std::async(std::launch::async, [&matrix, newDimension]
			{
				Matrix right(matrix);
				right.Resize(newDimension, newDimension);
				return right.SplitMatrixParallel();
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
			{ return (a11 + a22) * (b11 + b22); });

			std::future<Matrix> taskP2 = std::async(std::launch::async, [&a21, &a22, &b11]
			{ return (a21 + a22) * b11; });

			std::future<Matrix> taskP3 = std::async(std::launch::async, [&a11, &b12, &b22]
			{ return a11 * (b12 - b22); });

			std::future<Matrix> taskP4 = std::async(std::launch::async, [&a22, &b21, &b11]
			{ return a22 * (b21 - b11); });

			std::future<Matrix> taskP5 = std::async(std::launch::async, [&a11, &a12, &b22]
			{ return (a11 + a12) * b22; });

			std::future<Matrix> taskP6 = std::async(std::launch::async, [&a21, &a11, &b11, &b12]
			{ return  (a21 - a11) * (b11 + b12); });

			std::future<Matrix> taskP7 = std::async(std::launch::async, [&a12, &a22, &b21, &b22]
			{ return (a12 - a22) * (b21 + b22); });

			Matrix p1 = taskP1.get();
			Matrix p2 = taskP2.get();
			Matrix p3 = taskP3.get();
			Matrix p4 = taskP4.get();
			Matrix p5 = taskP5.get();
			Matrix p6 = taskP6.get();
			Matrix p7 = taskP7.get();

			std::future<Matrix> taskC11 = std::async(std::launch::async, [&p1, &p4, &p7, &p5]
			{ return p1 + p4 + p7 - p5; });

			std::future<Matrix> taskC12 = std::async(std::launch::async, [&p3, &p5]
			{ return p3 + p5; });

			std::future<Matrix> taskC21 = std::async(std::launch::async, [&p2, &p4]
			{ return p2 + p4; });

			std::future<Matrix> taskC22 = std::async(std::launch::async, [&p1, &p2, &p3, &p6]
			{ return p1 - p2 + p3 + p6; });

			Matrix c11 = taskC11.get();
			Matrix c12 = taskC12.get();
			Matrix c21 = taskC21.get();
			Matrix c22 = taskC22.get();

			return CollectMatrix({ std::move(c11), std::move(c12), std::move(c21), std::move(c22) });
		}

		Matrix MultiplyTranspose(const Matrix& matrix) const 
		noexcept(std::is_nothrow_constructible_v<Matrix, std::size_t, std::size_t> && std::is_nothrow_copy_assignable_v<Matrix> &&
			std::is_nothrow_default_constructible_v<T> && std::is_nothrow_copy_assignable_v<T>)
		requires std::is_default_constructible_v<T> && std::is_copy_assignable_v<T> && 
			Concepts::Multiply<T> && Concepts::Summarize<T>
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

			return result;
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

#define ZERO_MATRIX_F64(rowCount, columnCount) \
ExtendedCpp::MatrixF64(rowCount, columnCount)

#define ZERO_MATRIX_F32(rowCount, columnCount) \
ExtendedCpp::MatrixF32(rowCount, columnCount)

#define ZERO_MATRIX_I64(rowCount, columnCount) \
ExtendedCpp::MatrixI64(rowCount, columnCount)

#define ZERO_MATRIX_I32(rowCount, columnCount) \
ExtendedCpp::MatrixI32(rowCount, columnCount)

#define ZERO_MATRIX_I16(rowCount, columnCount) \
ExtendedCpp::MatrixI16(rowCount, columnCount)

#define ZERO_MATRIX_I8(rowCount, columnCount) \
ExtendedCpp::MatrixI8(rowCount, columnCount)

#define ZERO_MATRIX_U64(rowCount, columnCount) \
ExtendedCpp::MatrixU64(rowCount, columnCount)

#define ZERO_MATRIX_U32(rowCount, columnCount) \
ExtendedCpp::MatrixU32(rowCount, columnCount)

#define ZERO_MATRIX_U16(rowCount, columnCount) \
ExtendedCpp::MatrixU16(rowCount, columnCount)

#define ZERO_MATRIX_U8(rowCount, columnCount) \
ExtendedCpp::MatrixU8(rowCount, columnCount)

#define E_MATRIX_F64(rowCount, columnCount) \
[]() \
{ \
	ExtendedCpp::MatrixF64 matrix(rowCount, columnCount); \
	for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
		matrix.SetElement(1, i, j); \
	return matrix; \
}() \

#define E_MATRIX_F32(rowCount, columnCount) \
[]() \
{ \
	ExtendedCpp::MatrixF32 matrix(rowCount, columnCount); \
	for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
		matrix.SetElement(1, i, j); \
	return matrix; \
}() \

#define E_MATRIX_I64(rowCount, columnCount) \
[]() \
{ \
	ExtendedCpp::MatrixI64 matrix(rowCount, columnCount); \
	for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
		matrix.SetElement(1, i, j); \
	return matrix; \
}() \

#define E_MATRIX_I32(rowCount, columnCount) \
[]() \
{ \
	ExtendedCpp::MatrixI32 matrix(rowCount, columnCount); \
	for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
		matrix.SetElement(1, i, j); \
	return matrix; \
}() \

#define E_MATRIX_I16(rowCount, columnCount) \
[]() \
{ \
	ExtendedCpp::MatrixI16 matrix(rowCount, columnCount); \
	for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
		matrix.SetElement(1, i, j); \
	return matrix; \
}() \

#define E_MATRIX_I8(rowCount, columnCount) \
[]() \
{ \
	ExtendedCpp::MatrixI8 matrix(rowCount, columnCount); \
	for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
		matrix.SetElement(1, i, j); \
	return matrix; \
}() \

#define E_MATRIX_U64(rowCount, columnCount) \
[]() \
{ \
	ExtendedCpp::MatrixU64 matrix(rowCount, columnCount); \
	for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
		matrix.SetElement(1, i, j); \
	return matrix; \
}() \

#define E_MATRIX_U32(rowCount, columnCount) \
[]() \
{ \
	ExtendedCpp::MatrixU32 matrix(rowCount, columnCount); \
	for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
		matrix.SetElement(1, i, j); \
	return matrix; \
}() \

#define E_MATRIX_U16(rowCount, columnCount) \
[]() \
{ \
	ExtendedCpp::MatrixU16 matrix(rowCount, columnCount); \
	for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
		matrix.SetElement(1, i, j); \
	return matrix; \
}() \

#define E_MATRIX_U8(rowCount, columnCount) \
[]() \
{ \
	ExtendedCpp::MatrixU8 matrix(rowCount, columnCount); \
	for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
		matrix.SetElement(1, i, j); \
	return matrix; \
}() \

#endif
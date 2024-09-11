#ifndef ExtendedCpp_Common_Macro_HPP
#define ExtendedCpp_Common_Macro_HPP

import ExtendedCpp.Common.Matrix;

#define ZERO_MATRIX_F64(rowCount, columnCount) \
ExtendedCpp::Common::MatrixF64(rowCount, columnCount)

#define ZERO_MATRIX_F32(rowCount, columnCount) \
ExtendedCpp::Common::MatrixF32(rowCount, columnCount)

#define ZERO_MATRIX_I64(rowCount, columnCount) \
ExtendedCpp::Common::MatrixI64(rowCount, columnCount)

#define ZERO_MATRIX_I32(rowCount, columnCount) \
ExtendedCpp::Common::MatrixI32(rowCount, columnCount)

#define ZERO_MATRIX_I16(rowCount, columnCount) \
ExtendedCpp::Common::MatrixI16(rowCount, columnCount)

#define ZERO_MATRIX_I8(rowCount, columnCount) \
ExtendedCpp::Common::MatrixI8(rowCount, columnCount)

#define ZERO_MATRIX_U64(rowCount, columnCount) \
ExtendedCpp::Common::MatrixU64(rowCount, columnCount)

#define ZERO_MATRIX_U32(rowCount, columnCount) \
ExtendedCpp::Common::MatrixU32(rowCount, columnCount)

#define ZERO_MATRIX_U16(rowCount, columnCount) \
ExtendedCpp::Common::MatrixU16(rowCount, columnCount)

#define ZERO_MATRIX_U8(rowCount, columnCount) \
ExtendedCpp::Common::MatrixU8(rowCount, columnCount)

#define E_MATRIX_F64(rowCount, columnCount) \
[]() \
{ \
    ExtendedCpp::Common::MatrixF64 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}()

#define E_MATRIX_F32(rowCount, columnCount) \
[]() \
{ \
    ExtendedCpp::Common::MatrixF32 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}()

#define E_MATRIX_I64(rowCount, columnCount) \
[]() \
{ \
    ExtendedCpp::Common::MatrixI64 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}()

#define E_MATRIX_I32(rowCount, columnCount) \
[]() \
{ \
    ExtendedCpp::Common::MatrixI32 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}()

#define E_MATRIX_I16(rowCount, columnCount) \
[]() \
{ \
    ExtendedCpp::Common::MatrixI16 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}()

#define E_MATRIX_I8(rowCount, columnCount) \
[]() \
{ \
    ExtendedCpp::Common::MatrixI8 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}()

#define E_MATRIX_U64(rowCount, columnCount) \
[]() \
{ \
    ExtendedCpp::Common::MatrixU64 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}()

#define E_MATRIX_U32(rowCount, columnCount) \
[]() \
{ \
    ExtendedCpp::Common::MatrixU32 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}()

#define E_MATRIX_U16(rowCount, columnCount) \
[]() \
{ \
    ExtendedCpp::Common::MatrixU16 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}()

#define E_MATRIX_U8(rowCount, columnCount) \
[]() \
{ \
    ExtendedCpp::Common::MatrixU8 matrix(rowCount, columnCount); \
    for (std::size_t i = 0, j = 0; i < (rowCount) && j < (columnCount); ++i, ++j) \
        matrix.SetElement(1, i, j); \
    return matrix; \
}()

#endif
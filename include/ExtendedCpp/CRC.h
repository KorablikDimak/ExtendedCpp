#ifndef COMMON_CRC_H
#define COMMON_CRC_H

#include <array>
#include <climits>
#include <limits>
#include <span>

namespace ExtendedCpp
{
    /// @brief
    /// @tparam Rank
    template<std::uint8_t Rank>
    struct RankType final
    {
        using Type = decltype([]() constexpr
        {
            if constexpr (Rank <= CHAR_BIT * sizeof(std::uint8_t))
                return std::uint8_t{};
            else if constexpr (Rank > CHAR_BIT * sizeof(std::uint8_t) && Rank <= CHAR_BIT * sizeof(std::uint16_t))
                return std::uint16_t{};
            else if constexpr (Rank > CHAR_BIT * sizeof(std::uint16_t) && Rank <= CHAR_BIT * sizeof(std::uint32_t))
                return std::uint32_t{};
            else if constexpr (Rank > CHAR_BIT * sizeof(std::uint32_t) && Rank <= CHAR_BIT * sizeof(std::uint64_t))
                return std::uint64_t{};
            else
                static_assert(false, "Unsupported rank of polynom");
        }());
    };

    /// @brief
    /// @tparam Rank
    /// @tparam Polynom
    /// @tparam InitRegister
    /// @tparam XorOut
    // ReSharper disable once CppInconsistentNaming
    template<std::uint8_t Rank, typename RankType<Rank>::Type Polynom,
             typename RankType<Rank>::Type InitRegister, typename RankType<Rank>::Type XorOut,
             bool ReflectIn = false, bool ReflectOut = false>
    requires (Rank >= CHAR_BIT && Rank <= CHAR_BIT * sizeof(std::uint64_t))
    class CRC final
    {
    private:
        using PolynomType = typename RankType<Rank>::Type;

        static constexpr PolynomType GetPolynomMask() noexcept
        {
            if constexpr (sizeof(PolynomType) * CHAR_BIT == Rank)
                return std::numeric_limits<PolynomType>::max();
            else
                return (PolynomType(1) << Rank) - 1;
        }

        static constexpr PolynomType POLYNOM_MASK = GetPolynomMask();
        static constexpr std::size_t TABLE_SIZE = std::numeric_limits<std::uint8_t>::max() + 1;

        static consteval std::array<PolynomType, TABLE_SIZE> EvalCrcTable() noexcept
        {
            std::array<PolynomType, TABLE_SIZE> table{};

            if constexpr (ReflectIn)
                for (std::size_t i = 0; i < TABLE_SIZE; ++i)
                {
                    auto crc = static_cast<PolynomType>(i);

                    for (std::uint8_t bit = 0; bit < CHAR_BIT; ++bit)
                    {
                        if (crc & PolynomType(1))
                            crc = (crc >> 1) ^ ReflectBits(Polynom, Rank);
                        else
                            crc >>= 1;
                    }

                    table[i] = crc & POLYNOM_MASK; // NOLINT(*-pro-bounds-constant-array-index)
                }
            else
                for (std::size_t i = 0; i < TABLE_SIZE; ++i)
                {
                    PolynomType crc = static_cast<PolynomType>(i) << (Rank - CHAR_BIT);

                    for (std::uint8_t bit = 0; bit < CHAR_BIT; ++bit)
                    {
                        if (crc & (PolynomType(1) << (Rank - 1)))
                            crc = (crc << 1) ^ Polynom;
                        else
                            crc <<= 1;
                    }

                    table[i] = crc & POLYNOM_MASK; // NOLINT(*-pro-bounds-constant-array-index)
                }

            return table;
        }

        static constexpr std::array<PolynomType, TABLE_SIZE> CRC_TABLE = EvalCrcTable();

        static_assert(Polynom != 0);
        static_assert((Polynom & ~POLYNOM_MASK) == 0);
        static_assert((InitRegister & ~POLYNOM_MASK) == 0);
        static_assert((XorOut & ~POLYNOM_MASK) == 0);

        static constexpr PolynomType ReflectBits(PolynomType value, const std::uint8_t bitCount) noexcept
        {
            PolynomType result = 0;

            for (std::uint8_t bit = 0; bit < bitCount; ++bit)
            {
                result = static_cast<PolynomType>((result << 1) | (value & PolynomType(1)));
                value >>= 1;
            }

            return result;
        }

    public:
        /// @brief
        /// @param data
        /// @return
        static constexpr PolynomType Eval(const std::span<const std::byte> data) noexcept
        {
            PolynomType crc = InitRegister;

            for (const std::byte byte : data)
            {
                if constexpr (ReflectIn)
                {
                    const auto index = static_cast<std::uint8_t>(crc ^ std::to_integer<std::uint8_t>(byte));
                    crc = (crc >> CHAR_BIT) ^ CRC_TABLE[index]; // NOLINT(*-pro-bounds-constant-array-index)
                }
                else
                {
                    const auto index = static_cast<std::uint8_t>((crc >> (Rank - CHAR_BIT)) ^ std::to_integer<std::uint8_t>(byte));
                    crc = (crc << CHAR_BIT) ^ CRC_TABLE[index]; // NOLINT(*-pro-bounds-constant-array-index)
                }

                crc &= POLYNOM_MASK;
            }

            if constexpr (ReflectIn != ReflectOut)
                crc = ReflectBits(crc, Rank);

            if constexpr (XorOut != 0)
                crc ^= XorOut;

            return crc;
        }
    };

    // NOLINTBEGIN(*-magic-numbers)
    // ReSharper disable CppInconsistentNaming

    using CRC12_DECT = CRC<12, 0x80F, 0x000, 0x000>;
    using CRC15_CAN = CRC<15, 0x4599, 0x0000, 0x0000>;
    using CRC64_ECMA = CRC<64, 0x42F0E1EBA9EA3693, 0x0000000000000000, 0x0000000000000000>;
    using CRC11_FLEXRAY = CRC<11, 0x385, 0x01A, 0x000>;
    using CRC30_CDMA = CRC<30, 0x2030B9C7, 0x3FFFFFFF, 0x3FFFFFFF>;
    using CRC24_LTEA = CRC<24, 0x864CFB, 0x000000, 0x000000>;
    using CRC8_SMBUS = CRC<8, 0x07, 0x00, 0x00>;
    using CRC8_SAE_J1850 = CRC<8, 0x1D, 0xFF, 0xFF>;
    using CRC8_AUTOSAR = CRC<8, 0x2F, 0xFF, 0xFF>;
    using CRC16_IBM_3740 = CRC<16, 0x1021, 0xFFFF, 0x0000>;
    using CRC16_XMODEM = CRC<16, 0x1021, 0x0000, 0x0000>;
    using CRC16_GENIBUS = CRC<16, 0x1021, 0xFFFF, 0xFFFF>;
    using CRC16_UMTS = CRC<16, 0x8005, 0x0000, 0x0000>;
    using CRC24_OPENPGP = CRC<24, 0x864CFB, 0xB704CE, 0x000000>;
    using CRC32_BZIP2 = CRC<32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF>;
    using CRC32_MPEG2 = CRC<32, 0x04C11DB7, 0xFFFFFFFF, 0x00000000>;
    using CRC64_WE = CRC<64, 0x42F0E1EBA9EA3693, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF>;
    using CRC16_XMODEM = CRC<16, 0x1021, 0x0000, 0x0000>;
    using CRC16_KERMIT = CRC<16, 0x1021, 0x0000, 0x0000, true, true>;
    using CRC16_MODBUS = CRC<16, 0x8005, 0xFFFF, 0x0000, true, true>;
    using CRC32_ISO_HDLC = CRC<32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true, true>;
    using CRC8_MAXIM_DOW = CRC<8, 0x31, 0x00, 0x00, true, true>;
    using CRC8_DVB_S2 = CRC<8, 0xD5, 0x00, 0x00>;
    using CRC8_ROHC = CRC<8, 0x07, 0xFF, 0x00, true, true>;
    using CRC16_USB = CRC<16, 0x8005, 0xFFFF, 0xFFFF, true, true>;
    using CRC16_X25 = CRC<16, 0x1021, 0xFFFF, 0xFFFF, true, true>;
    using CRC16_DNP = CRC<16, 0x3D65, 0x0000, 0xFFFF, true, true>;
    using CRC16_T10_DIF = CRC<16, 0x8BB7, 0x0000, 0x0000>;
    using CRC32C = CRC<32, 0x1EDC6F41, 0xFFFFFFFF, 0xFFFFFFFF, true, true>;
    using CRC32_AUTOSAR = CRC<32, 0xF4ACFB13, 0xFFFFFFFF, 0xFFFFFFFF, true, true>;
    using CRC32_JAMCRC = CRC<32, 0x04C11DB7, 0xFFFFFFFF, 0x00000000, true, true>;
    using CRC64_XZ = CRC<64, 0x42F0E1EBA9EA3693, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, true, true>;

    namespace Asserts
    {
        static constexpr std::array TEST_DATA = {
            std::byte{'1'}, std::byte{'2'}, std::byte{'3'},
            std::byte{'4'}, std::byte{'5'}, std::byte{'6'},
            std::byte{'7'}, std::byte{'8'}, std::byte{'9'} };

        static_assert(CRC12_DECT::Eval(TEST_DATA) == 0xF5B);
        static_assert(CRC15_CAN::Eval(TEST_DATA) == 0x59E);
        static_assert(CRC64_ECMA::Eval(TEST_DATA) == 0x6C40DF5F0B497347);
        static_assert(CRC11_FLEXRAY::Eval(TEST_DATA) == 0x5A3);
        static_assert(CRC30_CDMA::Eval(TEST_DATA) == 0x04C34ABF);
        static_assert(CRC24_LTEA::Eval(TEST_DATA) == 0xCDE703);
        static_assert(CRC8_SMBUS::Eval(TEST_DATA) == 0xF4);
        static_assert(CRC8_SAE_J1850::Eval(TEST_DATA) == 0x4B);
        static_assert(CRC8_AUTOSAR::Eval(TEST_DATA) == 0xDF);
        static_assert(CRC16_IBM_3740::Eval(TEST_DATA) == 0x29B1);
        static_assert(CRC16_XMODEM::Eval(TEST_DATA) == 0x31C3);
        static_assert(CRC16_GENIBUS::Eval(TEST_DATA) == 0xD64E);
        static_assert(CRC16_UMTS::Eval(TEST_DATA) == 0xFEE8);
        static_assert(CRC24_OPENPGP::Eval(TEST_DATA) == 0x21CF02);
        static_assert(CRC32_BZIP2::Eval(TEST_DATA) == 0xFC891918);
        static_assert(CRC32_MPEG2::Eval(TEST_DATA) == 0x0376E6E7);
        static_assert(CRC64_WE::Eval(TEST_DATA) == 0x62EC59E3F1A4F00A);
        static_assert(CRC16_XMODEM::Eval(TEST_DATA) == 0x31C3);
        static_assert(CRC16_KERMIT::Eval(TEST_DATA) == 0x2189);
        static_assert(CRC16_MODBUS::Eval(TEST_DATA) == 0x4B37);
        static_assert(CRC32_ISO_HDLC::Eval(TEST_DATA) == 0xCBF43926);
        static_assert(CRC8_MAXIM_DOW::Eval(TEST_DATA) == 0xA1);
        static_assert(CRC8_DVB_S2::Eval(TEST_DATA) == 0xBC);
        static_assert(CRC8_ROHC::Eval(TEST_DATA) == 0xD0);
        static_assert(CRC16_USB::Eval(TEST_DATA) == 0xB4C8);
        static_assert(CRC16_X25::Eval(TEST_DATA) == 0x906E);
        static_assert(CRC16_DNP::Eval(TEST_DATA) == 0xEA82);
        static_assert(CRC16_T10_DIF::Eval(TEST_DATA) == 0xD0DB);
        static_assert(CRC32C::Eval(TEST_DATA) == 0xE3069283);
        static_assert(CRC32_AUTOSAR::Eval(TEST_DATA) == 0x1697D06A);
        static_assert(CRC32_JAMCRC::Eval(TEST_DATA) == 0x340BC6D9);
        static_assert(CRC64_XZ::Eval(TEST_DATA) == 0x995DC9BBDF1939FA);
    }

    // NOLINTEND(*-magic-numbers)
}

#endif
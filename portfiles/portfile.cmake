vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO KorablikDimak/ExtendedCpp
        REF "${VERSION}"
        SHA512 cae2d980f2c8deeabac95affda68d70ed5af8ca4f68ba92f60283146f1a8278767c5a2008559b3e53b0bac07ddf02584476cc468c0a98eb1f98e5db68d37246d
        HEAD_REF master)

vcpkg_cmake_configure(SOURCE_PATH "${SOURCE_PATH}")

vcpkg_cmake_install()
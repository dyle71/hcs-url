/*
 * This file is part of the headcode.space url.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_URL_VERSION_HPP
#define HEADCODE_SPACE_URL_VERSION_HPP

#include <cstdint>
#include <string>


#ifndef MAKE_VERSION
#define MAKE_VERSION(x, y, z) ((x) << 24 | (y) << 16 | (z))
#endif


/**
 * @brief   Version check for 0.1.0
 * @return  A value, representing the version.
 */
inline std::uint32_t GetHCSUrlVersion_0_1_0() {
    return MAKE_VERSION(0, 1, 0);
}

/**
 * @brief   The headcode url namespace
 */
namespace headcode::url {

/**
 * @brief   Returns the current version of the headcode-url.
 * @return  A value, representing the current version.
 */
inline std::uint32_t GetCurrentVersion() {
    return GetHCSUrlVersion_0_1_0();
}

/**
 * @brief   Returns a version as string
 * @return  a string holding the current version.
 */
inline std::string GetVersionString() {
    return "0.1.0";
}


}


#endif

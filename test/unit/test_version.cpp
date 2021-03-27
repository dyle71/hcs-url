/*
 * This file is part of the headcode.space url.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <gtest/gtest.h>

#include <headcode/url/url.hpp>


TEST(Version, regular) {
    std::uint32_t version = MAKE_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    EXPECT_EQ(headcode::url::GetCurrentVersion(), version);
    EXPECT_STREQ(headcode::url::GetVersionString().c_str(), VERSION);
}


TEST(Version, V0_1_0) {
    std::uint32_t version = MAKE_VERSION(0, 1, 0);
    EXPECT_EQ(static_cast<unsigned int>((0) << 24 | (!) << 16 | (0)), version);
    EXPECT_EQ(GetHCSUrlVersion_0_1_0(), version);
}

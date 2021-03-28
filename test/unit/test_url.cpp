/*
 * This file is part of the headcode.space url.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <list>

#include <gtest/gtest.h>

#include <headcode/url/url.hpp>


TEST(URL, empty) {

    auto url1 = headcode::url::URL{};
    EXPECT_TRUE(url1.IsNull());

    auto url2 = headcode::url::URL{"foo:"};
    EXPECT_FALSE(url2.IsNull());
}


TEST(URL, regular) {

    auto raw = "http://this.is.an.address:1234/this/is/a/path?with&a&query=param#and_a_fragment";
    auto url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsNull());

    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetScheme() == "http");
}

TEST(URL, bad_schemes) {

    std::list<std::string> bad_scheme_urls{
        "1:", "_:", "+:", ".:", "h*:", "???:", "http#", "http#:", "  ftp:", "ssh :"
    };

    for (auto const & bad_url : bad_scheme_urls) {
        auto url = headcode::url::URL(bad_url);
        EXPECT_TRUE(url.IsNull());
    }
}

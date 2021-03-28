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
    EXPECT_TRUE(url.GetAuthority() == "this.is.an.address:1234");
    EXPECT_TRUE(url.GetPath() == "/this/is/a/path");

    raw = "ftp://ftp.is.co.za/rfc/rfc1808.txt";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetScheme() == "ftp");
    EXPECT_TRUE(url.GetAuthority() == "ftp.is.co.za");
    EXPECT_TRUE(url.GetPath() == "/rfc/rfc1808.txt");

    raw = "http://www.ietf.org/rfc/rfc2396.txt";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetScheme() == "http");
    EXPECT_TRUE(url.GetAuthority() == "www.ietf.org");
    EXPECT_TRUE(url.GetPath() == "/rfc/rfc2396.txt");

    raw = "ldap://[2001:db8::7]/c=GB?objectClass?one";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetScheme() == "ldap");
    EXPECT_TRUE(url.GetAuthority() == "[2001:db8::7]");
    EXPECT_TRUE(url.GetPath() == "/c=GB");
    // TODO

    raw = "mailto:John.Doe@example.com";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetScheme() == "mailto");
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetPath() == "John.Doe@example.com");

    raw = "news:comp.infosystems.www.servers.unix";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetScheme() == "news");
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetPath() == "comp.infosystems.www.servers.unix");

    raw = "tel:+1-816-555-1212";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetScheme() == "tel");
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetPath() == "+1-816-555-1212");

    raw = "telnet://192.0.2.16:80/";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetScheme() == "telnet");
    EXPECT_TRUE(url.GetAuthority() == "192.0.2.16:80");
    EXPECT_TRUE(url.GetPath() == "/");

    raw = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetScheme() == "urn");
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetPath() == "oasis:names:specification:docbook:dtd:xml:4.1.2");
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

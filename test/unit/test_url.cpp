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
    EXPECT_EQ(url1.GetError(), headcode::url::ParseError::kURLEmpty);

    auto url2 = headcode::url::URL{"foo:"};
    EXPECT_FALSE(url2.IsNull());
    EXPECT_EQ(url2.GetError(), headcode::url::ParseError::kNoError);
}


TEST(URL, regular) {

    auto raw = "url://user@email:password@address:1234/this/is/a/path?with&a&query=param#and_a_fragment";
    auto url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsNull());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kNoError);

    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "user@email:password@address:1234");
    EXPECT_TRUE(url.GetHost() == "address");
    EXPECT_TRUE(url.GetPath() == "/this/is/a/path");
    EXPECT_TRUE(url.GetPort() == "1234");
    EXPECT_TRUE(url.GetScheme() == "url");
    EXPECT_TRUE(url.GetUserInfo() == "user@email:password");

    raw = "blah://";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetHost().empty());
    EXPECT_TRUE(url.GetPath().empty());
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "blah");
    EXPECT_TRUE(url.GetUserInfo().empty());

    raw = "blah:/";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetHost().empty());
    EXPECT_TRUE(url.GetPath() == "/");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "blah");
    EXPECT_TRUE(url.GetUserInfo().empty());

    raw = "blah:/absolute/path";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetHost().empty());
    EXPECT_TRUE(url.GetPath() == "/absolute/path");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "blah");
    EXPECT_TRUE(url.GetUserInfo().empty());

    raw = "ftp://ftp.is.co.za/rfc/rfc1808.txt";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "ftp.is.co.za");
    EXPECT_TRUE(url.GetHost() == "ftp.is.co.za");
    EXPECT_TRUE(url.GetPath() == "/rfc/rfc1808.txt");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "ftp");
    EXPECT_TRUE(url.GetUserInfo().empty());

    raw = "ftp://john.doe@ftp.is.co.za/rfc/rfc1808.txt";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "john.doe@ftp.is.co.za");
    EXPECT_TRUE(url.GetHost() == "ftp.is.co.za");
    EXPECT_TRUE(url.GetPath() == "/rfc/rfc1808.txt");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "ftp");
    EXPECT_TRUE(url.GetUserInfo() == "john.doe");

    raw = "https://[::1]";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "[::1]");
    EXPECT_TRUE(url.GetHost() == "::1");
    EXPECT_TRUE(url.GetPath().empty());
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "https");
    EXPECT_TRUE(url.GetUserInfo().empty());

    raw = "https://[v123ab.345:32:!(999a99]";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "[v123ab.345:32:!(999a99]");
    EXPECT_TRUE(url.GetHost() == "v123ab.345:32:!(999a99");
    EXPECT_TRUE(url.GetPath().empty());
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "https");
    EXPECT_TRUE(url.GetUserInfo().empty());

    raw = "git+ssh://www.%20ietf%ff";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "www.%20ietf%ff");
    EXPECT_TRUE(url.GetHost() == "www.%20ietf%ff");
    EXPECT_TRUE(url.GetPath().empty());
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "git+ssh");

    raw = "http://www.ietf.org/rfc/rfc2396.txt";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "www.ietf.org");
    EXPECT_TRUE(url.GetHost() == "www.ietf.org");
    EXPECT_TRUE(url.GetPath() == "/rfc/rfc2396.txt");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "http");
    EXPECT_TRUE(url.GetUserInfo().empty());

    raw = "ldap://[2001:db8::7]/c=GB?objectClass?one";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "[2001:db8::7]");
    EXPECT_TRUE(url.GetHost() == "2001:db8::7");
    EXPECT_TRUE(url.GetPath() == "/c=GB");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "ldap");
    EXPECT_TRUE(url.GetUserInfo().empty());

    raw = "mailto:John.Doe@example.com";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetHost().empty());
    EXPECT_TRUE(url.GetPath() == "John.Doe@example.com");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "mailto");
    EXPECT_TRUE(url.GetUserInfo().empty());

    raw = "news:comp.infosystems.www.servers.unix";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetHost().empty());
    EXPECT_TRUE(url.GetPath() == "comp.infosystems.www.servers.unix");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "news");
    EXPECT_TRUE(url.GetUserInfo().empty());

    raw = "tel:+1-816-555-1212";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetHost().empty());
    EXPECT_TRUE(url.GetPath() == "+1-816-555-1212");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "tel");
    EXPECT_TRUE(url.GetUserInfo().empty());

    raw = "telnet://192.0.2.16:80/";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "192.0.2.16:80");
    EXPECT_TRUE(url.GetHost() == "192.0.2.16");
    EXPECT_TRUE(url.GetPath() == "/");
    EXPECT_TRUE(url.GetPort() == "80");
    EXPECT_TRUE(url.GetScheme() == "telnet");
    EXPECT_TRUE(url.GetUserInfo().empty());

    raw = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetHost().empty());
    EXPECT_TRUE(url.GetPath() == "oasis:names:specification:docbook:dtd:xml:4.1.2");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "urn");
    EXPECT_TRUE(url.GetUserInfo().empty());

    raw = "https://user:password@some.host:999/path/to/resource#token";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "user:password@some.host:999");
    EXPECT_TRUE(url.GetHost() == "some.host");
    EXPECT_TRUE(url.GetPath() == "/path/to/resource");
    EXPECT_TRUE(url.GetPort() == "999");
    EXPECT_TRUE(url.GetScheme() == "https");
    EXPECT_TRUE(url.GetUserInfo() == "user:password");
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


TEST(URL, bad_userinfo) {

    auto raw = "http://this.is.an.address:1234/this/is/a/path?with&a&query=param#and_a_fragment";
    auto url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsNull());

    raw = "http://user @this.is.an.address:1234/this/is/a/path?with&a&query=param#and_a_fragment";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsNull());

    raw = "http://user%@this.is.an.address:1234/this/is/a/path?with&a&query=param#and_a_fragment";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsNull());

    raw = "http://user%0x@this.is.an.address:1234/this/is/a/path?with&a&query=param#and_a_fragment";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsNull());

    raw = "http://user$@this.is.an.address:1234/this/is/a/path?with&a&query=param#and_a_fragment";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsNull());
}


TEST(URL, bad_host) {

    auto raw = "http://this.is.an.address /";
    auto url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsNull());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidHost);

    raw = "http://123.4.";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsNull());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidHost);

    raw = "http://312.412.512.110";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsNull());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidHost);

    raw = "http://012.412.512.110";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsNull());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidHost);

    raw = "http://02.412.512.110";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsNull());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidHost);

    raw = "http://[::3";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsNull());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidHost);

    raw = "http://[]";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsNull());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidHost);
}

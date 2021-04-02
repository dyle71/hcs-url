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


TEST(url, normalize) {

    auto raw = "url://user:password@address:1234/this/is/a/path?with&a&query=param#and_a_fragment";
    auto url = headcode::url::URL{raw}.Normalize();
    EXPECT_TRUE(url.IsValid());
    EXPECT_TRUE(url.GetURL() == "url://user:password@address:1234/this/is/a/path?with&a&query=param#and_a_fragment");

    raw = "eXamPLE://us%65r:pa%20wor%7f@address:1234/this/is/a/path?with&a&query=param#and_a_fragment";
    url = headcode::url::URL{raw}.Normalize();
    EXPECT_TRUE(url.IsValid());
    auto u = url.GetURL();
    EXPECT_TRUE(url.GetURL() ==
                "example://user:pa%20wor%7F@address:1234/this/is/a/path?with&a&query=param#and_a_fragment");
}


TEST(URL, empty) {

    auto url1 = headcode::url::URL{};
    EXPECT_FALSE(url1.IsValid());
    EXPECT_EQ(url1.GetError(), headcode::url::ParseError::kURLEmpty);

    auto url2 = headcode::url::URL{"foo:"};
    EXPECT_TRUE(url2.IsValid());
    EXPECT_EQ(url2.GetError(), headcode::url::ParseError::kNoError);
}


TEST(URL, regular) {

    auto raw = "url://user:password@address:1234/this/is/a/path?with&a&query=param#and_a_fragment";
    auto url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kNoError);

    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "user:password@address:1234");
    EXPECT_TRUE(url.GetHost() == "address");
    EXPECT_TRUE(url.GetPath() == "/this/is/a/path");
    auto segments = url.GetSegments();
    ASSERT_EQ(segments.size(), 4u);
    EXPECT_TRUE(segments[0] == "this");
    EXPECT_TRUE(segments[1] == "is");
    EXPECT_TRUE(segments[2] == "a");
    EXPECT_TRUE(segments[3] == "path");
    EXPECT_TRUE(url.GetPathPart(0) == "/this");
    EXPECT_TRUE(url.IsPathAbsolute());
    EXPECT_TRUE(url.GetPathPart(1) == "/this/is");
    EXPECT_TRUE(url.GetPathPart(2) == "/this/is/a");
    EXPECT_TRUE(url.GetPathPart(3) == "/this/is/a/path");
    EXPECT_TRUE(url.GetPathPart(1000) == "/this/is/a/path");
    EXPECT_TRUE(url.GetPort() == "1234");
    EXPECT_TRUE(url.GetScheme() == "url");
    EXPECT_TRUE(url.GetUserInfo() == "user:password");
    EXPECT_TRUE(url.GetQuery() == "with&a&query=param");
    auto query_items = url.GetQueryItems();
    ASSERT_TRUE(query_items.size() == 3);
    EXPECT_TRUE(query_items[0] == "with");
    EXPECT_TRUE(query_items[1] == "a");
    EXPECT_TRUE(query_items[2] == "query=param");
    EXPECT_TRUE(url.GetFragment() == "and_a_fragment");

    raw = "url://user:password@address:1234/this/is/a/path#a_fragment";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kNoError);
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "user:password@address:1234");
    EXPECT_TRUE(url.GetHost() == "address");
    EXPECT_TRUE(url.GetPath() == "/this/is/a/path");
    EXPECT_TRUE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_EQ(segments.size(), 4u);
    EXPECT_TRUE(segments[0] == "this");
    EXPECT_TRUE(segments[1] == "is");
    EXPECT_TRUE(segments[2] == "a");
    EXPECT_TRUE(segments[3] == "path");
    EXPECT_TRUE(url.GetPathPart(0) == "/this");
    EXPECT_TRUE(url.GetPathPart(1) == "/this/is");
    EXPECT_TRUE(url.GetPathPart(2) == "/this/is/a");
    EXPECT_TRUE(url.GetPathPart(3) == "/this/is/a/path");
    EXPECT_TRUE(url.GetPathPart(1000) == "/this/is/a/path");
    EXPECT_TRUE(url.GetPort() == "1234");
    EXPECT_TRUE(url.GetScheme() == "url");
    EXPECT_TRUE(url.GetUserInfo() == "user:password");
    EXPECT_TRUE(url.GetQuery().empty());
    query_items = url.GetQueryItems();
    EXPECT_TRUE(query_items.empty());
    EXPECT_TRUE(url.GetFragment() == "a_fragment");

    raw = "url://user:password@address:1234/this/is/a/path?with&a&query=param";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kNoError);
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "user:password@address:1234");
    EXPECT_TRUE(url.GetHost() == "address");
    EXPECT_TRUE(url.GetPath() == "/this/is/a/path");
    EXPECT_TRUE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_EQ(segments.size(), 4u);
    EXPECT_TRUE(segments[0] == "this");
    EXPECT_TRUE(segments[1] == "is");
    EXPECT_TRUE(segments[2] == "a");
    EXPECT_TRUE(segments[3] == "path");
    EXPECT_TRUE(url.GetPathPart(0) == "/this");
    EXPECT_TRUE(url.GetPathPart(1) == "/this/is");
    EXPECT_TRUE(url.GetPathPart(2) == "/this/is/a");
    EXPECT_TRUE(url.GetPathPart(3) == "/this/is/a/path");
    EXPECT_TRUE(url.GetPathPart(1000) == "/this/is/a/path");
    EXPECT_TRUE(url.GetPort() == "1234");
    EXPECT_TRUE(url.GetScheme() == "url");
    EXPECT_TRUE(url.GetUserInfo() == "user:password");
    EXPECT_TRUE(url.GetQuery() == "with&a&query=param");
    query_items = url.GetQueryItems();
    ASSERT_TRUE(query_items.size() == 3);
    EXPECT_TRUE(query_items[0] == "with");
    EXPECT_TRUE(query_items[1] == "a");
    EXPECT_TRUE(query_items[2] == "query=param");
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "blah://";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority().empty());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.empty());
    EXPECT_TRUE(url.GetHost().empty());
    EXPECT_TRUE(url.GetPath().empty());
    EXPECT_FALSE(url.IsPathAbsolute());
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "blah");
    EXPECT_TRUE(url.GetUserInfo().empty());
    EXPECT_TRUE(url.GetQuery().empty());
    query_items = url.GetQueryItems();
    EXPECT_TRUE(query_items.empty());
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "blah:/";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetHost().empty());
    EXPECT_TRUE(url.GetPath() == "/");
    EXPECT_TRUE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.size() == 1);
    ASSERT_TRUE(segments[0].empty());
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "blah");
    EXPECT_TRUE(url.GetUserInfo().empty());
    EXPECT_TRUE(url.GetQuery().empty());
    query_items = url.GetQueryItems();
    EXPECT_TRUE(query_items.empty());
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "blah:/absolute/path";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetHost().empty());
    EXPECT_TRUE(url.GetPath() == "/absolute/path");
    EXPECT_TRUE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.size() == 2);
    EXPECT_TRUE(segments[0] == "absolute");
    EXPECT_TRUE(segments[1] == "path");
    EXPECT_TRUE(url.GetPathPart(0) == "/absolute");
    EXPECT_TRUE(url.GetPathPart(1) == "/absolute/path");
    EXPECT_TRUE(url.GetPathPart(1000) == "/absolute/path");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "blah");
    EXPECT_TRUE(url.GetUserInfo().empty());
    EXPECT_TRUE(url.GetQuery().empty());
    query_items = url.GetQueryItems();
    EXPECT_TRUE(query_items.empty());
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "ftp://ftp.is.co.za/rfc/rfc1808.txt";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "ftp.is.co.za");
    EXPECT_TRUE(url.GetHost() == "ftp.is.co.za");
    EXPECT_TRUE(url.GetPath() == "/rfc/rfc1808.txt");
    EXPECT_TRUE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.size() == 2);
    EXPECT_TRUE(segments[0] == "rfc");
    EXPECT_TRUE(segments[1] == "rfc1808.txt");
    EXPECT_TRUE(url.GetPathPart(0) == "/rfc");
    EXPECT_TRUE(url.GetPathPart(1) == "/rfc/rfc1808.txt");
    EXPECT_TRUE(url.GetPathPart(1000) == "/rfc/rfc1808.txt");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "ftp");
    EXPECT_TRUE(url.GetUserInfo().empty());
    EXPECT_TRUE(url.GetQuery().empty());
    query_items = url.GetQueryItems();
    EXPECT_TRUE(query_items.empty());
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "ftp://john.doe@ftp.is.co.za/rfc/rfc1808.txt";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "john.doe@ftp.is.co.za");
    EXPECT_TRUE(url.GetHost() == "ftp.is.co.za");
    EXPECT_TRUE(url.GetPath() == "/rfc/rfc1808.txt");
    EXPECT_TRUE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.size() == 2);
    EXPECT_TRUE(segments[0] == "rfc");
    EXPECT_TRUE(segments[1] == "rfc1808.txt");
    EXPECT_TRUE(url.GetPathPart(0) == "/rfc");
    EXPECT_TRUE(url.GetPathPart(1) == "/rfc/rfc1808.txt");
    EXPECT_TRUE(url.GetPathPart(1000) == "/rfc/rfc1808.txt");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "ftp");
    EXPECT_TRUE(url.GetUserInfo() == "john.doe");
    EXPECT_TRUE(url.GetQuery().empty());
    query_items = url.GetQueryItems();
    EXPECT_TRUE(query_items.empty());
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "https://[::1]";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "[::1]");
    EXPECT_TRUE(url.GetHost() == "::1");
    EXPECT_TRUE(url.GetPath().empty());
    EXPECT_FALSE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.empty());
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "https");
    EXPECT_TRUE(url.GetUserInfo().empty());
    EXPECT_TRUE(url.GetQuery().empty());
    query_items = url.GetQueryItems();
    EXPECT_TRUE(query_items.empty());
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "https://[v123ab.345:32:!(999a99]";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "[v123ab.345:32:!(999a99]");
    EXPECT_TRUE(url.GetHost() == "v123ab.345:32:!(999a99");
    EXPECT_TRUE(url.GetPath().empty());
    EXPECT_FALSE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.empty());
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "https");
    EXPECT_TRUE(url.GetUserInfo().empty());
    EXPECT_TRUE(url.GetQuery().empty());
    query_items = url.GetQueryItems();
    EXPECT_TRUE(query_items.empty());
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "git+ssh://www.%20ietf%ff";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "www.%20ietf%ff");
    EXPECT_TRUE(url.GetHost() == "www.%20ietf%ff");
    EXPECT_TRUE(url.GetPath().empty());
    EXPECT_FALSE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.empty());
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "git+ssh");
    EXPECT_TRUE(url.GetQuery().empty());
    query_items = url.GetQueryItems();
    EXPECT_TRUE(query_items.empty());
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "http://www.ietf.org/rfc/rfc2396.txt";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "www.ietf.org");
    EXPECT_TRUE(url.GetHost() == "www.ietf.org");
    EXPECT_TRUE(url.GetPath() == "/rfc/rfc2396.txt");
    EXPECT_TRUE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.size() == 2);
    EXPECT_TRUE(segments[0] == "rfc");
    EXPECT_TRUE(segments[1] == "rfc2396.txt");
    EXPECT_TRUE(url.GetPathPart(0) == "/rfc");
    EXPECT_TRUE(url.GetPathPart(1) == "/rfc/rfc2396.txt");
    EXPECT_TRUE(url.GetPathPart(1000) == "/rfc/rfc2396.txt");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "http");
    EXPECT_TRUE(url.GetUserInfo().empty());
    EXPECT_TRUE(url.GetQuery().empty());
    query_items = url.GetQueryItems();
    EXPECT_TRUE(query_items.empty());
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "ldap://[2001:db8::7]/c=GB?objectClass?one";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "[2001:db8::7]");
    EXPECT_TRUE(url.GetHost() == "2001:db8::7");
    EXPECT_TRUE(url.GetPath() == "/c=GB");
    EXPECT_TRUE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.size() == 1);
    EXPECT_TRUE(segments[0] == "c=GB");
    EXPECT_TRUE(url.GetPathPart(0) == "/c=GB");
    EXPECT_TRUE(url.GetPathPart(1000) == "/c=GB");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "ldap");
    EXPECT_TRUE(url.GetUserInfo().empty());
    EXPECT_TRUE(url.GetQuery() == "objectClass?one");
    query_items = url.GetQueryItems();
    ASSERT_TRUE(query_items.size() == 1);
    EXPECT_TRUE(query_items[0] == "objectClass?one");
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "mailto:John.Doe@example.com";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetHost().empty());
    EXPECT_TRUE(url.GetPath() == "John.Doe@example.com");
    EXPECT_FALSE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.size() == 1);
    EXPECT_TRUE(segments[0] == "John.Doe@example.com");
    EXPECT_TRUE(url.GetPathPart(0) == "John.Doe@example.com");
    EXPECT_TRUE(url.GetPathPart(1000) == "John.Doe@example.com");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "mailto");
    EXPECT_TRUE(url.GetUserInfo().empty());
    EXPECT_TRUE(url.GetQuery().empty());
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "news:comp.infosystems.www.servers.unix";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetHost().empty());
    EXPECT_TRUE(url.GetPath() == "comp.infosystems.www.servers.unix");
    EXPECT_FALSE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.size() == 1);
    EXPECT_TRUE(segments[0] == "comp.infosystems.www.servers.unix");
    EXPECT_TRUE(url.GetPathPart(0) == "comp.infosystems.www.servers.unix");
    EXPECT_TRUE(url.GetPathPart(1000) == "comp.infosystems.www.servers.unix");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "news");
    EXPECT_TRUE(url.GetUserInfo().empty());
    EXPECT_TRUE(url.GetQuery().empty());
    query_items = url.GetQueryItems();
    EXPECT_TRUE(query_items.empty());
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "tel:+1-816-555-1212";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetHost().empty());
    EXPECT_TRUE(url.GetPath() == "+1-816-555-1212");
    EXPECT_FALSE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.size() == 1);
    EXPECT_TRUE(segments[0] == "+1-816-555-1212");
    EXPECT_TRUE(url.GetPathPart(0) == "+1-816-555-1212");
    EXPECT_TRUE(url.GetPathPart(1000) == "+1-816-555-1212");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "tel");
    EXPECT_TRUE(url.GetUserInfo().empty());
    EXPECT_TRUE(url.GetQuery().empty());
    query_items = url.GetQueryItems();
    EXPECT_TRUE(query_items.empty());
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "telnet://192.0.2.16:80/";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "192.0.2.16:80");
    EXPECT_TRUE(url.GetHost() == "192.0.2.16");
    EXPECT_TRUE(url.GetPath() == "/");
    EXPECT_TRUE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.size() == 1);
    EXPECT_TRUE(segments[0].empty());
    EXPECT_TRUE(url.GetPort() == "80");
    EXPECT_TRUE(url.GetScheme() == "telnet");
    EXPECT_TRUE(url.GetUserInfo().empty());
    EXPECT_TRUE(url.GetQuery().empty());
    query_items = url.GetQueryItems();
    EXPECT_TRUE(query_items.empty());
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority().empty());
    EXPECT_TRUE(url.GetHost().empty());
    EXPECT_TRUE(url.GetPath() == "oasis:names:specification:docbook:dtd:xml:4.1.2");
    EXPECT_FALSE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.size() == 1);
    EXPECT_TRUE(segments[0] == "oasis:names:specification:docbook:dtd:xml:4.1.2");
    EXPECT_TRUE(url.GetPathPart(0) == "oasis:names:specification:docbook:dtd:xml:4.1.2");
    EXPECT_TRUE(url.GetPathPart(1000) == "oasis:names:specification:docbook:dtd:xml:4.1.2");
    EXPECT_TRUE(url.GetPort().empty());
    EXPECT_TRUE(url.GetScheme() == "urn");
    EXPECT_TRUE(url.GetUserInfo().empty());
    EXPECT_TRUE(url.GetQuery().empty());
    query_items = url.GetQueryItems();
    EXPECT_TRUE(query_items.empty());
    EXPECT_TRUE(url.GetFragment().empty());

    raw = "https://user:password@some.host:999//path///to%30%31/__resource#token/with?some;special/valid/chars";
    url = headcode::url::URL{raw};
    EXPECT_STREQ(url.GetURL().data(), raw);
    EXPECT_FALSE(url.GetScheme().empty());
    EXPECT_TRUE(url.GetAuthority() == "user:password@some.host:999");
    EXPECT_TRUE(url.GetHost() == "some.host");
    EXPECT_TRUE(url.GetPath() == "//path///to%30%31/__resource");
    EXPECT_TRUE(url.IsPathAbsolute());
    segments = url.GetSegments();
    ASSERT_TRUE(segments.size() == 6);
    EXPECT_TRUE(segments[0].empty());
    EXPECT_TRUE(segments[1] == "path");
    EXPECT_TRUE(segments[2].empty());
    EXPECT_TRUE(segments[3].empty());
    EXPECT_TRUE(segments[4] == "to%30%31");
    EXPECT_TRUE(segments[5] == "__resource");
    EXPECT_TRUE(url.GetPathPart(0) == "/");
    EXPECT_TRUE(url.GetPathPart(1) == "//path");
    EXPECT_TRUE(url.GetPathPart(2) == "//path/");
    EXPECT_TRUE(url.GetPathPart(3) == "//path//");
    EXPECT_TRUE(url.GetPathPart(4) == "//path///to%30%31");
    EXPECT_TRUE(url.GetPathPart(5) == "//path///to%30%31/__resource");
    EXPECT_TRUE(url.GetPathPart(1000) == "//path///to%30%31/__resource");
    EXPECT_TRUE(url.GetPort() == "999");
    EXPECT_TRUE(url.GetScheme() == "https");
    EXPECT_TRUE(url.GetUserInfo() == "user:password");
    EXPECT_TRUE(url.GetQuery().empty());
    query_items = url.GetQueryItems();
    EXPECT_TRUE(query_items.empty());
    EXPECT_TRUE(url.GetFragment() == "token/with?some;special/valid/chars");
}


TEST(URL, bad_schemes) {

    auto url = headcode::url::URL("");
    EXPECT_FALSE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kURLEmpty);

    url = headcode::url::URL("foo");
    EXPECT_FALSE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidScheme);

    std::list<std::string> bad_scheme_urls{"1:",        // Illegal characters at start ...
                                           "_:",
                                           "+:",
                                           ".:",
                                           "???:",
                                           "h*:",        // Illegal characters within ...
                                           "http#",
                                           "http#:",
                                           "  ftp:",
                                           "ssh :"};

    for (auto const & bad_url : bad_scheme_urls) {
        auto url = headcode::url::URL(bad_url);
        EXPECT_FALSE(url.IsValid());
        EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidSchemeChar);
    }
}


TEST(URL, bad_userinfo) {

    // Illegal characters in user info.

    auto raw = "http://user @this.is.an.address:1234/this/is/a/path?with&a&query=param#and_a_fragment";
    auto url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidUserInfo);

    raw = "http://user%@this.is.an.address:1234/this/is/a/path?with&a&query=param#and_a_fragment";
    url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidUserInfo);

    raw = "http://user%0x@this.is.an.address:1234/this/is/a/path?with&a&query=param#and_a_fragment";
    url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidUserInfo);
}


TEST(URL, bad_host) {

    // Illegal characters in host.

    auto raw = "http://this.is.an.address /";
    auto url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidHost);

    // Hence: "123.4." is not an IPv4 but is qualified as a reg-name by the RFC.
    raw = "http://123.4.";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsValid());

    // Hence: "312.412.512.110" is not an IPv4 but is qualified as a reg-name by the RFC.
    // I.e. "312" is **not** IPv4 but the ascii literal "312".
    raw = "http://312.412.512.110";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsValid());

    // Hence: "312.412.512.110" is not an IPv4 but is qualified as a reg-name by the RFC.
    // I.e. "012" is **not** IPv4 but the ascii literal "312".
    raw = "http://012.412.512.110";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsValid());

    raw = "http://[::3";
    url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidHost);

    raw = "http://[]";
    url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidHost);
}


TEST(URL, bad_port) {

    // Illegal characters in port.

    auto raw = "http://this.is.an.address: 12";
    auto url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidPort);

    raw = "http://127.0.0.1:123x";
    url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidPort);

    raw = "http://127.0.0.1:*";
    url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidPort);

    raw = "http://127.0.0.1:?%$";
    url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidPort);

    raw = "http://127.0.0.1:port";
    url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidPort);

    // Hence, the RFC approves port number bigger than 65535.
    // Therefore "url://host:1234567890" constitutes a valid port number, according to the RFC.

    raw = "http://127.0.0.1:1234567890";
    url = headcode::url::URL{raw};
    EXPECT_TRUE(url.IsValid());
}


TEST(URL, bad_path) {

    // Illegal characters in path.

    auto raw = "http://host:1234/bad/path /";
    auto url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidPath);

    raw = "http://host:1234/bad/[path]";
    url = headcode::url::URL{raw};
    EXPECT_FALSE(url.IsValid());
    EXPECT_EQ(url.GetError(), headcode::url::ParseError::kInvalidPath);
}


TEST(URL, move_or_url) {

    headcode::url::URL url;
    {
        headcode::url::URL temp_url{"temp://"};
        url = headcode::url::URL{temp_url};
    }

    EXPECT_TRUE(url.GetScheme() == "temp");
    EXPECT_TRUE(url.IsValid());
}

/*
 * This file is part of the headcode.space url.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_URL_URL_CORE_BITS_HPP
#define HEADCODE_SPACE_URL_URL_CORE_BITS_HPP

#include <algorithm>


#ifndef HEADCODE_SPACE_URL_URL_CORE_HPP
#error "Do not include this file directly."
#endif


#ifdef URL_PARSE_GENERIC_DELIMITERS
#error "URL_PARSE_GENERIC_DELIMITERS already defined."
#else
#define URL_PARSE_GENERIC_DELIMITERS ":/?#[]@"
#endif

#ifdef URL_PARSE_SUB_DELIMITERS
#error "URL_PARSE_SUB_DELIMITERS already defined."
#else
#define URL_PARSE_SUB_DELIMITERS "!$&'()*+,;="
#endif

#ifdef URL_PARSE_ALPHA
#error "URL_PARSE_ALPHA already defined."
#else
#define URL_PARSE_ALPHA "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#endif

#ifdef URL_PARSE_DIGIT
#error "URL_PARSE_DIGIT already defined."
#else
#define URL_PARSE_DIGIT "0123456789"
#endif

#ifdef URL_PARSE_UNRESERVED
#error "URL_PARSE_UNRESERVED already defined."
#else
#define URL_PARSE_UNRESERVED URL_PARSE_ALPHA##URL_PARSE_DIGIT##"-._~"
#endif


inline bool headcode::url::URL::IsAlpha(char c) const {
    return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'));
}


inline bool headcode::url::URL::IsDigit(char c) const {
    return (c >= '0') && (c <= '9');
}


inline bool headcode::url::URL::IsSchemeChar(char c) const {
    return IsAlpha(c) || IsDigit(c) || (c == '+') || (c == '-') || (c == '.');
}


inline void headcode::url::URL::Parse(std::string const & url) {

    if (url.empty()) {
        return;
    }

    auto colon = url.find_first_of(':');
    if (colon == std::string::npos) {
        return;
    }

    // the internal parsed_ std::string_view classes will
    // hold pointers to the url_ field. --> optimistic approach.
    url_ = url;
    auto scheme = std::string_view(url_.data(), colon);
    auto [hier_part, query, fragment] =
            Split(std::string_view(url_.data() + colon + 1, url_.size() - (scheme.size() + 1)));

    auto valid_scheme = ParseScheme(scheme);
    if (valid_scheme) {

        parsed_.scheme = scheme;

    } else {
        url_ = std::string{};
    }
}


inline bool headcode::url::URL::ParseScheme(std::string_view const & scheme) const {
    if (!scheme.empty() && IsAlpha(scheme[0])) {
        return std::all_of(scheme.cbegin() + 1, scheme.cend(), [&](auto c) { return IsSchemeChar(c); });
    }
    return false;
}


inline std::tuple<std::string_view, std::string_view, std::string_view> headcode::url::URL::Split(
        std::string_view url_rest) const {

    std::string_view hier_part;
    std::string_view query;
    std::string_view fragment;

    if (!url_rest.empty()) {

        hier_part = url_rest;
        auto question_or_hash = url_rest.find_first_of("?#");
        if (question_or_hash != std::string::npos) {
            hier_part = std::string_view{url_rest.data(), question_or_hash};
            url_rest = std::string_view{url_rest.data() + hier_part.size(), url_rest.size() - hier_part.size()};
        }

        auto question = url_rest.find_first_of('?');
        auto hash = url_rest.find_first_of('#');
        if (question != std::string::npos) {
            query = std::string_view{url_rest.data() + 1, hash - 1};
            url_rest = std::string_view{url_rest.data() + query.size() + 1, url_rest.size() - query.size()};
            hash = url_rest.find_first_of('#');
        }

        if (hash != std::string::npos) {
            fragment = std::string_view{url_rest.data() + 1, url_rest.size() - 1};
        }
    }

    return {hier_part, query, fragment};
}


#endif

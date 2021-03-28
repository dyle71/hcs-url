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


inline void headcode::url::URL::Parse(std::string url) {

    if (url.empty()) {
        return;
    }

    // optimistic approach
    url_ = url;
    if (ParseScheme()) {


    } else {
        // we failed to simply parse the scheme, which is essential.
        url_ = std::string{};
    }
}


inline bool headcode::url::URL::ParseScheme() {

    if (url_.empty()) {
        return false;
    }

    auto colon = url_.find_first_of(':');
    if (colon == std::string::npos) {
        return false;
    }

    auto scheme = std::string_view(url_.data(), colon);
    if (!scheme.empty() && IsAlpha(scheme[0])) {
        if (std::all_of(scheme.cbegin() + 1, scheme.cend(), [&](auto c){ return IsSchemeChar(c); })) {
            parsed_.scheme = scheme;
            return true;
        }
    }

    return false;
}


#endif

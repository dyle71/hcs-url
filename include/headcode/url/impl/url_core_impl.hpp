/*
 * This file is part of the headcode.space url.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_URL_URL_CORE_IMPL_HPP
#define HEADCODE_SPACE_URL_URL_CORE_IMPL_HPP


#ifndef HEADCODE_SPACE_URL_URL_CORE_HPP
#error "Do not include this file directly."
#endif


#include <algorithm>
#include <sstream>
#include <tuple>


/**
 * @brief namespace for inner implementation details.
 */
namespace headcode::url::impl {


/**
 * @brief Different states while parsing.
 */
enum class ParserState {

    kParsingScheme = 0,             //!< @brief Currently parsing the scheme value.
    kParsingHierPart,               //!< @brief Currently parsing authority or a path.
    kParsingAuthority,              //!< @brief Currently parsing the authority.
    kParsingPath,                   //!< @brief Currently parsing the path.
    kParsingQueryOrFragment,        //!< @brief Currently parsing either the query or fragment.
    kParsingQuery,                  //!< @brief Currently parsing the query.
    kParsingFragment                //!< @brief Currently parsing the fragment.
};


/**
 * @brief   Checks if the given character is a valid ASCII (according to the RFC).
 * @param   c       the character to check.
 * @return  true, if it is.
 */
inline bool IsAlpha(char c) {
    return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'));
}


/**
 * @brief   Checks if the given character is a valid DIGIT (according to the RFC).
 * @param   c       the character to check.
 * @return  true, if it is.
 */
inline bool IsDigit(char c) {
    return (c >= '0') && (c <= '9');
}


/**
 * @brief   Checks if the given character is a valid dec-octet ('0' - '255').
 * @param   dec_octet       the dec-octet to test.
 * @return  true, if it is.
 */
inline bool IsDecOctet(std::string_view const & dec_octet) {

    // a dec-octet is a string representing a value between 0 and 255.
    int value = 0;
    switch (dec_octet.size()) {

        case 0:
            return false;

        case 1:
            // valid: 0-9
            if (IsDigit(dec_octet[0])) {
                value = dec_octet[0] - '0';
            }
            return value > 0;

        case 2:
            // valid: 10-99
            if (IsDigit(dec_octet[0]) && IsDigit(dec_octet[1])) {
                value = (dec_octet[0] - '0') * 10 + (dec_octet[1] - '0');
            }
            return value >= 10 && value < 100;

        case 3:
            // valid: 100-255
            if (IsDigit(dec_octet[0]) && IsDigit(dec_octet[1]) && IsDigit(dec_octet[2])) {
                value = (dec_octet[0] - '0') * 100 + (dec_octet[1] - '0') * 10 + (dec_octet[2] - '0');
            }
            return value >= 100 && value < 256;
    }

    return false;
}


/**
 * @brief   Checks if the given character is a valid HEXDIG (according to the RFC).
 * @param   c       the character to check.
 * @return  true, if it is.
 */
inline bool IsHexDigit(char c) {
    return IsDigit(c) || ((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f'));
}


/**
 * @brief   Checks if the given character is a valid scheme character (according to the RFC).
 * @param   c       the character to check.
 * @return  true, if it is.
 */
inline bool IsSchemeChar(char c) {
    // scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
    return IsAlpha(c) || IsDigit(c) || (c == '+') || (c == '-') || (c == '.');
}


/**
 * @brief   Checks if the given character is a valid sub delimiter (according to the RFC).
 * @param   c       the character to check.
 * @return  true, if it is.
 */
inline bool IsSubDelimiter(char c) {
    // sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
    return (c == '!') || (c == '$') || (c == '&') || (c == '\'') || (c == '(') || (c == ')') || (c == '*') ||
           (c == '+') || (c == ',') || (c == ';') || (c == '=');
}


/**
 * @brief   Checks if the given character is a valid unreserved (according to the RFC).
 * @param   c       the character to check.
 * @return  true, if it is.
 */
inline bool IsUnreserved(char c) {
    return IsAlpha(c) || IsDigit(c) || (c == '-') || (c == '.') || (c == '_') || (c == '~');
}


/**
 * @brief   Checks if the given string represents a IPv4 address
 * @param   host            the host to check.
 * @return  true, if it is.
 */
inline bool IsIPv4(std::string_view const & host) {

    // strategy: split the sting in 4 dec-octet and check each dec-octet afterwards
    // exit immediately if the char ain't a digit (0-9) or dot (.).

    std::array<std::string_view, 4> octet;

    std::size_t index{0};
    std::string::size_type last{0};
    std::string::size_type i{0};
    for (; (i < host.size()) && (index < 4); ++i) {

        if (host[i] == '.') {
            octet[index++] = host.substr(last, i - last);
            last = i + 1;
            continue;
        }

        if (!IsDigit(host[i])) {
            return false;
        }
    }

    // capturing final dec-octet
    if ((i == host.size()) && (index != 4)) {
        octet[index++] = host.substr(last, host.size() - last);
    }

    return (index == 4) && std::all_of(octet.cbegin(), octet.cend(), [](auto const & sv) { return IsDecOctet(sv); });
}


/**
 * @brief   Checks if the given string represents a IPv6 address
 * @param   host            the host to check.
 * @return  true, if it is.
 */
inline bool IsIPv6(std::string_view const & host) {

    // TODO: this is not a real IPv6 check. Improve this!
    bool colon_present{false};
    bool ipv4_present{false};

    for (auto const & c : host) {
        if (IsHexDigit(c)) {
            continue;
        }
        if ((c == ':') && !ipv4_present) {
            colon_present = true;
            continue;
        }
        if ((c == '.') && colon_present) {
            ipv4_present = true;
            continue;
        }

        return false;
    }

    return true;
}


/**
 * @brief   Checks if the given string represents a IPvFuture address
 * @param   host            the host to check.
 * @return  true, if it is.
 */
inline bool IsIPvFuture(std::string_view const & host) {

    std::string_view::size_type start_of_address{0};
    if ((host.size() >= 4) && (host[0] == 'v')) {

        bool version{true};
        for (std::string_view::size_type i = 1; i < host.size(); ++i) {

            if (version && IsHexDigit(host[i])) {
                continue;
            }
            if (version && (host[i] == '.')) {
                version = false;
                continue;
            }

            if (!version && (IsUnreserved(host[i]) || IsSubDelimiter(host[i]) || (host[i] == ':'))) {
                if (!start_of_address) {
                    start_of_address = i;
                }
                continue;
            }

            return false;
        }
    }

    return start_of_address != 0;
}


/**
 * @brief   Checks if the given string represents an IPLiteral
 * @param   host            the host to check.
 * @return  true, if it is.
 */
inline bool IsIPLiteral(std::string_view const & host) {

    bool res = !host.empty();
    if (res) {
        res = host[0] == '[' && host[host.size() - 1] == ']';
        if (res) {
            auto address = host.substr(1, host.size() - 2);
            res = !address.empty() && (IsIPv6(address) || IsIPvFuture(address));
        }
    }

    return res;
}


/**
 * @brief   Checks if the given string represents a valid host regname.
 * @param   host            the host to check.
 * @return  true, if it is.
 */
inline bool IsRegName(std::string_view const & host) {

    if (host.empty()) {
        return true;
    }

    for (std::string_view::size_type i = 0; i < host.size(); ++i) {

        if (IsUnreserved(host[i]) || IsSubDelimiter(host[i])) {
            continue;
        }

        // pct-encoded
        if (host[i] == '%' && i < (host.size() - 2) && IsHexDigit(host[i + 1]) && IsHexDigit(host[i + 2])) {
            i += 2;
            continue;
        }

        return false;
    }

    return true;
}


/**
 * @brief   Checks if the given fragment is valid.
 * @param   fragment        the fragment to check.
 * @return  true, if it is.
 */
inline bool IsValidFragment(std::string_view const & fragment) {

    if (fragment.empty()) {
        return true;
    }

    for (std::string_view::size_type i = 0; i < fragment.size(); ++i) {

        if (IsUnreserved(fragment[i]) || IsSubDelimiter(fragment[i]) || (fragment[i] == ':') || (fragment[i] == '@') ||
            (fragment[i] == '/') || (fragment[i] == '?')) {
            continue;
        }

        // pct-encoded
        if (fragment[i] == '%' && i < (fragment.size() - 2) && IsHexDigit(fragment[i + 1]) &&
            IsHexDigit(fragment[i + 2])) {
            i += 2;
            continue;
        }

        return false;
    }

    return true;
}


/**
 * @brief   Checks if the given host is valid.
 * @param   host                the host to check.
 * @return  true, if it is.
 */
inline bool IsValidHost(std::string_view const & host) {
    if (host.find_first_of('[') != std::string_view::npos) {
        return IsIPLiteral(host);
    }
    return IsIPv4(host) || IsRegName(host);
}


/**
 * @brief   Checks if the given port is valid.
 * @param   port                the port to check.
 * @return  true, if it is.
 */
inline bool IsValidPort(std::string_view const & port) {
    return std::all_of(port.cbegin(), port.cend(), [](char c) { return IsDigit(c); });
}


/**
 * @brief   Checks if the given path segment is valid.
 * @param   segment             the segment to check.
 * @return  true, if it is.
 */
inline bool IsValidPathSegment(std::string_view const & segment) {

    for (std::string_view::size_type i = 0; i < segment.size(); ++i) {

        if (IsUnreserved(segment[i]) || IsSubDelimiter(segment[i]) || (segment[i] == ':') || (segment[i] == '@')) {
            continue;
        }

        // pct-encoded
        if (segment[i] == '%' && i < (segment.size() - 2) && IsHexDigit(segment[i + 1]) && IsHexDigit(segment[i + 2])) {
            i += 2;
            continue;
        }

        return false;
    }

    return true;
}


/**
 * @brief   Checks if the given query is valid.
 * @param   query           the query to check.
 * @return  true, if it is.
 */
inline bool IsValidQuery(std::string_view const & query) {
    // The rules to fragments apply also to queries.
    return IsValidFragment(query);
}


/**
 * @brief   Checks if the given userinfo is valid.
 * @param   userinfo            the userinfo to check.
 * @return  true, if it is.
 */
inline bool IsValidUserInfo(std::string_view const & userinfo) {

    if (userinfo.empty()) {
        return true;
    }

    for (std::string_view::size_type i = 0; i < userinfo.size(); ++i) {

        if (IsUnreserved(userinfo[i]) || IsSubDelimiter(userinfo[i]) || (userinfo[i] == ':')) {
            continue;
        }

        // pct-encoded
        if (userinfo[i] == '%' && i < (userinfo.size() - 2) && IsHexDigit(userinfo[i + 1]) &&
            IsHexDigit(userinfo[i + 2])) {
            i += 2;
            continue;
        }

        return false;
    }

    return true;
}


/**
 * @brief   Convert ASCII range A..Z to lower case.
 * @param   c       the character to convert.
 * @return  lower case c (or c if c not in range).
 */
inline char ToLower(char c) {
    if ((c >= 'A') && (c < 'Z')) {
        c += 0x20;
    }
    return c;
}


/**
 * @brief   Convert ASCII range a..z to upper case.
 * @param   c       the character to convert.
 * @return  upper case c (or c if c not in range).
 */
inline char ToUpper(char c) {
    if ((c >= 'a') && (c < 'z')) {
        c -= 0x20;
    }
    return c;
}


/**
 * @brief   Normalizes a percent encoded part of an URL.
 * @param   percent_encoded     an percent encoded particle.
 * @return  A string holding the percent encoded value (or upper case percent encoded).
 */
inline std::string NormalizePercentEncoded(std::string_view const & percent_encoded) {

    if ((percent_encoded.size() < 3) || (percent_encoded[0] != '%')) {
        return std::string{};
    }

    char v1 = percent_encoded[1];
    char v2 = percent_encoded[2];
    if (!IsHexDigit(v1) && !IsHexDigit(v2)) {
        return std::string{};
    }
    v1 = ToUpper(v1);
    v2 = ToUpper(v2);

    std::stringstream ss;
    ss << "%" << v1 << v2;
    char value = (IsDigit(v1) ? v1 - '0' : (v1 - 'A' + 0x0a)) << 4;
    value |= IsDigit(v2) ? v2 - '0' : (v2 - 'A' + 0x0a);

    if (IsUnreserved(value)) {
        return std::string{value};
    }

    return ss.str();
}


/**
 * @brief   Normalizes the authority part of an URL.
 * @param   authority       the authority to normalize.
 * @return  A string holding the normalized authority.
 */
inline std::string NormalizeAuthority(std::string_view const & authority) {

    std::stringstream ss;

    for (std::size_t i = 0; i < authority.size(); ++i) {

        // pct-encoded
        if (authority[i] == '%' && i < (authority.size() - 2) && IsHexDigit(authority[i + 1]) && IsHexDigit(authority[i + 2])) {
            ss << NormalizePercentEncoded(authority.substr(i, 3));
            i += 2;
            continue;
        }

        ss << ToLower(authority[i]);
    }

    return ss.str();
}


/**
 * @brief   Normalizes the scheme part of an URL
 * @param   scheme          the scheme to normalize.
 * @return  A string holding the normalized scheme.
 */
inline std::string NormalizeScheme(std::string_view const & scheme) {
    std::string res{scheme};
    std::transform(res.begin(), res.end(), res.begin(), [](char c) { return ToLower(c); });
    return res;
}


/**
 * @brief   Parse the port part inside an authority string.
 * @param   authority       the authority to parse.
 * @param   port            the start and length of the port value found in the authority.
 * @return  ParseError value.
 */
inline ParseError ParsePort(std::string_view const & authority, std::pair<std::size_t, std::size_t> & port) {

    port = std::make_pair(0, 0);
    if (authority.empty()) {
        return ParseError::kNoError;
    }

    // Some special treatment since a colon ':' may also appear inside
    // the IPLiteral (IPv6 and IPvFuture) of the authority. Luckily those
    // IPLiterals are placed inside '[' and ']'.
    auto port_part = authority;
    auto closing_ipliteral_bracket = authority.find_last_of(']');
    if (closing_ipliteral_bracket != std::string::npos) {
        port_part = authority.substr(closing_ipliteral_bracket + 1);
    }
    auto port_pos = port_part.find_last_of(':');
    if (port_pos != std::string::npos) {
        port = std::make_pair(port_pos + 1, authority.size() - (port_pos + 1));
    }

    return ParseError::kNoError;
}


/**
 * @brief   Parse the authority in the URL beginning at start.
 * @param   url             the url to parse.
 * @param   start           start of authority part in url
 * @param   authority       the (full) authority to write
 * @param   userinfo        the userinfo to write
 * @param   host            the host to write
 * @param   port            the port to write
 * @return  ParseError value and end position of parsing (i.e. ':' or end).
 */
inline std::tuple<ParseError, std::string::size_type> ParseAuthority(std::string_view const & url,
                                                                     std::size_t start,
                                                                     std::pair<std::size_t, std::size_t> & authority,
                                                                     std::pair<std::size_t, std::size_t> & userinfo,
                                                                     std::pair<std::size_t, std::size_t> & host,
                                                                     std::pair<std::size_t, std::size_t> & port) {

    // Strategy: Identify the portion of the authority inside the URL.
    // Then detect the port, if any. Afterwards split the remaining part
    // in user info and host.
    // If all strings have been identified, make a check on each for
    // validity (i.e. not containing illegal characters).

    authority = std::make_pair(start, 0);
    userinfo = std::make_pair(start, 0);
    host = std::make_pair(start, 0);
    port = std::make_pair(start, 0);
    if (url.empty()) {
        return {ParseError::kNoError, url.size()};
    }

    auto authority_string = url.substr(start);
    std::string::size_type last = authority_string.find_first_of('/');
    if (last == std::string_view::npos) {
        last = authority_string.size();
    }
    authority = std::make_pair(start, last);

    // To here: authority identified done.

    host = std::make_pair(start, last);
    last += start;

    ParseError error = ParsePort(url.substr(authority.first, authority.second), port);
    port.first += start;
    if (error != ParseError::kNoError) {
        return {error, port.first + port.second};
    }
    auto port_part = url.substr(port.first, port.second);

    // To here: port identified done.

    auto host_part = url.substr(start, last - start);
    if (port.second) {
        host.second = host.second - 1 - port.second;
        host_part = url.substr(host.first, host.second);
    }

    std::string_view userinfo_part;
    auto userinfo_end = host_part.find_last_of('@');
    if (userinfo_end != std::string::npos) {

        host.first = start + userinfo_end + 1;
        host.second = host_part.size() - userinfo_end - 1;
        host_part = url.substr(host.first, host.second);

        userinfo.first = start;
        userinfo.second = userinfo_end;
        userinfo_part = url.substr(userinfo.first, userinfo.second);
    }

    // To here: separated user info and host part.

    if (!IsValidUserInfo(userinfo_part)) {
        error = ParseError::kInvalidUserInfo;
    }
    if ((error == ParseError::kNoError) && !IsValidHost(host_part)) {
        error = ParseError::kInvalidHost;
    }
    if (IsIPLiteral(host_part)) {
        // IPLiterals are enclosed in '[' and ']' --> omit them in the pure host value
        host.first++;
        host.second = host_part.find_first_of(']') - 1;
    }

    if ((error == ParseError::kNoError) && !IsValidPort(port_part)) {
        error = ParseError::kInvalidPort;
    }

    return {error, last};
}


/**
 * @brief   Parse the fragment in the URL beginning at start.
 * @param   url         the url to parse, beginning at the scheme.
 * @param   start       the start index of the query in url.
 * @param   fragment    the identified fragment start and length in url.
 * @return  ParseError value and end position of parsing.
 */
inline std::tuple<ParseError, std::string::size_type> ParseFragment(std::string_view const & url,
                                                                    std::size_t start,
                                                                    std::pair<std::size_t, std::size_t> & fragment) {
    auto last = url.size();
    fragment = std::make_pair(start, last - start);
    if (!IsValidFragment(url.substr(fragment.first, fragment.second))) {
        return {ParseError::kInvalidFragment, last};
    }

    return {ParseError::kNoError, last};
}


/**
 * @brief   Parse the path of the url and provide each segment.
 * @param   url                 the url to parse.
 * @param   start               start of path part in url.
 * @param   authority_present   flag indicating that we have an authority
 * @param   path                the path parsed.
 * @param   segments            vector of segments found.
 * @return  ParseError value and end position of parsing (i.e. '?' or '#' or end).
 */
inline std::tuple<ParseError, std::string::size_type> ParsePath(
        std::string_view const & url,
        std::size_t start,
        bool authority_present,
        std::pair<std::size_t, std::size_t> & path,
        std::vector<std::pair<std::size_t, std::size_t>> & segments) {

    // Strategy: split path by '/' and collect each segment.
    // However, there are a number of special cases in the RFC to treat.

    segments.clear();
    path = std::make_pair(start, 0);
    auto path_part = url.substr(start);
    auto last = path_part.find_first_of("?#");
    if (last != std::string_view::npos) {
        path_part = url.substr(start, last);
    }
    path.second = path_part.size();
    last += start;

    bool is_absolute = !path_part.empty() && (path_part[0] == '/');
    bool is_absolute_or_empty = is_absolute || path_part.empty();
    if (authority_present && !is_absolute_or_empty) {
        return {ParseError::kInvalidPath, path.second + start};
    }

    if (path_part.empty()) {
        return {ParseError::kNoError, last};
    }

    // To here: path seems ok so far w.r.t. RFC. --> split the path in segments.

    std::string_view::size_type i = path_part[0] == '/' ? 1 : 0;
    auto nibbled_path = path_part.substr(i);
    do {

        auto segment_part = nibbled_path.substr(0, nibbled_path.find_first_of('/'));
        segments.emplace_back(start + i, segment_part.size());

        if (!IsValidPathSegment(segment_part)) {
            return {ParseError::kInvalidPath, path.first + path.second};
        }

        i += segment_part.size() + 1;
        if (i < path_part.size()) {
            nibbled_path = path_part.substr(i);
        }

    } while (i < path_part.size());

    // RFC defines that the first segment may not be empty
    // if we lack an authority but start absolute.
    if (!authority_present && !segments.empty() && (segments.at(0).second == 0)) {
        return {ParseError::kInvalidPath, last};
    }

    return {ParseError::kNoError, last};
}


/**
 * @brief   Parse the query in the URL beginning at start.
 * @param   url             the url to parse, beginning at the scheme.
 * @param   start           the start index of the query in url.
 * @param   query           the identified query start and length in url.
 * @param   query_items     the identified separated query items in url.
 * @return  ParseError value and end position of parsing.
 */
inline std::tuple<ParseError, std::string::size_type> ParseQuery(
        std::string_view const & url,
        std::size_t start,
        std::pair<std::size_t, std::size_t> & query,
        std::vector<std::pair<std::size_t, std::size_t>> & query_items) {

    auto last = url.substr(start).find_first_of('#');
    if (last == std::string_view::npos) {
        last = url.size() - start;
    }
    query = std::make_pair(start, last);
    last += start;

    auto query_part = url.substr(query.first, query.second);
    if (!IsValidQuery(query_part)) {
        return {ParseError::kInvalidQuery, last};
    }

    std::size_t i{0};
    auto nibbled_query = query_part;
    do {

        auto query_item_part = nibbled_query.substr(0, nibbled_query.find_first_of('&'));
        query_items.emplace_back(start + i, query_item_part.size());

        i += query_item_part.size() + 1;
        if (i < nibbled_query.size()) {
            nibbled_query = query_part.substr(i);
        }

    } while (i < query_part.size());

    return {ParseError::kNoError, last};
}


/**
 * @brief   Parse the scheme in the URL beginning at start.
 * @param   url         the url to parse, beginning at the scheme.
 * @param   start       the start index of the scheme in url.
 * @param   scheme      the identified scheme start and length in url.
 * @return  ParseError value and end position of parsing (i.e. ':' or end).
 */
inline std::tuple<ParseError, std::string::size_type> ParseScheme(std::string_view const & url,
                                                                  std::size_t start,
                                                                  std::pair<std::size_t, std::size_t> & scheme) {

    scheme = std::make_pair(0, 0);
    auto scheme_string = url.substr(start);
    if (scheme_string.empty()) {
        return {ParseError::kEmptyScheme, url.size()};
    }

    if (!IsAlpha(scheme_string[0])) {
        return {ParseError::kInvalidSchemeChar, url.size()};
    }

    auto pos = start;
    while (pos < url.size()) {
        if (!IsSchemeChar(url[pos])) {
            if (url[pos] == ':') {
                scheme.second = pos;
                return {ParseError::kNoError, pos};
            }
            return {ParseError::kInvalidSchemeChar, pos};
        }
        ++pos;
    }

    return {ParseError::kInvalidScheme, pos};
}


}


inline headcode::url::URL headcode::url::URL::Normalize() const {

    if (url_.empty()) {
        return URL{};
    }

    using namespace headcode::url::impl;

    std::stringstream ss;

    ss << NormalizeScheme(GetScheme()) << ":";
    if (!GetAuthority().empty()) {
        ss << "//" << NormalizeAuthority(GetAuthority());
    }

    return URL{ss.str()};
}


inline void headcode::url::URL::Parse() {

    // Strategy: work on the url_ field and parse in stages:
    // scheme -> hier-part -> authority -> path -> query -> fragment.

    using namespace headcode::url::impl;

    if (url_.empty()) {
        error_ = ParseError::kURLEmpty;
        return;
    }

    std::size_t pos{0};
    error_ = ParseError::kNoError;
    ParserState state = ParserState::kParsingScheme;
    std::string_view url_sv{url_};

    for (std::string::size_type i = 0; (i < url_.size()) & (error_ == ParseError::kNoError); ++i) {

        switch (state) {

            case ParserState::kParsingScheme:

                std::tie(error_, pos) = ParseScheme(url_sv, i, scheme_);
                if (error_ == ParseError::kNoError) {
                    i = pos;
                    state = ParserState::kParsingHierPart;
                }
                break;

            case ParserState::kParsingHierPart:

                authority_ = std::make_pair(i, 0);
                host_ = std::make_pair(i, 0);
                path_ = std::make_pair(i, 0);
                segments_.clear();
                port_ = std::make_pair(i, 0);
                userinfo_ = std::make_pair(i, 0);
                query_ = std::make_pair(i, 0);
                query_items_.clear();
                fragment_ = std::make_pair(i, 0);

                if ((i <= (url_.size() - 2)) && (url_.substr(i, 2) == "//")) {
                    state = ParserState::kParsingAuthority;
                    ++i;
                } else {
                    state = ParserState::kParsingPath;
                    --i;
                }
                break;

            case ParserState::kParsingAuthority:
                std::tie(error_, pos) = ParseAuthority(url_sv, i, authority_, userinfo_, host_, port_);
                if (error_ == ParseError::kNoError) {
                    // -1 since in we ought to catch the very next char (maybe a path with '/' ?)
                    i = pos - 1;
                    state = ParserState::kParsingPath;
                }
                break;

            case ParserState::kParsingPath:
                std::tie(error_, pos) = ParsePath(url_sv, i, authority_.second != 0, path_, segments_);
                if (error_ == ParseError::kNoError) {
                    i = pos - 1;
                    state = ParserState::kParsingQueryOrFragment;
                }
                break;

            case ParserState::kParsingQueryOrFragment:
                if (url_[i] == '?') {
                    state = ParserState::kParsingQuery;
                } else if (url_[i] == '#') {
                    state = ParserState::kParsingFragment;
                }
                break;

            case ParserState::kParsingQuery:
                std::tie(error_, pos) = ParseQuery(url_sv, i, query_, query_items_);
                if (error_ == ParseError::kNoError) {
                    i = pos;
                    state = ParserState::kParsingFragment;
                }
                break;

            case ParserState::kParsingFragment:
                std::tie(error_, pos) = ParseFragment(url_sv, i, fragment_);
                if (error_ == ParseError::kNoError) {
                    i = pos;
                    state = ParserState::kParsingFragment;
                }
                break;
        }
    }
}


#endif

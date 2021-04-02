/*
 * This file is part of the headcode.space url.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_URL_URL_CORE_HPP
#define HEADCODE_SPACE_URL_URL_CORE_HPP

#include <string>
#include <utility>
#include <vector>
#include <utility>


/**
 * @brief   The headcode url namespace.
 */
namespace headcode::url {


/**
 * @brief Different parsing errors we ecounter.
 */
enum class ParseError {
    kNoError = 0,              //!< @brief No error occurred, all good.
    kURLEmpty,                 //!< @brief The passed URL to parse has been empty.
    kInvalidSchemeChar,        //!< @brief Invalid character encountered in scheme parsing.
    kInvalidScheme,            //!< @brief Scheme is invalid.
    kEmptyScheme,              //!< @brief Empty scheme parsing.
    kInvalidUserInfo,          //!< @brief User info in URL is invalid.
    kInvalidHost,              //!< @brief Host part in URL is invalid.
    kInvalidPort,              //!< @brief Port part in URL is invalid.
    kInvalidPath,              //!< @brief Path part in URL is invalid.
    kInvalidQuery,             //!< @brief Query part in URL is invalid.
    kInvalidFragment           //!< @brief Fragment part in URL is invalid.
};


/**
 * @brief   The URL class.
 *
 * Taken from RFC 3986:
 * The following are two example URIs and their component parts:
 *
 *       foo://example.com:8042/over/there?name=ferret#nose
 *       \_/   \______________/\_________/ \_________/ \__/
 *        |           |            |            |        |
 *     scheme     authority       path        query   fragment
 *        |   _____________________|__
 *       / \ /                        \
 *       urn:example:animal:ferret:nose
 *
 * Objects of this class are constructed with a string. This string is parsed
 * and the found particles of the URI are indexed. This is done by holding the
 * starting position and the length of the sub string with the url as a
 * std::pair<std::size_t, std::size_t>.
 *
 * The GetError() method reports if the parsing has been successful.
 *
 * This implementation strictly adheres RFC 3986 (https://www.ietf.org/rfc/rfc3986.txt).
 *
 * We also added some convenient methods:
 *
 *      GetPathPart(std::size_t n)      ... return the path with the n-segments.
 *
 * Example:
 * @code
 *      headcode::url::URL url{"https://www.some.address.com/this/is/a/path"};
 *      std::cout << url.GetPathPart(2) << std::endl;       // <-- yields "/this/is"
 * @endcode
 *
 *      GetQueryItems()         ... all identified query sub-items with the '&' delimiter.
 *
 * Also the url itself is not copied around but the accessor functions
 * return std::string_view objects, meaning very little to nearly no overhead.
 * However objects of this class must still exist, if you access the the data.
 *
 * Good example:
 * @code
 *      // good
 *      void foo() {
 *          headcode::url::URL url{"https://www.some.address.com"};
 *          auto path = url.GetPath();
 *          std::cout << path << std::endl;
 *      }
 * @endcode
 *
 * Bad example:
 * @code
 *      // bad
 *      std::string_view foo() {
 *          headcode::url::URL url{"https://www.some.addresss.com"};
 *          return url.GetPath();           // <- returning object with pointer to inner temporary!
 *      }
 *      ...
 *      void bar() {
 *          std::cout << foo() << std::endl;
 *      }
 * @endcode
 */
class URL {

    std::string url_;        //!< @brief The input URL given to be parsed.

    ParseError error_{ParseError::kURLEmpty};        //!< @brief The error encountered during parsing.

    std::pair<std::size_t, std::size_t> authority_;        //!< @brief The parsed authority of the URL.
    std::pair<std::size_t, std::size_t> host_;             //!< @brief The parsed host of the URL.
    std::pair<std::size_t, std::size_t> path_;             //!< @brief The parsed path of the URL.
    std::vector<std::pair<std::size_t, std::size_t>>
            segments_;                                    //!< @brief The parsed segments of the path in the URL.
    std::pair<std::size_t, std::size_t> port_;            //!< @brief The parsed port of the URL.
    std::pair<std::size_t, std::size_t> scheme_;          //!< @brief The parsed scheme of the URL.
    std::pair<std::size_t, std::size_t> userinfo_;        //!< @brief The parsed userinfo of the URL.
    std::pair<std::size_t, std::size_t> query_;           //!< @brief The parsed query of the URL.
    std::vector<std::pair<std::size_t, std::size_t>>
            query_items_;                                 //!< @brief The parsed query items of the query in the URL.
    std::pair<std::size_t, std::size_t> fragment_;        //!< @brief The parsed fragment of the URL.

public:
    /**
     * @brief   Ctor.
     * @param   url         the url to parse.
     */
    explicit URL(std::string url = std::string{}) : url_{std::move(url)} {
        Parse();
    }

    /**
     * @brief   Copy Ctor.
     */
    URL(URL const &) = default;

    /**
     * @brief   Move Ctor.
     */
    URL(URL &&) noexcept = default;

    /**
     * @brief   Dtor.
     */
    ~URL() = default;

    /**
     * @brief   Copy assigment.
     * @return  (*this)
     */
    URL & operator=(URL const &) = default;

    /**
     * @brief   Move assigment.
     * @return  (*this)
     */
    URL & operator=(URL &&) noexcept = default;

    /**
     * @brief   Returns the parsed authority.
     * @return  The authority parsed.
     */
    [[nodiscard]] std::string_view GetAuthority() const {
        return std::string_view{url_}.substr(authority_.first, authority_.second);
    }

    /**
     * @brief   The error after parsing.
     * @return  The error value encountered.
     */
    [[nodiscard]] ParseError GetError() const {
        return error_;
    }

    /**
     * @brief   Returns the parsed fragment.
     * @return  The fragment parsed.
     */
    [[nodiscard]] std::string_view GetFragment() const {
        return std::string_view{url_}.substr(fragment_.first, fragment_.second);
    }

    /**
     * @brief   Returns the parsed host.
     * @return  The host parsed.
     */
    [[nodiscard]] std::string_view GetHost() const {
        return std::string_view{url_}.substr(host_.first, host_.second);
    }

    /**
     * @brief   Returns the parsed path.
     * @return  The path parsed.
     */
    [[nodiscard]] std::string_view GetPath() const {
        return std::string_view{url_}.substr(path_.first, path_.second);
    }

    /**
     * @brief   Returns the path up to the n-th segment
     * @param   n       the number of segments (maxed to the total count of all segments)
     * @return  The path including up to the n-th segment
     */
    [[nodiscard]] std::string_view GetPathPart(std::size_t n) const {

        if (segments_.empty()) {
            return std::string_view{};
        }
        n = std::min(n, segments_.size() - 1);
        auto total_length = segments_[n].first - path_.first + segments_[n].second;
        return std::string_view{url_}.substr(path_.first, total_length);
    }

    /**
     * @brief   Returns the parsed port.
     * @return  The port parsed.
     */
    [[nodiscard]] std::string_view GetPort() const {
        return std::string_view{url_}.substr(port_.first, port_.second);
    }

    /**
     * @brief   Returns the parsed query.
     * @return  The query parsed.
     */
    [[nodiscard]] std::string_view GetQuery() const {
        return std::string_view{url_}.substr(query_.first, query_.second);
    }

    /**
     * @brief   Returns the parsed query items.
     * @return  The query items parsed.
     */
    [[nodiscard]] std::vector<std::string_view> GetQueryItems() const {
        std::vector<std::string_view> query_items;
        for (auto const & p : query_items_) {
            query_items.emplace_back(std::string_view{url_}.substr(p.first, p.second));
        }
        return query_items;
    }

    /**
     * @brief   Returns the parsed scheme.
     * @return  The scheme parsed.
     */
    [[nodiscard]] std::string_view GetScheme() const {
        return std::string_view{url_}.substr(scheme_.first, scheme_.second);
    }

    /**
     * @brief   Returns the segments of the path as vector.
     * @return  The parsed segments of the path.
     */
    [[nodiscard]] std::vector<std::string_view> GetSegments() const {
        std::vector<std::string_view> segments;
        for (auto const & p : segments_) {
            segments.emplace_back(std::string_view{url_}.substr(p.first, p.second));
        }
        return segments;
    }

    /**
     * @brief   Returns the internal used URL string.
     * @return  The string used to create this object instance.
     */
    [[nodiscard]] std::string const & GetURL() const {
        return url_;
    }

    /**
     * @brief   Returns the parsed userinfo.
     * @return  The userinfo parsed.
     */
    [[nodiscard]] std::string_view GetUserInfo() const {
        return std::string_view{url_}.substr(userinfo_.first, userinfo_.second);
    }

    /**
     * @brief   Checks if the path is absolute.
     * @return  true, if the path is absolute.
     */
    [[nodiscard]] bool IsPathAbsolute() const {
        return !url_.empty() && (path_.second > 0) && (url_[path_.first] == '/');
    }

    /**
     * @brief   Checks if this is a valid URL object.
     * @return  true, if we have successfully parsed the given URL.
     */
    [[nodiscard]] bool IsValid() const {
        return error_ == ParseError::kNoError;
    }

    /**
     * @brief   Returns a normalized version of the given URL.
     * @return  A new URL object with the very same url, but normalized.
     */
    [[nodiscard]] URL Normalize() const;

private:
    /**
     * @brief   Parses the given url.
     */
    void Parse();
};


}


#include "headcode/url/impl/url_core_impl.hpp"


#endif

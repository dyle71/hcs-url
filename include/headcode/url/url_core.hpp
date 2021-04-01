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
    kInvalidPath               //!< @brief Path part in URL is invalid.
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
     * @brief   Returns the parsed port.
     * @return  The port parsed.
     */
    [[nodiscard]] std::string_view GetPort() const {
        return std::string_view{url_}.substr(port_.first, port_.second);
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
        for (auto const & p: segments_) {
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
     * @brief   Checks if this is a Null object.
     * @return  true, if this represents a Null object.
     */
    [[nodiscard]] bool IsValid() const {
        return error_ == ParseError::kNoError;
    }

private:
    /**
     * @brief   Parses the given url.
     */
    void Parse();
};


}


#include "headcode/url/impl/url_core_impl.hpp"


#endif

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


/**
 * @brief   The headcode url namespace
 */
namespace headcode::url {


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

    /**
     * @brief   The result of a parsed url.
     */
    struct ParsedUrl {
        std::string_view scheme;        //!< @brief The parsed scheme part of the URL.
    } parsed_;

public:
    /**
     * @brief   Ctor.
     * @param   url         the url to parse.
     */
    explicit URL(std::string url = std::string{}) {
        if (!url.empty()) {
            Parse(std::move(url));
        }
    }

    /**
     * @brief   Returns the parsed scheme.
     * @return  The scheme parsed.
     */
    [[nodiscard]] std::string_view const & GetScheme() const {
        return parsed_.scheme;
    }

    /**
     * @brief   Returns the internal used URL string.
     * @return  The string used to create this object instance.
     */
    [[nodiscard]] std::string const & GetURL() const {
        return url_;
    }

    /**
     * @brief   Checks if this is a Null object.
     * @return  true, if this represents a Null object.
     */
    [[nodiscard]] bool IsNull() const {
        return url_.empty();
    }

private:

    /**
     * @brief   Checks if the given character belongs to the ALPHA characters.
     * @param   c       the character to test.
     * @return  true, if the characters belongs to ALPHA as defined in the RFC.
     */
    [[nodiscard]] bool IsAlpha(char c) const;

    /**
     * @brief   Checks if the given character belongs to the DIGIT characters.
     * @param   c       the character to test.
     * @return  true, if the characters belongs to DIGIT as defined in the RFC.
     */
    [[nodiscard]] bool IsDigit(char c) const;

    /**
     * @brief   Checks if the given character is a valid character for a scheme
     * @param   c       the character to test.
     * @return  true, if the character can be used in a scheme string.
     */
    [[nodiscard]] bool IsSchemeChar(char c) const;

    /**
     * @brief   Parses the given url.
     * If parsing succeeds, then the given url will be copied inside.
     * @param   url     the url to parse.
     */
    void Parse(std::string url);

    /**
     * @param   Parses the scheme part of the inner url.
     * @return  true if the scheme has been set.
     */
    bool ParseScheme();
};


}


#include "url_core_bits.hpp"


#endif

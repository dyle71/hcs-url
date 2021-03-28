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
#include <tuple>
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

        std::string_view scheme;           //!< @brief The parsed scheme of the URL.
        std::string_view authority;        //!< @brief The parsed authority of the URL.
        std::string_view path;             //!< @brief The parsed path of the URL.

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
     * @brief   Returns the parsed authority.
     * @return  The authority parsed.
     */
    [[nodiscard]] std::string_view const & GetAuthority() const {
        return parsed_.authority;
    }

    /**
     * @brief   Returns the parsed path.
     * @return  The path parsed.
     */
    [[nodiscard]] std::string_view const & GetPath() const {
        return parsed_.path;
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
     * @brief   Checks if the given character is a valid path character
     * @param   c       the character to test.
     * @return  true, if the characters can be used in a path segment.
     */
    [[nodiscard]] bool IsPathCharacter(char c) const;

    /**
     * @brief   Checks if the given character is a valid character for a scheme
     * @param   c       the character to test.
     * @return  true, if the character can be used in a scheme string.
     */
    [[nodiscard]] bool IsSchemeChar(char c) const;

    /**
     * @brief   Checks if the given character is in the set of unreserved characters
     * @param   c       the character to test.
     * @return  true, if the characters is part of the unreserved set.
     */
    [[nodiscard]] bool IsUnreserved(char c) const;

    /**
     * @brief   Parses the given url.
     * If parsing succeeds, then the given url will be copied inside.
     * @param   url     the url to parse.
     */
    void Parse(std::string const & url);

    /**
     * @param   Parses the scheme part of the inner url.
     * @param   scheme          the scheme to parse.
     * @return  true if the scheme has been set.
     */
    bool ParseScheme(std::string_view const & scheme) const;

    /**
     * @brief   Split the url rest (all without the "scheme:" part) into hier_part, query and fragment
     * @param   url_rest            the url without the "scheme:"
     * @return  the hier_part, the query, and the fragment
     */
    [[nodiscard]] std::tuple<std::string_view, std::string_view, std::string_view> Split(
            std::string_view url_rest) const;

    /**
     * @brief   Split the hier-part into authority and path
     * @param   hier_path           the hier-path part of the URL
     * @return  parsing ok, the authority and the path
     */
    [[nodiscard]] std::tuple<bool, std::string_view, std::string_view> SplitHierPart(std::string_view hier_part) const;
};


}


#include "headcode/url/impl/url_core_impl.hpp"


#endif

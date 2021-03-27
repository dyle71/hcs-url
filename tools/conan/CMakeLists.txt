# ------------------------------------------------------------
# This file is the main CMakeLists of url of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2020-2021 headcode.space e.U.
# Oliver Maurhart <info@headcode.space>, https://www.headcode.space
# ------------------------------------------------------------


# ------------------------------------------------------------
# Creates the conan package.
#
# Define these variables:
#
#   - CONAN_USER, CONAN_CHANNEL     The conan package manager user/channel
#   - CONAN_REMOTE                  The conan remote to upload to

configure_file(conanfile.py conanfile.py @ONLY)
add_custom_target(conan
    COMMAND conan create . ${CONAN_USER}/${CONAN_CHANNEL}
    COMMENT "Creating and installing conan package locally...")

add_custom_target(conan_upload
    COMMAND conan upload ${CMAKE_PROJECT_NAME}/${VERSION}@${CONAN_USER}/${CONAN_CHANNEL} --all --remote=${CONAN_REMOTE}
    COMMENT "Uploading conan package to remote...")

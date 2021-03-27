# ------------------------------------------------------------
# This file is the main CMakeLists of url of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2020-2021 headcode.space e.U.
# Oliver Maurhart <info@headcode.space>, https://www.headcode.space
# ------------------------------------------------------------


# ------------------------------------------------------------
# Creates the docker builder images.
#
# Define these variables:
#
#   - DOCKER_TAG            The tag name of the docker images.

find_program(DOCKER docker)
set(SHELL $ENV{SHELL})
if (DOCKER AND SHELL)
    add_custom_target(docker_images
        COMMAND ${SHELL} -c ${CMAKE_CURRENT_SOURCE_DIR}/create-containers.sh ${DOCKER_TAG}
        COMMENT "Creating docker builder images...")
endif ()

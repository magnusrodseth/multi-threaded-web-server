#pragma once

#include "worker.h"
#include "logger.h"
#include <string.h>
#include <stdio.h>

/*
 * @brief Check if the provided string corresponds to the root of the directory to serve, i.e. the path "/".
 *
 * @param string is the string to check.
 * @return a boolean determining if the provided string is the root of the directory to serve.
 */
bool is_index_page(const char *string);

/**
 * @brief Parses the file at the provided path and writes the file contents to the provided buffer.
 *
 * @param path is the path of the file to parse.
 * @param buffer is the buffer to write the file content to.
 * @return a boolean determining if the file was successfully parsed
 */
bool parse_file(char *path, char *buffer);

/**
 * @brief Checks if a provided path is illegal.
 *
 * A path is illegal if it attempts to navigate outside the server's base directory.
 *
 * @param path is the path of the file to check.
 * @return a boolean indicating if the path is illegal or not.
 */
bool is_illegal_path(char *path);

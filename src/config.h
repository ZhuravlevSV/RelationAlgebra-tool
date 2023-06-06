#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <regex>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <deque>
#include <filesystem>
#include <cassert>

// colored text
#define TEXT_IS_COLORED true

// folders names
#define EXAMPLES_FOLDER "examples/"
#define BUFFER_FOLDER "examples/buffer/"
#define WORKSPACE_FOLDER "examples/workspace/"
#define SAVED_FILES_FOLDER "examples/saved_files/"

// fonts
#if TEXT_IS_COLORED == true
#define BOLD_FONT "\033[1m"
#define UNDERLINE_FONT "\033[4m"
#define RESET_FONT "\033[0m"
#endif

#if TEXT_IS_COLORED == false
#define BOLD_FONT ""
#define UNDERLINE_FONT ""
#define RESET_FONT ""
#endif

// colors
#if TEXT_IS_COLORED == true
#define RED_COLOR "\033[31m"
#define BLUE_COLOR "\033[34m"
#define GREEN_COLOR "\033[32m"
#define YELLOW_COLOR "\033[33m"
#define CYAN_COLOR "\033[36m"
#endif

#if TEXT_IS_COLORED == false
#define RED_COLOR ""
#define BLUE_COLOR ""
#define GREEN_COLOR ""
#define YELLOW_COLOR ""
#define CYAN_COLOR ""
#endif

// messages
#if TEXT_IS_COLORED == true
#define WARNING_TEXT "\033[1;33;1mWarning\033[0m"
#define SUCCESS_TEXT "\033[1;32;1mSuccess\033[0m"
#define NOTE_TEXT "\033[1;36;1mNote\033[0m"
#define ERROR_TEXT "\033[1;31;1mError\033[0m"
#endif

#if TEXT_IS_COLORED == false
#define WARNING_TEXT "Warning"
#define SUCCESS_TEXT "Success"
#define NOTE_TEXT "Note"
#define ERROR_TEXT "Error"
#endif

#endif

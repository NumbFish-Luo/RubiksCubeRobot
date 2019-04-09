#pragma once

#include <iostream>
#include <vector>

#include "k_CubeParser.hpp"
#include "k_FaceletCube.hpp"
#include "k_RubiksCube.hpp"
#include "k_Solver.hpp"

#define READ_FAIL	 0
#define READ_SUCCESS 1

extern const std::string fName;
extern unsigned int status;

bool ColorInputCorrecter(std::vector<std::string>& faceletStrings);
bool stringToCube(
    std::vector<std::string>& faceletStrings,
    FaceletCube &faceletCube,
    RubiksCube &cube);
bool SolveCube(std::vector<std::string>& faceletStrings);
bool TranslateToLD();

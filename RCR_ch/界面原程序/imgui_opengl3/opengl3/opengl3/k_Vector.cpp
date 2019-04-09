//
//  Vector.cpp
//  Rubiks Cube Solver
//
//  Created by Morris Chen on 2017-07-26.
//  Copyright © 2017 Morris Chen. All rights reserved.
//

#include <cstring>		// For memcpy()
#include <iostream>

#include "k_Vector.hpp"

// Vector manipulation routines

// CopyVector - Clone a vector
void CopyVector(int *fromVector, int *toVector, int n) {
    memcpy(toVector, fromVector, n * sizeof(int));
}

// PrintVector - Print the contents of the vector
void PrintVector(int *vector, int n) {
    for (int i = 0; i < n; i++)
        std::cout << vector[i] << " ";
    std::cout << std::endl;
}


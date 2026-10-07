#include "Utilities.h"

#include <iostream>

using std::cerr;


void exitWithError(MatamErrorType error) {
    std::cerr << "Matam Error: ";
    switch (error) {
        case MatamErrorType::UnmatchedSizes:
            std::cerr << "Unmatched matrices" << std::endl;
            break;
        case MatamErrorType::OutOfBounds:
            std::cerr << "Out of bounds" << std::endl;
            break;
        case MatamErrorType::NotSquareMatrix:
            std::cerr << "Matrix must be square to calculate determinant" << std::endl;
            break;
    }
    exit(1);
}

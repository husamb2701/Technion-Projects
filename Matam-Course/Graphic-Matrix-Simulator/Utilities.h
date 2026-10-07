
#pragma once

enum class MatamErrorType {
    UnmatchedSizes,
    OutOfBounds,
    NotSquareMatrix
};

void exitWithError(MatamErrorType error);

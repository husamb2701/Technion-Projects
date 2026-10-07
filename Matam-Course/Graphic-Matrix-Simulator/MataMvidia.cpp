#include "Matrix.h"
#include "MataMvidia.h"
#include <iostream>
#include "Utilities.h"
MataMvidia::MataMvidia(const string& movie_name,const string& maker_name,const Matrix* array,int frames) {
    this->maker_name = maker_name;
    this->movie_name = movie_name;
    this->frames = frames;
    this->array = nullptr;
    if (array == nullptr || frames == 0) {
        this->array = nullptr;
        return;
    }else {
        this->array = new Matrix[frames];
        for (int i = 0; i < frames; i++) {
            this->array[i] = array[i];
        }
    }
}

MataMvidia::MataMvidia(MataMvidia const& movie) {
    this->maker_name = movie.maker_name;
    this->movie_name = movie.movie_name;
    this->frames = movie.frames;
    this->array = nullptr;
    if (movie.array == nullptr || frames == 0) {
        this->array = nullptr;
        return;
    }else {
        this->array = new Matrix[frames];
        for (int i = 0; i < frames; i++) {
            this->array[i] = movie.array[i];
        }
    }
}
MataMvidia& MataMvidia::operator=(const MataMvidia& movie){
    if (this == &movie) {
        return *this;
    }

    delete [] this->array;
    this->array = nullptr;

    this->maker_name = movie.maker_name;
    this->movie_name = movie.movie_name;
    this->frames = movie.frames;

    if (movie.array == nullptr || movie.frames == 0) {
        this->frames = 0;
        return *this;
    }

    this->array = new Matrix[movie.frames];
    for (int i = 0; i < movie.frames; i++) {
        this->array[i] = movie.array[i];
    }

    return *this;
}

const Matrix& MataMvidia::operator[](const int index)const {
     if (index < 0 || index >= frames) {
         exitWithError(MatamErrorType::OutOfBounds);
         exit(-1);
     }else {
         return array[index];
     }
}

Matrix& MataMvidia::operator[](const int index) {
    if (index < 0 || index >= frames) {
        exitWithError(MatamErrorType::OutOfBounds);
        exit(-1);
    }else {
        return array[index];
    }
}

void MataMvidia::operator+= (MataMvidia const& movie) {
	int new_frames_count = this->frames + movie.frames;
    auto temp = new Matrix[new_frames_count];

    for (int i = 0; i < this->frames; i++) {
        temp[i] = this->array[i];
    }

    for (int i = 0; i < movie.frames; i++) {
        temp[this->frames + i] = movie.array[i];
    }

    delete [] this->array;
    this->array = temp;
    this->frames = new_frames_count;
}

void MataMvidia::operator+= (Matrix const& matrix) {
    auto temp = new Matrix[this->frames + 1];

    for (int i = 0; i < this->frames; i++) {
        temp[i] = this->array[i];
    }

    temp[this->frames] = matrix;

    delete [] this->array;
    this->array = temp;
    this->frames += 1;
}

MataMvidia MataMvidia::operator+ (MataMvidia const& movie)const {
    MataMvidia temp = *this;
    temp += movie;
    return temp;
}

std::ostream& operator<<(std::ostream& os, const MataMvidia& movie) {
    os << "Movie Name: " << movie.movie_name << endl;
    os << "Author: " << movie.maker_name << endl << endl;
    for (int i = 0; i < movie.frames; i++) {
        os << "Frame " << i << ":" << endl;
        os << movie.array[i] << endl;
    }
    os << "-----End of Movie-----" << endl;
    return os;
}

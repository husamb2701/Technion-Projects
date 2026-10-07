#pragma once
#include <string>
#include "Matrix.h"
using namespace std;


class MataMvidia {
    string movie_name;
    string maker_name;
    Matrix* array = nullptr;
    int frames = 0;
public:
    MataMvidia(const string& movie_name,const string& maker_name,const Matrix* array,int frames);
    MataMvidia(MataMvidia const& movie);
    MataMvidia&operator=(const MataMvidia& movie);
    ~MataMvidia(){
        delete [] array;
    }
    const Matrix&operator[](int index)const;
    Matrix&operator[](int index);
    void operator+= (MataMvidia const& movie);
    void operator+= (Matrix const& matrix);
    MataMvidia operator+ (MataMvidia const& movie)const;
    friend std::ostream& operator<<(std::ostream& os, const MataMvidia& movie);
};

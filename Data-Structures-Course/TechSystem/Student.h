#pragma once

class Student {
private:
    int id;
    int points;
public:
    Student(int id, int points) : id(id), points(points) {};
    ~Student() = default;
    int getId() const {
        return id;
    }
    int getPoints() const {
        return points;
    }
    void setPoints(int points) {
        this->points = points;
    }
};
#pragma once
#include <memory>
#include "AVL.h"
#include "Student.h"

class course {
private:
    int courseId;
    int coursePoints;
    AVL<int, std::shared_ptr<Student>> studentsTree;

public:
    course(const int id,const int points) : courseId(id), coursePoints(points) {}
    ~course() = default;
    int getCourseID() const {
        return courseId;
    }
    int getCoursePoints() const {
        return coursePoints;
    }
    //student management
    bool addStudent(const std::shared_ptr<Student>& student) {
        if (student == nullptr) return false;
        return studentsTree.insert(student->getId(), student);
    }

    void removeStudent(int studentId) {
        studentsTree.remove(studentId);
    }

    bool isStudentEnrolled(const int studentId) {
        return (studentsTree.search(studentId) != nullptr);
    }
    Student* getStudent(const int studentId ) {
        std::shared_ptr<Student>* Ptr = studentsTree.search(studentId);
        if (Ptr == nullptr) return nullptr;
        return Ptr->get();
    }
    bool hasStudents() const {
        return !studentsTree.isEmpty();
    }
};


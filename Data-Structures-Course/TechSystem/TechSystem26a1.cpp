#include "TechSystem26a1.h"
#include <memory>

using std::make_shared;
using std::shared_ptr;

TechSystem::TechSystem() = default;

TechSystem::~TechSystem() = default;

StatusType TechSystem::addStudent(int studentId)
{
    if (studentId <= 0) return StatusType::INVALID_INPUT;
    shared_ptr<Student> newStudent = make_shared<Student>(studentId, -totalBonus);
    try {
        if (allStudents.insert(studentId, newStudent)) {
            return StatusType::SUCCESS;
        }
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::FAILURE;
}

StatusType TechSystem::removeStudent(int studentId)
{
    if (studentId <= 0) return StatusType::INVALID_INPUT;
    shared_ptr<Student> *tempStudent = allStudents.search(studentId);
    if (tempStudent == nullptr) return StatusType::FAILURE;
    if ((*tempStudent).use_count() > 1) return StatusType::FAILURE;
    allStudents.remove(studentId);
    return StatusType::SUCCESS;
}

StatusType TechSystem::addCourse(int courseId, int points)
{
    if (courseId <= 0 || points <= 0) return StatusType::INVALID_INPUT;
    try {
        shared_ptr<course> newCourse = make_shared<course>(courseId, points);
        if (courses.insert(courseId, newCourse)) return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::FAILURE;
}

StatusType TechSystem::removeCourse(int courseId)
{
    if (courseId <= 0) return StatusType::INVALID_INPUT;
    shared_ptr<course>* tempCourse = courses.search(courseId);
    if (tempCourse == nullptr) return StatusType::FAILURE;
    shared_ptr<course> targetCourse = *tempCourse;
    if (targetCourse->hasStudents()) return StatusType::FAILURE;
    courses.remove(courseId);
    return StatusType::SUCCESS;
}

StatusType TechSystem::enrollStudent(int studentId, int courseId)
{
    if (studentId <= 0 || courseId <= 0) return StatusType::INVALID_INPUT;
    shared_ptr<course>* tempCourse = courses.search(courseId);
    if (tempCourse == nullptr) return StatusType::FAILURE;
    shared_ptr<course> targetCourse = *tempCourse;
    shared_ptr<Student>* tempStudent = allStudents.search(studentId);
    if (tempStudent == nullptr) return StatusType::FAILURE;
    shared_ptr<Student> targetStudent = *tempStudent;
    try {
        if (targetCourse->isStudentEnrolled(studentId)) return StatusType::FAILURE;
        bool success = targetCourse->addStudent(targetStudent);
        if (!success) return StatusType::FAILURE;
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType TechSystem::completeCourse(int studentId, int courseId)
{
    if (studentId <= 0 || courseId <= 0) return StatusType::INVALID_INPUT;
    shared_ptr<course>* tempCourse = courses.search(courseId);
    if (tempCourse == nullptr) return StatusType::FAILURE;
    shared_ptr<course> targetCourse = *tempCourse;
    Student* targetStudent = targetCourse->getStudent(studentId);
    if (targetStudent == nullptr) return StatusType::FAILURE;
    int pointsToAdd = targetCourse->getCoursePoints();
    targetStudent->setPoints(targetStudent->getPoints() + pointsToAdd);
    targetCourse->removeStudent(studentId);
    return StatusType::SUCCESS;
}

StatusType TechSystem::awardAcademicPoints(int points)
{
    if (points <= 0) return StatusType::INVALID_INPUT;
    totalBonus += points;
    return StatusType::SUCCESS;
}

output_t<int> TechSystem::getStudentPoints(int studentId)
{
    if (studentId <= 0) return StatusType::INVALID_INPUT;
    std::shared_ptr<Student>* tempStudent = allStudents.search(studentId);
    if (tempStudent == nullptr) return StatusType::FAILURE;
    std::shared_ptr<Student> targetStudent = *tempStudent;
    int finalPoints = totalBonus + targetStudent->getPoints();
    return finalPoints;
}


#ifndef __HEADER__
#define __HEADER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct bus Bus;
typedef struct student Student;
typedef struct studentNode *StdNodePtr;
typedef struct busNode *BusNodePtr;
typedef BusNodePtr BusList;
typedef StdNodePtr StudentList;

struct student
{
    int id;
    int date;
    int time;
    char from[40];
    char destination[40];
};

struct bus
{
    int number;
    int date;
    int time;
    char from[40];
    char destination[40];
    float price;
    int capacity;
    StudentList students;
};

struct busNode
{
    Bus Data;
    BusNodePtr NextBus;
};

struct studentNode
{
    Student Data;
    StdNodePtr NextStd;
};


void DeleteBusList(BusList L);
void DeleteStdList(StudentList L);
void DisposeStdList(StudentList L);

BusList loadBusFile(BusList list);
void loadStdFile(BusList list);
void displayBusInfo(BusNodePtr p);
void assignPassenger(Student s, BusList list);
void reassignUnmatched(BusList list, BusNodePtr unmatched);
void removePassenger(int id, BusList list);
void clearOldAssignment(BusList list);
void createUnmatched(BusList list, BusNodePtr *unmatched);
void sortBuses(BusList list);
Student scanStudentInput();
short isDuplicate(Student s, BusList list);

#endif
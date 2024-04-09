
//ID: 1200275, Name: Mahmoud Khatib, Sec: 4

#include "headers.h"

BusList MakeEmpty_Bus(BusList L)
{
    if(L != NULL)
        DeleteBusList(L); //DeleteBusList keeps the head of the list
    else
    {
        L = (BusList)malloc(sizeof(struct busNode));

        if(L == NULL)
            printf("Out of Memory\n");

        L->NextBus = NULL;
    }
    return L;
}

StudentList MakeEmpty_Std(StudentList L)
{
    if(L != NULL)
        DeleteStdList(L);
    else
    {
        L = (StudentList)malloc(sizeof(struct studentNode));

        if(L == NULL)
            printf("Out of Memory\n");

        L->NextStd = NULL;
    }
    return L;
}

short IsEmpty_Bus(BusList L)
{
    return L->NextBus == NULL;
}

short IsEmpty_Std(StudentList L)
{
    return L->NextStd == NULL;
}

short IsLast_Bus(BusNodePtr p, BusList L)
{
    return p->NextBus == NULL;
}

short IsLast_Std(StdNodePtr p, StudentList L)
{
    return p->NextStd == NULL;
}

BusNodePtr FindBus(int number, BusList L)
{
    BusNodePtr p;
    p = L->NextBus;

    while(p != NULL && p->Data.number != number)
        p = p->NextBus;

    return p;
}

StdNodePtr FindStudent(int id, StudentList L)
{
    StdNodePtr p = L->NextStd;

    while(p != NULL && p->Data.id != id)
        p = p->NextStd;

    return p;
}

BusNodePtr FindPreviousBus(int number, BusList L)
{
    BusNodePtr p;
    p = L;

    while(p->NextBus != NULL && p->NextBus->Data.number != number)
        p = p->NextBus;

    return p;
}

StdNodePtr FindPreviousStd(int id, StudentList L)
{
    StdNodePtr p = L;

    while(p->NextStd != NULL && p->NextStd->Data.id != id)
        p = p->NextStd;

    return p;
}

void DeleteBus(int number, BusList L)
{
    BusNodePtr p, temp;
    p = FindPreviousBus(number, L);
    StdNodePtr s;

    if(!IsLast_Bus(p, L))
    {
        s = p->NextBus->Data.students->NextStd;// pointing to the first student node in the bus

        // reassigning the students for the bus which will be deleted
        while(s != NULL)
        {
            assignPassenger(s->Data, p->NextBus);
            s = s->NextStd;
        }
        temp = p->NextBus;
        p->NextBus = temp->NextBus;
        DisposeStdList(temp->Data.students);
        free(temp);
    }
    else
        printf("Bus: %d is not found, No Change has been made\n", number);
}

/*
returns 1 if the deletion of a student went successfully
and 0 if the student we are looking for is not found.
It is used as a flag in some functions
*/
short DeleteStudent(int id, BusNodePtr b)
{
    StdNodePtr p, temp;
    p = FindPreviousStd(id, b->Data.students);

    if(p->NextStd != NULL)
    {
        temp = p->NextStd;
        p->NextStd = temp->NextStd;
        free(temp);
        b->Data.capacity++;
        return 1;
    }
    return 0;
}

void InsertBus(Bus b, BusList L, BusNodePtr p)
{
    BusNodePtr temp;
    temp = (BusNodePtr)malloc(sizeof(struct busNode));
    temp->Data = b;
    temp->NextBus = p->NextBus;
    p->NextBus = temp;

    //creation of the head for the student list
    temp->Data.students = NULL;
    temp->Data.students = MakeEmpty_Std(temp->Data.students);
}

void InsertStudent(Student s, StudentList L, StdNodePtr p)
{
    StdNodePtr temp;
    temp = (StdNodePtr)malloc(sizeof(struct studentNode));
    temp->Data = s;
    temp->NextStd = p->NextStd;
    p->NextStd = temp;
}

void PrintBuses(BusList L)
{
    BusNodePtr p = L;
    StdNodePtr p2;

    if(IsEmpty_Bus(L))
        printf("Empty Bus List!\n");
    else
    {
        do
        {
            p = p->NextBus;
            displayBusInfo(p);
            printf("---------------------------------\n");

        }while(!IsLast_Bus(p->NextBus, L));
    }
}

void PrintStudents(StudentList L)
{
    StdNodePtr p = L;

    if(IsEmpty_Std(L))
        printf("Empty Student List!\n");
    else
    {
        do
        {
            p = p->NextStd;
            printf("ID:%d, ", p->Data.id);
            printf("Date:%d, ", p->Data.date);
            printf("Departure Time:%02d:%02d, ", (p->Data.time / 100), (p->Data.time % 100));
            printf("From:%s, ", p->Data.from);
            printf("Destination:%s\n", p->Data.destination);
        }while(!IsLast_Std(p, L));
    }
}

void DeleteStdList(StudentList L)
{
    StdNodePtr p, temp;
    p = L->NextStd;
    L->NextStd = NULL;

    while(p != NULL)
    {
        temp = p->NextStd;
        free(p);
        p = temp;
    }
}

void DisposeStdList(StudentList L)
{
    DeleteStdList(L);
    free(L);
}

void DeleteBusList(BusList L)
{
    BusNodePtr p, temp;

    if(L != NULL)
    {
        p = L->NextBus;
        L->NextBus = NULL;

        while(p != NULL)
        {
            temp = p->NextBus;
            DisposeStdList(p->Data.students);
            free(p);
            p = temp;
        }
    }
}

void DisposeBusList(BusList L)
{
    if(L != NULL)
    {
        DeleteBusList(L);
        free(L);
    }
}

int SizeOfStudents(StudentList L)
{
    int count = 0;
    StdNodePtr p = L;

    while(p->NextStd != NULL)
    {
        p = p->NextStd;
        count++;
    }
    return count;
}

/*
this function will return a bus list sorted after reading
the data from a file
*/
BusList loadBusFile(BusList list)
{
    list = MakeEmpty_Bus(list);
    char scanner[300];
    FILE *file;

    file = fopen("busses.txt", "r");

    while(fgets(scanner, 300, file) != NULL)
    {
        Bus b;

        b.number = atoi(strtok(scanner, "#"));
        b.date = atoi(strtok('\0', "#"));
        b.time = 100 * atoi(strtok('\0', ":"));
        b.time += atoi(strtok('\0', "#"));
        strcpy(b.from, strtok('\0', "#"));
        strcpy(b.destination, strtok('\0', "#"));
        b.price = atof(strtok('\0', "#"));
        b.capacity = atoi(strtok('\0', "\n"));

        InsertBus(b, list, list);
    }
    fclose(file);

    sortBuses(list);

    return list;
}

void loadStdFile(BusList list)
{
    char scanner[300];
    FILE *file;

    file = fopen("passengers.txt", "r");

    while(fgets(scanner, 300, file) != NULL)
    {
        Student s;

        s.id = atoi(strtok(scanner, "#"));
        s.date = atoi(strtok('\0', "#"));
        s.time = 100 * atoi(strtok('\0', ":"));
        s.time += atoi(strtok('\0', "#"));
        strcpy(s.from, strtok('\0', "#"));
        strcpy(s.destination, strtok('\0', "\n"));

        assignPassenger(s, list);
    }
    fclose(file);
}

void displayBusInfo(BusNodePtr p)
{
    StdNodePtr p2 = p->Data.students->NextStd;

    printf("Number: %d\n", p->Data.number);
    printf("Departure time: %02d/", (p->Data.date / (int)pow(10, 6)));
    printf("%02d/%02d", (p->Data.date / 10000) % 100, p->Data.date % 10000);
    printf(" at %02d:%02d\n", p->Data.time / 100, p->Data.time % 100);
    printf("From: %s\t", p->Data.from);
    printf("Destination: %s\n", p->Data.destination);
    printf("Price: %.1f\t", p->Data.price);
    printf("Capacity: %d\n", p->Data.capacity);

    printf("Registered Students: [");

    while(p2 != NULL)
    {
        printf(", ");

        //if the printed student is the first one then the comma before the ID is deleted
        if(p2 == p->Data.students->NextStd)
            printf("\b\b");

        printf("%d", p2->Data.id);
        p2 = p2->NextStd;
    }
    printf("]\n\n");
}

/*assigns one student to a matching bus*/
void assignPassenger(Student s, BusList list)
{
    if(list == NULL)
        return;

    BusNodePtr p = list->NextBus;

    while(p->Data.number != -1)
    {
        if(p->Data.date == s.date && p->Data.time >= s.time
            && stricmp(p->Data.from, s.from) == 0 && stricmp(p->Data.destination, s.destination) == 0
            && p->Data.capacity > 0)
        {
            InsertStudent(s, p->Data.students, p->Data.students);
            p->Data.capacity--;
            return;
        }
        p = p->NextBus;
    }
    InsertStudent(s, p->Data.students, p->Data.students);
}

/*
reassigns the unmatched students after deleting a student
because some students could have been inserted to unmatched due
to no more capacity.
*/
void reassignUnmatched(BusList list, BusNodePtr unmatched)
{
    StdNodePtr temp, p = unmatched->Data.students;


    while(p != NULL && p->NextStd != NULL)
    {
        assignPassenger(p->NextStd->Data, list); //if there is no fit for that student it will be assigned to unmatched again

        //deleting the assigned node(will prevent duplicating)
        temp = p->NextStd;
        p->NextStd = p->NextStd->NextStd;
        free(temp);
        p = p->NextStd;
    }
}

void removePassenger(int id, BusList list)
{
    BusNodePtr p;

    p = list->NextBus;

    while(!DeleteStudent(id, p) && p != NULL)
        p = p->NextBus;
}

/*
clears old assignment if the user loads the student file again.
File data could have been changed, so old assignment is deleted
*/
void clearOldAssignment(BusList list)
{
    BusNodePtr p;

    if(list != NULL)
    {
        p = list->NextBus;

        while(p != NULL)
        {
            p->Data.capacity += SizeOfStudents(p->Data.students);
            DeleteStdList(p->Data.students);
            p = p->NextBus;
        }
    }
}

/*
this function will create the unmatched node for unmatched students
and insert it at the end of the buses list while changing the unmatched pointer
in the main to point at the inserted node
*/
void createUnmatched(BusList list, BusNodePtr *unmatched)
{
    Bus b;
    b.number = -1;
    BusNodePtr p = list;

    while(p->NextBus != NULL)
        p = p->NextBus;

    InsertBus(b, list, p);
    *unmatched = p->NextBus;
}

/*
sorting data based on time so when a student is assigned
it will be assigned to the earliest departing (leaving) bus
*/
void sortBuses(BusList list)
{
    BusNodePtr a[10], p, rear[10]; //rear is pointing at the end of each list

    for(int i = 0; i < 10; i++)
    {
        a[i] = NULL;
        a[i] = MakeEmpty_Bus(a[i]);
        rear[i] = a[i];
    }
    for(int i = 0; i < 4; i++)
    {
        int k = 9;
        p = list->NextBus;

        while(p != NULL)
        {
            int m = (p->Data.time % (int)pow(10, i+1)) / pow(10, i);
            InsertBus(p->Data, a[m], rear[m]);
            rear[m] = rear[m]->NextBus;
            p = p->NextBus;
        }
        DeleteBusList(list);

        for(int j = 8; j >= 0; j--)
        {
            if(!IsEmpty_Bus(a[j]))
            {
                list->NextBus = a[j]->NextBus;
                rear[j]->NextBus = a[k]->NextBus;
                a[k]->NextBus = NULL;
                rear[k] = a[k];
                k = j;
            }
        }
        a[k]->NextBus = NULL;
        rear[k] = a[k];
    }
}

Student scanStudentInput()
{
    int temp1, temp2, temp3;
    char ch[6];
    Student s;

    printf("Enter passenger id: ");
    scanf("%d", &s.id);

    printf("\nEnter Departure Date(day/month/year): ");
    scanf("%d/%d/%d", &temp1, &temp2, &temp3);

    printf("\nEnter Departure time(xx:xx): ");
    scanf("\n%s", ch);

    printf("\nEnter From: ");
    scanf("\n%[^\n]s", s.from); //reads a full line instead of stopping at the first space

    printf("\nEnter Destination: ");
    scanf("\n%[^\n]s", s.destination);

    s.date = temp1 * pow(10, 6);
    s.date += temp2 * pow(10, 4);
    s.date += temp3;

    strtok(ch, ":");
    strcat(ch, strtok('\0', "\n"));
    s.time = atoi(ch);

    return s;
}

/*
this function is used to avoid adding a duplicate student
based on his\her id number
*/
short isDuplicate(Student s, BusList list)
{
    BusNodePtr p = list->NextBus;

    while(p != NULL && FindStudent(s.id, p->Data.students) == NULL)
        p = p->NextBus;

    if(p != NULL)
    {
        printf("A user with the same ID already exists!!!\n");
        return 1;
    }
    return 0;
}


// Main Function
int main()
{
    int menu, input;
    BusList buses = NULL;
    BusNodePtr unmatched;

    //flags to make sure that data is loaded from files first before any operation
    short loadedBus = 0, loadedStd = 0;

    do
    {
        printf("\nPlease select one of the following options:-\n");
        printf("1. Load the bus information file\n");
        printf("2. Load the students information file\n");
        printf("3. Assign passengers and print assignment information of all buses\n");
        printf("4. Print a specific bus information along with its passengers information\n");
        printf("5. Print unmatched passengers\n");
        printf("6. Add new passenger\n");
        printf("7. Delete passenger\n");
        printf("8. Delete bus number\n");
        printf("9. Exit\n");
        
        scanf("%d", &menu);

        //making sure that data is loaded before processing data
        if((!loadedBus || !loadedStd) && (menu != 9 && menu != 1 && menu != 2))
        {
            printf("Please load the data from files first\n");
            continue;
        }

        Student s;//could be used for adding a new student(option: 6)

        switch(menu)
        {
        case 1:
            buses = loadBusFile(buses);
            createUnmatched(buses, &unmatched);
            loadedBus = 1;
            printf("File loaded successfully\n");
            break;

        case 2:
            clearOldAssignment(buses);
            loadStdFile(buses);
            loadedStd = 1;
            printf("File loaded successfully\n");
            break;

        case 3:
            PrintBuses(buses);
            printf("Unmatched Students:-\n");
            PrintStudents(unmatched->Data.students);
            break;

        case 4:
            printf("Enter Bus Number: ");
            scanf("%d", &input);
            displayBusInfo(FindBus(input, buses));
            break;

        case 5:
            PrintStudents(unmatched->Data.students);
            break;

        case 6:
            s = scanStudentInput();

            if(!isDuplicate(s, buses))
                assignPassenger(s, buses);

            break;

        case 7:
            printf("\nPlease enter the passenger number: ");
            scanf("%d", &input);
            removePassenger(input, buses);
            reassignUnmatched(buses, unmatched);
            break;

        case 8:
            printf("\nEnter a bus number: ");
            scanf("%d", &input);
            DeleteBus(input, buses);
            break;

        case 9:
            printf("\nProgram Terminated...\n");
            break;

        default:
            printf("Invalid Input Please try again\n");
            break;
        }
    }while(menu != 9);

    DisposeBusList(buses);

    return 0;
}
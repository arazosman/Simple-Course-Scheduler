/*
    ATTENTION: This code designed for Linux, may not work correctly in Windows.
*/

/*
    Programming Languages - Spring 2017 - Assignment 2

    A program that simulates course enrollment system of a student.

    @author
    Name: Osman Araz
    Student NO: 16011020
    Date: 11.11.2017
    E-Mail: arazosman@outlook.com
    Compiler used: GCC
    Computer Hardware: 64 bit Quad Core CPU, 8 GB RAM
    IDE: None (Visual Studio Code used as a text editor.)
    Operating System: Manjaro KDE Edition 17.0.6
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> // used for strcpy() function
#include <unistd.h> // used for getting arrow keys from the user
#include <termios.h> // used for getting arrow keys from the user

// ANSI color defining
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

// defining sizes of the matrix of weekly program
#define ROW 10 // represents hours (9 to 18)
#define COL 5 // represents days

/*
    DATE struct: It stores the time of the course for each occurence.
*/
typedef struct
{
    short day, beginHour, endHour;
} DATE;

/*
    COURSE struct: It stores the name, ID, class info, number of 
    occurences and a DATE array for the courses.
*/
typedef struct
{
    char name[50];
    char ID[10];
    short class;
    short occurence;
    DATE *dates;
} COURSE;

COURSE *courses; // array of the courses
int courseCount = 0; // count for the courses

int Schedule[ROW][COL]; // matrix of the weekly program
int enrolledCourses[20]; // array of the enrolled courses ID's
int countEnrolled = 0; // count for the enrolled courses

// menuElements array: It stores the menu elements which will be showed to the user.
char *menuElements[] = 
    {"Define Course", "Add Course", "Delete Course", "Display Weekly Plan", "Enrolled Courses", "Exit"};
const int menuSize = 6;

// variables for the student:
char stdName[20], stdSurname[30];
long stdId;
short stdClass;

/*
    Main function. It gets the student informations from the user and creates 
    dynamically allocated array of COURSE.
*/
int readKey();
void printBanner();
void printMenu();
void crashAllocation();
int main()
{
    void clearTable();
    void deallocateArrays();

    clearTable(); // initializing Schedule matrix elements to -1
    printBanner(); // printing logo/banner of the program

    // getting student informations from the user
    printf(COLOR_YELLOW "\tHello! Please fill your informations below: \n\n" COLOR_RESET);
    printf("\tName: ");
    scanf("%s", stdName);
    printf("\tSurname: ");
    scanf("%s", stdSurname);
    printf("\tUniversity ID: ");
    scanf("%ld", &stdId);
    printf("\tClass: ");
    scanf("%d", &stdClass);
    getchar();
    
    // creating dynamically allocated array for the courses
    courses = (COURSE *)malloc(50*sizeof(COURSE));

    // checking whether does the memory aloccated or not
    if (!courses)
        crashAllocation();

    printMenu(); // printing the menu
    deallocateArrays(); // deallocation arrays

    return 0;
}

/*
    Function which initalizing Schedule matrix elements to -1
    "-1" represents "null" for the whole program.
*/
void clearTable()
{
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
            Schedule[i][j] = -1;
}

/*
    Function which prints the logo/banner of the program.
*/
void printBanner()
{
    system("clear"); // cleaning the screen
    printf(COLOR_CYAN "\n\t#######################################\n");
    printf("\t####" COLOR_MAGENTA "       COURSE ENROLLMENT       " COLOR_CYAN "####\n");
    printf("\t#######################################\n\n\n" COLOR_RESET);
}

void printMenu()
{
    void defineCourse();
    void addCourse();
    void deleteCourse();
    void printWeeklyPlan();
    void printEnrolledCourses();

	int ch, ind = 0; // ch: arrow key value, ind: an iterator on the menu elements

    do
	{
		system("clear");
        printBanner();
        printf(COLOR_YELLOW "\tHello, %s %s. %s\n\n", stdName, stdSurname, COLOR_RESET);

        ind %= menuSize; // iterator always must bounded with range(-menuSize, +menuSize)

        if (ind < 0)
            ind +=  menuSize;

        for (int i = 0; i < menuSize; i++)
        {
            // the menu element which has the iterator indice, will be printed colorful
            if (i != ind)
                printf("\t>  %s\n", menuElements[i]);
            else
                printf(COLOR_MAGENTA "\t>  %s %s\n", menuElements[i], COLOR_RESET);
        }

        printf("\n");
        ch = readKey(); // getting arrow key from the user

		if (ch == 66) // represents DOWN key, it increases iterator
			ind++;
		else if (ch == 65) // represents UP key, it decreases iterator
            ind--;
    }
    while (ch != 10); // represents ENTER key, it breakes the loop

    // according to the iterator, desired function will be called
    if (ind == 0)
        defineCourse();
    else if (ind == 1)
        addCourse();
    else if (ind == 2)
        deleteCourse();
    else if (ind == 3)
        printWeeklyPlan();
    else if (ind == 4)
        printEnrolledCourses();
    else
    {
        system("clear");
        exit(0);
    }
}

/*
    Function which ensures to going back to the main menu.
*/
void goBackToMenu()
{
    printf("\tTo back to the main menu, press any key on your keyboard.\n");
    getchar();
    int ch = readKey();
    printMenu();
}

/*
    Function which ensures to defining the courses. It gets the course 
    informations from the user and then adds them to the array of the courses.
*/
void defineCourse()
{
    printBanner();
    printf(COLOR_YELLOW "\tDefine Course:%s\n\n\n", COLOR_RESET);

    // getting course informations from the user
    printf("\tCourse Name: ");
    fgets(courses[courseCount].name, 50, stdin);
    printf("\tCourse ID: ");
    scanf("%s", courses[courseCount].ID);
    printf("\tClass Info: ");
    scanf("%d", &courses[courseCount].class);
    printf("\n\tHow many times does the course occur in a week?: ");
    scanf("%d", &courses[courseCount].occurence);

    // the "dates" array in the courses allocating as the number of occurence
    courses[courseCount].dates = (DATE *)malloc(courses[courseCount].occurence*sizeof(DATE));

    // checking whether does the memory aloccated or not
    if (!courses[courseCount].dates)
        crashAllocation();

    // getting course times for each occurence
    for (int i = 0; i < courses[courseCount].occurence; i++)
    {
        printf("\n\tThe day of the week for the #%d occurence (1 to 5): ", i+1);
        scanf("%d", &courses[courseCount].dates[i].day);
        printf("\tThe beginning hour for the #%d occurence (9 to 17): ", i+1);
        scanf("%d", &courses[courseCount].dates[i].beginHour);
        printf("\tThe ending hour for the #%d occurence (10 to 18): ", i+1);
        scanf("%d", &courses[courseCount].dates[i].endHour);
    }

    courseCount++; // course count increases

    printBanner();
    printf(COLOR_GREEN "\tCourse added successfully!%s\n\n", COLOR_RESET);
    goBackToMenu();
}

/*
    Function which ensures to user to adding the defined courses to his/her weekly program.
*/
void addCourse()
{
    int searchCourse(char *);
    bool isValidTime(int);

    char courseID[10];

    printBanner();
    printf(COLOR_YELLOW "\tAdd Course:%s\n\n", COLOR_RESET);
    printf("\tThe ID of the course: ");
    scanf("%s", courseID); // getting course ID from the user
    printBanner();

    int courseIndice = searchCourse(courseID); // searching the course, it returns the course indice

    if (courseIndice == -1) // the course must found on the system
        printf(COLOR_RED "\n\tCourse not found!%s\n\n", COLOR_RESET);
    else if (stdClass < courses[courseIndice].class) // the user's class info must be sufficient
        printf(COLOR_RED "\n\tYour class info isn't enough to add this course!\n\n%s", COLOR_RESET);
    else if (!isValidTime(courseIndice)) // the courses must not overlaps
        printf(COLOR_RED "\n\tCourses are overlapping!\n\n%s", COLOR_RESET);
    else
    {
        // if there is no problem, then the indice of the course will be saved on the Schedule matrix 
        for (int i = 0; i < courses[courseIndice].occurence; i++)
        {
            int day = courses[courseIndice].dates[i].day - 1;

            for (int j = courses[courseIndice].dates[i].beginHour - 9; j < courses[courseIndice].dates[i].endHour - 9; j++)
                Schedule[j][day] = courseIndice;
        }

        // indice of course will be added to the array of enrolled courses also
        enrolledCourses[countEnrolled++] = courseIndice;
        printf(COLOR_GREEN "\n\n\tEnrolled to the course successfully!\n\n%s", COLOR_RESET);
    }

    goBackToMenu();
}

/*
    Function which ensures to user to delete the added courses from his/her weekly program.
*/
void deleteCourse()
{
    int searchCourse(char *);
    int searchEnrolledCourse(int);

    char courseID[10];

    printBanner();
    printf(COLOR_YELLOW "\tDelete Course:%s\n\n", COLOR_RESET);
    printf("\tThe ID of the course: ");
    scanf("%s", courseID); // getting course ID from the user
    printBanner();

    int courseIndice = searchCourse(courseID); // searching the course
    int enrolledIndice;

    // if the course found, then it will be searched on the array of the enrolled courses also
    if (courseIndice != -1)
        enrolledIndice = searchEnrolledCourse(courseIndice);

    if (courseIndice == -1) // the course must found in the defined courses
        printf(COLOR_RED "\t\nCourse not found!%s\n", COLOR_RESET);
    else if (enrolledIndice == -1) // the course must found in the enrolled courses
        printf(COLOR_RED "\t\nYou aren't enrolled this course already!%s\n", COLOR_RESET);
    else
    {
        // if there is no problem, then the indice of the course will be deleted on the Schedule matrix 
        for (int i = 0; i < courses[courseIndice].occurence; i++)
        {
            int day = courses[courseIndice].dates[i].day - 1;

            for (int j = courses[courseIndice].dates[i].beginHour - 9; j < courses[courseIndice].dates[i].endHour - 9; j++)
                Schedule[j][day] = -1;
        }

        // the enrolled courses are shifting
        for (int i = enrolledIndice; i < countEnrolled-1; i++)
            enrolledCourses[i] = enrolledCourses[i+1];

        countEnrolled--; // the count of the enrolled courses decreases

        printf(COLOR_GREEN "\n\n\tDeleted the course successfully!\n\n%s", COLOR_RESET);
    }

    goBackToMenu();
}

/*
    Function which searching a course on the array of the courses.
    @param courseID: The ID of the course which will be searched.
    return: If the course found, it returns the indice of the course. Otherwise it returns -1.
*/
int searchCourse(char *courseID)
{
    for (int i = 0; i < courseCount; i++)
        if (!strcmp(courses[i].ID, courseID))
            return i;

    return -1;
}

/*
    Function which checks the Schedule matrix to avoid the overlaps for the courses.
    @param courseIndice: The indice of the course which will be checked.
    @return: If it's valid, then returns true. Otherwise it returns false.
*/
bool isValidTime(int courseIndice)
{
    for (int i = 0; i < courses[courseIndice].occurence; i++)
    {
        int day = courses[courseIndice].dates[i].day - 1;
    
        for (int j = courses[courseIndice].dates[i].beginHour - 9; j < courses[courseIndice].dates[i].endHour - 9; j++)
            if (Schedule[j][day] != -1)
                return false;
    }

    return true;
}

/*
    Function which searches the course on the array of the enrolled courses.
    @param courseIndice: The indice of the course which will be searched.
    @return: If the course found, it returns the indice of the course. Otherwise it returns -1.
*/
int searchEnrolledCourse(int courseIndice)
{
    for (int i = 0; i < countEnrolled; i++)
        if (enrolledCourses[i] == courseIndice)
            return i;

    return -1;
}

/*
    Function which prints the weekly program.
*/
void printWeeklyPlan()
{
    printBanner();
    printf(COLOR_YELLOW "\tWeekly Plan:%s\n\n\n", COLOR_RESET);
    printf(COLOR_RED "\t             MON         TUE         WED         THR         FRI%s\n\n", COLOR_RESET);

    for (int i = 0; i < ROW; i++)
    {
        printf(COLOR_RED "\t%.2d:00   %s| ", i+9, COLOR_RESET);

        for (int j = 0; j < COL; j++)
        {
            int courseIndice = Schedule[i][j];

            if (courseIndice != -1)
                printf(COLOR_BLUE "%8s  %s| ", courses[courseIndice].ID, COLOR_RESET);
            else
                printf("          | ");
        }

        printf("\n");
    }

    printf("\n\n");
    goBackToMenu();
}

/*
    Function which prints the details of the enrolled courses.
*/
void printEnrolledCourses()
{
    char *week[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

    printBanner();
    printf(COLOR_YELLOW "\tThe Enrolled Courses:%s\n\n", COLOR_RESET);

    for (int i = 0; i < countEnrolled; i++)
    {
        int courseIndice = enrolledCourses[i];

        printf("\tCourse Name: %s%s%s", COLOR_BLUE, courses[courseIndice].name, COLOR_RESET);
        printf("\tCourse ID: %s%s%s\n", COLOR_BLUE, courses[courseIndice].ID, COLOR_RESET);
        printf("\tCourse Class Info: %s%d%s\n", COLOR_BLUE, courses[courseIndice].class, COLOR_RESET);

        for (int j = 0; j < courses[courseIndice].occurence; j++)
        {
            int day = courses[courseIndice].dates[j].day - 1;
            int beginHour = courses[courseIndice].dates[j].beginHour;
            int endHour = courses[courseIndice].dates[j].endHour;

            printf("\tTime for the #%d occurence: %s%s, ", j+1, COLOR_BLUE, week[day]);
            printf("%.2d:00 - %.2d:00%s\n", beginHour, endHour, COLOR_RESET);
        }

        printf("\n");
    }

    printf("\n");
    goBackToMenu();
}

/*
    Function which shows the memory crashing message to the user.
*/
void crashAllocation()
{
    system("clear");
    printf("\n\tNot enugh space.\n");
    exit(1);
}

/*
    Function which deallocate the array of the courses.
*/
void deallocateArrays()
{
    for (int i = 0; i < courseCount; i++)
        free(courses[i].dates);

    free(courses);
}

/*
    Function which ensures to getting arrow keys from the keyboard. It's a script 
    code that I found on the internet. (I don't know how it works.)
*/
int readKey()
{
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

/*
 * Names: Ahnaf Rahman, Hamza Makrod, Sayed Nuhan
 * Class: CPS188
 * Date: April 3 2022
 * Objective: 
 * The objective of this term project program was to take in data regarding the surface temperature of the 
 * great lakes of North America and calculate various values from the data to be viewed by the user in the terminal.
 * Language & Compiler: C Programing ; GCC Compiler
 * Description:
 * This program takes in annual surface temperature data for the great lakes of North America from a file and subsequently
 * calculates/extrapolates various values and information regarding the lakes based on the input data.
 */

#pragma region //Header Definitions

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#define MAX_FILE_NAME 100
#define DATA_ROWS 366
#define DATA_COLUMNS 8
#define LAKE_COUNT (DATA_COLUMNS - 2)

#pragma endregion

char lakeNames[LAKE_COUNT][400] = {"Lake Superior", "Lake Michigan", "Lake Huron", "Lake Erie", "Lake Ontario", "Lake St. Clair"};

#pragma region //Function Declaration

int dataExtract (char filename[MAX_FILE_NAME], double outputArray[DATA_ROWS][DATA_COLUMNS]);

double calculateyearlyLakeTempAverages (double inputArray[][DATA_COLUMNS], double lakeAverages[LAKE_COUNT], int year, int daysInYear);
void printYearlyLakeTempAverages(double inputArray[LAKE_COUNT], double combinedYearlyAverage, int year);
void temperatureGradient (double inputArray[], double combinedYearlyAverage);

void strSwap (char *t1, char *t2);

void formatDate(double inputArray[][DATA_COLUMNS], char formatedDate[1], double dayOfYear);
void coldestWarmestDay (double inputArray[][DATA_COLUMNS]);
void overallColdestWarmestDay (double inputArray[][DATA_COLUMNS]);

void summerAverage (double inputArray[][DATA_COLUMNS]); // Hamza
void winterAverage (double inputArray[][DATA_COLUMNS]); // Hamza

void daysSwimming (double inputArray[][DATA_COLUMNS]); //Ahnaf
void daysFrozen (double inputArray[][DATA_COLUMNS]); //Ahnaf

void compareData (double inputArrayA[][DATA_COLUMNS], double inputArrayB[][DATA_COLUMNS]); //Nuhan

#pragma endregion

//Main Program

int main() {

    #pragma region //Open dataFile and Fill 2D array

    
    double lakesData2019[DATA_ROWS][DATA_COLUMNS];
    double lakesData2020[DATA_ROWS][DATA_COLUMNS];
    char filename[MAX_FILE_NAME];

    do {
        system("CLS");
        printf("Enter Name of File Containing %c[4m\e[1m2019\e[0m%c[0m Temperature Data for the Great Lakes: ", 27, 27);
        scanf("%s", filename); 
    } while (dataExtract (filename, lakesData2019) == 0);

    //dataExtract ("Lakes_data_2019.txt", lakesData2019);

    do {
        system("CLS");
        printf("Enter Name of File Containing %c[4m\e[1m2020\e[0m%c[0m Temperature Data for the Great Lakes: ", 27, 27);
        scanf("%s", filename); 
    } while (dataExtract (filename, lakesData2020) == 0);

    //dataExtract ("Lakes_data_2020.txt", lakesData2020);


    #pragma endregion
    
    double combinedYearlyAverage;
    double lakeAverages[LAKE_COUNT];
    char date[400];

    printf("\n\e[1m%c[4m!!! ALL TEMPERATURE VALUES ARE IN DEGREES CELSIUS (ROUNDED TO NEAREST HUNDREDTHS) UNLESS OTHERWISE SPECIFIED !!!%c[0m\e[0m\n", 27, 27);
    printf("\n\e[1mPress ANY key to continue...\e[0m");
    getch();
    system("CLS");
    combinedYearlyAverage = calculateyearlyLakeTempAverages (lakesData2020, lakeAverages, 2020, DATA_ROWS);
    printYearlyLakeTempAverages(lakeAverages, combinedYearlyAverage, 2020);
    printf("\n\e[1mPress ANY key to continue...\e[0m");
    getch();
    system("CLS");
    temperatureGradient (lakeAverages, combinedYearlyAverage);
    printf("\n\e[1mPress ANY key to continue...\e[0m");
    getch();
    system("CLS");
    coldestWarmestDay (lakesData2020);
    printf("\n\e[1mPress ANY key to continue...\e[0m");
    getch();
    system("CLS");
    overallColdestWarmestDay(lakesData2020);
    printf("\n\e[1mPress ANY key to continue...\e[0m");
    getch();
    system("CLS");
    summerAverage(lakesData2020);
    winterAverage(lakesData2020);
    printf("\n\e[1mPress ANY key to continue...\e[0m");
    getch();
    system("CLS");
    daysSwimming(lakesData2020);
    daysFrozen(lakesData2020);
    printf("\n\e[1mPress ANY key to continue...\e[0m");
    getch();
    system("CLS");
    compareData(lakesData2019, lakesData2020);

    return (0);
}

int dataExtract (char filename[MAX_FILE_NAME], double outputArray[DATA_ROWS][DATA_COLUMNS]) {
    
    FILE *dataFile;

    char c;

    dataFile = fopen(filename, "r");

    system("CLS");

    if (dataFile == NULL)
    {
        printf("Could Not Find/Open File Named '%s' \nPress Any Key To Re-enter File Name \n", filename);
        getch();
        return (0);
    }

    //Check If Data File Has Content, Then Proceed

    c = fgetc(dataFile);

    if (c == EOF) {
        printf("Data File is Empty! \nPress Any Key To Re-enter File Name \n");
        getch();
        return (0);
    } else {
        fseek(dataFile, 0, SEEK_SET); //Reset File Pointer To Begining of File
    }

    //Main Loop
    while (c != EOF) {
        
        for (int i = 0; i < DATA_ROWS; ++i) { //tracks & iterates horizontal (row) position of 2D matrix
            for (int j = 0; j < DATA_COLUMNS; ++j) { //tracks & iterates vertical (column) position of 2D matrix
                fscanf (dataFile, " %lf", &outputArray[i][j]); //fills given cell with integer 77 each iteration
            }
        }

        c = fgetc(dataFile);
    }

    fclose(dataFile);
    return(1);
}

double calculateyearlyLakeTempAverages (double inputArray[][DATA_COLUMNS], double lakeAverages[LAKE_COUNT], int year, int daysInYear) {
    
    double sum = 0;
    double sumAll = 0;
    double combinedYearlyAverage;
    
    for (int i = 2; i < DATA_COLUMNS; ++i) { 

        sum = 0;

        for (int j = 0; j < daysInYear; ++j) { 
            sum = sum + inputArray[j][i];
            lakeAverages[i - 2] = sum/daysInYear;
        }
        sumAll = sumAll + sum;
    }
    
    combinedYearlyAverage = (sumAll / (daysInYear*6));

    return (combinedYearlyAverage);
}

void printYearlyLakeTempAverages(double inputArray[LAKE_COUNT], double combinedYearlyAverage, int year) {

    printf ("\n\e[1m> Average Surface Temperature of the Great Lakes in %d <\e[0m\n", year);

    printf ("\n%c[4mLake\t\t\tAverage Surface Temperature%c[0m \n\n", 27, 27);
    
    for (int i = 0; i < LAKE_COUNT; i++) {
        printf("%s\t\t%.2lf\n", lakeNames[i], inputArray[i]);
    }

    printf("\n%s\t%.2lf\n", "All Lakes Combined", combinedYearlyAverage);
}

void strSwap (char *t1, char *t2) {
    char buf[400];
    strcpy(buf, t1);
    strcpy(t1, t2);
    strcpy(t2, buf);
}

void temperatureGradient (double inputArray[], double combinedYearlyAverage) {

    int i, j;
    double temp;
    double sortedArray[LAKE_COUNT];
    char sortedlakeNames[LAKE_COUNT][400] = {"Lake Superior", "Lake Michigan", "Lake Huron", "Lake Erie", "Lake Ontario", "Lake St. Clair"};

    for (int k = 0; k < LAKE_COUNT; k++) {
	    sortedArray[k] = inputArray[k];
    }

    for (int i = 0; i < LAKE_COUNT; i++) {
        for (j = 0; j < (LAKE_COUNT - 1); j++) {
            if (sortedArray[j] > sortedArray [j + 1]) {
                temp = sortedArray [j];       
                sortedArray [j] = sortedArray [j + 1];
                sortedArray [j + 1] = temp;

                strSwap(sortedlakeNames[j], sortedlakeNames[j + 1]);
            }
        }
    }

    printf ("\n\e[1m> Sorted Average Surface Temperature of the Great Lakes in 2020 (Ordered Warmest to Coldest) <\e[0m\n");

    printf ("\n%c[4mLake\t\t\tAverage Surface Temperature%c[0m \n\n", 27, 27);
    
    for (int i = 0; i < LAKE_COUNT; i++) {
        printf("%s\t\t%.2lf\n", sortedlakeNames[(LAKE_COUNT - 1) - i], sortedArray[(LAKE_COUNT - 1) - i]);
    }

    printf("\n%s\t%.2lf\n", "All Lakes Combined", combinedYearlyAverage);
    printf("\nThe %c[4m\e[1mWarmest Lake\e[0m%c[0m on Average was %c[4m\e[1m%s\e[0m%c[0m", 27, 27, 27, sortedlakeNames[LAKE_COUNT - 1], 27);
    printf("\nThe %c[4m\e[1mColdest Lake\e[0m%c[0m on Average was %c[4m\e[1m%s\e[0m%c[0m", 27, 27, 27, sortedlakeNames[0], 27);

    printf ("\n\n\e[1m> Great Lakes Sorted By If They Were Above or Below The Annual Average Temperature Across All Lakes <\e[0m\n\n");

    int fillA = 0;
    int fillB = 0;
    char belowAverage[LAKE_COUNT][400];
    char aboveAverage[LAKE_COUNT][400];

    for (int i = 0; i < LAKE_COUNT; i++) {
        if (sortedArray[i] < combinedYearlyAverage) {
            strcpy(belowAverage[fillA], sortedlakeNames[i]);
            fillA++;
        }
        if (sortedArray[i] > combinedYearlyAverage) {
            strcpy(aboveAverage[fillB], sortedlakeNames[i]);
            fillB++;
        }
    }
    
    printf("%c[4mLakes Above Average Temperature:%c[0m\n\n", 27, 27);
    for (int i = 0; i < fillB; i++) {
        printf("%s\n", aboveAverage[i]);
    }

    printf("\n%c[4mLakes Below Average Temperature:%c[0m\n\n", 27, 27);
    for (int i = 0; i < fillA; i++) {
        printf("%s\n", belowAverage[i]);
    }

}

void formatDate (double inputArray[][DATA_COLUMNS], char formatedDate[400], double dayOfYear) {
    
    int day; 
    int year = inputArray[0][0];
    char monthString[8];
    int daysInMonth[12];
    char monthNames[12][400] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    
    for (int i = 0; i < 12; i += 2) {
        daysInMonth[i] = 31;
        for (int j = 1; j < 12; j += 2) {
            daysInMonth[j] = 30;
        }
    }


    if ((int)inputArray[0][0]  % 4 == 0 && (int)inputArray[0][0] % 100 != 0) {
        daysInMonth[1] = 29;
    } else if ((int)inputArray[0][0]  % 4 == 0 && (int)inputArray[0][0] % 100 == 0 && (int)inputArray[0][0] % 400 == 0) {
        daysInMonth[1] = 29;
    } else if ((int)inputArray[0][0]  % 4 == 0 && (int)inputArray[0][0] % 100 == 0 && (int)inputArray[0][0] % 400 != 0) {
        daysInMonth[1] = 28;
    } else if ((int)inputArray[0][0]  % 4 != 0) {
        daysInMonth[1] = 28;
    }

    int monthNumber = 0;

    if (dayOfYear <= 31){
        day = dayOfYear;
        strcpy(monthString, monthNames[0]);
    } else {
        do {
            dayOfYear = dayOfYear - daysInMonth[monthNumber];
            monthNumber++;
        } while (dayOfYear > 31);
        day = dayOfYear;
        strcpy(monthString, monthNames[monthNumber]);
    }

    sprintf(formatedDate, "%02d/%02d", day, monthNumber + 1);

}

void sortList (double inputArray[], double sortedArray[], int size) {

    int i, j;
    double temp;

    //Basic idea is that each array cell value is compared with its right hand neighbour cell value and swapped
    //with each other if they are out of order, the process is repeated until the whole array is sorted

    for (int k = 0; k < size; k++) {
	    sortedArray[k] = inputArray[k];
    }

    for (int i = 0; i < size; i++) {
        for (j = 0; j < (size - 1); j++) {
            if (sortedArray[j] > sortedArray [j + 1]) {
                temp = sortedArray [j];       //array cell swapping/ordering procedure using temporary storage variable
                sortedArray [j] = sortedArray [j + 1];
                sortedArray [j + 1] = temp;
            }
        }
    }
}

void coldestWarmestDay (double inputArray[][DATA_COLUMNS]) {

    double sorted[DATA_ROWS];

    double warmestTemp[LAKE_COUNT];
    double coldestTemp[LAKE_COUNT];

    double warmestDays[400][LAKE_COUNT];
    double coldestDays[400][LAKE_COUNT];

    char warmestDaysString[400][LAKE_COUNT][400];
    char coldestDaysString[400][LAKE_COUNT][400];
    
    int warmCounter[LAKE_COUNT];
    int coldCounter[LAKE_COUNT];


    for (int k = 0; k < LAKE_COUNT; k++) {
        
        double tempSort[DATA_ROWS];

        for (int i = 0; i < DATA_ROWS; i++) {
            tempSort[i] = inputArray[i][k + 2];
        }

        sortList(tempSort, sorted, DATA_ROWS);
        
        warmestTemp[k] = sorted[DATA_ROWS - 1];
        coldestTemp[k] = sorted[0];

        warmCounter[k] = 0;
        coldCounter[k] = 0;

        for (int j = 0; j < DATA_ROWS; j++) {
            if (inputArray[j][k + 2] == warmestTemp[k]) {
                warmestDays[warmCounter[k]][k] = inputArray[j][1];
                warmCounter[k]++;
            }
        }  

        for (int m = 0; m < DATA_ROWS; m++) {
            if (inputArray[m][k + 2] == coldestTemp[k]) {
                coldestDays[coldCounter[k]][k] = inputArray[m][1];
                coldCounter[k]++;
            }
        }    

    }
 
    printf ("\n\e[1m> Warmest & Coldest Days for Each Great Lake in 2020 (Date Format: Day/Month) <\e[0m\n");

    printf ("\n%c[4mLake\t\t\tWarmest Temp.\t\tWarmest Date(s)%c[0m \n\n", 27, 27);

    
    for (int k = 0; k < LAKE_COUNT; k++) {

        printf("%s\t\t%.2lf\t\t\t", lakeNames[k], warmestTemp[k]);
        
        for (int i = 0; i < warmCounter[k]; i++) {
            char temp[400]; 
            formatDate(inputArray, temp, warmestDays[i][k]);
            strcpy(warmestDaysString[i][k], temp);
            if (i == warmCounter[k] - 1) {
                printf("%s ", warmestDaysString[i][k]);
            } else {
                printf("%s, ", warmestDaysString[i][k]);
            } 
        } 
        printf("\n");
    }
    
    printf ("\n%c[4mLake\t\t\tColdest Temp.\t\tColdest Date(s)%c[0m \n\n", 27, 27);

    for (int k = 0; k < LAKE_COUNT; k++) {

        printf("%s\t\t%.2lf\t\t\t", lakeNames[k], coldestTemp[k]);
        
        for (int i = 0; i < coldCounter[k]; i++) {
            char temp[400]; 
            formatDate(inputArray, temp, coldestDays[i][k]);
            strcpy(coldestDaysString[i][k], temp);
            if (i == coldCounter[k] - 1) {
                printf("%s ", coldestDaysString[i][k]);
            } else {
                printf("%s, ", coldestDaysString[i][k]);
            } 
        } 
        printf("\n");
    }
}

void overallColdestWarmestDay (double inputArray[][DATA_COLUMNS]) {
    
    double sorted[DATA_ROWS];

    double warmestTempPerLake [LAKE_COUNT];
    double coldestTempPerLake [LAKE_COUNT];

    double overallWarmestTemp;
    double overallColdestTemp;

    double overallWarmestDays [400][LAKE_COUNT];
    double overallColdestDays [400][LAKE_COUNT];

    int warmestDaysCounter[LAKE_COUNT] = {0, 0, 0, 0, 0, 0};
    int coldestDaysCounter[LAKE_COUNT] = {0, 0, 0, 0, 0, 0};

    char overallWarmestDaysString[400][LAKE_COUNT][400];
    char overallColdestDaysString[400][LAKE_COUNT][400];

    for (int k = 0; k < LAKE_COUNT; k++) {

        double tempSort[DATA_ROWS];

        for (int i = 0; i < DATA_ROWS; i++) {
            tempSort[i] = inputArray[i][k + 2];
        }

        sortList(tempSort, sorted, DATA_ROWS);
        
        warmestTempPerLake[k] = sorted[DATA_ROWS - 1];
        coldestTempPerLake[k] = sorted[0];

    }

    sortList(warmestTempPerLake, sorted, LAKE_COUNT);
    overallWarmestTemp = sorted[LAKE_COUNT - 1];

    sortList(coldestTempPerLake, sorted, LAKE_COUNT);
    overallColdestTemp = sorted[0];

    // printf("\n%lf\n", overallWarmestTemp);
    // printf("\n%lf\n", overallColdestTemp);

    for (int k = 0; k < LAKE_COUNT; k++) {
        
        for (int i = 0; i < DATA_ROWS; i++) {
            if (inputArray[i][k + 2] == overallWarmestTemp) {
                overallWarmestDays[warmestDaysCounter[k]][k] = inputArray[i][1];
                warmestDaysCounter[k]++;
            }
        }

        for (int i = 0; i < DATA_ROWS; i++) {
            if (inputArray[i][k + 2] == overallColdestTemp) {
                overallColdestDays[coldestDaysCounter[k]][k] = inputArray[i][1];
                coldestDaysCounter[k]++;
            }
        }
    }

    printf ("\n\e[1m> Overall Warmest Day(s) On The Great Lakes <\e[0m\n\n");
    printf ("Overall Warmest Temperature: \e[1m%.2lf degrees Celsius\e[0m\n\n", overallWarmestTemp);
    printf ("%c[4mLake\t\t\tOverall Warmest Date(s)%c[0m \n\n", 27, 27);
    for (int k = 0; k < LAKE_COUNT; k++) {

        printf("%s\t\t", lakeNames[k]);
        
        if (warmestDaysCounter[k] == 0) {
            printf("N/A");
        }

        for (int i = 0; i < warmestDaysCounter[k]; i++) {
            
            char temp[400];
            formatDate(inputArray, temp, overallWarmestDays[i][k]);
            strcpy(overallWarmestDaysString[i][k], temp);
            
            if (i == warmestDaysCounter[k] - 1) {
                printf("%s ", overallWarmestDaysString[i][k]);
            } else {
                printf("%s, ", overallWarmestDaysString[i][k]);
            } 
        } 
        printf("\n");
    }

    printf ("\n\e[1m> Overall Coldest Day(s) On The Great Lakes <\e[0m\n\n");
    printf ("Overall Coldest Temperature: \e[1m%.2lf degrees Celsius\e[0m\n\n", overallColdestTemp);
    printf ("%c[4mLake\t\t\tOverall Coldest Date(s)%c[0m \n\n", 27, 27);
    for (int k = 0; k < LAKE_COUNT; k++) {

        printf("%s\t\t", lakeNames[k]);
        
        if (coldestDaysCounter[k] == 0) {
            printf("N/A");
        }

        for (int i = 0; i < coldestDaysCounter[k]; i++) {
            
            char temp[400];
            formatDate(inputArray, temp, overallColdestDays[i][k]);
            strcpy(overallColdestDaysString[i][k], temp);

            if (i == coldestDaysCounter[k] - 1) {
                printf("%s ", overallColdestDaysString[i][k]);
            } else {
                printf("%s, ", overallColdestDaysString[i][k]);
            } 
        } 
        printf("\n");
    }   
}

void summerAverage (double inputArray[][DATA_COLUMNS]) {
    
    double summerAverages[LAKE_COUNT];
    char tempSortLakeNames[LAKE_COUNT][400] = {"Lake Superior", "Lake Michigan", "Lake Huron", "Lake Erie", "Lake Ontario", "Lake St. Clair"};
    
    //printf ("\n\e[1mSummer & Winter Averages For Each Lake In 2020\e[0m\n");

    //printf ("\n%c[4mLake\t\t\tSummer Average%c[0m \n\n", 27, 27);
    for (int k = 0; k < LAKE_COUNT; k++) {
        
        double sum = 0;
        int counter = 0;
        
        for (int j = 172 - 1; j < 265; j++) {
            counter++;
            sum = sum + inputArray[j][k + 2];
        }

        summerAverages[k] = (sum/counter);
        
        //printf("%s\t\t%lf\n", lakeNames[k], summerAverages[k]);
    } 
 
        int i, j;
        double a;
        for (i = 0; i < LAKE_COUNT; ++i) 
        {
            for (j = i + 1; j < LAKE_COUNT; ++j) 
            {
                if (summerAverages[i] < summerAverages[j]) 
                {
                    a = summerAverages[i];
                    summerAverages[i] = summerAverages[j];
                    summerAverages[j] = a;
                    strSwap(tempSortLakeNames[j], tempSortLakeNames[i]);
                }
            }
        }
 
        printf("\n\e[1m> Average Summer Temperature For Each Great Lake In 2020 (Ordered Warmest To Coldest) <\e[0m\n");

        printf ("\n%c[4mLake\t\t\tSummer Average Temperature%c[0m \n\n", 27, 27);
        
        for (i = 0; i < LAKE_COUNT; ++i) 
        {
            printf("%s\t\t%.2lf\n", tempSortLakeNames[i], summerAverages[i]);
        }
 
}

void winterAverage (double inputArray[][DATA_COLUMNS]) {

    double winterAverages[LAKE_COUNT];
    char tempSortLakeNames[LAKE_COUNT][400] = {"Lake Superior", "Lake Michigan", "Lake Huron", "Lake Erie", "Lake Ontario", "Lake St. Clair"};
    //printf ("\n%c[4mLake\t\t\tWinter Average%c[0m \n\n", 27, 27);
    for (int k = 0; k < LAKE_COUNT; k++) {
        
        double sum = 0;
        int counter = 0;
        
        for (int j = 0; j < 79; j++) {
            counter++;
            sum = sum + inputArray[j][k + 2];
        } 
        for (int j = 354; j < 366; j++) {
            counter++;
            sum = sum + inputArray[j][k + 2];
        }
        // printf("\n%lf", sumA);

/* */
        winterAverages[k] = (sum/91);
        
        //printf("%s\t\t%lf\n", lakeNames[k], winterAverages[k]);
    } 
        int i, j;
        double a;
        for (i = 0; i < LAKE_COUNT; ++i) 
        {
            for (j = i + 1; j < LAKE_COUNT; ++j) 
            {
                if (winterAverages[i] < winterAverages[j]) 
                {
                    a = winterAverages[i];
                    winterAverages[i] = winterAverages[j];
                    winterAverages[j] = a;
                    strSwap(tempSortLakeNames[j], tempSortLakeNames[i]);
                }
            }
        }
 
        printf("\n\e[1m> Average Winter Temperature For Each Great Lake In 2020 (Ordered Warmest To Coldest) <\e[0m\n");

        printf ("\n%c[4mLake\t\t\tWinter Average Temperature%c[0m \n\n", 27, 27);
        for (i = 0; i < LAKE_COUNT; ++i) 
        {
        
        printf("%s\t\t%.2lf\n", tempSortLakeNames[i], winterAverages[i]);
        }
 
    }


void daysSwimming (double inputArray[][DATA_COLUMNS]) 
{
    int daysSwimmable[LAKE_COUNT];
    int i, j;

    for (i=0; i<LAKE_COUNT; ++i) 
    {
        int dayCountSWM = 0;
        for (j=0; j<DATA_ROWS; ++j)
        {
            if (inputArray[j][i+2] > 20)
            {
                dayCountSWM = dayCountSWM + 1;
            }
        }
    daysSwimmable[i] = dayCountSWM;
    }
    
    printf("\n\e[1m> Number of Days Swimmable For Each Great Lake In 2020 <\e[0m\n");

    printf ("\n%c[4mLake\t\t\tNumber of Days Swimmable%c[0m \n\n", 27, 27);
    for (int i = 0; i < 6; ++i) 
    {
        printf("%s\t\t%d\n", lakeNames[i], daysSwimmable[i]);
    }
}

void daysFrozen (double inputArray[][DATA_COLUMNS]) 
{
    int daysFrozen[LAKE_COUNT];
    int i, j;

    for (i=0; i<LAKE_COUNT; ++i) 
    {
        int dayCountFRZ = 0;
        for (j=0; j<DATA_ROWS; ++j)
        {
            if (inputArray[j][i+2] < 0)
            {
                dayCountFRZ = dayCountFRZ + 1;
            }
        }
    daysFrozen[i] = dayCountFRZ;
    }
    
    printf("\n\e[1m> Number of Days Frozen For Each Great Lake In 2020 <\e[0m\n");

    printf ("\n%c[4mLake\t\t\tNumber of Days Frozen%c[0m \n\n", 27, 27);
    for (int i = 0; i < 6; ++i) 
    {
        printf("%s\t\t%d\n", lakeNames[i], daysFrozen[i]);
    }
}

void compareData (double inputArrayA[][DATA_COLUMNS], double inputArrayB[][DATA_COLUMNS]) {

    double lakeAverages2020[LAKE_COUNT];
    double lakeAverages2019[LAKE_COUNT];
    double combinedYearlyAverage2019;
    double combinedYearlyAverage2020;

    printf ("\n\e[1m> Average Surface Temperature (A.S.T) of the Great Lakes (2019 & 2020 Comparison Table) <\e[0m\n");

    combinedYearlyAverage2019 = calculateyearlyLakeTempAverages (inputArrayA, lakeAverages2019, 2019, DATA_ROWS - 1);
    combinedYearlyAverage2020 = calculateyearlyLakeTempAverages (inputArrayB, lakeAverages2020, 2020, DATA_ROWS);

    printf ("\n%c[4mLake\t\t\tA.S.T (2019)\t\tA.S.T (2020)%c[0m \n\n", 27, 27);

    for (int k = 0; k < LAKE_COUNT; k++) {
        
        printf("%s\t\t%.2lf\t\t\t%.2lf\n", lakeNames[k], lakeAverages2019[k], lakeAverages2020[k]);
    
    }
    printf("\n%s\t%.2lf\t\t\t%.2lf\n\n", "All Lakes Combined", combinedYearlyAverage2019, combinedYearlyAverage2020);

}

/******************
Name:
ID:
Assignment: ex3
*******************/

#include <stdio.h>

// Assignment given constants
#define NUM_OF_BRANDS 5
#define BRANDS_NAMES 15
#define NUM_OF_TYPES 4
#define TYPES_NAMES 10
#define DAYS_IN_YEAR 365

char brands[NUM_OF_BRANDS][BRANDS_NAMES] = {"Toyoga", "HyunNight", "Mazduh", "FolksVegan", "Key-Yuh"};
char types[NUM_OF_TYPES][TYPES_NAMES] = {"SUV", "Sedan", "Coupe", "GT"};

#define ADD_ONE 1
#define ADD_ALL 2
#define STATS 3
#define PRINT 4
#define INSIGHTS 5
#define DELTAS 6
#define DONE 7

// Used as uninitialized value for arrays (specifically, the cube array)
#define DEFAULT_VALUE (-1)

void printMenu() {
    printf("Welcome to the Cars Data Cube! What would you like to do?\n"
           "1.Enter Daily Data For A Brand\n"
           "2.Populate A Day Of Sales For All Brands\n"
           "3.Provide Daily Stats\n"
           "4.Print All Data\n"
           "5.Provide Overall (simple) Insights\n"
           "6.Provide Average Delta Metrics\n"
           "7.exit\n");
}

/*
 Gets cube and days arrays
 Returns to menu if scanned carBrand is invalid
 Scans daily sales for a specific brand, and inserts it to the cube
*/
void scanBrandDailySales(int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES], int days[NUM_OF_BRANDS]) {
    int carBrand;
    scanf("%d", &carBrand);
    int brandDay = days[carBrand];

    /*
     Validates:
        - carBrand is between 0 and max brand index available (= NUM_OF_BRANDS - 1)
        - carBrand at brandDay isn't already set
        // TODO Before submit - check their exe works by cube[][][0]
    */
    if (carBrand < 0 || carBrand > NUM_OF_BRANDS - 1 || cube[brandDay][carBrand][0] != DEFAULT_VALUE) {
        printf("This brand is not valid\n");
        return;
    }

    // Insert values of all types sales to given brand in cube
    int dailySales[NUM_OF_TYPES] = {0};
    for (int carType = 0; carType < NUM_OF_TYPES; carType++) {
        scanf("%d", &dailySales[carType]);
        cube[brandDay][carBrand][carType] = dailySales[carType];
    }
}

/*
 Gets cube, days and remainingBrands arrays
 Sets remainingBrands to an array with this values format:
    - 0 means uninitialized brand sales
    - 1 means brand sales is initialized

 Returns whether all brands are initialized, 1 is true and 0 is false
*/
int getUninitializedBrands(int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES], int days[NUM_OF_BRANDS],\
    int remainingBrands[NUM_OF_BRANDS]) {
    int isDataCompleted = 1;
    for (int carBrand = 0; carBrand < NUM_OF_BRANDS; carBrand++) {
        int brandDay = days[carBrand];
        // TODO assuming first is all, like the todo in scanBrandDailySales()
        if (cube[brandDay][carBrand][0] == DEFAULT_VALUE)
            isDataCompleted = 0;
        else
            remainingBrands[carBrand] = 1;
    }
    return isDataCompleted;
}

int main() {
    // Initialize all cube values to default values
    int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES];
    for (int i = 0; i < DAYS_IN_YEAR; i++) {
        for (int j = 0; j < NUM_OF_BRANDS; j++) {
            for (int k = 0; k < NUM_OF_TYPES; k++) {
                cube[i][j][k] = DEFAULT_VALUE;
            }
        }
    }

    // Days array to keep track of the day for each brand
    int days[NUM_OF_BRANDS] = {0};
    int choice;
    printMenu();
    scanf("%d", &choice);
    while (choice != DONE) {
        switch(choice) {
            case ADD_ONE:
                scanBrandDailySales(cube, days);
                break;
            case ADD_ALL:
                // Init remainingBrands values to 0s, where 0 means uninitialized brand
                int remainingBrands[NUM_OF_BRANDS] = {0};
                // Verify if data is completed, meaning no brands remained without sales data
                int isDataCompleted = getUninitializedBrands(cube, days, remainingBrands);

                // Scan brand sales until all brands are scanned
                while (isDataCompleted == 0) {
                    printf("No data for brands");
                    for (int i = 0; i < NUM_OF_BRANDS; i++) {
                        if (remainingBrands[i] == 0)
                            printf(" %s", brands[i]);
                    }
                    printf("\nPlease complete the data\n");
                    scanBrandDailySales(cube, days);
                    isDataCompleted = getUninitializedBrands(cube, days, remainingBrands);
                }

                // Increase days counter
                for (int i = 0; i < NUM_OF_BRANDS; i++) {
                    days[i]++;
                }
                break;
            default:
                printf("Invalid input\n");
        }
        printMenu();
        scanf("%d", &choice);
    }
    printf("Goodbye!\n");
    return 0;
}



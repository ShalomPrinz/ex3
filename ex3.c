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
// App first day
#define FIRST_DAY 0
// Minimum of days needed to calculate deltas (Part 6)
#define MIN_DELTAS 1

/* Assumptions (verified with exe file / instructions file / forum answers):
    - If a type is initialized, I assume:
        1. all other types of the brand are initialized
        2. all other brands are initialized
    - User will insert positive (>0) number on Part 3
    - Part 1 will NOT BE USED as it can mess the system
 */

// App Logic
int scanBrandDailySales(int cube[][NUM_OF_BRANDS][NUM_OF_TYPES], int days[], int typesNum);
int getUninitializedBrands(int cube[][NUM_OF_BRANDS][NUM_OF_TYPES], int days[], int remainingBrands[], int brandsNum);
void scanFullDaySales(int cube[][NUM_OF_BRANDS][NUM_OF_TYPES], int days[], int brandsNum, int typesNum);
void printDailyStats(int cube[][NUM_OF_BRANDS][NUM_OF_TYPES], int statsDay, int brandsNum, int typesNum);
void printOverallInsights(int cube[][NUM_OF_BRANDS][NUM_OF_TYPES], int days[], int brandsNum, int typesNum);
void printBrandSalesDelta(int cube[][NUM_OF_BRANDS][NUM_OF_TYPES], int days[], int brand, int typesNum);

// Utils
int findArrayMax(int array[], int length);
int toUserDay(int day);
int toAppDay(int day);
int scanDay();

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

int main() {
    // Initialize all cube values to default values
    int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES];
    for (int day = FIRST_DAY; day < DAYS_IN_YEAR; day++) {
        for (int brand = 0; brand < NUM_OF_BRANDS; brand++) {
            for (int type = 0; type < NUM_OF_TYPES; type++) {
                cube[day][brand][type] = DEFAULT_VALUE;
            }
        }
    }

    // Days array to keep track of the day for each brand
    int days[NUM_OF_BRANDS] = {FIRST_DAY, FIRST_DAY, FIRST_DAY, FIRST_DAY, FIRST_DAY};
    int choice;
    printMenu();
    scanf("%d", &choice);

    while (choice != DONE) {
        switch (choice) {
            case ADD_ONE:
                printf("What brand?\n");
                int brandSet = scanBrandDailySales(cube, days, NUM_OF_TYPES);
                if (brandSet != -1) days[brandSet]++;
                break;

            case ADD_ALL:
                scanFullDaySales(cube, days, NUM_OF_BRANDS, NUM_OF_TYPES);

                // Increase days counter for all brands
                for (int brand = 0; brand < NUM_OF_BRANDS; brand++) {
                    days[brand]++;
                }
                break;

            case STATS:
                printf("What day would you like to analyze?\n");
                int statsDay = scanDay();

                /* Validates:
                    - statsDay is between 0 and max possible day (DAYS_IN_YEAR)
                    - statsDay is a day with initialized data in cube
                */
                while (statsDay < FIRST_DAY || statsDay > DAYS_IN_YEAR || cube[statsDay][0][0] == DEFAULT_VALUE) {
                    printf("Please enter a valid day.\nWhat day would you like to analyze?\n");
                    statsDay = scanDay();
                }

                // Calculate and print required stats
                printDailyStats(cube, statsDay, NUM_OF_BRANDS, NUM_OF_TYPES);
                break;

            case PRINT:
                printf("*****************************************\n");

                // Print dataset in given format
                for (int brand = 0; brand < NUM_OF_BRANDS; brand++) {
                    printf("\nSales for %s:", brands[brand]);
                    for (int day = FIRST_DAY; day < days[brand]; day++) {
                        printf("\nDay %d-", toUserDay(day));
                        for (int type = 0; type < NUM_OF_TYPES; type++) {
                            printf(" %s: %d", types[type], cube[day][brand][type]);
                        }
                    }
                }

                printf("\n\n*****************************************\n");
                break;

            case INSIGHTS:
                printOverallInsights(cube, days, NUM_OF_BRANDS, NUM_OF_TYPES);
                break;

            case DELTAS:
                for (int brand = 0; brand < NUM_OF_BRANDS; brand++)
                    printBrandSalesDelta(cube, days, brand, NUM_OF_TYPES);
                break;

            case DONE:
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

/*
 Gets cube and days arrays, along with types of car the brand sells
 If scanned carBrand is valid:
    Scans daily sales for a specific brand, and inserts it to the cube
    Returns carBrand
 Otherwise returns -1, which represents invalid car brand.
*/
int scanBrandDailySales(int cube[][NUM_OF_BRANDS][NUM_OF_TYPES], int days[], int typesNum) {
    int carBrand;
    scanf("%d", &carBrand);
    int brandDay = days[carBrand];

    /*
     Validates:
        - carBrand is between 0 and max brand index available (= NUM_OF_BRANDS - 1)
        - carBrand at brandDay isn't already set
    */
    if (carBrand < 0 || carBrand > NUM_OF_BRANDS - 1 || cube[brandDay][carBrand][0] != DEFAULT_VALUE) {
        printf("This brand is not valid\n");
        return -1;
    }

    // Insert values of all types sales to given brand in cube
    for (int carType = 0; carType < typesNum; carType++)
        scanf("%d", &cube[brandDay][carBrand][carType]);

    return carBrand;
}

/*
 Gets cube, days, remainingBrands and brandsNum
 Sets remainingBrands to an integers array with this values format:
    - 0 means uninitialized brand sales
    - 1 means brand sales are initialized

 Returns whether all brands are initialized, 1 is true and 0 is false
*/
int getUninitializedBrands(int cube[][NUM_OF_BRANDS][NUM_OF_TYPES], int days[],\
    int remainingBrands[], int brandsNum) {
    int isDataCompleted = 1;
    for (int carBrand = 0; carBrand < brandsNum; carBrand++) {
        int brandDay = days[carBrand];
        if (cube[brandDay][carBrand][0] == DEFAULT_VALUE)
            isDataCompleted = 0;
        else
            remainingBrands[carBrand] = 1;
    }
    return isDataCompleted;
}

/*
 Gets cube, days, brandsNum and typesNum
 Scans new daily data for each brand and inserts data to cube
 Returns when all brands are filled with new daily data
*/
void scanFullDaySales(int cube[][NUM_OF_BRANDS][NUM_OF_TYPES], int days[], int brandsNum, int typesNum) {
    // Init remainingBrands values to 0s, where 0 means uninitialized brand
    int remainingBrands[NUM_OF_BRANDS] = {0};
    // Verify if data is completed, meaning no brands remained without sales data
    int isDataCompleted = getUninitializedBrands(cube, days, remainingBrands, brandsNum);

    // Scan brand sales until all brands are scanned
    while (isDataCompleted == 0) {
        printf("No data for brands");
        for (int i = 0; i < brandsNum; i++) {
            if (remainingBrands[i] == 0)
                printf(" %s", brands[i]);
        }
        printf("\nPlease complete the data\n");
        scanBrandDailySales(cube, days, typesNum);
        isDataCompleted = getUninitializedBrands(cube, days, remainingBrands, brandsNum);
    }
}

/*
 Gets cube and selected day
 Calculates required stats of the given statsDay
 Prints stats in given format
 */
void printDailyStats(int cube[][NUM_OF_BRANDS][NUM_OF_TYPES], int statsDay, int brandsNum, int typesNum) {
    int totalSales = 0;
    int typeSales[NUM_OF_TYPES] = {0};
    int brandSales[NUM_OF_BRANDS] = {0};

    for (int carBrand = 0; carBrand < brandsNum; carBrand++) {
        int currentBrandSales = 0;

        // Count brand sales, and insert type sales into typeSales array
        for (int carType = 0; carType < typesNum; carType++) {
            int sales = cube[statsDay][carBrand][carType];
            typeSales[carType] += sales;
            currentBrandSales += sales;
        }

        brandSales[carBrand] = currentBrandSales;
        totalSales += currentBrandSales;
    }

    // Find best-selling type
    int bestSellingType = findArrayMax(typeSales, typesNum);
    int mostTypeSales = typeSales[bestSellingType];

    // Find best-selling brand
    int bestSellingBrand = findArrayMax(brandSales, brandsNum);
    int mostBrandSales = brandSales[bestSellingBrand];

    // Print stats in given format
    printf("In day number %d:\n"
           "The sales total was %d\n"
           "The best sold brand with %d sales was %s\n"
           "The best sold type with %d sales was %s\n",
           toUserDay(statsDay), totalSales, mostBrandSales, brands[bestSellingBrand],
           mostTypeSales, types[bestSellingType]);
}

/*
 Gets cube and days arrays with their lengths
 Calculates required insights on all data days on each brand
 Prints insights in given format
*/
void printOverallInsights(int cube[][NUM_OF_BRANDS][NUM_OF_TYPES], int days[], int brandsNum, int typesNum) {
    int brandTotals[NUM_OF_BRANDS] = {0};
    int daysTotals[DAYS_IN_YEAR] = {0};
    int typeTotals[NUM_OF_TYPES] = {0};

    for (int brand = 0; brand < brandsNum; brand++) {
        int currentBrandSales = 0;
        for (int day = FIRST_DAY; day < days[brand]; day++) {
            int currentDaySales = 0;

            // Count all types sold of "brand" at "day"
            for (int type = 0; type < typesNum; type++) {
                int sales = cube[day][brand][type];
                typeTotals[type] += sales;
                currentDaySales += sales;
            }
            // Count daily sales and brand sales
            daysTotals[day] += currentDaySales;
            currentBrandSales += currentDaySales;
        }
        brandTotals[brand] += currentBrandSales;
    }

    // Find best-selling type
    int bsTypeIndex = findArrayMax(typeTotals, typesNum);
    int bsTypeSales = typeTotals[bsTypeIndex];

    // Find best-selling brand
    int bsBrandIndex = findArrayMax(brandTotals, brandsNum);
    int bsBrandSales = brandTotals[bsBrandIndex];

    // Find best-selling day
    int bsDayIndex = findArrayMax(daysTotals, DAYS_IN_YEAR);
    int bsDaySales = daysTotals[bsDayIndex];

    printf("The best-selling brand overall is %s: %d$\n"
           "The best-selling type of car is %s: %d$\n"
           "The most profitable day was day number %d: %d$\n",
           brands[bsBrandIndex], bsBrandSales, types[bsTypeIndex],
           bsTypeSales, toUserDay(bsDayIndex), bsDaySales);
}

/*
 Gets cube, lastDay and typesNum
 Calculates given brand average delta metric by the given formula:
    sum of delta in sales between each day to the next one / deltas count
 The formula can be simplified to another formula (both formulas are equivalent):
    delta in sales between last day and first day / deltas count

 Prints given brand average delta metric in float number, precision of 6 digits after decimal point
*/
void printBrandSalesDelta(int cube[][NUM_OF_BRANDS][NUM_OF_TYPES], int days[], int brand, int typesNum) {
    // calculate sales of "brand" per day
    int daysTotal[DAYS_IN_YEAR] = {0};
    int day = FIRST_DAY;
    for (; day < days[brand]; day++) {
        int currentDaySales = 0;
        for (int type = 0; type < typesNum; type++)
            currentDaySales += cube[day][brand][type];
        daysTotal[day] += currentDaySales;
    }

    // Calculate average delta metrics on "daysTotal" array
    int lastDay = day - 1; // last day with data
    // deltas are sales changes between a day to the next day, where changes count could be 0
    int deltasCount = lastDay - FIRST_DAY;
    float averageDeltaMetric = 0;

    // Calculates deltas average if enough data is gathered
    if (deltasCount >= MIN_DELTAS) {
        // nominator in average delta formula: last day sales minus first day sales (edgesDelta)
        int edgesDelta = daysTotal[lastDay] - daysTotal[FIRST_DAY];
        // Formula: averageDelta = nominator (edgesDelta) / denominator (deltasCount)
        averageDeltaMetric = (float) edgesDelta / (float) deltasCount;
    }
    printf("Brand: %s, Average Delta: %.6f\n", brands[brand], averageDeltaMetric);
}

/*
 Gets an integers array and its length
 Returns the index of the biggest number in the array
*/
int findArrayMax(int array[], int length) {
    int index = 0;
    int max = array[index];
    for (int i = 0; i < length; i++) {
        if (array[i] > max) {
            max = array[i];
            index = i;
        }
    }
    return index;
}

/*
 Converts app day to user day
 App days: 0 - 364
 User days: 1 - 365
*/
int toUserDay(int day) {
    return day + 1;
}

/*
 Converts user day to app day
 App days: 0 - 364
 User days: 1 - 365
*/
int toAppDay(int day) {
    return day - 1;
}

/*
 Shortcut to scan a day from user and convert it to app day
*/
int scanDay() {
    int day;
    scanf("%d", &day);
    return toAppDay(day);
}
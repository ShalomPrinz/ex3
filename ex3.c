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
#define FIRST_DAY 1
// Minimum of days needed to calculate deltas (Part 6)
#define MIN_DELTAS 3

// TODO are pointers allowed? arrays as parameters? forum

// TODO average metrics
//      - does it matter +- 0.0000?
//      - doesn't calculate for

/* Assumptions (verified with exe file / instructions file / forum answers):
    - If a type is initialized, I assume:
        1. all other types of the brand are initialized
        2. all other brands are initialized
    - User will insert positive (>0) number on Part 3
    - Part 1 will NOT BE USED as it can mess the system
 */

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
 Gets an array TODO
 Sets etc.
*/
void findArrayMax(int array[], int length, int *index, int *sales) {
    for (int i = 0; i < length; i++) {
        if (array[i] > *sales) {
            *sales = array[i];
            *index = i;
        }
    }
}

/*
 Gets cube and days arrays
 Gets back to menu if scanned carBrand is invalid
 Scans daily sales for a specific brand, and inserts it to the cube
 Returns -1 if no brand had been set, otherwise the set brand index
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
    // Increase day counter for the brand set
    days[carBrand]++;
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

/*
 Gets cube, selected day for calculations and stats array that will contain the results
 Sets stats to an array containing 5 required stats, by this order:
    0: total sales
    1: count of most sales a brand sold
    2: the brand who sold the most
    3: count of most sales a type sold
    4: the type who sold the most
 */
void calculateDailyStats(int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES], int statsDay, int stats[]) {
    int totalSales = 0, bestSellingBrand = 0, mostBrandSales = 0;
    int typeSales[NUM_OF_TYPES] = {0};

    for (int carBrand = 0; carBrand < NUM_OF_BRANDS; carBrand++) {
        int currentBrandSales = 0;

        // Count brand sales, and insert type sales into typeSales array
        for (int carType = 0; carType < NUM_OF_TYPES; carType++) {
            int sales = cube[statsDay][carBrand][carType];
            typeSales[carType] += sales;
            currentBrandSales += sales;
        }

        // Find best-selling brand
        if (currentBrandSales > mostBrandSales) {
            mostBrandSales = currentBrandSales;
            bestSellingBrand = carBrand;
        }
        totalSales += currentBrandSales;
    }

    // Find best-selling type
    int bestSellingType = 0, mostTypeSales = 0;
    findArrayMax(typeSales, NUM_OF_TYPES, &bestSellingType, &mostTypeSales);

    // Assign calculation results to stats array
    stats[0] = totalSales;
    stats[1] = mostBrandSales;
    stats[2] = bestSellingBrand;
    stats[3] = mostTypeSales;
    stats[4] = bestSellingType;
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
    int days[NUM_OF_BRANDS] = {FIRST_DAY, FIRST_DAY, FIRST_DAY, FIRST_DAY, FIRST_DAY};
    int choice;
    printMenu();
    scanf("%d", &choice);

    while (choice != DONE) {
        switch (choice) {
            case ADD_ONE:
                printf("What brand?\n");
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
                break;

            case STATS:
                printf("What day would you like to analyze?\n");
                int statsDay;
                scanf("%d", &statsDay);

                /* Validates:
                    - statsDay is between 0 and max possible day (DAYS_IN_YEAR)
                    - statsDay is a day with initialized data in cube
                */
                while (statsDay < FIRST_DAY || statsDay > DAYS_IN_YEAR || cube[statsDay][0][0] == DEFAULT_VALUE) {
                    printf("Please enter a valid day.\nWhat day would you like to analyze?\n");
                    scanf("%d", &statsDay);
                }

                // Calculate required stats
                int stats[5] = {0};
                calculateDailyStats(cube, statsDay, stats);

                // Print stats in given format
                printf("In day number %d:\n"
                       "The sales total was %d\n"
                       "The best sold brand with %d sales was %s\n"
                       "The best sold type with %d sales was %s\n",
                       statsDay, stats[0], stats[1], brands[stats[2]], stats[3], types[stats[4]]);
                break;

            case PRINT:
                printf("*****************************************\n");

                // Print dataset in given format
                for (int brand = 0; brand < NUM_OF_BRANDS; brand++) {
                    printf("\nSales for %s:", brands[brand]);
                    for (int day = FIRST_DAY; day < days[brand]; day++) {
                        printf("\nDay %d-", day);
                        for (int type = 0; type < NUM_OF_TYPES; type++) {
                            printf(" %s: %d", types[type], cube[day][brand][type]);
                        }
                    }
                }

                printf("\n\n*****************************************\n");
                break;

            case INSIGHTS: {
                int brandTotals[NUM_OF_BRANDS] = {0};
                int daysTotals[DAYS_IN_YEAR] = {0};
                int typeTotals[NUM_OF_TYPES] = {0};

                for (int brand = 0; brand < NUM_OF_BRANDS; brand++) {
                    int currentBrandSales = 0;
                    for (int day = FIRST_DAY; day < days[brand]; day++) {
                        int currentDaySales = 0;

                        // Count all types sold of "brand" at "day"
                        for (int type = 0; type < NUM_OF_TYPES; type++) {
                            int sales = cube[day][brand][type];
                            typeTotals[type] += sales;
                            currentDaySales += sales;
                        }
                        daysTotals[day] += currentDaySales;
                        currentBrandSales += currentDaySales;
                    }
                    brandTotals[brand] += currentBrandSales;
                }

                // Find best-selling type
                int bsTypeIndex = 0, bsTypeSales = 0;
                findArrayMax(typeTotals, NUM_OF_TYPES, &bsTypeIndex, &bsTypeSales);

                // Find best-selling brand
                int bsBrandIndex = 0, bsBrandSales = 0;
                findArrayMax(brandTotals, NUM_OF_BRANDS, &bsBrandIndex, &bsBrandSales);

                // Find best-selling day
                int bsDayIndex = 0, bsDaySales = 0;
                findArrayMax(daysTotals, DAYS_IN_YEAR, &bsDayIndex, &bsDaySales);

                printf("The best-selling brand overall is %s: %d$\n"
                       "The best-selling type of car is %s: %d$\n"
                       "The most profitable day was day number %d: %d$\n",
                       brands[bsBrandIndex], bsBrandSales, types[bsTypeIndex],
                       bsTypeSales, bsDayIndex, bsDaySales);
                break;
            }

            case DELTAS:
                for (int brand = 0; brand < NUM_OF_BRANDS; brand++) {
                    int daysTotal[DAYS_IN_YEAR] = {0};
                    int day = FIRST_DAY;
                    for (; day < days[brand]; day++) {
                        int currentDaySales = 0;
                        for (int type = 0; type < NUM_OF_TYPES; type++)
                            currentDaySales += cube[day][brand][type];
                        daysTotal[day] += currentDaySales;
                    }

                    int deltasCount = day - 1;
                    float averageDeltaMetric = 0;
                    // Calculates deltas average if enough data is gathered
                    if (deltasCount >= MIN_DELTAS) {
                        int nominator = 0;
                        for (int d = FIRST_DAY + 1; d < day; d++) {
                            nominator += daysTotal[d] - daysTotal[d - 1];
                        }
                        averageDeltaMetric = (float) nominator / (float) deltasCount;
                    }
                    printf("Brand: %s, Average Delta: %.6f\n", brands[brand], averageDeltaMetric);
                }
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



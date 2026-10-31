/* 2026FA-ENGR-2304-00003-TRAVIS-ILEX-P07
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DEFAULT_FILE "data.csv"
#define MAX_RECORDS 2000
#define PLANTS 6
#define LINE_LENGTH 256

typedef struct {
    int year, month, day;
    int units[PLANTS];
} Record;

typedef struct {
    int minimum, maximum, mode, mode_frequency;
    long total;
    double mean, variance, standard_deviation;
} Statistics;

static Record records[MAX_RECORDS];
static Statistics stats[PLANTS];
static int record_count = 0;

/* Reads CSV records and skips the header or any malformed lines. */
int load_data(const char *filename)
{
    FILE *file = fopen(filename, "r");
    char line[LINE_LENGTH];
    int loaded = 0;

    if (file == NULL) {
        printf("Error: Could not open '%s'.\n", filename);
        printf("Upload/rename the datafile to data.txt, or provide its filename.\n");
        return 0;
    }

    while (fgets(line, sizeof line, file) != NULL && loaded < MAX_RECORDS) {
        Record r;
        int fields = sscanf(line, " %d , %d , %d , %d , %d , %d , %d , %d , %d",
            &r.year, &r.month, &r.day,
            &r.units[0], &r.units[1], &r.units[2],
            &r.units[3], &r.units[4], &r.units[5]);
        if (fields == 9) {
            records[loaded++] = r;
        }
    }

    fclose(file);
    if (loaded == MAX_RECORDS)
        printf("Warning: approached the maximum of %d records.\n", MAX_RECORDS);
    return loaded;
}

/* Finds the most frequent value for a plant. Ties keep the smaller value. */
void find_mode(int plant, int *mode, int *frequency)
{
    int i, j;
    *mode = records[0].units[plant];
    *frequency = 0;

    for (i = 0; i < record_count; i++) {
        int value = records[i].units[plant];
        int occurrences = 0;
        for (j = 0; j < record_count; j++) {
            if (records[j].units[plant] == value)
                occurrences++;
        }
        if (occurrences > *frequency ||
            (occurrences == *frequency && value < *mode)) {
            *frequency = occurrences;
            *mode = value;
        }
    }
}

/* Calculates descriptive statistics for all six production lines. */
void calculate_statistics(void)
{
    int p, i;
    for (p = 0; p < PLANTS; p++) {
        double squared_differences = 0.0;
        Statistics *s = &stats[p];
        s->minimum = records[0].units[p];
        s->maximum = records[0].units[p];
        s->total = 0;

        for (i = 0; i < record_count; i++) {
            int value = records[i].units[p];
            if (value < s->minimum) s->minimum = value;
            if (value > s->maximum) s->maximum = value;
            s->total += value;
        }
        s->mean = (double)s->total / record_count;

        for (i = 0; i < record_count; i++) {
            double difference = records[i].units[p] - s->mean;
            squared_differences += difference * difference;
        }
        /* Population variance and standard deviation, using all recorded days. */
        s->variance = squared_differences / record_count;
        s->standard_deviation = sqrt(s->variance);
        find_mode(p, &s->mode, &s->mode_frequency);
    }
}

void show_statistics(void)
{
    int p;
    printf("\n=== Production Statistics (%d records) ===\n", record_count);
    printf("Plant   Min   Max   Mode      Mean   Variance    StdDev       Total\n");
    for (p = 0; p < PLANTS; p++) {
        Statistics *s = &stats[p];
        printf("L%-5d %5d %5d %6d %10.2f %10.2f %9.2f %11ld\n",
               p + 1, s->minimum, s->maximum, s->mode, s->mean,
               s->variance, s->standard_deviation, s->total);
    }
    printf("Mode frequency is the number of days the mode occurred.\n");
    for (p = 0; p < PLANTS; p++)
        printf("L%d mode occurred %d time(s).\n", p + 1, stats[p].mode_frequency);
}

void show_report(void)
{
    int p, i, best_plant = 0, worst_plant = 0;
    int best_single_value = records[0].units[0], best_single_record = 0, best_single_plant = 0;
    int best_day_total = -1, best_day_record = 0;

    for (p = 1; p < PLANTS; p++) {
        if (stats[p].total > stats[best_plant].total) best_plant = p;
        if (stats[p].total < stats[worst_plant].total) worst_plant = p;
    }
    for (i = 0; i < record_count; i++) {
        int day_total = 0;
        for (p = 0; p < PLANTS; p++) {
            day_total += records[i].units[p];
            if (records[i].units[p] > best_single_value) {
                best_single_value = records[i].units[p];
                best_single_record = i;
                best_single_plant = p;
            }
        }
        if (day_total > best_day_total) {
            best_day_total = day_total;
            best_day_record = i;
        }
    }

    printf("\n=== Manufacturing Production Report ===\n");
    printf("Records analyzed: %d days\n", record_count);
    printf("Highest total production: L%d, %ld units (average %.2f units/day)\n",
           best_plant + 1, stats[best_plant].total, stats[best_plant].mean);
    printf("Lowest total production:  L%d, %ld units (average %.2f units/day)\n",
           worst_plant + 1, stats[worst_plant].total, stats[worst_plant].mean);
    printf("Largest single-line daily output: L%d made %d units on %02d/%02d/%04d\n",
           best_single_plant + 1, best_single_value,
           records[best_single_record].month, records[best_single_record].day,
           records[best_single_record].year);
    printf("Highest combined daily output: %d units on %02d/%02d/%04d\n",
           best_day_total, records[best_day_record].month,
           records[best_day_record].day, records[best_day_record].year);
}

/* Returns the record index for a date, or -1 if that date is not present. */
int search_date(int year, int month, int day)
{
    int i;
    for (i = 0; i < record_count; i++) {
        if (records[i].year == year && records[i].month == month && records[i].day == day)
            return i;
    }
    return -1;
}

int get_integer(const char *prompt, int *value)
{
    char input[80];
    char extra;
    printf("%s", prompt);
    if (fgets(input, sizeof input, stdin) == NULL) return 0;
    return sscanf(input, " %d %c", value, &extra) == 1;
}

void search_production(void)
{
    int year, month, day, plant, index, p, total = 0;
    printf("\n=== Search Production Data by Date ===\n");
    if (!get_integer("Year: ", &year) || !get_integer("Month (1-12): ", &month) ||
        !get_integer("Day (1-31): ", &day)) {
        printf("Invalid date input.\n");
        return;
    }
    index = search_date(year, month, day);
    if (index == -1) {
        printf("No record found for %02d/%02d/%04d.\n", month, day, year);
        return;
    }
    if (!get_integer("Plant number 1-6, or 0 for all plants: ", &plant) || plant < 0 || plant > PLANTS) {
        printf("Invalid plant selection.\n");
        return;
    }
    printf("\nProduction on %02d/%02d/%04d:\n", month, day, year);
    if (plant == 0) {
        for (p = 0; p < PLANTS; p++) {
            printf("  L%d: %d units\n", p + 1, records[index].units[p]);
            total += records[index].units[p];
        }
        printf("  Combined total: %d units\n", total);
    } else {
        printf("  L%d: %d units\n", plant, records[index].units[plant - 1]);
    }
}

void show_menu(void)
{
    printf("\n========== PRODUCTION DATA MENU ==========\n");
    printf("1. Display statistical analysis\n");
    printf("2. Display production report\n");
    printf("3. Search production by date\n");
    printf("4. Exit\n");
    printf("==========================================\n");
}

int main(int argc, char *argv[])
{
    const char *filename = argc > 1 ? argv[1] : DEFAULT_FILE;
    int choice = 0;

    record_count = load_data(filename);
    if (record_count == 0) {
        printf("No valid recordwere loaded. Program ending.\n");
        return 1;
    }
    printf("Successfully loaded %d records from %s.\n", record_count, filename);
    calculate_statistics();

    do {
        show_menu();
        if (!get_integer("Choose an option (1-4): ", &choice)) {
            printf("Please enter a number from 1 to 4.\n");
            choice = 0;
            continue;
        }
        switch (choice) {
            case 1: show_statistics(); break;
            case 2: show_report(); break;
            case 3: search_production(); break;
            case 4: printf("Goodbye.\n"); break;
            default: printf("Invalid option. Choose 1, 2, 3, or 4.\n");
        }
    } while (choice != 4);

    return 0;
}

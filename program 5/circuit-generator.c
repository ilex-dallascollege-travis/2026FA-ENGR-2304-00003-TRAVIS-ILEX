/*2026FA-ENGR-2304-00003-TRAVIS-ILEX-P04*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
 
#define TOLERANCE 1e-3
 
double truncate3(double value) {
    return trunc(value * 1000.0) / 1000.0;
}
int isCorrect(double userValue, double correctValue) {
    return fabs(userValue - correctValue) <= TOLERANCE;
}
double askForValue(const char *label) {
    double value;
    while (1) {
        printf("Enter your value for %s: ", label);
        int result = scanf("%lf", &value);
        if (result == 1) {
            return value;
        } else if (result == EOF) {
            printf("\nNo more input available. Exiting.\n");
            exit(0);
        } else {
            printf("Invalid input. Please enter a numeric value.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
        }
    }
}
int checkAndReport(const char *label, double userValue, double correctValue) {
    double truncatedCorrect = truncate3(correctValue);
    if (isCorrect(userValue, truncatedCorrect)) {
        printf("  %-4s -> SUCCESS  (your answer: %.3f, correct: %.3f)\n",
               label, userValue, truncatedCorrect);
        return 1;
    } else {
        printf("  %-4s -> FAILURE  (your answer: %.3f, correct: %.3f)\n",
               label, userValue, truncatedCorrect);
        return 0;
    }
}
void solve2x2(double a, double b, double c, double d,
              double e, double f, double *i1, double *i2) {
    double D = a * d - b * c;
    *i1 = (e * d - b * f) / D;
    *i2 = (a * f - e * c) / D;
}
 
int main(void) {
    char choice[16];
    int roundNumber = 1;
 
    srand((unsigned int)time(NULL));
 
    printf("=====================================================\n");
    printf(" Circuit Problem Generator\n");
    printf(" Vs in series with R1, feeding the shared node of R2 and R3\n");
    printf(" (mesh 1: Vs-R1-R2 | mesh 2: R2-R3)\n");
    printf("=====================================================\n\n");
 
    do {
        
        int Vs = (rand() % 10) + 1; 
        int R1 = (rand() % 20) + 1;  
        int R2 = (rand() % 20) + 1;  
        int R3 = (rand() % 20) + 1;  
 
        double i1, i2;
        solve2x2(R1 + R2, -R2, -R2, R2 + R3, Vs, 0.0, &i1, &i2);
 
        double v1 = i1 * R1;
        double v2 = (i1 - i2) * R2;
        double v3 = i2 * R3;
 
        printf("\n--- Round %d ---\n", roundNumber);
        printf("Circuit values:\n");
        printf("  Vs = %d V\n", Vs);
        printf("  R1 = %d ohm\n", R1);
        printf("  R2 = %d ohm\n", R2);
        printf("  R3 = %d ohm\n", R3);
        printf("\nMesh 1 (i1) runs through Vs, R1 and R2.\n");
        printf("Mesh 2 (i2) runs through R2 and R3.\n\n");
 
        double userI1 = askForValue("i1 (mesh current 1, in A)");
        double userI2 = askForValue("i2 (mesh current 2, in A)");
        double userV1 = askForValue("v1 (voltage across R1, in V)");
        double userV2 = askForValue("v2 (voltage across R2, in V)");
        double userV3 = askForValue("v3 (voltage across R3, in V)");
 
        printf("\nResults:\n");
        int correctCount = 0;
        correctCount += checkAndReport("i1", userI1, i1);
        correctCount += checkAndReport("i2", userI2, i2);
        correctCount += checkAndReport("v1", userV1, v1);
        correctCount += checkAndReport("v2", userV2, v2);
        correctCount += checkAndReport("v3", userV3, v3);
 
        printf("\nYou got %d out of 5 correct.\n", correctCount);
 
        /* -------- Step 5: restart or exit -------- */
        printf("\nType 'exit' to quit, or press Enter to try a new circuit: ");
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        if (fgets(choice, sizeof(choice), stdin) == NULL) {
            break;
        }
 
        roundNumber++;
 
    } while (choice[0] != 'e' && choice[0] != 'E');
 
    printf("\nGoodbye!\n");
    return 0;
}

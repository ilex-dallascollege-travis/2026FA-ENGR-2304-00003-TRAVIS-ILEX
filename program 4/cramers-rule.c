/*2026FA-ENGR-2304-80003-TRAVIS-ILEX-P03*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define EPSILON 1e-9
/* ---------- Input helper ---------- */
/* Reads a double from the keyboard, re-prompting on invalid input. */
double read_double(const char *prompt)
{
double value;
int c;
for (;;) {
printf("%s", prompt);
if (scanf("%lf", &value) == 1) {
return value;
}
if (feof(stdin)) {
printf("\nInput ended unexpectedly. Exiting.\n");
exit(EXIT_FAILURE);
}
printf("Invalid input. Please enter a number.\n");
while ((c = getchar()) != '\n' && c != EOF) {
/* discard the rest of the bad line */
}
}
}
/* Avoids printing "-0.0000" for tiny negative values. */
double clean(double v)
{

return (fabs(v) < 5e-7) ? 0.0 : v;
}
/* ---------- 2 x 2 system ---------- */
/* Determinant of | a b |
* | c d | = ad - bc */
double det2(double a, double b, double c, double d)
{
return a * d - b * c;
}
void solve_2x2(void)
{
double a1, b1, c1, a2, b2, c2;
double D, Dx, Dy, x, y;
printf("\n=== 2 EQUATIONS, 2 UNKNOWNS ===\n");
printf("Format: a1*x + b1*y = c1\n");
printf(" a2*x + b2*y = c2\n\n");
a1 = read_double("Enter a1: ");
b1 = read_double("Enter b1: ");
c1 = read_double("Enter c1: ");
a2 = read_double("Enter a2: ");
b2 = read_double("Enter b2: ");
c2 = read_double("Enter c2: ");
printf("\nSystem entered:\n");
printf(" %.4f x + %.4f y = %.4f\n", a1, b1, c1);
printf(" %.4f x + %.4f y = %.4f\n", a2, b2, c2);
/* D = determinant of the coefficient matrix
* Dx = D with the x-column replaced by the constants
* Dy = D with the y-column replaced by the constants */
D = det2(a1, b1, a2, b2);
Dx = det2(c1, b1, c2, b2);
Dy = det2(a1, c1, a2, c2);
printf("\nD = %.4f\n", clean(D));
printf("Dx = %.4f\n", clean(Dx));
printf("Dy = %.4f\n", clean(Dy));
if (fabs(D) < EPSILON) {
printf("\nD = 0, so Cramer's rule cannot give a unique solution.\n");
printf("The system is either inconsistent (no solution) or\n");
printf("dependent (infinitely many solutions).\n");

return;
}
x = Dx / D;
y = Dy / D;
printf("\nSolution:\n");
printf(" x = Dx / D = %.4f\n", clean(x));
printf(" y = Dy / D = %.4f\n", clean(y));
}
/* ---------- 3 x 3 system (cofactors) ---------- */
/* Minor M(row,col): determinant of the 2x2 matrix left after
* deleting the given row and column from the 3x3 matrix m. */
double minor3(double m[3][3], int row, int col)
{
double s[4];
int i, j, k = 0;
for (i = 0; i < 3; i++) {
if (i == row) {
continue;
}
for (j = 0; j < 3; j++) {
if (j == col) {
continue;
}
s[k++] = m[i][j];
}
}
return det2(s[0], s[1], s[2], s[3]);
}
/* Cofactor C(row,col) = (-1)^(row+col) * M(row,col) */
double cofactor3(double m[3][3], int row, int col)
{
double sign = ((row + col) % 2 == 0) ? 1.0 : -1.0;
return sign * minor3(m, row, col);
}
/* Determinant by cofactor expansion along the first row:
* det = a11*C11 + a12*C12 + a13*C13 */
double det3(double m[3][3])
{
double det = 0.0;
int j;

for (j = 0; j < 3; j++) {
det += m[0][j] * cofactor3(m, 0, j);
}
return det;
}
void solve_3x3(void)
{
double A[3][3], b[3];
double M[3][3];
double D, Dk[3], sol[3];
int i, j, k;
const char names[3] = {'x', 'y', 'z'};
printf("\n=== 3 EQUATIONS, 3 UNKNOWNS ===\n");
printf("Format: a1*x + b1*y + c1*z = d1\n");
printf(" a2*x + b2*y + c2*z = d2\n");
printf(" a3*x + b3*y + c3*z = d3\n\n");
for (i = 0; i < 3; i++) {
char prompt[64];
for (j = 0; j < 3; j++) {
sprintf(prompt, "Equation %d, coefficient of %c: ", i + 1, names[j]);
A[i][j] = read_double(prompt);
}
sprintf(prompt, "Equation %d, constant (right side): ", i + 1);
b[i] = read_double(prompt);
}
printf("\nSystem entered:\n");
for (i = 0; i < 3; i++) {
printf(" %.4f x + %.4f y + %.4f z = %.4f\n",
A[i][0], A[i][1], A[i][2], b[i]);
}
/* Show the cofactor matrix of the coefficient matrix */
printf("\nCofactor matrix of the coefficient matrix:\n");
for (i = 0; i < 3; i++) {
printf(" ");
for (j = 0; j < 3; j++) {
printf("%10.4f ", clean(cofactor3(A, i, j)));
}
printf("\n");
}
D = det3(A);

printf("\nD = %.4f\n", clean(D));
/* Dk: replace column k of A with the constants, then take det */
for (k = 0; k < 3; k++) {
for (i = 0; i < 3; i++) {
for (j = 0; j < 3; j++) {
M[i][j] = (j == k) ? b[i] : A[i][j];
}
}
Dk[k] = det3(M);
printf("D%c = %.4f\n", names[k], clean(Dk[k]));
}
if (fabs(D) < EPSILON) {
printf("\nD = 0, so Cramer's rule cannot give a unique solution.\n");
printf("The system is either inconsistent (no solution) or\n");
printf("dependent (infinitely many solutions).\n");
return;
}
for (k = 0; k < 3; k++) {
sol[k] = Dk[k] / D;
}
printf("\nSolution:\n");
for (k = 0; k < 3; k++) {
printf(" %c = D%c / D = %.4f\n", names[k], names[k], clean(sol[k]));
}
}
/* ---------- Main menu ---------- */
int main(void)
{
int choice;
int c;
printf("=========================================\n");
printf(" Linear Equation Solver (Cramer's Rule)\n");
printf("=========================================\n");
for (;;) {
printf("\nMenu:\n");
printf(" 1. Solve 2 equations (2 unknowns)\n");
printf(" 2. Solve 3 equations (3 unknowns)\n");
printf(" 3. Quit\n");
printf("Choice: ");

if (scanf("%d", &choice) != 1) {
if (feof(stdin)) {
printf("\n");
break;
}
printf("Invalid choice. Enter 1, 2, or 3.\n");
while ((c = getchar()) != '\n' && c != EOF) {
/* discard bad input */
}
continue;
}
switch (choice) {
case 1:
solve_2x2();
break;
case 2:
solve_3x3();
break;
case 3:
printf("Goodbye!\n");
return 0;
default:
printf("Invalid choice. Enter 1, 2, or 3.\n");
}
}
return 0;
}

#include <stdio.h>

#define NO 1
#define YES 0

#define ERROR_CODE_BASE 100
#define ERROR_INSUFFICIENT_PARAMETERS       (ERROR_CODE_BASE + 1)
#define ERROR_INVALID_YEAR                  (ERROR_CODE_BASE + 2)
#define ERROR_INVALID_MONTH                 (ERROR_CODE_BASE + 3)
#define ERROR_INVALID_DAY                   (ERROR_CODE_BASE + 4)

typedef struct DATE_TYPE {
    int year;
    int month;
    int day;
} DATE;

/* valid range is between 1582 and 4000 */
int is_leap_year(int year)
{
    if (year % 400 == 0) return YES;
    else if (year % 100 == 0) return NO;
    else if (year % 4 == 0) return YES;
    else return NO;
}

/* year [1583-3999], month [1-12] */
int is_valid_date(DATE dt)
{
    int days[] = {
        /*Jan*/ 31, /*Feb*/ 28, /*Mar*/ 31,
        /*Apr*/ 30, /*May*/ 31, /*Jun*/ 30,
        /*Jul*/ 31, /*Aug*/ 31, /*Sep*/ 30,
        /*Oct*/ 31, /*Nov*/ 30, /*Dec*/ 31
    };
    if (is_leap_year(dt.year) == YES) days[1] = 29;

    if (dt.year < 1583 || dt.year > 3999) {
        return ERROR_INVALID_YEAR;
    }
    else if (dt.month < 1 || dt.month > 12) {
        return ERROR_INVALID_MONTH;
    }
    else if (dt.day < 1 || dt.day > days[dt.month-1]) {
        return ERROR_INVALID_DAY;
    }
    return YES;
}

/* Returns Julian day number for a given Gregorian date.
   Assumes that the date is valid.
   month [1-12] */
double jdn(int year, int month, int day)
{
    double jd;
    int a, g;

    if (month < 3) {
        month += 12;
        year -= 1;
    }
    if (year < 0) {
        year++;
        jd = -(int)(0.75 - 365.25 * year) + (int)(30.6001 * (month + 1)) + day + 1720995;
    }
    else {
        if ((year + month / 100.0 + day / 10000.0) > 1582.1004) {
            a = (int)(year / 100.0);
            g = 2 - a + (int)(a / 4.0);
        }
        else {
            g = 0;
        }
        jd = (int)(365.25 * year) + (int)(30.6001 * (month + 1)) + day + g + 1720995L;
    }
    return jd;
}

/* Assumes that the dates are valid.
   Returns:
   0 if dates are equal,
   1 if d1 is more recent, 
  -1 if d2 is more recent */
int compare_dates(DATE d1, DATE d2)
{
    if (jdn(d1.year, d1.month, d1.day) == jdn(d2.year, d2.month, d2.day))
        return 0;
    else if (jdn(d1.year, d1.month, d1.day) > jdn(d2.year, d2.month, d2.day))
        return 1;
    else if (jdn(d1.year, d1.month, d1.day) < jdn(d2.year, d2.month, d2.day))
        return -1;
}

int main(int argc, char* argv[])
{
    if (argc < 4) {
        printf("Usage: %s <year> <month> <day>\n", argv[0]);
        return ERROR_INSUFFICIENT_PARAMETERS;
    }

    if (argc > 4) {
        DATE d1, d2;
        d1.year = atoi(argv[1]);
        d1.month = atoi(argv[2]);
        d1.day = atoi(argv[3]);
        d2.year = atoi(argv[4]);
        d2.month = atoi(argv[5]);
        d2.day = atoi(argv[6]);
        return compare_dates(d1, d2);
    }
    else {
        DATE dt;
        dt.year = atoi(argv[1]);
        dt.month = atoi(argv[2]);
        dt.day = atoi(argv[3]);
        return is_valid_date(dt);
    }

    return 0;
}

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#define MIN(X, Y) (X < Y) ? (X) : (Y)

#define NUM_OPTIONS 9

#define MAX_QUANTUM 256
#define MAX_PRIORITY 10

#define MIN_PROCESSES 1
#define MAX_PROCESSES 27

#define BLUE "\033[0;34m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

typedef enum {E_SUCCESS, E_NOMEM} error_codes;

int getInt (int li, int ls);
void showMenu ();

#endif // COMMON_H_INCLUDED

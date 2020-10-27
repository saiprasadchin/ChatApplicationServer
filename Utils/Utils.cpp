#include "Utils.h"

int ErrorCheck(int expression, const char* message) {
    if(expression == SCOKETERROR) {
        perror(message);
        return EXIT_FAILURE;
    }
    return expression;
}

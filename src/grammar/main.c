#include <stdio.h>

const char* INSTITUTION = "California State University Fullerton";
const char* AUTHOR = "Jared Dyreson";

void version(void){
  printf(
    "Shed Grammar Checking (%s) 1.0\n"
    "Copyright (C) 2020 Comrade Software Foundation, Inc.\n"
    "MIT License\n"
    "This is free software, and provided as is without warranty\n"
    "Written by %s\n",
  INSTITUTION, AUTHOR); 
}

int main(int argc, const char* argv[]) {
    version();
}

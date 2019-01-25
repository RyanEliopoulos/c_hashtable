/*  Ryan Paulos
 *  CS 360 
 *  Spring 2019
 */

#include<stdio.h>
#include<stdlib.h>

/* For checking file paths */ 
#include<sys/stat.h>
#include<unistd.h>

/*
 *  This function screens the command-line arguments passed to the program to ensure
 *  smooth operation and offer guidance on use as needed.
 *
 *  <count> must be an integer greater than zero and at least one filepath must be supplied.
 *  Program will fail if any of the supplied filepaths are invalid. 
 *  An exit code of 1XX indicates the program has failed this initial screen.
 *
 */

int parseArgs(int, char**);

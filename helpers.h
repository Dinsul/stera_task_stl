#pragma once

class Request;
class Stopper;

Request *RequestsGenerator();

void ParseArgs (int argc, char **argv, int &threads, int &duration);

void PrintHelp(const char * pr_name);

#pragma once

class Request;

Request *RequestsGenerator();

void ProcessEmulator(Request *request);

void ParseArgs (int argc, char **argv, int &threads, int &duration);

void PrintHelp(const char * pr_name);

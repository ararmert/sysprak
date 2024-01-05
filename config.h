#ifndef CONFIG_H_
#define CONFIG_H_

FILE* openFile(char* name);
struct config;
int config(char* fileName,struct config* con);

 #endif
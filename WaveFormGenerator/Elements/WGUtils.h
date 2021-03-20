#pragma once

#include <fstream>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
using namespace std;

void PrintHeader(char const *caption);
void PrintFooter(void);

void PrintChapter(char const *caption);
void PrintSection(char const *caption);
void PrintStep(char const *caption);

int  PrintLine(char const *format, ...);
int  FormatLine(char *stream, char const *format, ...);
void ShowByteStream(char const *str_def, uint8_t *p_bytes, uint32_t u32_len);

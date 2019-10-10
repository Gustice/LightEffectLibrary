#pragma once

#include <iostream>
#include <stdint.h>
#include "../../Source/Modules/EffectProcessor.h"
#include "../../Source/Modules/EffectWaveforms.h"

extern char const * cs_pass;
extern char const * cs_fail;
extern char const * cs_note;
extern char const * cs_warn;

void PrintHeader (char const * caption);
void PrintFooter (void);

void PrintChapter (char const * caption);
void PrintSection (char const * caption);
void PrintTestCase (char const * caption);
void PrintTestStep (char const * caption);
void Print_TestNote(char const * str_TestNote);
void Print_TestNote(char const * str_TestNote, uint32_t u32_value);

void ResetErrorCnt(void);
uint32_t GetErrCnt(void);
int printfLine(char const * format, ...);

template <typename T>
void CheckCondition_Equal(char const * output, T condition, T actual);
template <typename T>
void CheckCondition_UnEqual(char const * output, T condition, T actual);

void ShowByteStream(char const * str_def, uint8_t * p_bytes, uint32_t u32_len);

#include "TestProtocol.h"
#include <stdarg.h>

const char * cs_pass = "passed ";
const char * cs_fail = "FAILED ";
const char * cs_note = "note   ";
const char * cs_warn = "warning";

const char Banner[] = "###############################################################################";

static uint32_t u32_errCnt = 0;
static uint32_t u32_secNum = 0;
static uint32_t u32_caseNum = 0;
static uint32_t u32_testStep = 0;


void PrintHeader (char const * caption) {
    printfLine(Banner);
    printfLine("# %s", caption);
    printfLine("# Test-Date: %s : %s", __DATE__, __TIME__);
    printfLine(Banner);
    printfLine("");
}

void PrintFooter (void) {
    printfLine("");
    printfLine(Banner);
    printfLine("# Test End");
    printfLine("# Number Errors: %d", u32_errCnt);
    printfLine(Banner);
}

void PrintChapter (char const * caption) {
    u32_secNum = 0;
    u32_caseNum = 0;
    u32_testStep = 0;
    printfLine("###");
    printfLine("### %s", caption);
    printfLine("###");
}

void PrintSection (char const * caption) {
    u32_caseNum = 0;
    u32_testStep = 0;
    printfLine("### %d: %s", ++u32_secNum, caption);
    printfLine("");
}

void PrintTestCase (char const * caption) {
    u32_testStep = 0;
    printfLine("  # %d.%d: %s", u32_secNum, ++u32_caseNum, caption);
}

void PrintTestStep (char const * caption) {
    printfLine("      %d.%d.%d: %s", u32_secNum, ++u32_caseNum, ++u32_testStep, caption);
}

void Print_TestNote(char const * str_TestNote) {
    printfLine("        %s",str_TestNote);
}

void Print_TestNote(char const * str_TestNote, uint32_t u32_value) {
    printfLine("        %s: %d",str_TestNote, u32_value);
}

template <typename T>
void CheckCondition_Equal(char const * output, T condition, T actual)
{
    if (condition == actual) {
        printfLine("        %s: %s", cs_pass, output);
    }
    else {
        printfLine("        %s: %s", cs_fail, output);
        printfLine("            expected value: %d <=> actual value: %d", condition, actual);
        u32_errCnt++;
    }
}
template void CheckCondition_Equal<bool>(char const * output, bool, bool);
template void CheckCondition_Equal<int>(char const * output, int, int);
template void CheckCondition_Equal<uint16_t>(char const * output, uint16_t, uint16_t);
template void CheckCondition_Equal<uint32_t>(char const * output, uint32_t, uint32_t);

template <typename T>
void CheckCondition_UnEqual(char const * output, T condition, T actual)
{
    if (condition != actual) {
        printfLine("        %s: %s", cs_pass, output);
    }
    else {
        printfLine("        %s: %s", cs_fail, output);
        printfLine("            expected value: %d <=> actual value: %d", condition, actual);
        u32_errCnt++;
    }
}
template void CheckCondition_UnEqual<bool>(char const * output, bool, bool);
template void CheckCondition_UnEqual<int>(char const * output, int, int);
template void CheckCondition_UnEqual<uint16_t>(char const * output, uint16_t, uint16_t);
template void CheckCondition_UnEqual<uint32_t>(char const * output, uint32_t, uint32_t);


void ResetErrorCnt(void) {
    u32_errCnt = 0;
}

uint32_t GetErrCnt(void) {
    return (u32_errCnt);
}


void ShowByteStream(char const * str_def, uint8_t * p_bytes, uint32_t u32_len) {
  printfLine("   %s",str_def);
  
  printf("   ");
  uint32_t i;
  for (i=0u; i<u32_len; i++)
  {
    printf("0x%.2x\t",p_bytes[i]);
  }
  printfLine("");
}

int printfLine(char const * format, ...) {
    int retVal;
    va_list args;
    va_start(args, format);
    retVal = vprintf(format, args);
    printf("\n");
    va_end(args);
    return (retVal);
}






// static int pintfLine(const char * format, va_list args) {
//     vprintf(format, args);
//     printf("\n");
// }
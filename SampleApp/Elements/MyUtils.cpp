#include "MyUtils.h"
#include <stdarg.h>

const char Banner[] = "###############################################################################";

static uint32_t u32_errCnt = 0;
static uint32_t u32_secNum = 0;
static uint32_t u32_step = 0;

void PrintHeader (char const * caption) {
    PrintLine(Banner);
    PrintLine("# %s", caption);
    PrintLine(Banner);
    PrintLine("");
}

void PrintFooter (void) {
    PrintLine("");
    PrintLine(Banner);
    PrintLine("# Test End");
    PrintLine(Banner);
}

void PrintChapter (char const * caption) {
    u32_secNum = 0;
    u32_step = 0;
    PrintLine("###");
    PrintLine("### %s", caption);
    PrintLine("###");
}

void PrintSection (char const * caption) {
    u32_step = 0;
    PrintLine("### %d: %s", ++u32_secNum, caption);
    PrintLine("");
}

void PrintStep (char const * caption) {
    PrintLine("      %d.%d: %s", u32_secNum, ++u32_step, caption);
}

void ShowByteStream(char const * str_def, uint8_t * p_bytes, uint32_t u32_len) {
  PrintLine("   %s",str_def);
  
  printf("   ");
  uint32_t i;
  for (i=0u; i<u32_len; i++)
  {
    printf("0x%.2x\t",p_bytes[i]);
  }
  PrintLine("");
}

int PrintLine(char const * format, ...) {
    int retVal;
    va_list args;
    va_start(args, format);
    retVal = vprintf(format, args);
    printf("\n");
    va_end(args);
    return (retVal);
}

int FormatLine(char *stream, char const * format, ...) {
    int retVal;
    va_list args;
    va_start(args, format);
    retVal = vsprintf(stream, format, args);
    printf("\n");
    va_end(args);
    return (retVal);
}



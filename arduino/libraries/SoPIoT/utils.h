// get argument from data by name
void* GetArgumentByName(void* pData, const char* name);

// get string argument from data by name
int GetStringArgumentByName(void* pData, const char* name, char** ppszOut);

// get integer argument from data by name
int GetIntArgumentByName(void* pData, const char* name, int* pnOut);

// get double argument from data by name
int GetDoubleArgumentByName(void* pData, const char* name, double* pdbOut);

// handle mqtt connlost mesage
// TODO(ikess): check if it is needed.
// void brokerConnLost(void *context, char *cause);

// dtostrf works differently on ARM board.
char* safe_dtostrf(double val, signed char width, unsigned char prec,
                   char* sout);
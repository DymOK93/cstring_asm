Implementation of <cstring> (str* and mem* functions,  except those using locales) in pure x64 assembler. No optimizations (only memset and memcpy use some tricks for enchanced 8-byte strings), SSE and AVX are not used.
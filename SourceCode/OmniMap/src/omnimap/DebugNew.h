#ifdef _CRTDBG_MAP_ALLOC
// Include all other headers here!
// Include CRTDBG.H after all other headers
// from msdn tip 40 by john@wintellect.com,
// http://msdn.microsoft.com/msdnmag/issues/1200/bugslayer/

#include <stdlib.h>
#include <crtdbg.h>
#define NEW_INLINE_WORKAROUND new ( (_NORMAL_BLOCK ),  (__FILE__ ), (__LINE__ ))
#define new NEW_INLINE_WORKAROUND
#endif

#include "functions.h"

bool is_qcfw()
{
	return (lv1_peek(0x240) == 0x11223344aabbccddULL);
}
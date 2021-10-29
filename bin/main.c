#include "main.h"

int main(void)
{
	SEOS_Boot(); 	
	while(1)
	{
		SEOS_Dispatch();
		SEOS_Sleep();
	}
}

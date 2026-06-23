#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timer.h>
#include "functions.h"
#include "gccpch.h"
#include "log.h"
#include "overclock.h"
#include <cell/fs/cell_fs_file_api.h>

static wchar_t wchar_string[360]; // Global variable for swprintf

int getClockSpeeds()
{
	uint8_t memoryArray[8], coreArray[8];
	uint64_t initialValue;	

	// HEN
	if(!is_hen())
	{
		showMessage("msg_hen_notsupported_error", (char *)XAI_PLUGIN, (char *)TEX_ERROR);
		return 1;
	}

	// Check if CFW Syscalls are disabled
	if(checkSyscalls(LV1))
	{
		showMessage("msg_cfw_syscalls_disabled", (char *)XAI_PLUGIN, (char *)TEX_ERROR);
		return 1;
	}

	initialValue = lv1_peek(RSX_MEMORY_FREQ_OFFSET);
	memcpy(memoryArray, &initialValue, 8);

	initialValue = lv1_peek(RSX_CORE_FREQ_OFFSET);
	memcpy(coreArray, &initialValue, 8);

	int string = RetrieveString("msg_clock_speeds", (char*)XAI_PLUGIN);	
	swprintf_(wchar_string, 120, (wchar_t*)string, (int)coreArray[6] * 50, (int)memoryArray[6] * 25);	
	PrintString(wchar_string, (char*)XAI_PLUGIN, (char*)TEX_SUCCESS);

	return 0;
}

// Thanks to Chattrapat Sangmanee (@chattrapat_s) for his research on overclocking 
int set_overclock_freq(uint8_t core, uint8_t memory, int mode)
{
	uint8_t memoryArray[8], coreArray[8];
	uint64_t value;	

	// HEN
	if(!is_hen())
	{
		showMessage("msg_hen_notsupported_error", (char *)XAI_PLUGIN, (char *)TEX_ERROR);
		return 1;
	}

	// Check if CFW Syscalls are disabled
	if(checkSyscalls(LV1))
	{
		showMessage("msg_cfw_syscalls_disabled", (char *)XAI_PLUGIN, (char *)TEX_ERROR);
		return 1;
	}			

	// Core
	if(mode == MODE_FULL || mode == MODE_CORE)
	{
		value = lv1_peek(RSX_CORE_FREQ_OFFSET);
		memcpy(coreArray, &value, 8);
		coreArray[6] = core;
		value = *(uint64_t *)coreArray;
		lv1_poke(RSX_CORE_FREQ_OFFSET, value);
	}

	// Memory
	// Must apply slowly in 25mhz step, wait, repeat until reach target
	if(mode == MODE_FULL || mode == MODE_MEMORY)
	{		
		value = lv1_peek(RSX_MEMORY_FREQ_OFFSET);		
		memcpy(memoryArray, &value, 8);

		while(memoryArray[6] != memory)
		{
			sys_timer_usleep(20000);
			memoryArray[6] += (memory > memoryArray[6] ? 1 : -1);
			value = *(uint64_t *)memoryArray;
			lv1_poke(RSX_MEMORY_FREQ_OFFSET, value);
			eieio();
		}
	}

	getClockSpeeds();

	return 0;
}
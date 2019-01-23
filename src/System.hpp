#pragma once

#include "yggdrasil/kernel/Processor.hpp"
#include "yggdrasil/kernel/Kernel.hpp"
#include "yggdrasil/interfaces/ISystem.hpp"
#include "yggdrasil/kernel/IntVectManager.hpp"


namespace kernel
{
	class System : public interfaces::ISystem
	{
	public:
		
		bool initSystemClock() override
		{
			if(s_initialised)
				return true;
			RCC->CR |= RCC_CR_HSEON;
			uint32_t timeout = 1000;
			while ((RCC->CR & RCC_CR_HSERDY) != (RCC_CR_HSERDY))
			{
				timeout--;
				if (timeout == 0)
					return false;
			}
			RCC->CFGR = ((RCC->CFGR & ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL)) | (RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL2));
			RCC->CFGR2 = ((RCC->CFGR2 & ~(RCC_CFGR2_PREDIV1)) | (RCC_CFGR2_PREDIV1_DIV2 & RCC_CFGR2_PREDIV1));
			RCC->CR |= RCC_CR_PLLON;
			timeout = 1000;
			while ((RCC->CR & RCC_CR_PLLRDY) != (RCC_CR_PLLRDY))
			{
				timeout--;
				if (timeout == 0)
					return false;
			}
			
			RCC->CFGR = ((RCC->CFGR & ~(RCC_CFGR_HPRE)) | (RCC_CFGR_HPRE_DIV1));
			RCC->CFGR = ((RCC->CFGR & ~(RCC_CFGR_PPRE1)) | (RCC_CFGR_PPRE1_DIV1));
			RCC->CFGR = ((RCC->CFGR & ~(RCC_CFGR_PPRE2)) | (RCC_CFGR_PPRE2_DIV1));
			RCC->CFGR = ((RCC->CFGR & ~(RCC_CFGR_SW)) | (RCC_CFGR_SW_PLL));
			
			timeout = 1000;
			while ((RCC->CFGR & RCC_CFGR_SWS) != (RCC_CFGR_SWS_PLL))
			{
				timeout--;
				if (timeout == 0)
					return false;
			}
			
			
			s_systemCoreClock = 24000000;
			s_peripheralClock1 = 24000000;
			s_peripheralClock2 = 24000000;
			return true;
		}
		
		uint32_t getSystemCoreFrequency() override
		{
			return s_systemCoreClock;
		}
		
		uint32_t getPeripheralClock1Frequency() override
		{
			return s_peripheralClock1;
		}
		
		uint32_t getPeripheralClock2Frequency() override
		{
			return s_peripheralClock2;
		}
		
		static System& instance()
		{
			return s_instance;
		}
		
		
	private:
		static uint32_t s_systemCoreClock;
		static uint32_t s_peripheralClock1;
		static uint32_t s_peripheralClock2;

		static System s_instance;
		static bool s_initialised;
	};
}


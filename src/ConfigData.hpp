#pragma once

#include <stdint.h>


namespace RS41
{
	class ConfigData
	{
	public:
		constexpr ConfigData(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g) : config1(a), config2(b), config3(c),
		config4(d), config5(e), config6(f), config7(g)
		{
		}
		
		constexpr ConfigData()
			: config1(0x11111111)
			, config2(0x22222222)
			, config3(0x33333333)
			, config4(0x44444444)
			, config5(0x55555555)
			, config6(0x66666666)
			, config7(0x77777777)

		{
			
		}
		
		void setData(uint32_t a, uint32_t b, uint32_t c)
		{
			config1 = a;
			config2 = b;
			config3 = c;
		}

	private:
		uint32_t config1;
		uint32_t config2;
		uint32_t config3;
		uint32_t config4;
		uint32_t config5;
		uint32_t config6;
		uint32_t config7;
	};
	}

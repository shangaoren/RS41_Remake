
#include "yggdrasil/kernel/Processor.hpp"
#include "yggdrasil/kernel/Task.hpp"
#include "yggdrasil/kernel/Api.hpp"

#include "System.hpp"
#include "flashMemory.hpp"
#include "ConfigData.hpp"

extern volatile uint32_t _eflashData;
extern volatile uint32_t _sflashData;
const uint32_t endFlashData  = reinterpret_cast<uint32_t>((&_eflashData));
const uint32_t startFlashData = reinterpret_cast<uint32_t>((&_sflashData));
uint8_t testTab[128];
uint32_t _estack;

using namespace kernel;

void ledFct(uint32_t p)
{
	//Green PB7 - Red PB8
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIOB->CRL = ((GPIOB->CRL & ~(GPIO_CRL_CNF7)) | (GPIO_CRL_CNF7_0 ));
	GPIOB->CRL = ((GPIOB->CRL & ~(GPIO_CRL_MODE7)) |  GPIO_CRL_MODE7_1 | GPIO_CRL_MODE7_0);
	while (true)
	{
		GPIOB->BSRR = GPIO_BSRR_BR7;
		Api::sleep(500);
		GPIOB->BSRR = GPIO_BSRR_BS7;
		Api::sleep(500);
	}
}
auto ledTask = TaskWithStack<128>(ledFct,2,"test",0);

void testFlash(uint32_t p)
{
	bool debug = false;
	RS41::ConfigData data;
	drivers::FlashData<RS41::ConfigData> memory = drivers::FlashData<RS41::ConfigData>(startFlashData, endFlashData);

	while (true)
	{
		data = memory.load();
		if(debug)
			memory.save(data);
	}
}
auto testFlashTask = TaskWithStack<1024>(testFlash, 2, "testFlash", 0);


int main(void)
{
	SCnSCB->ACTLR |= SCnSCB_ACTLR_DISDEFWBUF_Msk;
	ledTask.start();
	testFlashTask.start();
	Api::startKernel(System::instance(), 0, 2);
}

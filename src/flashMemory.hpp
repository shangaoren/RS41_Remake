#pragma once

#include <cstdint>
#include "yggdrasil/kernel/Processor.hpp"


namespace drivers
{
	template<typename T>
	class FlashData : public T
	{
	public:
		
		FlashData(uint32_t memoryStart, uint32_t memoryEnd) : m_startFlashMemory(reinterpret_cast<uint16_t*>(memoryStart)), m_endFlashMemory(reinterpret_cast<uint16_t*>(memoryEnd))
		{
			
		}
		
		T load()
		{
			uint16_t* dataPointer = reinterpret_cast<uint16_t*>(&m_flashData);
			for(uint32_t i = 0; i < (sizeof(T)/sizeof(uint16_t)); i++)
			{
				dataPointer[i] = m_startFlashMemory[i];
			}
			return m_flashData;
		}
		
		bool save(T toSave)
		{
			erasePage();
			while((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY);
			unlockFlash();
			FLASH->CR |= FLASH_CR_PG;
			uint16_t* data = reinterpret_cast<uint16_t*>(&toSave);
			uint32_t i = 0;
			while ((i < (sizeof(T)/sizeof(uint16_t))) && (m_startFlashMemory + i ) < m_endFlashMemory)
			{
				m_startFlashMemory[i] = data[i];
				while ((FLASH->SR  & FLASH_SR_BSY) == FLASH_SR_BSY);
				i++;
			}
			FLASH->CR &= ~FLASH_CR_PG;
			lockFlash();
			return true;
		}
		

		bool erasePage()
		{
			while((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY);
			unlockFlash();
			FLASH->CR |= FLASH_CR_PER;
			FLASH->AR = reinterpret_cast<uint32_t>(m_startFlashMemory);
			FLASH->CR |= FLASH_CR_STRT;
			while ((FLASH->SR  & FLASH_SR_BSY) == FLASH_SR_BSY);
			FLASH->CR &= ~FLASH_CR_PER;
			lockFlash();
			return true;
		}
	private:
		T m_flashData; 
		uint16_t* m_startFlashMemory;
		uint16_t* m_endFlashMemory;
		static constexpr uint32_t kFlashKey1 = 0x45670123;
		static constexpr uint32_t kFlashKey2 = 0xCDEF89AB;
		static constexpr uint32_t kRdprt = 0x00A5;


		void unlockFlash()
		{
			FLASH->KEYR = kFlashKey1;
			FLASH->KEYR = kFlashKey2;
		}


		void lockFlash()
		{
			FLASH->CR |= FLASH_CR_LOCK;
		}
	};
}

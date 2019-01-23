#pragma once

#include <cstdint>
#include "yggdrasil/kernel/Processor.hpp"


namespace drivers
{
	template<typename T>
	class FlashData : public T
	{
	public:
		
		/**
		 * Construct an object that will be stored in flash between memoryStart and memoryEnd
		 * @param memoryStart : the beginning of the reserved space for data
		 * @param memoryEnd : The end of the reserved space for data
		 * @warning This class will erase a sector of flash when writing, a sector is 1K in stm32f100
		 */
		FlashData(uint32_t memoryStart, uint32_t memoryEnd) : m_startFlashMemory(reinterpret_cast<uint16_t*>(memoryStart)), m_endFlashMemory(reinterpret_cast<uint16_t*>(memoryEnd))
		{
			
		}
		

		/**
		 * Load data from flash, will return an object T holding the data in flash
		 * @return the data holded by flash, formatted in an object of type T
		 */
		T load()
		{
			uint16_t* dataPointer = reinterpret_cast<uint16_t*>(&m_flashData);
			for(uint32_t i = 0; i < (sizeof(T)/sizeof(uint16_t)); i++)
			{
				dataPointer[i] = m_startFlashMemory[i];
			}
			return m_flashData;
		}
		
		/**
		 * Save an object into flash
		 * @param toSave the object to save
		 * @return true
		 * @warning : will erase an entire sector of flash before write (1K in stm32f100)
		 */
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
		

		/**
		 * Erase an entire sector of flash (1K for stm32F100)
		 * @return true
		 */
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
		T m_flashData;  	//The image of the object stored in flash
		uint16_t* m_startFlashMemory;	//beginning of the flash reserved for data storage
		uint16_t* m_endFlashMemory;	//end of the flash reserved for data storage
		static constexpr uint32_t kFlashKey1 = 0x45670123; //key 1 to unlock flash
		static constexpr uint32_t kFlashKey2 = 0xCDEF89AB;	//key 2 to unlock flash
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

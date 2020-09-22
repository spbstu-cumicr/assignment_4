#include "mcu_support_package/inc/stm32f4xx.h"
#include "led.h"
#include "timer.h"
#include "main/Lab4_Test.h"

#include <stdint.h>

// Последовательность загорания светодиодов
// Укажите последовательность в соответствии с вариантом
// Используйте макроопределения цветом светодиодов:
// LED_GREEN — зелёный
// LED_YELLOW — жёлтый
// LED_RED — красный
// LED_BLUE — синий
static const uint8_t ledColour[] = {
	LED_BLUE,
	LED_GREEN,
	LED_BLUE,
	LED_YELLOW,
	LED_RED,
	LED_YELLOW,
	LED_BLUE,
	LED_GREEN
};

static const uint32_t delay[] = {
	2400,
	3000,
	4000,
	3300,
	1400,
	4200,
	4100,
	3600
};

// Таймаут таймера
const uint32_t timeout_ms = 1;

// Фамилия
const char *lastName = "Pankov";

int main(void)
{
	// Вариант задания
	static uint8_t variant = 0;	
	variant = Lab4_Test_ini((char *)lastName);	

	// Контрольная сумма
	static uint32_t checksum = 0;
	static uint8_t decision[] = {0, 0, 0};
	
	// Инициализируем клавиатуру
	static Led *led = NULL;
	led = initLed();
	
	// Инициализируем секундомер
	static Clock *clock = NULL;
	clock = initClock(timeout_ms);

	while(1)
	{
		// Номер светодиода
		static uint8_t ledNumber = 0;
		
		Led *currentLed = &led[ledColour[ledNumber]];
		if (currentLed->isOn(currentLed) == false)
		{
			// Цвет светодиода
			static uint8_t colour = 0;
			
			// Гасим предыдущий светодиод и зажигаем следующий
			led[colour].turnOff(&led[colour]);
			colour = ledColour[ledNumber];
			led[colour].turnOn(&led[colour]);
		}
		
		if (clock->getTime(clock) >= delay[ledNumber])
		{
			// Переходим к следующему светодиоду
			ledNumber++;
			ledNumber %= ARRAY_SIZE(ledColour);
			
			clock->reset(clock);
		}
		
		checksum = read_flag(decision);
	}
	
	return 0;
}

#ifdef USE_FULL_ASSERT

// эта функция вызывается, если assert_param обнаружил ошибку
void assert_failed(uint8_t *file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
	ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	
	(void)file;
	(void)line;
	
	__disable_irq();
	while(1)
	{
		// это ассемблерная инструкция "отладчик, стой тут"
		// если вы попали сюда, значит вы ошиблись в параметрах вызова функции из SPL. 
		// Смотрите в call stack, чтобы найти ее
		__BKPT(0xAB);
	}
}

#endif

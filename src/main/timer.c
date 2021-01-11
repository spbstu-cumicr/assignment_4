#include "timer.h"

static volatile Clock clock =
{
	.getTime   = getClockTime,
	.reset     = resetClock,
	.startTime = 0,
	.counter   = 0
};

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		clock.counter++;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void initTimer(const uint32_t timeout_ms)
{
	// Вычисляем предделитель и период таймера в тактах
	uint16_t prescaler = SystemCoreClock / 2 / REFERENCE_FREQUENCY_HZ - 1;
	uint32_t period = REFERENCE_FREQUENCY_HZ * timeout_ms / 1000 - 1;
	
	// Подаём питание на порт таймера общего назначения TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	// Настраиваем таймер общего назначения TIM3
	TIM_TimeBaseInitTypeDef ledTimer;
	TIM_TimeBaseStructInit(&ledTimer);
	
	ledTimer.TIM_Prescaler = prescaler;
	ledTimer.TIM_CounterMode = TIM_CounterMode_Up;
	ledTimer.TIM_Period = period;
	
	TIM_TimeBaseInit(TIM3, &ledTimer);
	
	// Включаем таймер общего назначения TIM3
	TIM_Cmd(TIM3, ENABLE);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM3_IRQn);
}

Clock *initClock(const uint32_t timeout_ms)
{
	initTimer(timeout_ms);
	
	return (Clock *)&clock;
}

uint32_t getClockTime(Clock *clock)
{
	return clock->counter - clock->startTime;
}

void resetClock(Clock *clock)
{
	clock->startTime = clock->counter;
}

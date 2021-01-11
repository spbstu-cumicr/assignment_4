#include "variants.h"
#include "led.h"

#include <stdio.h>

static const uint8_t ledColour[30][8] = {
    { BLUE   , YELLOW , GREEN  , BLUE   , RED    , BLUE   , RED    , YELLOW }, // №00 B->Y->G->B->R->B->R->Y
    { RED    , GREEN  , RED    , BLUE   , GREEN  , YELLOW , RED    , BLUE   }, // №01 R->G->R->B->G->Y->R->B
    { GREEN  , YELLOW , GREEN  , RED    , GREEN  , RED    , BLUE   , YELLOW }, // №02 G->Y->G->R->G->R->B->Y
    { BLUE   , YELLOW , RED    , BLUE   , RED    , BLUE   , RED    , GREEN  }, // №03 B->Y->R->B->R->B->R->G
    { RED    , GREEN  , BLUE   , YELLOW , BLUE   , RED    , BLUE   , YELLOW }, // №04 R->G->B->Y->B->R->B->Y
    { BLUE   , GREEN  , BLUE   , YELLOW , RED    , YELLOW , BLUE   , GREEN  }, // №05 B->G->B->Y->R->Y->B->G
    { YELLOW , RED    , YELLOW , GREEN  , BLUE   , RED    , YELLOW , GREEN  }, // №06 Y->R->Y->G->B->R->Y->G
    { YELLOW , RED    , YELLOW , BLUE   , GREEN  , YELLOW , RED    , GREEN  }, // №07 Y->R->Y->B->G->Y->R->G
    { YELLOW , GREEN  , BLUE   , GREEN  , RED    , BLUE   , YELLOW , BLUE   }, // №08 Y->G->B->G->R->B->Y->B
    { YELLOW , GREEN  , RED    , BLUE   , GREEN  , RED    , BLUE   , GREEN  }, // №09 Y->G->R->B->G->R->B->G
    { RED    , GREEN  , BLUE   , GREEN  , YELLOW , BLUE   , RED    , YELLOW }, // №10 R->G->B->G->Y->B->R->Y
    { BLUE   , GREEN  , YELLOW , GREEN  , RED    , GREEN  , YELLOW , GREEN  }, // №11 B->G->Y->G->R->G->Y->G
    { RED    , GREEN  , YELLOW , GREEN  , BLUE   , GREEN  , BLUE   , YELLOW }, // №12 R->G->Y->G->B->G->B->Y
    { GREEN  , YELLOW , RED    , BLUE   , YELLOW , GREEN  , BLUE   , YELLOW }, // №13 G->Y->R->B->Y->G->B->Y
    { BLUE   , GREEN  , YELLOW , BLUE   , GREEN  , RED    , BLUE   , GREEN  }, // №14 B->G->Y->B->G->R->B->G
    { BLUE   , RED    , BLUE   , RED    , GREEN  , RED    , YELLOW , RED    }, // №15 B->R->B->R->G->R->Y->R
    { RED    , GREEN  , BLUE   , RED    , GREEN  , RED    , YELLOW , BLUE   }, // №16 R->G->B->R->G->R->Y->B
    { YELLOW , BLUE   , RED    , BLUE   , YELLOW , GREEN  , RED    , GREEN  }, // №17 Y->B->R->B->Y->G->R->G
    { BLUE   , GREEN  , RED    , GREEN  , BLUE   , GREEN  , RED    , YELLOW }, // №18 B->G->R->G->B->G->R->Y
    { GREEN  , YELLOW , GREEN  , BLUE   , GREEN  , YELLOW , GREEN  , BLUE   }, // №19 G->Y->G->B->G->Y->G->B
    { GREEN  , YELLOW , GREEN  , YELLOW , BLUE   , RED    , YELLOW , BLUE   }, // №20 G->Y->G->Y->B->R->Y->B
    { GREEN  , YELLOW , RED    , GREEN  , BLUE   , GREEN  , YELLOW , BLUE   }, // №21 G->Y->R->G->B->G->Y->B
    { RED    , GREEN  , RED    , YELLOW , GREEN  , RED    , YELLOW , BLUE   }, // №22 R->G->R->Y->G->R->Y->B
    { GREEN  , BLUE   , RED    , BLUE   , RED    , BLUE   , GREEN  , YELLOW }, // №23 G->B->R->B->R->B->G->Y
    { GREEN  , RED    , YELLOW , GREEN  , BLUE   , RED    , GREEN  , YELLOW }, // №24 G->R->Y->G->B->R->G->Y
    { GREEN  , YELLOW , GREEN  , BLUE   , GREEN  , YELLOW , GREEN  , RED    }, // №25 G->Y->G->B->G->Y->G->R
    { GREEN  , BLUE   , RED    , YELLOW , BLUE   , GREEN  , RED    , YELLOW }, // №26 G->B->R->Y->B->G->R->Y
    { GREEN  , BLUE   , GREEN  , YELLOW , GREEN  , YELLOW , BLUE   , YELLOW }, // №27 G->B->G->Y->G->Y->B->Y
    { BLUE   , RED    , BLUE   , RED    , YELLOW , RED    , GREEN  , RED    }, // №28 B->R->B->R->Y->R->G->R
    { BLUE   , RED    , GREEN  , BLUE   , YELLOW , BLUE   , RED    , YELLOW }  // №29 B->R->G->B->Y->B->R->Y
};

static const uint32_t delay[30][8] = {
    { 1100 , 4500 , 2900 , 4000 , 4600 , 4400 , 3800 , 4800 }, // №00 1.1->4.5->2.9->4.0->4.6->4.4->3.8->4.8
    { 2000 , 3900 , 1500 , 2600 , 4200 , 4800 ,  400 , 1900 }, // №01 2.0->3.9->1.5->2.6->4.2->4.8->0.4->1.9
    { 1300 , 4400 , 1400 , 2600 , 1700 ,  900 , 3500 , 3000 }, // №02 1.3->4.4->1.4->2.6->1.7->0.9->3.5->3.0
    { 2200 , 2600 , 5000 , 1000 , 5000 , 2100 ,  100 , 4600 }, // №03 2.2->2.6->5.0->1.0->5.0->2.1->0.1->4.6
    { 2000 , 3800 , 2100 , 4000 , 1500 , 3200 , 2800 ,  900 }, // №04 2.0->3.8->2.1->4.0->1.5->3.2->2.8->0.9
    { 2400 , 3000 , 4000 , 3300 , 1400 , 4200 , 4100 , 3600 }, // №05 2.4->3.0->4.0->3.3->1.4->4.2->4.1->3.6
    { 4900 , 3400 , 1100 , 1200 , 2300 , 2200 , 3400 ,  700 }, // №06 4.9->3.4->1.1->1.2->2.3->2.2->3.4->0.7
    {  100 , 3500 , 4600 , 4800 , 2400 , 2100 , 1400 , 2700 }, // №07 0.1->3.5->4.6->4.8->2.4->2.1->1.4->2.7
    { 4700 ,  400 ,  400 , 1400 , 5000 ,  700 , 2500 , 3800 }, // №08 4.7->0.4->0.4->1.4->5.0->0.7->2.5->3.8
    { 3800 , 4000 , 2200 , 1400 , 3700 , 2500 , 3200 , 1200 }, // №09 3.8->4.0->2.2->1.4->3.7->2.5->3.2->1.2
    { 4900 ,  300 ,  800 ,  600 ,  600 , 3300 , 1100 , 3600 }, // №10 4.9->0.3->0.8->0.6->0.6->3.3->1.1->3.6
    {  300 , 2700 ,  400 , 1200 , 1300 , 3500 , 4700 , 4700 }, // №11 0.3->2.7->0.4->1.2->1.3->3.5->4.7->4.7
    { 1900 , 1300 , 1200 ,  900 , 2000 , 2300 , 4100 , 3000 }, // №12 1.9->1.3->1.2->0.9->2.0->2.3->4.1->3.0
    { 4200 , 4200 , 1800 ,  400 , 1400 , 4400 ,  100 , 4200 }, // №13 4.2->4.2->1.8->0.4->1.4->4.4->0.1->4.2
    { 2000 ,  300 ,  600 , 3800 , 2800 , 1300 , 3100 , 1000 }, // №14 2.0->0.3->0.6->3.8->2.8->1.3->3.1->1.0
    {  300 , 4000 , 2200 , 2300 ,  800 , 3300 , 3300 , 5000 }, // №15 0.3->4.0->2.2->2.3->0.8->3.3->3.3->5.0
    { 4900 , 1000 , 3400 , 4300 , 3500 , 3400 ,  900 , 2300 }, // №16 4.9->1.0->3.4->4.3->3.5->3.4->0.9->2.3
    { 3700 , 2000 , 2900 , 1700 , 3600 , 1900 , 4600 , 3400 }, // №17 3.7->2.0->2.9->1.7->3.6->1.9->4.6->3.4
    { 1900 , 4000 , 2600 , 3700 , 3400 , 4800 , 3100 , 4500 }, // №18 1.9->4.0->2.6->3.7->3.4->4.8->3.1->4.5
    { 2800 , 1800 ,  800 ,  300 , 4500 , 1100 ,  100 , 2400 }, // №19 2.8->1.8->0.8->0.3->4.5->1.1->0.1->2.4
    {  600 , 1700 , 4300 , 1100 ,  200 , 2600 ,  400 , 4200 }, // №20 0.6->1.7->4.3->1.1->0.2->2.6->0.4->4.2
    { 4100 ,  100 , 4100 ,  200 , 1000 , 2000 , 3800 , 2800 }, // №21 4.1->0.1->4.1->0.2->1.0->2.0->3.8->2.8
    { 2600 , 1600 , 1900 , 2200 , 1000 , 2300 , 3200 , 4900 }, // №22 2.6->1.6->1.9->2.2->1.0->2.3->3.2->4.9
    {  400 , 1500 ,  600 , 3300 , 4500 , 2100 , 1500 , 3300 }, // №23 0.4->1.5->0.6->3.3->4.5->2.1->1.5->3.3
    { 3800 , 3200 , 3400 , 1400 , 2100 , 1600 , 2100 , 3900 }, // №24 3.8->3.2->3.4->1.4->2.1->1.6->2.1->3.9
    { 3400 , 1400 , 2800 , 2900 , 2700 , 4900 , 3000 , 3700 }, // №25 3.4->1.4->2.8->2.9->2.7->4.9->3.0->3.7
    { 4800 , 4800 , 2600 ,  200 , 2200 , 1400 ,  900 , 1100 }, // №26 4.8->4.8->2.6->0.2->2.2->1.4->0.9->1.1
    { 2800 , 4200 , 4000 , 4000 , 3200 , 2200 ,  600 , 3300 }, // №27 2.8->4.2->4.0->4.0->3.2->2.2->0.6->3.3
    { 1900 , 4000 , 1300 , 5000 , 2800 , 5000 , 4200 , 2900 }, // №28 1.9->4.0->1.3->5.0->2.8->5.0->4.2->2.9
    {  700 ,  800 , 4600 , 4000 ,  600 , 3200 , 2700 , 2100 }  // №29 0.7->0.8->4.6->4.0->0.6->3.2->2.7->2.1
};

const uint8_t *getLedColourList(const uint8_t variant)
{
    if (variant >= 30)
    {
        return NULL;
    }

    return ledColour[variant];
}

uint8_t ledColourSize(void)
{
    return 8;
}

const uint32_t *getDelayList(const uint8_t variant)
{
    if (variant >= 30)
    {
        return NULL;
    }

    return delay[variant];
}

uint8_t delaySize(void)
{
    return 8;
}

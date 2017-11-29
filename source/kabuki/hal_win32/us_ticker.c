/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdafx.h>
#include "../../hal/include/us_ticker_api.h"
#include "PeripheralNames.h"

#define US_TICKER_TIMER      ((LPC_TIM_TypeDef *)LPC_TIM3_BASE)
#define US_TICKER_TIMER_IRQn TIMER3_IRQn

int us_ticker_inited = 0;

void us_ticker_init(void) {
    
}

uint32_t us_ticker_read() {
    return 0;
}

void us_ticker_set_interrupt(timestamp_t timestamp) {
    
}

void us_ticker_disable_interrupt(void) {
}

void us_ticker_clear_interrupt(void) {
}
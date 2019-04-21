/**
  ******************************************************************************
  * @file    FMC_SDRAM/stm32f4xx_it.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#include "stm32f4xx.h"
#include "config/config.h"

#define KEY_MSD_Enable_Handler  EXTI0_IRQHandler
#define KEY_MSD_Disable_Handler EXTI15_10_IRQHandler
#define USART1_Handler          USART1_IRQHandler
#define MSD_D_Handler           TIM2_IRQHandler
#define MSD_L_Handler           TIM3_IRQHandler
#define Clock_Handler           TIM1_UP_TIM10_IRQHandler

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void KEY_MSD_Enable_Handler(void);
void KEY_MSD_Disable_Handler(void);
void USART1_Handler(void);

void MSD_D_Handler(void);
void MSD_L_Handler(void);
void Clock_Handler(void);

#endif /* __STM32F4xx_IT_H */

#include "main.h"
#include <stdio.h>
#include <string.h>

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart1;

void SystemClock_Config(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);

int main(void)
{
    uint32_t adc_value;
    uint32_t voltage_mv;
    char buffer[100];

    HAL_Init();

    SystemClock_Config();
    MX_ADC1_Init();
    MX_USART1_UART_Init();

    if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    while (1)
    {
        if (HAL_ADC_Start(&hadc1) != HAL_OK)
        {
            Error_Handler();
        }

        if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
        {
            adc_value = HAL_ADC_GetValue(&hadc1);

            voltage_mv = (adc_value * 3300UL) / 4095UL;

            snprintf(
                buffer,
                sizeof(buffer),
                "ADC = %lu, Voltage = %lu mV\r\n",
                adc_value,
                voltage_mv
            );

            HAL_UART_Transmit(
                &huart1,
                (uint8_t *)buffer,
                strlen(buffer),
                HAL_MAX_DELAY
                
            );
        }

        HAL_ADC_Stop(&hadc1);
        HAL_Delay(1000);
    }

}


void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_HSE;

    RCC_OscInitStruct.HSEState =
        RCC_HSE_ON;

    RCC_OscInitStruct.HSEPredivValue =
        RCC_HSE_PREDIV_DIV1;

    RCC_OscInitStruct.HSIState =
        RCC_HSI_ON;

    RCC_OscInitStruct.PLL.PLLState =
        RCC_PLL_ON;

    RCC_OscInitStruct.PLL.PLLSource =
        RCC_PLLSOURCE_HSE;

    RCC_OscInitStruct.PLL.PLLMUL =
        RCC_PLL_MUL9;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource =
        RCC_SYSCLKSOURCE_PLLCLK;

    RCC_ClkInitStruct.AHBCLKDivider =
        RCC_SYSCLK_DIV1;

    RCC_ClkInitStruct.APB1CLKDivider =
        RCC_HCLK_DIV2;

    RCC_ClkInitStruct.APB2CLKDivider =
        RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(
            &RCC_ClkInitStruct,
            FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_RCC_ADC_CONFIG(RCC_ADCPCLK2_DIV6);
}

static void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    hadc1.Instance = ADC1;

    hadc1.Init.ScanConvMode =
        ADC_SCAN_DISABLE;

    hadc1.Init.ContinuousConvMode =
        DISABLE;

    hadc1.Init.DiscontinuousConvMode =
        DISABLE;

    hadc1.Init.ExternalTrigConv =
        ADC_SOFTWARE_START;

    hadc1.Init.DataAlign =
        ADC_DATAALIGN_RIGHT;

    hadc1.Init.NbrOfConversion = 1;

    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    sConfig.Channel =
        ADC_CHANNEL_0;

    sConfig.Rank =
        ADC_REGULAR_RANK_1;

    sConfig.SamplingTime =
        ADC_SAMPLETIME_55CYCLES_5;

    if (HAL_ADC_ConfigChannel(
            &hadc1,
            &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;

    huart1.Init.BaudRate = 115200;

    huart1.Init.WordLength =
        UART_WORDLENGTH_8B;

    huart1.Init.StopBits =
        UART_STOPBITS_1;

    huart1.Init.Parity =
        UART_PARITY_NONE;

    huart1.Init.Mode =
        UART_MODE_TX_RX;

    huart1.Init.HwFlowCtl =
        UART_HWCONTROL_NONE;

    huart1.Init.OverSampling =
        UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (hadc->Instance == ADC1)
    {
        __HAL_RCC_ADC1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_InitStruct.Pin =
            GPIO_PIN_0;

        GPIO_InitStruct.Mode =
            GPIO_MODE_ANALOG;

        HAL_GPIO_Init(
            GPIOA,
            &GPIO_InitStruct
        );
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (huart->Instance == USART1)
    {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_InitStruct.Pin =
            GPIO_PIN_9;

        GPIO_InitStruct.Mode =
            GPIO_MODE_AF_PP;

        GPIO_InitStruct.Speed =
            GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(
            GPIOA,
            &GPIO_InitStruct
        );

        GPIO_InitStruct.Pin =
            GPIO_PIN_10;

        GPIO_InitStruct.Mode =
            GPIO_MODE_INPUT;

        GPIO_InitStruct.Pull =
            GPIO_NOPULL;

        HAL_GPIO_Init(
            GPIOA,
            &GPIO_InitStruct
        );
    }
}
void SysTick_Handler(void)
{
    HAL_IncTick();
}
void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}

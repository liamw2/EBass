#pragma once

#include <stdio.h>

/**
 * @brief Initialize button inputs
 * 
 */
void button_init(void);

/**
 * @brief Fetch bitmap of button status
 * 
 * @return uint8_t 
 */
uint8_t button_status(void);
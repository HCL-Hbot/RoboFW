/*
 * Copyright 2024 JUNE - HAN University of Applied Sciences, Health Concept Lab
 * Project HealthBot
 * Authors: Victor Hogeweij, Marelle Vink and Richard Kroesen
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

#ifndef BRAINBOARD_DRIVER_HPP
#define BRAINBOARD_DRIVER_HPP

#include "command_defines.hpp"

#include "embedded_cli.h"
#include "UART_streamer.hpp"
#include "board_defines.h"

#include <eye_driver.hpp>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "motor_driver_binding.hpp"
#include "display_driver_binding.hpp"
#include "ledstrip_driver_binding.hpp"
#include "radar_sensor_binding.hpp"
#include "led_driver_binding.hpp"

#define CLI_PROCESSING_PERIOD       100
#define UART_RECEIVE_TASK_PERIOD    100
#define CLI_HANDLER_STACK_SIZE      900
#define QUEUE_SIZE_DEFUALT          10

namespace COM {

// Global, but namespace limited Queues:
inline QueueHandle_t motorCommandQueue;
inline QueueHandle_t eyeControlCommandQueue;
inline QueueHandle_t ledStripCommandQueue;
inline QueueHandle_t radarCommandQueue;
inline QueueHandle_t leddriver_command_queue;

class BrainBoardDriver {
public:
    BrainBoardDriver(const uint32_t baudRate, const uint8_t txPin, 
    const uint8_t rxPin, EmbeddedCli* cli) :
        uart_driver_(baudRate, txPin, rxPin), cli_(cli) {
        instance_ = this;
        initialize();
        cli->writeChar = writeChar;
        setupCliBindings();

        // Queeue Creation: 
        motorCommandQueue = xQueueCreate(QUEUE_SIZE_DEFUALT, sizeof(MotorCommand));
        eyeControlCommandQueue = xQueueCreate(QUEUE_SIZE_DEFUALT, sizeof(DisplayCommand));
        ledStripCommandQueue = xQueueCreate(QUEUE_SIZE_DEFUALT, sizeof(LedstripCommand));
        radarCommandQueue = xQueueCreate(QUEUE_SIZE_DEFUALT, sizeof(RadarCommand));
        leddriver_command_queue = xQueueCreate(QUEUE_SIZE_DEFUALT, sizeof(LedDriverCommand));

        if (motorCommandQueue == nullptr) {
            printf("Failed to create MOTOR command queue.\n");
        }
        if (eyeControlCommandQueue == nullptr) {
            printf("Failed to create DISPL command queue.\n");
        }
        if (ledStripCommandQueue == nullptr) {
            printf("Failed to create LEDS command queue.\n");
        }
        if (radarCommandQueue == nullptr) {
            printf("Failed to create RADAR command queue.\n");
        }
        if (leddriver_command_queue == nullptr) {
            printf("Failed to create LED command queue.\n");
        }
    }

    ~BrainBoardDriver() {
        uart_driver_.deinitialize();
    }

    void initialize() {
        uart_driver_.initialize();
    }

    void startTasks() {
        xTaskCreate(&BrainBoardDriver::cliTaskHandle, "EMB-CLI-Handler", CLI_HANDLER_STACK_SIZE, this, 1, nullptr);
    }

    void writeByte(uint8_t byte) {
        uart_driver_.writeByte(byte);
    }

    void sendRadarData(uint16_t valueI, uint16_t valueQ) {
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "I: %d, Q: %d\n", valueI, valueQ);
        uart_driver_.uart_send_non_blocking(buffer);
    }

private:
    SERIAL::UART_RTOS_Driver uart_driver_;
    EmbeddedCli* cli_;
    static inline BrainBoardDriver* instance_ = nullptr;

    void setupCliBindings() {
        addMotorCliBindings(cli_);
        addDisplayCliBindings(cli_);
        addLedstripCliBindings(cli_);
        addRadarCliBindings(cli_);
        addLedCliBindings(cli_);
    }

    static inline void writeChar(EmbeddedCli* embeddedCli, char c) {
        instance_->writeByte(static_cast<uint8_t>(c));
    }

    static void cliTaskHandle(void* pvParameters) {
        BrainBoardDriver* driver = static_cast<BrainBoardDriver*>(pvParameters);
        SERIAL::uart_buffer_t rxBuffer;
        
        while (true) {
            if (driver->uart_driver_.uart_check_rx_non_blocking(&rxBuffer, sizeof(rxBuffer), pdMS_TO_TICKS(UART_RECEIVE_TASK_PERIOD))) {
                for (uint16_t i = 0; i < rxBuffer.length; i++) {
                    embeddedCliReceiveChar(driver->cli_, rxBuffer.data[i]);
                }
                embeddedCliProcess(driver->cli_);
            } else {
                taskYIELD();
            }
        }
    }
}; // class BrainBoardDriver
} // namespace COM
#endif // BRAINBOARD_DRIVER_HPP
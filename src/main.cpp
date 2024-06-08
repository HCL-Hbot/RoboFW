#include "hardware/structs/rosc.h"
#include <stdio.h>

#include <string.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include <task.h>
#include "board_defines.h"
#include <hardware/spi.h>
#include <lv_drv_conf.h>
#include <lvgl.h>
#include "lv_port_disp.h"
#include "ui.h"
#include "tusb.h"

#include <eye_driver.hpp>
#include <UART_streamer.hpp>
#include "cli_port.hpp"

#include <ledstrip_driver.hpp>
#include <Effects/Fade.hpp>
#include "PicoLedEffect.hpp"

#include <brainboard_driver.hpp>
#include <step_motor_manager.hpp>
#include <ledstrip_manager.hpp>

#include <bgt60ltr11XXX_driver.hpp>
#include <expander_driver.hpp>

#define NUM_LEDS1           (30 + 1)
#define NUM_LEDS2           (60 + 1)
#define LED_BRIGHTNESS      100

int main() {
    stdio_init_all();
    lv_init();
    lv_port_disp_init();

    // Initialize CLI
    EmbeddedCliConfig* config = embeddedCliDefaultConfig();
    config->cliBufferSize = 255;
    config->maxBindingCount = 20;
    config->cmdBufferSize = 300;
    EmbeddedCli* cli = embeddedCliNew(config);

    lv_disp_t *disp1 = lv_disp_get_default();
    lv_disp_t *disp2 = lv_disp_get_next(disp1);

    /* IO Expander Test*/
    i2c_inst_t *i2c = i2c1;
    uint8_t address = 0x20; // A2 & A1 are L in the PCB so Addr 32DEC. 
    uint8_t sda_pin = IO_EXPANDER_SDA;    // Replace with your SDA pin
    uint8_t scl_pin = IO_EXPANDER_SCL;    // Replace with your SCL pin
    uint32_t baudrate = 100'000;

    EXPANDER::TCA9534DWR &expander 
        = EXPANDER::TCA9534DWR::getInstance(i2c, address, sda_pin, scl_pin, baudrate);

    // Initialize the expander
    if (!expander.init()) {
        printf("Failed to initialize TCA9534DWR\n");
        return 1;
    }

    MOTOR::MotorManager motorManager;
    motorManager.addMotor(1, std::make_unique<MOTOR::StepMotorDriver>(expander, MOTOR1_STEP, MOTOR1_DIR, PEXP0_MOTOR1_ENABLE));
    motorManager.addMotor(2, std::make_unique<MOTOR::StepMotorDriver>(expander, MOTOR2_STEP, MOTOR2_DIR, PEXP1_MOTOR2_ENABLE));

    // Get the motor instance from the MotorManager
    MOTOR::StepMotorDriver* motor1 = motorManager.getMotor(1);
    MOTOR::StepMotorDriver* motor2 = motorManager.getMotor(2);

    if (motor1) {
        // Enable Motor 1
        // motor1->enable();
        // printf("Motor 1 enabled\n");
        // motor1->initPulseGenerator(0, 250, 300);
        // Perform some operations on Motor 1
        motor1->rotateAngle(MOTOR::DefaultAngles::ANGLE_90); // Rotate 90 degrees
        motor2->rotateAngle(MOTOR::DefaultAngles::ANGLE_240); // Rotate 90 degrees

        // Check the speed of Motor 1
        float speed_dps = motor1->getSpeed_dps();
        printf("Motor 1 speed: %.2f deg/sec\n", speed_dps);
    }

    // const PicoLed::DataFormat DATA_FORMAT1 = PicoLed::DataFormat::FORMAT_RGB;

    // DISPLAY::EyeDisplayDriver driver(disp1, disp2);
    // LED::LedStripManager ledStripManager;
    // // MOTOR::MotorManager motorManager;

    // auto strip1 = std::make_unique<LED::LedStripDriver>(pio0, 0, LEDSTRIP1_DATAPIN, NUM_LEDS1, DATA_FORMAT1);
    // auto strip2 = std::make_unique<LED::LedStripDriver>(pio1, 0, LEDSTRIP2_DATAPIN, NUM_LEDS1, DATA_FORMAT1);
    
    // ledStripManager.addLedStrip(1, std::move(strip1));
    // ledStripManager.addLedStrip(2, std::move(strip2));
    // // motorManager.addMotor(1, std::make_unique<MOTOR::StepMotorDriver>(MOTOR1_STEP, MOTOR1_DIR, PEXP0_MOTOR1_ENABLE));
    // // motorManager.addMotor(2, std::make_unique<MOTOR::StepMotorDriver>(MOTOR2_STEP, MOTOR2_DIR, PEXP1_MOTOR2_ENABLE));
    
    // // motorManager.startMotorTask();
    // driver.startTasks();
    // ledStripManager.start();

    // COM::BrainBoardDriver commDriver(UART_BAUD_RATE, UART_TX_PICO, UART_RX_PICO, cli); 
    // commDriver.startTasks();

    // vTaskStartScheduler();

    while (true) {;}

    delete config;
    delete cli;
    delete disp1;
    delete disp2;

    return 0;
}
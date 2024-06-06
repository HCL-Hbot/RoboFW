#ifndef EYE_DISPLAY_DRIVER_HPP
#define EYE_DISPLAY_DRIVER_HPP

#include <memory>
#include <lv_drv_conf.h>
#include <lvgl.h>
#include "lv_port_disp.h"
#include "ui.h"
#include "ui_helpers.h"
#include "lvgl.h"

#include "display_driver_binding.hpp"
#include "eye_component.hpp"
#include <command_defines.hpp>

namespace DISPLAY {

class EyeDisplayDriver {
public:
    EyeDisplayDriver(lv_disp_t* disp1, lv_disp_t* disp2) {
        if (disp1) {
            lv_disp_set_default(disp1);
            eyeLeft = std::make_unique<EyeComponent>(lv_scr_act(), &ui_img_oog_gekozen_8_gesplitst_png, nullptr, nullptr);
        }
        if (disp2) {
            lv_disp_set_default(disp2);
            eyeRight = std::make_unique<EyeComponent>(lv_scr_act(), &ui_img_oog_gekozen_8_gesplitst_png, nullptr, nullptr);
        }
    }

    ~EyeDisplayDriver() {}

    void startTasks() {
        xTaskCreate(EyeDisplayDriver::displayHandler, "EyeDisplayHandler", 1200, this, 1, nullptr);
        xTaskCreate(EyeDisplayDriver::runCommandHandle, "EyeControlHandle", 1200, this, 1, nullptr);
    }

    static void displayHandler(void *pvParameters) {
        auto *driver = static_cast<EyeDisplayDriver*>(pvParameters);
        lv_init();
        lv_port_disp_init();

        while (true) {
            lv_tick_inc(1);
            vTaskDelay(1 / portTICK_PERIOD_MS);
            lv_task_handler();
        }
    }

    static void runCommandHandle(void *pvParameters) {
        auto *driver = static_cast<EyeDisplayDriver*>(pvParameters);
        COM::DisplayCommand command;

        while (true) {
            if (xQueueReceive(COM::eyeControlCommandQueue, &command, portMAX_DELAY) == pdPASS) {
                driver->handleEyeCommand(command);
            }
        }
    }

private:
    std::unique_ptr<EyeComponent> eyeLeft;
    std::unique_ptr<EyeComponent> eyeRight;

    void handleEyeCommand(const COM::DisplayCommand& command) {
        switch (command.type) {
            case COM::DisplayCommandType::MOVE_:
                if (command.displayId == 0 || command.displayId == 2) {
                    eyeLeft->animate_to_xy(eyeLeft->getPupil(), command.x_pos, command.y_pos, command.duration);
                }
                if (command.displayId == 1 || command.displayId == 2) {
                    eyeRight->animate_to_xy(eyeRight->getPupil(), command.x_pos, command.y_pos, command.duration);
                }
                break;
            case COM::DisplayCommandType::BLINK_ANIM:
                if (command.displayId == 0 || command.displayId == 2) {
                    eyeLeft->animate_blink();
                }
                if (command.displayId == 1 || command.displayId == 2) {
                    eyeRight->animate_blink();
                }
                break;
            case COM::DisplayCommandType::CONFUSED_ANIM:
                if (command.displayId == 0 || command.displayId == 2) {
                    eyeLeft->animate_confused();
                }
                if (command.displayId == 1 || command.displayId == 2) {
                    eyeRight->animate_confused();
                }
                break;
            case COM::DisplayCommandType::THINKING_ANIM:
                if (command.displayId == 0 || command.displayId == 2) {
                    eyeLeft->animate_thinking();
                }
                if (command.displayId == 1 || command.displayId == 2) {
                    eyeRight->animate_thinking();
                }
                break;
            case COM::DisplayCommandType::DISABLE:
                if (command.displayId == 0 || command.displayId == 2) {
                    eyeLeft->disable();
                }
                if (command.displayId == 1 || command.displayId == 2) {
                    eyeRight->disable();
                }
                break;
        }
    }
}; // class EyeDisplayDriver
} // namespace DISPLAY
#endif // EYE_DISPLAY_DRIVER_HPP
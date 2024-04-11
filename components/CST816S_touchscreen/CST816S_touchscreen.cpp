#include "esphome/core/log.h"
#include "CST816S_touchscreen.h"
#include "CST816S.h"
#define I2C_SDA 4
#define I2C_SCL 5
#define TP_RST 1
#define TP_INT 0

namespace esphome {
namespace cst816s_touchscreen {

unsigned long previousMillis = 0UL;
unsigned long interval = 500UL;

static const char *TAG = "cst816s_touchscreen.text_sensor";

CST816S touch(I2C_SDA, I2C_SCL, TP_RST, TP_INT);	// sda, scl, rst, irq

void CST816STouchScreen::setup() {
    ESP_LOGI("touchscreen", "Starting touchscreen");
    touch.begin();
}

void CST816STouchScreen::dump_config() { 
    ESP_LOGCONFIG(TAG, "CST816STouchScreen");
}

void CST816STouchScreen::loop() {
    unsigned long currentMillis = millis();

    if (touch.available()) {
        if(touch.data.gestureID != 0){                          //ignore None gesture type
            if(currentMillis - previousMillis > interval) {     //debounce
                char buf[20];
                char ans[20];
                char x_str[20]; // Assuming a maximum of 20 characters
                char y_str[20];
                sprintf(buf, "%s", touch.gesture());
                sprintf(x_str, "%d", touch.data.x);
                sprintf(y_str, "%d", touch.data.y);
                // Define separator character
                char separator = ':';
                // Calculate the total length of the resulting string
                int total_length = snprintf(NULL, 0, "%s%c%s%c%s", buf, separator, x_str, separator, y_str) + 1; // +1 for null terminator
                // Allocate memory for the resulting string
                char* result = (char*)malloc(total_length * sizeof(char));
                
                if (touch.data.gestureID == SWIPE_UP)
                    strcpy(ans, "1");
                else if (touch.data.gestureID == SWIPE_DOWN)
                    strcpy(ans, "2");
                else if (touch.data.gestureID == SWIPE_RIGHT)
                    strcpy(ans, "3");
                else if (touch.data.gestureID == SWIPE_LEFT)
                    strcpy(ans, "4");
                else if (touch.data.gestureID == LONG_PRESS)
                    strcpy(ans, "5");
                else
                    strcpy(ans, "0");
                snprintf(result, total_length, "%s%c%s%c%s", ans, separator, x_str, separator, y_str);
                
                ESP_LOGI("touchscreen", "pos: %s", result);

                this->publish_state(result);
                previousMillis = currentMillis;
                
                free(result);
        }
    }
}
}

}  // namespace cst816s_touchscreen
}  // namespace esphome

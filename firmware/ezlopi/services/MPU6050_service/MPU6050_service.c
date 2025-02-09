#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "debug.h"
#include "mpu6050.h"
#include "string.h"
#include "items.h"
#include "wss.h"
#include "driver/i2c.h"
#include "i2c_master_interface.h"

static uint16_t accel_x;
static uint16_t accel_y;
static uint16_t accel_z;
static uint16_t gyro_x;
static uint16_t gyro_y;
static uint16_t gyro_z;
static uint32_t device_index = 0xFF;

static void mpu_service_process(void *pv);
uint32_t web_provisioning_get_message_count(void);

void i2c_master_interface_init(int i2c_num, int sda, int scl, uint32_t clock_speed)
{
    // if (0 == i2c_port_status[i2c_num])
    // {
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda,
        .scl_io_num = scl,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = clock_speed};
    i2c_param_config(I2C_NUM_0, &i2c_config);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
    // i2c_port_status[i2c_num] = 1;
    // }
}

void mpu_service_init(uint8_t scl_pin, uint8_t sda_pin, uint32_t dev_idx)
{
    device_index = dev_idx;
    i2c_master_interface_init(I2C_NUM_0, sda_pin, scl_pin, 100000);
    xTaskCreatePinnedToCore(mpu_service_process, "mpu6050-service", 3072, NULL, 3, NULL, 1);
}

static void mpu_service_process(void *pv)
{
    while (1)
    {
        // ESP_ERROR_CHECK(MPU6050_i2c_master_init(SDA, SCL));
        // ESP_ERROR_CHECK(mpu6050_wake());
        mpu6050_wake();
        MPU_TASK();

        char *ret = items_update_with_device_index(NULL, 0, NULL, web_provisioning_get_message_count(), device_index);

        if (ret)
        {
            // TRACE_W(">> DHT-service TX(ret): %s", ret);
            wss_client_send(ret, strlen(ret));
            vPortFree(ret);
            ret = NULL;
        }

        vTaskDelay(15000 / portTICK_RATE_MS);
    }
}
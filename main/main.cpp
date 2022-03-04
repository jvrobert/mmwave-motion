#include <stdio.h>
#include "ProvisioningManager.h"
#include "MqttManager.h"
#include "Sen0395.h"
#include "mqtt_util.h"

#define TOPIC "garage_motion"

extern "C" {
    void app_main();
}

void Motion(bool detected)
{
    auto mqtt = MqttManager::GetInstance();
    std::string msg;
    AddJsonKv("detected", detected, msg);
    AddJsonKv("source", ProvisioningManager::GetInstance()->GetHostName(), msg);
    msg += "}";
    mqtt->Publish(TOPIC, msg);
    printf("Motion state: %d\n", detected);
}

void app_main(void)
{
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ProvisioningManager::Start(true);
    MqttManager::Start();
    Sen0395::Start();
    Sen0395::GetInstance()->OnDetectionChange(Motion);
}

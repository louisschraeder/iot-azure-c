cmake_minimum_required(VERSION 2.8.11)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_C_FLAGS "--std=c99 -g ${CMAKE_C_FLAGS}")
endif()

include_directories("/usr/local/include/azureiot"
        "/usr/local/include/azureiot/inc/")

set(SOURCE main.c bme280.c wiring.c telemetry.c parson.c config.h bme280.h wiring.h telemetry.h parson.h config.h dht11.c dht11.h soil_moisture_sensor.c soil_moisture_sensor.h Adafruit_ADS1115.cpp Adafruit_ADS1115.h)
add_executable(app ${SOURCE})
target_link_libraries(app wiringPi
        serializer
        iothub_client
        iothub_client_mqtt_transport
        umqtt
        aziotsharedutil
        ssl
        crypto
        curl
        pthread
        m
        ssl
        crypto)
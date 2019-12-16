#pragma once
#include <Arduino.h>
// The API module to handle service API calls
namespace EspWsApi {

    class Response {
        public:
            String message;
            uint8_t httpCode;
            String contentType;
            bool status;
        public:
            void set(uint8_t, const char*, const char*);
            void set(uint8_t, const char*, const char*, bool);
            String to_string();
    };
}
#pragma once
#include <Arduino.h>
#include "EspWsResponse.hpp"
// The API module to handle service API calls
namespace EspWsApi {

    class ApiHandler {
        public:
            Response call(String serverUri);
    };
}
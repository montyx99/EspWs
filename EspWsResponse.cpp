#include "EspWsResponse.hpp"

namespace EspWsApi {
    void Response::set(uint8_t code, const char* cType, const char* msg) {
        httpCode = code;
        contentType = cType;
        message = msg;
        status = true;
    };
    void Response::set(uint8_t code, const char* cType, const char* msg, bool sts) {
        httpCode = code;
        contentType = cType;
        message = msg;
        status = sts;
    };

    String Response::to_string() {
        String str = String(httpCode);
        str += " ";
        str += contentType;
        str += " ";
        str += message;
        str += " ";
        str += String(status);
        return str;
    };
}
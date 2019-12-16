#include "EspWsApiHandler.hpp"

namespace EspWsApi {
    Response ApiHandler::call(String serverUri) {
        Response res;
        res.set(200, "text/plain", "OK");
        return res;
    }
}
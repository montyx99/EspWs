#include "EspWsApiHandler.hpp"

namespace EspWsApi {
    Response call(String serverUri) {
        Response res;
        res.set(200, "text/plain", "OK");
        return res;
    }
}
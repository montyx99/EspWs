# EspWs

The EspWs is just a very simple SPIFFS based webserver with a very basic API call. This is mostly created to demonstrate the [EspReact](https://github.com/montyx99/espReact) library, but I have plans to make it much more better, with JSON API, with drop of the ESPWebserver dependency, authentication, etc.
Every contirbution is warmly welcomed. You can send me an [email](mailto:monty.whisp@gmail.com) or find me on discord as MonitQ#7059

## Usage

Clone the repo to your sketches folder:
```
git clone https://github.com/montyx99/EspWs.git
```
With Aruino IDE compile and upload the code to your ESP8266 device. Use the steps from the [tutorial](https://www.instructables.com/id/Using-ESP8266-SPIFFS/) of [Steve Quinn](https://www.instructables.com/member/SteveQuinn/). Thanks Steve, great job! :)

You can upload any HTML+JS+CSS files both as text or as gzip files to the SPIFFS storage, and these will be served. You can add more API calls to the [EspWsApiHandler](./EspWsApiHandler.cpp) module.

Enjoy to use, and Happy coding.

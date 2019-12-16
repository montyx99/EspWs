/*
  FSWebServer - Example WebServer with SPIFFS backend for esp8266
  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the ESP8266WebServer library for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  upload the contents of the data folder with MkSPIFFS Tool ("ESP8266 Sketch Data Upload" in Tools menu in Arduino IDE)
  or you can upload the contents of a folder if you CD in that folder and run the following command:
  for file in `\ls -A1`; do curl -F "file=@$PWD/$file" esp8266fs.local/edit; done

  access the sample web page at http://<HOSTNAME>.local
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

// Include the private data from the secrests header file
#include "secrets.h"
#include "EspWsResponse.hpp"
#include "EspWsApiHandler.hpp"

using EspWsApi::Response;
using EspWsApi::ApiHandler;

// Wifi settins from included secrets.h
const char* ssid = STASSID;
const char* password = STAPSK;
const char* hostname = HOSTNAME;
const int port = PORT;
//
ESP8266WebServer server(port);

// Format bytes
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

// Get extension by content type
String getContentType(String filename) {
  if (server.hasArg("download")) {
    return "application/octet-stream";
  } else if (filename.endsWith(".htm")) {
    return "text/html";
  } else if (filename.endsWith(".html")) {
    return "text/html";
  } else if (filename.endsWith(".css")) {
    return "text/css";
  } else if (filename.endsWith(".js")) {
    return "application/javascript";
  } else if (filename.endsWith(".png")) {
    return "image/png";
  } else if (filename.endsWith(".gif")) {
    return "image/gif";
  } else if (filename.endsWith(".jpg")) {
    return "image/jpeg";
  } else if (filename.endsWith(".jpeg")) {
    return "image/jpeg";
  } else if (filename.endsWith(".ico")) {
    return "image/x-icon";
  } else if (filename.endsWith(".xml")) {
    return "text/xml";
  } else if (filename.endsWith(".pdf")) {
    return "application/x-pdf";
  } else if (filename.endsWith(".zip")) {
    return "application/x-zip";
  } else if (filename.endsWith(".gz")) {
    return "application/x-gzip";
  }
  return "text/plain";
}

bool handleAPICalls(String path) {
    ApiHandler req;
    Response res;
    res = req.call(path);
    // How to handle API calls
    if(res.status) {
        Serial.println("API Sends: " + res.to_string());
        server.send(res.httpCode, res.contentType, res.message);
    }
    
    return true;
}

bool handleFileRequests(String path) {
    // Every root calls redirected to index.html
    if (path.equals("/")) {
        path += "index.html";
    }

    Serial.println("PATH: " + path);
    String contentType = getContentType(path);
    Serial.println("Content type: " + contentType);
    Serial.println("SPIFFS " + path + ".gz exists: " + SPIFFS.exists(path + ".gz"));

    // Check wether the requested file or gzipped version exists
    if(SPIFFS.exists(path) || SPIFFS.exists(path + ".gz")) {
            
        if(SPIFFS.exists(path + ".gz")) {
            path += ".gz";
        }

        Serial.println("PATH: " + path);
        File file = SPIFFS.open(path, "r");
        server.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;
}

bool handleAllRequests(String path) {
    bool found = false;
    Serial.println("handleFileRead: " + path);

    // Handle API Requests
    if (path.startsWith("/api/")) {
        found = handleAPICalls(path);
    }
    // Handle file requests
    else {
        found = handleFileRequests(path);
    }

    return found;
}

void setup(void) {
    Serial.begin(115200);
    Serial.print("\n");
    Serial.setDebugOutput(true);

    // Flash Filesystem INIT
    bool success = SPIFFS.begin();
    // Print file system content
    if(success){
        String str = "";
        Dir dir = SPIFFS.openDir("/");
        while (dir.next()) {
            str += dir.fileName();
            str += " / ";
            str += dir.fileSize();
            str += "\r\n";
        }
        Serial.print(str);
        Serial.println("File system mounted with success");
    }
    else{
        Serial.println("Error mounting the file system");  
    }

    //WIFI INIT
    Serial.printf("Connecting to %s\n", ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin(hostname)) {
        Serial.println("MDNS responder started");
    }
    Serial.print("Open http://");
    Serial.print(hostname);
    Serial.println(".local/edit to see the file browser");

    // Redirect every call to the root handle
    server.onNotFound([](){
        if(!handleAllRequests(server.uri())) {
            Serial.println("404: " + server.uri() + " not exists!");
            server.send(404, "text/plain", "FileNotFound");
        }
    });

    server.begin();
    Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
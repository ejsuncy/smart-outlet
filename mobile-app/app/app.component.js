"use strict";
var core_1 = require("@angular/core");
var http_1 = require("@angular/http");
var WS = require("nativescript-websockets");
var AppComponent = (function () {
    function AppComponent(http) {
        this.http = http;
        this.webSocketStatus = "Not connected.";
        this.message = "None";
        this.url = "ws://esp8266.local:81/";
        this.outlet1 = "on:1";
        this.outlet2 = "on:2";
        this.client = new WS(this.url, { protocols: [], timeout: 6000, allowCellular: true, headers: {} });
        this.setupWebsocketClient();
        this.webSocketStatus = "Connecting...";
        console.log("Opening ws client...");
        this.client.open();
        this.webSocketStatus = this.getStatus(this.client.readyState);
    }
    AppComponent.prototype.setupWebsocketClient = function () {
        console.log("setting up 'open' handler...");
        this.client.on('open', function (socket) {
            this.webSocketStatus = "WebSocket Connection Opened!";
            console.log(this.webSocketStatus);
            this.sendNumber();
        }, this);
        console.log("setting up 'message' handler");
        this.client.on('message', function (socket, message) {
            this.webSocketStatus = "Received Message!";
            this.message = message;
            console.log(this.webSocketStatus, this.message);
        }, this);
        console.log("setting up 'close' handler");
        this.client.on('close', function (socket, code, reason) {
            this.webSocketStatus = "WebSocket closed: " + reason + ". Code: " + code;
            console.log(this.webSocketStatus);
        }, this);
        console.log("setting up 'error' handler");
        this.client.on('error', function (socket, error) {
            this.webSocketStatus = "Error: " + error;
            console.log(this.webSocketStatus);
        }, this);
        console.log("done setting up handlers for websocket");
    };
    AppComponent.prototype.sendNumber = function () {
        this.webSocketStatus = this.getStatus(this.client.readyState);
        console.log("button tapped! Sending number and setting timeout to repeat this...");
        // if (!this.client.isOpen()) {
        //     this.client.open();
        // }
        this.outlet1 = this.outlet1 === 'on:1' ? 'off:1' : 'on:1';
        this.outlet2 = this.outlet2 === 'on:2' ? 'off:2' : 'on:2';
        this.client.send(this.outlet1);
        this.client.send(this.outlet2);
        console.log("message sent!");
        // setTimeout(this.sendNumber, 1000);
    };
    AppComponent.prototype.getStatus = function (code) {
        switch (code) {
            case 0:
                return "Connection";
            case 1:
                return "Open";
            case 2:
                return "Closing";
            case 3:
                return "Closed";
            default:
                return "Unknown";
        }
    };
    AppComponent = __decorate([
        core_1.Component({
            selector: "my-app",
            templateUrl: "app.component.html",
        }), 
        __metadata('design:paramtypes', [http_1.Http])
    ], AppComponent);
    return AppComponent;
}());
exports.AppComponent = AppComponent;
//# sourceMappingURL=app.component.js.map
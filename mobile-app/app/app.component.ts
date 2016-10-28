import {Component} from "@angular/core";
import {Http, Headers, Response} from "@angular/http";
import {Button} from "ui/button";

let WS = require("nativescript-websockets");

@Component({
    selector: "my-app",
    templateUrl: "app.component.html",
})
export class AppComponent {
    public webSocketStatus: string = "Not connected.";
    public message: string = "None";
    public url: string = "ws://esp8266.local:81/";
    public outlet1: string = "on:1";
    public outlet2: string = "on:2";
    public client: any;

    constructor(private http: Http) {
        this.client = new WS(this.url, {protocols: [], timeout: 6000, allowCellular: true, headers: {}});
        this.setupWebsocketClient();
        this.webSocketStatus = "Connecting...";
        console.log("Opening ws client...");
        this.client.open();
        this.webSocketStatus = this.getStatus(this.client.readyState);
    }

    public setupWebsocketClient(): void {
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
    }

    public sendNumber() : void {
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
    }

    private getStatus (code : number) : string {
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
    }
}

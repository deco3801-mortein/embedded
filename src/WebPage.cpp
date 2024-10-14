#include "WebPage.hpp"

const char web_page[] = R"EOF(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Setup Device</title>
    <style>
@font-face {
    font-family: "Pilser";
    src: url("/static/Pilser.otf") format("opentype");
}

body {
    font-family: "Pilser", sans-serif;
    margin: 0;
    padding: 0;
    background-color: #fcffe0;
}

h1 {
    text-align: center;
    margin-top: 2em;
    font-size: 48px;
}

form div {
    margin-top: 1em;
    text-align: center;
    font-size: 24px;
}

    </style>
</head>
<body>
    <h1>Set Up Device</h1>
    <form action="/settings" method="post">
        <div>
            <label for="wifissid">WiFi SSID</label>
            <input type="text" name="wifissid" id="wifissid" required>
        </div>
        <div>
            <label for="wifipass">WiFi Password</label>
            <input type="password" name="wifipass" id="wifipass">
        </div>
        <div>
            <label for="devname">Device Name</label>
            <input type="text" name="devname" id="devname" required>
        </div>
        <div>
            <input type="submit" value="Set">
        </div>
    </form>
</body>
</html>)EOF";

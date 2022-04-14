var width = 250;
var height = width;

var canvas = document.getElementById("cvs");
canvas.width = width;
canvas.height = height;
var ctx = canvas.getContext("2d");
ctx.fillStyle = "#FF0000";

var x = 0;
var y = height / 2;
var modX = 1;
var modY = 0;

// Directions:
// North = 0
// East = 1
// South = 2
// West = 3
var direction = 1;

var started = false;
var timer;

function start() {
    timer = setInterval(function() {
        if (isPathOpen()) {
            ctx.fillRect(x, y, 5, 5);
            x += modX;
            y += modY;
        } else {
            ctx.fillRect(x + modX, y + modY, 5, 5);
            stop();
            document.getElementById("start").value = "Crashed!";
            document.getElementById("start").onclick = "";
        }
    }, 50);
}

function isPathOpen() {
    var tmpX = x + modX;
    var tmpY = y + modY;

    if (tmpX < 0 || tmpX > width || tmpY < 0 || tmpY > height) {
        return false;
    }

    tmpX = direction == 0 || direction == 3 ? tmpX : tmpX + 5;
    tmpY = direction == 0 || direction == 3 ? tmpY : tmpY + 5;
    var path = ctx.getImageData(tmpX, tmpY, 1, 1).data;

    for (var i = 0; i < path.length; i++) {
        if (path[i] != 0) {
            return false;
        }
    }

    return true;
}

function stop() {
    if (started) {
        document.getElementById("start").value = "Start";
        clearInterval(timer);
    } else {
        document.getElementById("start").value = "Stop";
        start();
    }
    started = !started;
}

// true for right
// false for left
function turn(way) {
    var mod = (way ? 1 : -1);

    var turns = {
        0 : [1, 0],
        1 : [0, 1],
        2 : [-1, 0],
        3 : [0, -1]
    }
    
    modX = mod * turns[direction][0];
    modY = mod * turns[direction][1];

    direction = direction == (way ? 3 : 0) ? (way ? 0 : 3) : direction + mod;
}
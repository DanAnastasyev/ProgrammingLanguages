var transport = signalR.TransportType.WebSockets;
var connection = new signalR.HubConnection(`http://${document.location.host}/game`,
                                           { transport: transport });

var boardSize = 600;
var numOfCells = 10;
var canvasPadding = 10;
var cellSize = boardSize / numOfCells;

function getRandomInt(min, max) {
    return Math.floor(Math.random() * (max - min)) + min;
}

function comicLineTo(context, x1, y1, x2, y2, segmentsCount=20, delta=2) {
    context.moveTo(x1, y1);
    for (var j = 0; j < segmentsCount; ++j) {
        var nextX1 = x1 + (x2 - x1) * (j + 1 / 3) / segmentsCount;
        var nextY1 = y1 + (y2 - y1) * (j + 1 / 3) / segmentsCount;
        var nextX2 = x1 + (x2 - x1) * (j + 2 / 3) / segmentsCount;
        var nextY2 = y1 + (y2 - y1) * (j + 2 / 3) / segmentsCount;
        var nextX = x1 + (x2 - x1) * (j + 1) / segmentsCount;
        var nextY = y1 + (y2 - y1) * (j + 1) / segmentsCount;

        context.bezierCurveTo(
            nextX1 + getRandomInt(-delta, delta),
            nextY1 + getRandomInt(-delta, delta),
            nextX2 + getRandomInt(-delta, delta),
            nextY2 + getRandomInt(-delta, delta),
            nextX, nextY
        );
    }
}

function drawBoard() {
    var context = document.getElementById("board").getContext("2d");
    context.strokeStyle = "rgba(0, 0, 255, 0.65)";
    context.lineWidth = 1;
    context.beginPath();
    for (var i = 0; i <= numOfCells; ++i) {
        comicLineTo(context,
            canvasPadding + i * cellSize,
            canvasPadding,
            canvasPadding + i * cellSize,
            canvasPadding + boardSize);

        comicLineTo(context,
            canvasPadding,
            canvasPadding + i * cellSize,
            canvasPadding + boardSize,
            canvasPadding + i * cellSize);
    }
    context.stroke();
}

connection.on("broadcastMessage", (message) => {
    alert(message);
});

connection.on("setBoardSize", (newBoardSize) => {
    var board = document.getElementById("board");
    board.width = newBoardSize + canvasPadding * 2;
    board.height = newBoardSize + canvasPadding * 2;

    var padding = 20;
    document.body.style.backgroundSize =
        `${newBoardSize + padding}px ${newBoardSize + padding}px`;
    boardSize = newBoardSize;

    drawBoard();

    var boardX = board.getBoundingClientRect().left + canvasPadding;
    var boardY = board.getBoundingClientRect().top + canvasPadding;
    board.addEventListener("click", event => {
        connection.invoke("click", event.pageX - boardX, event.pageY - boardY);
    });

    var leftButton = document.getElementById("leftButton");
    leftButton.addEventListener("click", () => {
        connection.invoke("MoveLeft");
    });

    var rightButton = document.getElementById("rightButton");
    rightButton.addEventListener("click", () => {
        connection.invoke("MoveRight");
    });

    var downButton = document.getElementById("downButton");
    downButton.addEventListener("click", () => {
        connection.invoke("MoveDown");
    });

    var upButton = document.getElementById("upButton");
    upButton.addEventListener("click", () => {
        connection.invoke("MoveUp");
    });
});

connection.on("drawX", (i, j) => {
    var offset = 15;
    var context = document.getElementById("board").getContext("2d");
    context.strokeStyle = "rgba(244, 66, 66, 0.8)";
    context.lineWidth = 4;
    context.beginPath();

    comicLineTo(context,
        canvasPadding + i * cellSize + offset,
        canvasPadding + j * cellSize + offset,
        canvasPadding + (i + 1) * cellSize - offset,
        canvasPadding + (j + 1) * cellSize - offset, 5);

    comicLineTo(context,
        canvasPadding + i * cellSize + offset,
        canvasPadding + (j + 1) * cellSize - offset,
        canvasPadding + (i + 1) * cellSize - offset,
        canvasPadding + j * cellSize + offset, 5);

    context.stroke();
});

connection.on("drawZero", (i, j) => {
    var offset = 25;
    var context = document.getElementById("board").getContext("2d");
    context.strokeStyle = "rgba(65, 178, 244, 0.8)";
    context.lineWidth = 4;
    context.beginPath();
    var x = canvasPadding + (i + 0.5) * cellSize;
    var y = canvasPadding + (j + 0.5) * cellSize;
    var radius = (cellSize - offset) / 2;

    context.moveTo(x + radius, y);

    var arcsCount = 16;
    var deltaAngle = 2 * Math.PI / arcsCount;
    for (var ind = 0; ind < arcsCount; ++ind) {
        var prevAngle = ind * deltaAngle;
        var nextAngle = (ind + 1) * deltaAngle;
        var nextX1 = x + radius * Math.cos(prevAngle + deltaAngle / 3);
        var nextY1 = y + radius * Math.sin(prevAngle + deltaAngle / 3);
        var nextX2 = x + radius * Math.cos(prevAngle + 2 * deltaAngle / 3);
        var nextY2 = y + radius * Math.sin(prevAngle + 2 * deltaAngle / 3);
        var nextX = x + radius * Math.cos(nextAngle);
        var nextY = y + radius * Math.sin(nextAngle);

        context.bezierCurveTo(
            nextX1 + getRandomInt(-2, 2), nextY1 + getRandomInt(-2, 2),
            nextX2 + getRandomInt(-2, 2), nextY2 + getRandomInt(-2, 2),
            nextX, nextY
        );
    }
    
    context.stroke();
});

connection.on("clear", () => {
    var board = document.getElementById("board");
    var context = board.getContext("2d");
    context.clearRect(0, 0, board.width, board.height);
    drawBoard();
});

connection.start();

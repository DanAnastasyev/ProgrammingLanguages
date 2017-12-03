var transport = signalR.TransportType.WebSockets;
var connection = new signalR.HubConnection(`http://${document.location.host}/game`,
                                           { transport: transport });

var boardSize = 600;
var numOfCells = 10;
var cellSize = boardSize / numOfCells;

function drawBoard() {
    var context = document.getElementById("board").getContext("2d");
    context.strokeStyle = "rgba(0, 0, 255, 0.65)";
    context.lineWidth = 1;
    context.beginPath();
    for (var i = 0; i <= numOfCells; ++i) {
        context.moveTo(i * cellSize, 0);
        context.lineTo(i * cellSize, boardSize);
        context.moveTo(0, i * cellSize);
        context.lineTo(boardSize, i * cellSize);
    }
    context.stroke();
}

connection.on("broadcastMessage", (message) => {
    alert(message);
});

connection.on("setBoardSize", (newBoardSize) => {
    var board = document.getElementById("board");
    board.width = newBoardSize;
    board.height = newBoardSize;

    var padding = 20;
    document.body.style.backgroundSize =
        `${newBoardSize + padding}px ${newBoardSize + padding}px`;
    boardSize = newBoardSize;

    drawBoard();

    var boardX = board.getBoundingClientRect().left;
    var boardY = board.getBoundingClientRect().top;
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
    context.lineWidth = 3;
    context.beginPath();
    context.moveTo(i * cellSize + offset, j * cellSize + offset);
    context.lineTo((i + 1) * cellSize - offset, (j + 1) * cellSize - offset);
    context.moveTo(i * cellSize + offset, (j + 1) * cellSize - offset);
    context.lineTo((i + 1) * cellSize - offset, j * cellSize + offset);
    context.stroke();
});

connection.on("drawZero", (i, j) => {
    var offset = 25;
    var context = document.getElementById("board").getContext("2d");
    context.strokeStyle = "rgba(65, 178, 244, 0.8)";
    context.lineWidth = 3;
    context.beginPath();
    context.arc((i + 0.5) * cellSize, (j + 0.5) * cellSize,
        (cellSize - offset) / 2, 0, 2 * Math.PI);
    context.stroke();
});

connection.on("clear", () => {
    var board = document.getElementById("board");
    var context = board.getContext("2d");
    context.clearRect(0, 0, board.width, board.height);
    drawBoard();
});

connection.start();

var canvasWidth = 800;
var canvasHeight = 600;


function drawBlueFilledCircle() {
    drawFilledCircle(100, 100, 50, "#0000FF");
}


function drawTurquoiseOutlineCircle() {
    drawCircle(250, 100, 45, "#40E0D0", 3);
}


function drawTriangle() {
    drawTriangleShape(400, 50, 350, 150, 450, 150,
                      "#FF6B6B", "#C44D58", 2);
}


function drawRectangleShape() {
    drawRectangle(500, 70, 100, 80,
                  "#4ECDC4", "#2A9D8F", 2);
}


function drawFourLines() {
    drawLine(100, 250, 300, 250, "#FF0000", 3);
    drawLine(100, 280, 300, 280, "#00FF00", 3);
    drawLine(100, 310, 300, 310, "#0000FF", 3);
    drawLine(100, 340, 300, 340, "#9B59B6", 3);
}


function drawAllShapes() {
    drawBlueFilledCircle();
    drawTurquoiseOutlineCircle();
    drawTriangle();
    drawRectangleShape();
    drawFourLines();
}
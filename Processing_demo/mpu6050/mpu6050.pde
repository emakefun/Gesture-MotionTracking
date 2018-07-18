import processing.serial.*;
 
Serial myPort;  // serila port numble 
 
float [] RwEst = new float[3];
byte[] inBuffer = new byte[100];

PFont font;
final int VIEW_SIZE_X = 1080,VIEW_SIZE_Y = 720;

void setup() 
{
  size(1080, 720, P3D);
  myPort = new Serial(this, Serial.list()[0], 9600);
  // myPort = new Serial(this, "/dev/ttyUSB0", 9600);  
  // load Font CourierNew type  /date 
  font = loadFont("CourierNewPSMT-32.vlw");
}

void readSensors() {
  if (myPort.available() > 0) {
    if (myPort.readBytesUntil('\n', inBuffer) > 0) {
      String inputString = new String(inBuffer);
      String [] inputStringArr = split(inputString,',');
      RwEst[0] = float(inputStringArr[0]);
      RwEst[1] = float(inputStringArr[1]);
      RwEst[2] = float(inputStringArr[2]);
    }
  }
  
}

void buildBoxShape() {
  //box(60, 10, 40);
  noStroke();
  beginShape(QUADS);
 
  //Z+
  fill(#00ff00);
  vertex(-30, -5, 20);
  vertex(30, -5, 20);
  vertex(30, 5, 20);
  vertex(-30, 5, 20);
 
  //Z-
  fill(#0000ff);
  vertex(-30, -5, -20);
  vertex(30, -5, -20);
  vertex(30, 5, -20);
  vertex(-30, 5, -20);
 
  //X-
  fill(#ff0000);
  vertex(-30, -5, -20);
  vertex(-30, -5, 20);
  vertex(-30, 5, 20);
  vertex(-30, 5, -20);
 
  //X+
  fill(#ffff00);
  vertex(30, -5, -20);
  vertex(30, -5, 20);
  vertex(30, 5, 20);
  vertex(30, 5, -20);
 
  //Y-
  fill(#ff00ff);
  vertex(-30, -5, -20);
  vertex(30, -5, -20);
  vertex(30, -5, 20);
  vertex(-30, -5, 20);
 
  //Y+
  fill(#00ffff);
  vertex(-30, 5, -20);
  vertex(30, 5, -20);
  vertex(30, 5, 20);
  vertex(-30, 5, 20);
 
  endShape();
}
 
 
void drawCube() {  
  pushMatrix();
  // normalize3DVec(RwEst);
  translate(300, 450, 0);
  scale(4, 4, 4);
  rotateX(HALF_PI * -RwEst[1]);
  //rotateY(HALF_PI * -0.5);
  rotateZ(HALF_PI * -RwEst[0]);
  buildBoxShape();
  popMatrix();
}
 
void draw() {  
 // getInclination();
  readSensors();
  background(#214565);
  fill(#ffffff);
  textFont(font, 20);
  text("RwEst :\n" + RwEst[0] + "\n" + RwEst[1] + "\n" + RwEst[2], 220, 180);
  // display axes
  pushMatrix();
  translate(450, 250, 0);
  stroke(#ffffff);
 // scale(100, 100, 100);
  line(0, 0, 0, 100, 0, 0);
  line(0, 0, 0, 0, -100, 0);
  line(0, 0, 0, 0, 0, 100);
  line(0, 0, 0, -RwEst[0], RwEst[1], RwEst[2]);
  popMatrix();
  drawCube();
}

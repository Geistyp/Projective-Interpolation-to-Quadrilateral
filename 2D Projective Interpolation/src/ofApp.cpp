#include "ofApp.h"

GLuint VBO, VAO;

void calcQn(float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y,
	float& q0, float& q1, float &q2, float & q3)
{
	float ax = p2x - p0x;
	float ay = p2y - p0y;
	float bx = p3x - p1x;
	float by = p3y - p1y;

	float cross = ax * by - ay * bx;

	if (cross != 0) {
		float cy = p0y - p1y;
		float cx = p0x - p1x;

		float s = (ax * cy - ay * cx) / cross;

		if (s > 0 && s < 1) {
			float t = (bx * cy - by * cx) / cross;

			if (t > 0 && t < 1) {
				q0 = 1 / (1 - t);
				q1 = 1 / (1 - s);
				q2 = 1 / t;
				q3 = 1 / s;

				// you can now pass (u * q, v * q, q) to OpenGL
			}
		}
	}
}


//--------------------------------------------------------------
void ofApp::setup(){
	shader.load("calibrate");
	shader.printActiveAttributes();

	float w = ofGetWidth();
	float h = ofGetHeight();

	float q0, q1, q2, q3;
	q0 = q1 = q2 = q3 = 1.0;
	//calcQn(w*0.25, 0, w*0.75, 0, w, h, 0, h, q0, q1, q2, q3);


	GLfloat  v[] = { w*0.25, 0, w*0.75, 0, w, h, 0, h, 
					0*q0, 0*q0, q0, 
					1*q1, 0*q1, q1, 
					1*q2, 1*q2, q2,
					0*q3, 1*q3, q3 };
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(8*sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0);


	ofDisableArbTex();
	img.load("test.jpeg");
	grid.load("grid.png");

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(0);
	ofSetColor(255);

	shader.begin();
	shader.setUniformTexture("tex", img.getTexture(), 0);
	shader.setUniformTexture("grid", grid.getTexture(), 1);
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);

	shader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ')
	{
		float w = ofGetWidth();
		float h = ofGetHeight();

		float q0, q1, q2, q3;
		q0 = q1 = q2 = q3 = 1.0;
		calcQn(w*0.25, 0, w*0.75, 0, w, h, 0, h, q0, q1, q2, q3);


		GLfloat  v[] = { w*0.25, 0, w*0.75, 0, w, h, 0, h,
			0 * q0, 0 * q0, q0,
			1 * q1, 0 * q1, q1,
			1 * q2, 1 * q2, q2,
			0 * q3, 1 * q3, q3 };

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	}

	if (key == 'b')
	{
		float w = ofGetWidth();
		float h = ofGetHeight();

		float q0, q1, q2, q3;
		q0 = q1 = q2 = q3 = 1.0;
		//calcQn(w*0.25, 0, w*0.75, 0, w, h, 0, h, q0, q1, q2, q3);


		GLfloat  v[] = { w*0.25, 0, w*0.75, 0, w, h, 0, h,
			0 * q0, 0 * q0, q0,
			1 * q1, 0 * q1, q1,
			1 * q2, 1 * q2, q2,
			0 * q3, 1 * q3, q3 };

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

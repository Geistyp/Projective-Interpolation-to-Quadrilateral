#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		struct VertexData
		{
			ofVec3f v;
			ofVec3f t;
		};

		ofEasyCam cam;

		ofPlanePrimitive plane;

		ofImage img;

		ofShader shader;

		vector<ofVec3f> vertex;
		vector<ofVec3f> coord;

		float mua, mub;

		
};

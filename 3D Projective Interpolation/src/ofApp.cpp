#include "ofApp.h"

/*
Calculate the line segment PaPb that is the shortest route between
two lines P1P2 and P3P4. Calculate also the values of mua and mub where
Pa = P1 + mua (P2 - P1)
Pb = P3 + mub (P4 - P3)
Return FALSE if no solution exists.
*/
bool LineLineIntersect(
	ofPoint p1, ofPoint p2, ofPoint p3, ofPoint p4, ofPoint *pa, ofPoint *pb,
	double *mua, double *mub)
{
	ofPoint p13, p43, p21;
	double d1343, d4321, d1321, d4343, d2121;
	double numer, denom;

	p13 = p1 - p3;
	p43 = p4 - p3;
	if (ABS(p43.x) < FLT_EPSILON  && ABS(p43.y) < FLT_EPSILON  && ABS(p43.z) < FLT_EPSILON)
		return false;

	p21 = p2 - p1;
	if (ABS(p21.x) < FLT_EPSILON  && ABS(p21.y) < FLT_EPSILON  && ABS(p21.z) < FLT_EPSILON)
		return false;

	d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
	d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
	d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
	d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
	d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

	denom = d2121 * d4343 - d4321 * d4321;
	if (ABS(denom) < FLT_EPSILON)
		return false;
	numer = d1343 * d4321 - d1321 * d4343;

	*mua = numer / denom;
	*mub = (d1343 + d4321 * (*mua)) / d4343;

	*pa = p1 + *mua * p21;
	*pb = p3 + *mub * p43;

	return true;
}

/*
http://mathforum.org/library/drmath/view/62814.html
Let's try this with vector algebra. First write the two equations like
this.

L1 = P1 + a V1

L2 = P2 + b V2

P1 and P2 are points on each line. V1 and V2 are the direction vectors
for each line.

If we assume that the lines intersect, we can look for the point on L1
that satisfies the equation for L2. This gives us this equation to
solve.

P1 + a V1 = P2 + b V2

Now rewrite it like this.

a V1 = (P2 - P1) + b V2

Now take the cross product of each side with V2. This will make the
term with 'b' drop out.

a (V1 X V2) = (P2 - P1) X V2

If the lines intersect at a single point, then the resultant vectors
on each side of this equation must be parallel, and the left side must
not be the zero vector. We should check to make sure that this is
true. Once we have checked this, we can solve for 'a' by taking the
magnitude of each side and dividing. If the resultant vectors are
parallel, but in opposite directions, then 'a' is the negative of the
ratio of magnitudes. Once we have 'a' we can go back to the equation
for L1 to find the intersection point.
*/

// TODO: assume two line in same planar
bool intersection(ofPoint p1, ofPoint p2, ofPoint p3, ofPoint p4, float *a, float *b)
{
	ofPoint V1 = p3 - p1;
	ofPoint V2 = p4 - p2;
	ofPoint P21 = p2 - p1;

	ofPoint C12 = V1.getCrossed(V2);		// get cross

	*a = (P21.getCrossed(V2)).length() / C12.length();
	*b = ((-P21).getCrossed(V1)).length() / C12.length();

	return true;
}

GLuint VBO, VAO;

//--------------------------------------------------------------
void ofApp::setup(){

	shader.load("calibrate");

	plane.set(400, 300, 2, 2);
	plane.mapTexCoords(0, 0, 1, 1);

	ofDisableArbTex();
	img.load("grid.png");

	//float w = ofGetWidth()*.5;
	//float h = ofGetHeight()*.5;
	float w = 0;
	float h = 0;

	vector<ofVec3f> v = { ofVec3f(w - 100, h - 100, 50), ofVec3f(w + 100, h - 100, 50), ofVec3f(w - 150, h + 150, 0), ofVec3f(w + 150, h + 100, 0) };
	vector<ofVec3f> t = { ofVec3f(0, 0), ofVec3f(1, 0), ofVec3f(0, 1), ofVec3f(1, 1) };

	vertex	= v;
	coord	= t;

	// 0, 1, 3, 2
	if (intersection(vertex[0], vertex[1], vertex[3], vertex[2], &mua, &mub))
	{
		cout << mua << " " << mub << endl;
	}

	// http://www.reedbeta.com/blog/2012/05/26/quadrilateral-interpolation-part-1/
	// calculate qi
	// uvqi = (di+d(i+2))/d(i+2) (i=0бн3)
	float q[] = { 1.0 / (1.0 - mua), 1.0 / (1.0 - mub), 1.0 / mub, 1.0 / mua };

	vector<VertexData> vds;

	for (int i = 0; i < vertex.size(); i++)
	{
		vds.push_back(VertexData());
		vds.back().v = vertex[i];

		// uvqi = float3(ui,vi,1)б┴(di+d(i+2))/d(i+2) (i=0бн3)
		vds.back().t = ofVec3f(coord[i].x *q[i], coord[i].y*q[i], q[i]);
	}

	{

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vds.size()*sizeof(VertexData), &vds.front(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)0);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)(sizeof(ofVec3f)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	cam.begin();

	shader.begin();
	shader.setUniformTexture("tex", img.getTexture(), 0);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	shader.end();

	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == 'a')
	{
		float q[] = { 1.0, 1.0, 1.0, 1.0 };

		struct VertexData
		{
			ofVec3f v;
			ofVec3f t;
		};

		vector<VertexData> vds;

		for (int i = 0; i < vertex.size(); i++)
		{
			vds.push_back(VertexData());
			vds.back().v = vertex[i];
			vds.back().t = ofVec3f(coord[i].x *q[i], coord[i].y*q[i], q[i]);
		}

		{
			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vds.size()*sizeof(VertexData), &vds.front(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)0);

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)(sizeof(ofVec3f)));

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(0);
		}
	}
	if (key == 's')
	{
		// http://www.reedbeta.com/blog/2012/05/26/quadrilateral-interpolation-part-1/
		float q[] = { 1.0 / (1.0 - mua), 1.0 / (1.0 - mub), 1.0 / mub, 1.0 / mua };

		vector<VertexData> vds;

		for (int i = 0; i < vertex.size(); i++)
		{
			vds.push_back(VertexData());
			vds.back().v = vertex[i];

			// uvqi = float3(ui,vi,1)б┴(di+d(i+2))/d(i+2) (i=0бн3)
			vds.back().t = ofVec3f(coord[i].x *q[i], coord[i].y*q[i], q[i]);
		}

		{

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vds.size()*sizeof(VertexData), &vds.front(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)0);

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)(sizeof(ofVec3f)));

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(0);
		}
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

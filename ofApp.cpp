#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	auto seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));

	this->face.clear();
	this->line.clear();

	float phi_deg_step = 1.5;
	float theta_deg_step = 6;

	float R = 240;
	float r = R * 0.25;

	for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

		for (float theta_deg = 0; theta_deg < 360; theta_deg += theta_deg_step) {

			auto noise_value = ofNoise(glm::vec4(this->make_point(R, r, theta_deg, phi_deg) * 0.035, ofGetFrameNum() * 0.025));
			if (noise_value < 0.45) { continue; }

			auto noise_1 = ofNoise(glm::vec4(this->make_point(R, r, theta_deg - theta_deg_step, phi_deg) * 0.035, ofGetFrameNum() * 0.025));
			auto noise_2 = ofNoise(glm::vec4(this->make_point(R, r, theta_deg, phi_deg + phi_deg_step) * 0.035, ofGetFrameNum() * 0.025));
			auto noise_3 = ofNoise(glm::vec4(this->make_point(R, r, theta_deg, phi_deg - phi_deg_step) * 0.035, ofGetFrameNum() * 0.025));
			auto noise_4 = ofNoise(glm::vec4(this->make_point(R, r, theta_deg + theta_deg_step, phi_deg) * 0.035, ofGetFrameNum() * 0.025));

			auto index = this->face.getNumVertices();
			vector<glm::vec3> vertices;

			vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
			vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
			vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));
			vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));

			for (auto& vertex : vertices) {

				auto rotation = glm::vec3(
					ofMap(ofNoise(glm::vec4(vertex * 0.00085, seed.x + ofGetFrameNum() * 0.01)), 0, 1, -90, 90),
					ofMap(ofNoise(glm::vec4(vertex * 0.00085, seed.y + ofGetFrameNum() * 0.01)), 0, 1, -90, 90),
					ofMap(ofNoise(glm::vec4(vertex * 0.00085, seed.z + ofGetFrameNum() * 0.01)), 0, 1, -90, 90));

				auto rotation_x = glm::rotate(glm::mat4(), rotation.x * (float)DEG_TO_RAD, glm::vec3(1, 0, 0));
				auto rotation_y = glm::rotate(glm::mat4(), rotation.y * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
				auto rotation_z = glm::rotate(glm::mat4(), rotation.z * (float)DEG_TO_RAD, glm::vec3(0, 0, 1));

				vertex = glm::vec4(vertex, 0) * rotation_z * rotation_y * rotation_x;
			}

			this->face.addVertices(vertices);

			this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 3);
			this->face.addIndex(index + 0); this->face.addIndex(index + 3); this->face.addIndex(index + 2);

			if (noise_1 < 0.45) {

				this->line.addVertex(vertices[0]);
				this->line.addVertex(vertices[2]);

				this->line.addIndex(this->line.getNumVertices() - 1);
				this->line.addIndex(this->line.getNumVertices() - 2);
			}

			if (noise_2 < 0.45) {

				this->line.addVertex(vertices[2]);
				this->line.addVertex(vertices[3]);

				this->line.addIndex(this->line.getNumVertices() - 1);
				this->line.addIndex(this->line.getNumVertices() - 2);
			}

			if (noise_3 < 0.45) {

				this->line.addVertex(vertices[0]);
				this->line.addVertex(vertices[1]);

				this->line.addIndex(this->line.getNumVertices() - 1);
				this->line.addIndex(this->line.getNumVertices() - 2);
			}

			if (noise_4 < 0.45) {

				this->line.addVertex(vertices[1]);
				this->line.addVertex(vertices[3]);

				this->line.addIndex(this->line.getNumVertices() - 1);
				this->line.addIndex(this->line.getNumVertices() - 2);

			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.333333333333333 * 6);

	ofSetColor(255);
	this->line.draw();

	ofSetColor(0);
	this->face.draw();

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
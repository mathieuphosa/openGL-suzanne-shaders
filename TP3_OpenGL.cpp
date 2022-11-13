#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <string>
#include <time.h> 
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

#define TITLE "Hello OpenGL!"
#define _USE_MATH_DEFINES
#define BUFFER_OFFSET(offset) ((GLvoid*)(offset))

float ka = 0.1f;
float kd = 0.4f;
float ks = 0.5f;

int SCREEN_X = 1024;
int SCREEN_Y = 768;

float lumx = 0.0f;
float lumy = 0.0f;
float lumz = 1.0f;

float v1_x = 0.0;
float v1_y = 0.5;

float v2_x = -0.5;
float v2_y = -0.5;

float v3_x = 0.5;
float v3_y = -0.5;

double angle = -45.0;
double angleh = -0;
int last_x = -1;
int last_y = -1;
int pressed_button = -1;
int but = -1;
int cam = 1;

int smooth = 0;

GLuint DrawListCube;
GLuint vaoCube;
GLuint vboCube;
GLuint vaoSuzanne;
GLuint vbo_mesh_data;
GLuint vaoSmoothSuzanne;
GLuint vbo_smooth_mesh_data;
GLuint program;

GLuint texture;

std::vector<glm::vec4> vPositions;
std::vector<glm::vec4> vColors;

vector<GLfloat> suzanne_mesh_data;
vector<GLfloat> suzanne_smooth_mesh_data;

std::string title = "FPS -";

int frame = 0;

time_t last_update;

int vphong = 1;

void phong();
void gouraud();

void init();
void initCustom();
void display();
void cleanup();
void keyfunc(unsigned char key, int x, int y);
void mousefunc(int button, int state, int x, int y);
void motionfunc(int x, int y);
void load_obj(const char* filename, vector<GLfloat>& mesh_data, vector<GLfloat>& smooth_data);
void drawSuzanne();
GLchar* readShaderSource(const char* shaderFile);
GLuint initShaders(const char* vShaderFile, const char* fShaderFile);
GLuint loadTextureFromBMP(const char* filename, int width, int height);
GLuint createTexureChecker();

void updateMVP();

int main(int argc, char** argv)
{
	cout << "Launch" << endl;
	glutInit(&argc, argv);
	cout << "glutinit" << endl;
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	cout << "glutInitDisplayMode" << endl;
	glutInitWindowSize(SCREEN_X, SCREEN_Y);
	cout << "glutInitWindowSize" << endl;
	glutCreateWindow(TITLE);
	cout << "glutCreateWindow" << endl;
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	cout << "glutSetOption" << endl;
	glEnable(GL_DEPTH_TEST);
	cout << "glEnable" << endl;
	GLenum err = glewInit();
	cout << "before error" << endl;
	if (GLEW_OK != err)
	{
		/* glewInit failed*/
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		cout << "error true" << endl;
		exit(EXIT_FAILURE);
	}
	cout << "no error" << endl;
	fprintf(stdout, "Using GLEW %s\n", glewGetString(GLEW_VERSION));
	cout << "fprintf" << endl;
	initCustom();
	cout << "initCustom" << endl;
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glViewport(0, 0, SCREEN_X, SCREEN_Y);
	time(&last_update);
	texture = createTexureChecker();
	drawSuzanne();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyfunc);
	glutMotionFunc(motionfunc);
	glutMouseFunc(mousefunc);
	glutMainLoop();
}

void initCustom()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glViewport(0, 0, SCREEN_X, SCREEN_Y);
	time(&last_update);
	texture = loadTextureFromBMP("watercolor2_1.bmp", 1920, 1229);
	drawSuzanne();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyfunc);
	glutMotionFunc(motionfunc);
	glutMouseFunc(mousefunc);
	glutMainLoop();
}

void displayfunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	frame++;
	glutSwapBuffers();
}

void rawSuzanne() {
	glGenVertexArrays(1, &vaoSuzanne); glBindVertexArray(vaoSuzanne);
	glGenBuffers(1, &vbo_mesh_data); glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_data);
	glBufferData(GL_ARRAY_BUFFER, suzanne_mesh_data.size() * sizeof(GLfloat), &suzanne_mesh_data[0],
		GL_STATIC_DRAW);
}

void smoothSuzanne() {
	glGenVertexArrays(1, &vaoSmoothSuzanne); glBindVertexArray(vaoSmoothSuzanne);
	glGenBuffers(1, &vbo_smooth_mesh_data); glBindBuffer(GL_ARRAY_BUFFER, vbo_smooth_mesh_data);
	glBufferData(GL_ARRAY_BUFFER, suzanne_smooth_mesh_data.size() * sizeof(GLfloat), &suzanne_smooth_mesh_data[0],
		GL_STATIC_DRAW);
}

void phong() {
	program = initShaders("passthrough2.vert", "passthrough2.frag");
}

void gouraud() {
	program = initShaders("passthrough3.vert", "passthrough3.frag");
}

void drawSuzanne() {
	// create vao with one vbo containing suzanne_mesh_data
	if (smooth == 0)
		load_obj("suzanne.obj", suzanne_mesh_data, suzanne_smooth_mesh_data);
	if (smooth == 0)
		rawSuzanne();
	else
		smoothSuzanne();
	if (vphong == 0) {
		gouraud();
	}
	else {
		phong();
	}
	GLuint attribute;
	attribute = glGetAttribLocation(program, "v_coord"); glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
	attribute = glGetAttribLocation(program, "v_texcoord"); glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	attribute = glGetAttribLocation(program, "v_normal"); glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	attribute = glGetAttribLocation(program, "lightColor"); glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glm::vec3 light = glm::vec3(1.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(program, "lightColor"), light.x, light.y, light.z);
}

void cleanup() {
	glDeleteVertexArrays(1, &vaoSuzanne);
	glDeleteBuffers(1, &vbo_mesh_data);
	glDeleteVertexArrays(1, &vaoSmoothSuzanne);
	glDeleteBuffers(1, &vbo_smooth_mesh_data);
	glDeleteProgram(program);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// initiate vertex shader:
	glDrawArrays(GL_TRIANGLES, 0, suzanne_mesh_data.size() / 9);
	glutSwapBuffers();
}

GLchar* readShaderSource(const char* shaderFile)
{
	FILE* fp;
	fopen_s(&fp, shaderFile, "r");
	GLchar* buf;
	long size;
	if (fp == NULL) return NULL;
	fseek(fp, 0L, SEEK_END);//go to end
	size = ftell(fp); //getsize
	fseek(fp, 0L, SEEK_SET);//go to beginning
	buf = (GLchar*)malloc((size + 1) * sizeof(GLchar));
	fread(buf, 1, size, fp);
	buf[size] = 0;
	fclose(fp);
	return buf;
}

// Create a GLSL program object from vertex and fragment shader files
GLuint initShaders(const char* vShaderFile, const char* fShaderFile)
{
	struct Shader{
	const char * filename;
	GLenum type;
	GLchar* source;
	} shaders[2] = {
	{ vShaderFile, GL_VERTEX_SHADER, NULL },
	{ fShaderFile, GL_FRAGMENT_SHADER, NULL }
	};
	GLuint program = glCreateProgram();
	for (int i = 0; i < 2; ++i) {
		Shader& s = shaders[i];
		s.source = readShaderSource(s.filename);
		if (shaders[i].source == NULL) {
			printf("Failed to read %s\n", s.filename);
			exit(EXIT_FAILURE);
		}
		GLuint shader = glCreateShader(s.type);
		glShaderSource(shader, 1, (const GLchar**)&s.source, NULL);
		glCompileShader(shader);
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			printf("%s failed to compile:\n", s.filename);
			GLint logSize;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
			char* logMsg = new char[logSize];
			glGetShaderInfoLog(shader, logSize, NULL, logMsg);
			printf("%s\n", logMsg);
			delete[] logMsg;
			exit(EXIT_FAILURE);
		}
		delete[] s.source;
		glAttachShader(program, shader);
	}
	/* linkand errorcheck */
	glLinkProgram(program);
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		printf("Shaderprogram failed to link:\n");
		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog(program, logSize, NULL, logMsg);
		printf("%s\n", logMsg);
		delete[] logMsg;
		exit(EXIT_FAILURE);
	}
	/* use program object*/
	glUseProgram(program);
	return program;
}

void keyfunc(unsigned char key, int x, int y) {
	switch (key) {
	case 27: case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 'a': case 'A':
		cleanup();
		smooth = 0;
		init();
		break;
	case 'z': case 'Z':
		cleanup();
		smooth = 0;
		initCustom();
		break;
	case 'e': case 'E':
		cleanup();
		smooth = 1;
		init();
		break;
	case 'r': case 'R':
		cleanup();
		smooth = 1;
		initCustom();
		break;
	case 't': case 'T':
		cleanup();
		vphong = 1;
		init();
		break;
	case 'y': case 'Y':
		cleanup();
		vphong = 0;
		init();
		break;
	case 's': case 'S':
		ka = 0;
		break;
	case 'd': case 'D':
		ka = 0.1f;
		break;
	case 'f': case 'F':
		kd = 0.0f;
		break;
	case 'g': case 'G':
		kd = 0.4f;
		break;
	case 'h': case 'H':
		ks = 0.0f;
		break;
	case 'j': case 'J':
		ks = 0.5f;
		break;
	case 'k': case 'K':
		ka = 0.3f;
		kd = 0.2f;
		break;
	case 'w': case 'W':
		lumx += 0.5f;
		break;
	case 'x': case 'X':
		lumx -= 0.5f;
		break;
	case 'c': case 'C':
		lumy += 0.5f;
		break;
	case 'v': case 'V':
		lumy -= 0.5f;
		break;
	case 'b': case 'B':
		lumz += 0.5f;
		break;
	case 'n': case 'N':
		lumz -= 0.5f;
		break;
	}
	glutPostRedisplay();
}

void updateMVP() {
	glm::mat4 Projection = glm::perspective(45.0f, (float)SCREEN_X / (float)SCREEN_Y, 1.0f, 150.0f);
	glm::mat4 View = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
	glm::mat4 Model = glm::mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);// translate, rotate, scale
	Model = glm::rotate(Model, glm::radians((float)angle), glm::vec3(0.0f, 1.0f, 0.0f));
	Model = glm::rotate(Model, glm::radians((float)angleh), glm::vec3(0.0f, 0.0f, 0.1f));
	glm::mat4 MV = View * Model;
	glm::mat4 MVP = Projection * MV;
	glm::mat3 NM = glm::transpose(glm::inverse(glm::mat3(MV)));
	glm::vec3 light = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(
		glGetUniformLocation(program, "MVP"),
		1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix3fv(
		glGetUniformLocation(program, "NM"),
		1, GL_FALSE, &NM[0][0]);
	glUniform3f(glGetUniformLocation(program, "light_position"), lumx, lumy, lumz);
	glUniform1f(glGetUniformLocation(program, "csta"), ka);
	glUniform1f(glGetUniformLocation(program, "cstd"), kd);
	glUniform1f(glGetUniformLocation(program, "csts"), ks);
}

void motionfunc(int x, int y) {

	if (pressed_button == GLUT_LEFT_BUTTON) {
		if (last_x < x) {
			angle += 1;
		}
		else {
			angle -= 1;
		}
		last_x = x;
	}


	if (pressed_button == GLUT_RIGHT_BUTTON) {
		if (last_y < y) {
			angleh += 1;
		}
		else {
			angleh -= 1;
		}
		last_y = y;
	}

	updateMVP();

	glutPostRedisplay();
}

void mousefunc(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		pressed_button = button;
	}
	else {
		pressed_button = -1;
	}

	glutPostRedisplay();
}

void load_obj(const char* filename, vector<GLfloat>& mesh_data, vector<GLfloat>& smooth_data) {

	vector<glm::vec4> v;
	vector<glm::vec2> vt;
	vector<glm::vec3> vn;
	vector<vector<GLushort>> f; // [[ , , ], [ , , ], [ , , ], ...]
	// 1) read file data into v, vt, vn
	ifstream in(filename, ios::in);
	if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }
	string line;
	vector<glm::vec3> nvm;

	while (getline(in, line)) {

		if (line.substr(0, 2) == "v ") { // vertex data
			istringstream s(line.substr(2));
			glm::vec4 v4; s >> v4.x; s >> v4.y; s >> v4.z; v4.w = 1.0f;
			v.push_back(v4);
			nvm.push_back(glm::vec3(0,0,0));
		}

		if (line.substr(0, 2) == "vt") { // vertex data
			istringstream s(line.substr(3));
			glm::vec3 v2; s >> v2.x; s >> v2.y;
			vt.push_back(v2);
		}

		if (line.substr(0, 2) == "vn") { // vertex data
			istringstream s(line.substr(3));
			glm::vec3 v3; s >> v3.x; s >> v3.y; s >> v3.z;
			vn.push_back(v3);
		}

		if (line.substr(0, 2) == "f ") { // vertex data
			istringstream s(line.substr(2));
			for (size_t i = 0; i < 3; i++)
			{
				string a;
				s >> a;
				std::vector<GLushort> glus;
				std::string token;
				std::istringstream tokenStream(a);
				while (std::getline(tokenStream, token, '/'))
				{
					//tokens.push_back(token);
					glus.push_back(std::stoi(token));
				}
				f.push_back(glus);
			}
		}
	}

	for (size_t i = 0; i < f.size(); i++)
	{
		mesh_data.push_back(v[f[i][0] - 1].x);
		mesh_data.push_back(v[f[i][0] - 1].y);
		mesh_data.push_back(v[f[i][0] - 1].z);
		mesh_data.push_back(1.0);

		// vt.u, vt.v
		mesh_data.push_back(vt[f[i][1] - 1].x);
		mesh_data.push_back(vt[f[i][1] - 1].y);

		// vn.x, vn.y, vn.z
		mesh_data.push_back(vn[f[i][2] - 1].x);
		mesh_data.push_back(vn[f[i][2] - 1].y);
		mesh_data.push_back(vn[f[i][2] - 1].z);

		nvm[f[i][0] - 1] += vn[f[i][2] - 1];
	}

	for (int i = 0; i < nvm.size(); i++) {
		nvm[i] = glm::normalize(nvm[i]);
	}

	for (size_t i = 0; i < f.size(); i++)
	{
		smooth_data.push_back(v[f[i][0] - 1].x);
		smooth_data.push_back(v[f[i][0] - 1].y);
		smooth_data.push_back(v[f[i][0] - 1].z);
		smooth_data.push_back(1.0);

		// vt.u, vt.v
		smooth_data.push_back(vt[f[i][1] - 1].x);
		smooth_data.push_back(vt[f[i][1] - 1].y);

		// vn.x, vn.y, vn.z
		smooth_data.push_back(nvm[f[i][0] - 1].x);
		smooth_data.push_back(nvm[f[i][0] - 1].y);
		smooth_data.push_back(nvm[f[i][0] - 1].z);
	}
}

GLuint createTexureChecker() {
	const int CHECKERSIZE = 256;
	float* checkerImage = new float[CHECKERSIZE * CHECKERSIZE * 4];
	int i, j; int c;
	for (i = 0; i < CHECKERSIZE; i++)
		for (j = 0; j < CHECKERSIZE; j++) {
			c = ((i & 0x8) == 0) ^ ((j & 0x8) == 0); // one square = 8 pixels
			checkerImage[4 * (i * CHECKERSIZE + j)] = (GLfloat)c;
			checkerImage[4 * (i * CHECKERSIZE + j) + 1] = (GLfloat)c;
			checkerImage[4 * (i * CHECKERSIZE + j) + 2] = (GLfloat)c;
			checkerImage[4 * (i * CHECKERSIZE + j) + 3] = (GLfloat)1.0f;
		}
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERSIZE, CHECKERSIZE, 0, GL_RGBA, GL_FLOAT, checkerImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	delete[] checkerImage;
	return texture;
}

GLuint loadTextureFromBMP(const char* filename, int width, int height) {

	GLuint texture = 0; unsigned char* data;
	FILE* file; errno_t err;
	if ((err = fopen_s(&file, filename, "rb")) != 0) printf("Error: Texture was not opened.\n");
	else {

		data = (unsigned char*)malloc(width * height * 3);
		fread(data, width * height * 3, 1, file);
		fclose(file);
		for (int i = 0; i < width * height; ++i) { // bmp files are encoded BGR and not RGB

			int index = i * 3; unsigned char B, R;
			B = data[index]; R = data[index + 2];
			data[index] = R; data[index + 2] = B;

		}
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		free(data);

	}
	return texture;

}

#include "skybox.hpp"

int sky_front[]  = { 1, -1, -1,  1, -1,  1,  1,  1,  1,  1,  1, -1};
int sky_left[] 	 = { 1, -1,  1, -1, -1,  1, -1,  1,  1,  1,  1,  1};
int sky_back[] 	 = {-1, -1,  1, -1, -1, -1, -1,  1, -1, -1,  1,  1};
int sky_right[]  = {-1, -1, -1,  1, -1, -1,  1,  1, -1, -1,  1, -1};
int sky_top[] 	 = {-1,  1, -1,  1,  1, -1,  1,  1,  1, -1,  1,  1};

GLuint skybox[6];

void load_skybox(){
	string skybox_path = "objects/skybox/skybox";

	for(int i = 0; i < 5; i++){
		string full_path = skybox_path + std::to_string(i) + ".dds";
		skybox[i] = loadDDS(full_path.c_str());
	}
}

void draw_skyface(int pos, int *d, double D){
	glBindTexture(GL_TEXTURE_2D, skybox[pos]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(d[0]*D,  d[1]*D,  d[2]*D);
		glTexCoord2f(1, 0); glVertex3f(d[3]*D,  d[4]*D,  d[5]*D);
		glTexCoord2f(1, 1); glVertex3f(d[6]*D,  d[7]*D,  d[8]*D);
		glTexCoord2f(0, 1); glVertex3f(d[9]*D, d[10]*D, d[11]*D);
	glEnd();
}

void draw_skybox(double D){
	glUseProgram(0);
	glPushMatrix();
     
	glPushAttrib(GL_ENABLE_BIT);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	draw_skyface(0, sky_front,  D);
	draw_skyface(1, sky_left,   D);
	draw_skyface(2, sky_back,   D);
	draw_skyface(3, sky_right,  D);
	draw_skyface(4, sky_top,    D);

	glPopAttrib();
	glPopMatrix();
}

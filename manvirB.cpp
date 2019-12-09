//Author: Manvir Bal
//created on: 9/17/2019
//Four steps of debugging are:
//identify problem
//isolate problem
//fix problem
//review problem
#include "fonts.h"
#include <GL/glx.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include "BlenderObj.h"
#include "Image.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <fstream>
#include "GlobalSpace.h"
#include "MainCharacter.h"
using gvars::gl;
using gvars::arrow_keys;
using gvars::tp;
using gvars::mask;
#define PORT 443
#define USERAGENT "CMPS-3350"

struct Vec3
{
	float x, y, z;
	Vec3(){ z = (0), y = (0), x = (0);}
	Vec3(float x1, float y1, float z1) : x(x1), y(y1), z(z1) {}
	void setPoints(float x1, float y1, float z1)
	{
		x = x1;
		y = y1;
		z = z1;
	}
};
const int MAX_PARTICLES = 10000;
Vec3 particles[MAX_PARTICLES];

void creditManvir(Rect r)
{
    ggprint8b(&r, 16, 0x00004C00, "Manvir Bal");
}
int colorArray[100];
void randomColor(){
	for(int i = 0; i < 100; i++){
		colorArray[i] = rand()%255;
	}
}
void makeParticles(int w, int h)
{
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		particles[i].x = ((float)(rand() % w)) - (w / 2);
		particles[i].y = ((float)(rand() % h)) - (h / 2);
		particles[i].z = ((float)rand() / RAND_MAX) * -100;
	}
}

float t = 0.05f;
float inc = 0.00005;
double xp = t; 
double yp = t;
int angle = 5;
float z = 0.005;
void chaos_equations()
{
	//draw dots
	glTranslatef(0,0,z);
	glPushMatrix();
		glPointSize(3);
		
		glBegin(GL_POINTS);
			for(int i = 0; i < MAX_PARTICLES; i++){
				glVertex3f(particles[i].x, particles[i].y, particles[i].z);
			}
		glEnd();
	glPopMatrix();

	glPushMatrix();
		//glRotatef(angle, 0,0,1);
		glScalef(1000,500,1);
		glTranslatef(0,.18,-(z+1));
		glEnable(GL_POINT_SMOOTH);
		xp = t;
		yp = t;
		double x = xp; 
		double y = yp;

		for (int i =  0; i < 1000;i++){
			xp = x*x - x*t + y + t;
			yp = x*x + y*y + t*t - x*t - x + y;
			x = xp;
			y = yp;
			glPointSize(5);
			glColor3ub(colorArray[i%100], colorArray[(i+1)%100], colorArray[(i+2)%100]);
			glBegin(GL_POINTS);
				glVertex3d(xp,yp,-1.1);
			glEnd();

		}
	if(t >= 0.05f){
		inc = -inc;
	}else if(t <= -0.05f){
		inc = 0.00005;
	}
	t += inc;
	angle = (angle + 1)%360;

	glPopMatrix();
	if (z > 50)
		z = 0;
	z += 0.005;	
}

TypeObj::TypeObj(int idx, int x, int y, int z,int tx, 
		int ty,int tz, int mID)
{
	id = idx;
	matID = mID;
	size = 3;
	cord = new int[size];
	textureCoord = new int[size];
	cord[0] = x;
	cord[1] = y;
	cord[2] = z;
	textureCoord[0] = tx;
	textureCoord[1] = ty;
	textureCoord[2] = tz;
}
TypeObj::TypeObj(int idx, int x, int y, int x1, int y1,
		int t0, int t1,int t2, int t3, int mID)
{
	id = idx;
	matID = mID;
	size = 4;
	cord = new int[size];
	textureCoord = new int[size];
	cord[0] = x;
	cord[1] = y;
	cord[2] = x1;
	cord[3] = y1;
	textureCoord[0] = t0;
	textureCoord[1] = t1;
	textureCoord[2] = t2;
	textureCoord[3] = t3;
}

Material::Material(std::string name, std::vector<float> & s, 
	std::vector<float> &diff, std::vector<float> &a, 
	float specNS, float NI, float disolve, int light,
	int texID)
{
    matname = name;
    spec = s;
    diffuse = diff;
    amb = a;
    ns = specNS;
    ni = NI;
    d = disolve;
    illum = light;
    textureID = texID;
}


void Blender::readObj(const std::string fn)
{

    std::ifstream fin(fn.c_str());

    if(!fin.is_open()){
        std::cout << "file couldn't be opened" << std::endl;
        return;
    }

    std::string line;
    int usingmat = 0;

    while(getline(fin, line)){
        if(line[0] == '#')
            continue;
        //handle geometric vertices
        if(line[0] == 'v' && line[1] == ' '){
            float i, j, k;
            sscanf(line.c_str(), "v %f %f %f", &i, &j, &k);
            v.push_back(new Vect(i, j, k));
        } //handle vertex textures
        else if(line[0] == 'v' && line[1] == 't'){
            float i, j;
            sscanf(line.c_str(), "vt %f %f", &i, &j);
            vt.push_back(new Vect(i,1-j,0.0f));
        }//handle vectex normals
        else if(line[0] == 'v' && line[1] == 'n'){
            float i, j, k;
            sscanf(line.c_str(), "vn %f %f %f", &i, &j, &k);
            vn.push_back(new Vect(i, j, k));
        }//handle faces
        else if(line[0] == 'f'){
            //determine if the obj is created with triangles
            //or quads
            if(std::count(line.begin(), line.end(), ' ')==4){
                unsigned int vertId[4], faceId[4], normId[4];
                sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                        &vertId[0],&faceId[0],&normId[0], 
                        &vertId[1],&faceId[1],&normId[1], 
                        &vertId[2],&faceId[2],&normId[2],
                        &vertId[3],&faceId[3],&normId[3]);                

                faces.push_back(new TypeObj(normId[0], vertId[0], 
                            vertId[1], vertId[2], vertId[3],
                            faceId[0], faceId[1], faceId[2],
                            faceId[3], usingmat));
            }
            if(std::count(line.begin(), line.end(), ' ')==3){
                unsigned int vertId[3], faceId[3], normId[3];
                sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
                        &vertId[0],&faceId[0],&normId[0], 
                        &vertId[1],&faceId[1],&normId[1], 
                        &vertId[2],&faceId[2],&normId[2]);                

                faces.push_back(new TypeObj(normId[0], vertId[0], 
                            vertId[1], vertId[2],faceId[0], 
                            faceId[1], faceId[2],usingmat));
            }

        }//handle materials
        else if(line[0] == 'u' && line[1] == 's'){
            char fname[50];
            sscanf(line.c_str(), "usemtl %s",fname);
            for(unsigned int i = 0; i < mats.size();i++){
                if(strcmp(fname, mats[i]->matname.c_str()) == 0)
                    usingmat = i;
            }
        }
        else if(line[0] == 'm' && line[1] == 't'){
            char fname[50];
            sscanf(line.c_str(), "mtllib %s",fname);
            readMaterial(fname);
        }



    }

}
void Blender::readMaterial(const std::string fname)
{
    std::ifstream fin("./images/"+fname);
    if(!fin.is_open()){
        std::cout << "could not open file:"<< "./images/"+fname << std::endl;
        exit(0);
    }
    std::string line;
    bool foundmat = false;
    std::vector<float> specvar(3);
    std::vector<float> ambient(3);
    std::vector<float> diff(3);
    float ni;
    float ns;
    float d;
    int illumn;
    char matname[50];
    int textureID = -1;

    while(getline(fin, line)){
        if(line[0] == '#')
            continue;
        //if you see a new material add the other in the mat vec
        if(line[0] == 'n' && line[1] == 'e'){
            if(foundmat){
                mats.push_back(new Material(matname,specvar,diff, 
                            ambient, ns, ni, d, illumn, textureID));
                textureID = -1;
            }
            sscanf(line.c_str(), "newmtl %s", matname);
        }else if(line[0] == 'N' && line[1] == 's'){
            
            sscanf(line.c_str(), "Ns %f", &ns);
            foundmat = true;
        }else if(line[0] == 'K' && line[1] == 'a'){
            sscanf(line.c_str(), "Ka %f %f %f",&ambient[0], 
                    &ambient[1], &ambient[2]);
            foundmat = true;
        }else if(line[0] == 'K' && line[1] == 'd'){
            sscanf(line.c_str(), "Kd %f %f %f",&diff[0], 
                    &diff[1], &diff[2]);
            foundmat = true;
        }else if(line[0] == 'K' && line[1] == 's'){
            sscanf(line.c_str(), "Ks %f %f %f",&specvar[0], 
                    &specvar[1], &specvar[2]);
            foundmat = true;
        }else if(line[0] == 'N' && line[1] == 'i'){
            sscanf(line.c_str(), "Ni %f", &ni);
            foundmat = true;
        }else if(line[0] == 'd'){
            sscanf(line.c_str(), "d %f", &d);
            foundmat = true;
        }else if(line[0] == 'i' && line[1] == 'l'){
            sscanf(line.c_str(), "illum %d", &illumn);
            foundmat = true;
        }else if(line[0] == 'm' && line[1] == 'a' &&
                line[4] == 'K' && line[5] == 'd'){
            char texturefilename[50];
            sscanf(line.c_str(), "map_Kd %s",texturefilename);
            textureID = loadTexture(texturefilename);
            
        }

    }
    if(foundmat){
        mats.push_back(new Material(matname,specvar,diff, 
                    ambient, ns, ni, d, illumn, textureID));
    }
}
void Blender::renderObj(float x, float y, float z)
{
    int prevMat = -1;
    int currmat = 0;
    glPushMatrix();
    //glTranslatef(x, y, z);
    //glTranslatef(0, 0,-600);
	glScalef(500,100,500);
    
    glRotatef(x,1.0,0.0,0.0);
    glRotatef(y,0.0,1.0,0.0);
    glRotatef(-45,0.0,1.0,0.0);
    glRotatef(z,0.0,0.0,1.0);

    bool checkTexture;
    for(unsigned int i = 0; i < faces.size(); i++){

        //glColor3f(1.0f, 0.0f, 0.0f);
        currmat = faces[i]->matID;
        checkTexture = mats[currmat]->textureID != -1;
        if(mats.size() && prevMat != currmat){
            float spec[] ={mats[currmat]->spec[0], 
                            mats[currmat]->spec[1],
                            mats[currmat]->spec[2], 1.0};
            float amb[] = {mats[currmat]->amb[0],
                            mats[currmat]->amb[1],
                            mats[currmat]->amb[2], 1.0};
            float diff[] = {mats[currmat]->diffuse[0], 
                            mats[currmat]->diffuse[1], 
                            mats[currmat]->diffuse[2],1.0};
            glMaterialfv(GL_FRONT, GL_SPECULAR,spec);
            glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
            glMaterialfv(GL_FRONT, GL_SHININESS, &mats[currmat]->ns);
            if(checkTexture){
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, mats[currmat]->textureID);
            }else{
                glDisable(GL_TEXTURE_2D);
            }
            prevMat = currmat;
        }

        if(faces[i]->size == 4){
            glBegin(GL_QUADS);
            glNormal3f(vn[faces[i]->id-1]->x, vn[faces[i]->id-1]->y
                    , vn[faces[i]->id-1]->z);
            if(checkTexture){
                glTexCoord2f(vt[faces[i]->textureCoord[0]-1]->x, vt[faces[i]->textureCoord[0]-1]->y);
            }
            glVertex3f(v[faces[i]->cord[0]-1]->x, 
                    v[faces[i]->cord[0]-1]->y,
                    v[faces[i]->cord[0]-1]->z);
            if(checkTexture){
                glTexCoord2f(vt[faces[i]->textureCoord[1]-1]->x, vt[faces[i]->textureCoord[1]-1]->y);
            }
            glVertex3f(v[faces[i]->cord[1]-1]->x,
                    v[faces[i]->cord[1]-1]->y,
                    v[faces[i]->cord[1]-1]->z);
            if(checkTexture){
                glTexCoord2f(vt[faces[i]->textureCoord[2]-1]->x, vt[faces[i]->textureCoord[2]-1]->y);
            }
            glVertex3f(v[faces[i]->cord[2]-1]->x,
                    v[faces[i]->cord[2]-1]->y,
                    v[faces[i]->cord[2]-1]->z);
            if(checkTexture){
                glTexCoord2f(vt[faces[i]->textureCoord[3]-1]->x, vt[faces[i]->textureCoord[3]-1]->y);
            }
            glVertex3f(v[faces[i]->cord[3]-1]->x,
                    v[faces[i]->cord[3]-1]->y,
                    v[faces[i]->cord[3]-1]->z);
            glEnd();
        }else{
            glBegin(GL_TRIANGLES);
            glNormal3f(vn[faces[i]->id-1]->x, vn[faces[i]->id-1]->y
                    , vn[faces[i]->id-1]->z);
            if(checkTexture){
                glTexCoord2f(vt[faces[i]->textureCoord[0]-1]->x, vt[faces[i]->textureCoord[0]-1]->y);
            }
            glVertex3f(v[faces[i]->cord[0]-1]->x, 
                    v[faces[i]->cord[0]-1]->y,
                    v[faces[i]->cord[0]-1]->z);
            if(checkTexture){
                glTexCoord2f(vt[faces[i]->textureCoord[1]-1]->x, vt[faces[i]->textureCoord[1]-1]->y);
            }
            glVertex3f(v[faces[i]->cord[1]-1]->x,
                    v[faces[i]->cord[1]-1]->y,
                    v[faces[i]->cord[1]-1]->z);
            if(checkTexture){
                glTexCoord2f(vt[faces[i]->textureCoord[2]-1]->x, vt[faces[i]->textureCoord[2]-1]->y);
            }
            glVertex3f(v[faces[i]->cord[2]-1]->x,
                    v[faces[i]->cord[2]-1]->y,
                    v[faces[i]->cord[2]-1]->z);
            glEnd(); 
        }

    }
    glPopMatrix();


}
int Blender::loadTexture(std::string fname)
{
    fname = "./images/" + fname;
    Image img(fname.c_str());
    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, 
            GL_RGB,GL_UNSIGNED_BYTE, img.data);
    return id;
}
Blender::Blender()
{
    
}
/***************************
 * Main character class for rendering 
 * and controlling the hero of this game.
 * 
 * *************************/

MainCharacter::MainCharacter()
{
    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;
    face = 0;
    dir = Direction::S;
    trooper = new Texture[8];

    trooper[0].set("images/trooper_sprite/trooper_s.png");
    trooper[1].set("images/trooper_sprite/trooper_sw.png");
    trooper[2].set("images/trooper_sprite/trooper_w.png");
    trooper[3].set("images/trooper_sprite/trooper_nw2.png");
    trooper[4].set("images/trooper_sprite/trooper_n.png");
    trooper[5].set("images/trooper_sprite/trooper_ne2.png");
    trooper[6].set("images/trooper_sprite/trooper_e.png");
    trooper[7].set("images/trooper_sprite/trooper_se.png");

}
MainCharacter::~MainCharacter(){
    delete [] trooper;
}

void MainCharacter::calFace()
{
    int div = 4;
    int ns =   16* 1/div;
    int we =   32* 1/div;
    int dns =  8*  1/div;
    int dwe =  16* 1/div;

    int sum = arrow_keys[0]+arrow_keys[1]+arrow_keys[2]+arrow_keys[3];
    if (sum == 1) {
        if(arrow_keys[0]){
            dir = Direction::N;
			if(tp.Walk(-1.0/div, -1.0/div, tile, face))
            	pos[2] += ns;
        }else if(arrow_keys[1]){
			
            dir = Direction::S;
			if(tp.Walk(1.0/div, 1.0/div, tile, face))
            	pos[2] -= ns;
		

        }else if(arrow_keys[2]){
            dir = Direction::W;
			if(tp.Walk(1.0/div, -1.0/div, tile, face))
            	pos[0] -= we; 

        }else {
            dir = Direction::E;
			if(tp.Walk(-1.0/div, 1.0/div, tile, face))
            	pos[0] += we; 

        }
        return;
    }
    if (sum == 2) {
        if(arrow_keys[0]){
            if(arrow_keys[2]){
				// if(dir == Direction::N){
				// 	pos[2] -= ns;
                //     tp.Walk(1.0/div, 1.0/div);
				// }else if(dir == Direction::W){
				// 	pos[0] += we;
                //     tp.Walk(-1, 1);
				// }
                dir = Direction::NW;
                if(tp.Walk(0, -1.0/div, tile, face)){
                    pos[0] -= dwe; 
                    pos[2] += dns;
                }

            }else if(arrow_keys[3]){
				// if(dir == Direction::N){
				// 	pos[2] -= ns;
                //     tp.Walk(1.0/div, 1.0/div);
				// }else if(dir == Direction::E){
				// 	pos[0] -= we;
                //     tp.Walk(1.0/div, -1.0/div);
				// }
                dir = Direction::NE;
                if(tp.Walk(-1.0/div, 0, tile, face)){
                    pos[0] += dwe; 
                    pos[2] += dns;			
                }

            }
        }else if(arrow_keys[1]){
            if(arrow_keys[2]){
				// if(dir == Direction::S){
				// 	pos[2] += ns;
                //     tp.Walk(-1.0/div, -1.0/div);
				// }else if(dir == Direction::W){
				// 	pos[0] += we;
                //     tp.Walk(-1.0/div, 1/div);
				// }
                dir = Direction::SW;
                if(tp.Walk(1.0/div, 0, tile, face)){
                    pos[0] -= dwe; 
                    pos[2] -= dns;
                }
			

            } else if(arrow_keys[3]) {
				// if(dir == Direction::S){
				// 	pos[2] += ns;
                //     tp.Walk(-1.0/div, -1.0/div);
				// }else if(dir == Direction::E){
				// 	pos[0] -= we;
                //     tp.Walk(1.0/div, -1.0/div);
				// }
                dir = Direction::SE;
                if (tp.Walk(0, 1.0/div, tile, face)) {
                    pos[0] += dwe; 
                    pos[2] -= dns;
                }

            }
        }
    }

}
void MainCharacter::characterRender()
{
    int width = 8*1;
	int height = 16*1;
    glPushMatrix();
	glRotatef(0, 0.0, 1.0, 0.0);
	glTranslatef(0, 100, 0);
    glBindTexture(GL_TEXTURE_2D, trooper[(int)dir].getID());
    glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3i(-width+pos[0],height-80+pos[2], 0); 
        glTexCoord2f(0, 1);
        glVertex3i(-width+pos[0],-height-80+pos[2], 0); 
        glTexCoord2f(1, 1);
        glVertex3i(width+pos[0], -height-80+pos[2], 0);      
        glTexCoord2f(1,0);
        glVertex3i(width+pos[0],height-80+pos[2], 0);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
	//std::cout << "x:" << pos[0] << " and this is y:" << pos[2] << std::endl;
}

void MainCharacter::setFace(int f)
{
    face = f;
}

/**************************************
 * communicate with the server
 * ***********************************/
void readScores(char *pagename) 
{
    std::fstream fout;
    BIO *ssl_setup_bio(void);
    void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname);
    void set_to_non_blocking(const int sock);
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;
    BIO *outbio = NULL;
    //--------------------
    //SSLv23_client_method() returns a const SSL_METHOD
    //--------------------
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl;
    char req[1000];
    int req_len;
    int port = PORT;
    int bytes, nreads, nerrs;
    char buf[1024];
    int ret;
    const int MAX_READ_ERRORS = 100;
    char hostname[256] = "www.cs.csub.edu";
    //Get any command-line arguments.
    // if (argc > 1) {
    //     strcpy(hostname, argv[1]);
    // }
    // if (argc > 2){
    //     strcpy(pagename, argv[2]);
    // } 
    //Setup the SSL BIO
    outbio = ssl_setup_bio();
    //Initialize the SSL library
    if (SSL_library_init() < 0) {
        BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
    }
    //--------------------
    //SSLv23_client_method() returns a const SSL_METHOD
    //---------------------
    method = SSLv23_client_method();
    ctx = SSL_CTX_new(method);
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);
    //next 2 lines of code are not currently needed.
    //SSL_MODE_AUTO_RETRY flag of the SSL_CTX_set_mode call.
    //SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);
    //
    //Setup the socket used for connection.
    host = gethostbyname(hostname);
    sd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        BIO_printf(outbio,"%s:Cannot connect to host %s [%s] on port %d.\n",
                "file", hostname, inet_ntoa(addr.sin_addr), port);
    }
    //Connect using the SSL certificate.
    ssl = SSL_new(ctx); 
    SSL_set_fd(ssl, sd);
    SSL_connect(ssl);
    //
    //Show the certificate data
    //show_cert_data(ssl, outbio, hostname);
    //
    //A non-blocking socket will make the ssl_read() not block.
    set_to_non_blocking(sd);
    //
    //Send the http request to the host server.
    sprintf(req, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n",
            pagename, hostname, USERAGENT);    
    req_len = strlen(req);
    ret = SSL_write(ssl, req, req_len);

    //----------------
    //needed brackets bc there are two statements 
    //---------------
    if (ret <= 0) {
        fprintf(stderr, "ERROR: SSL_write\n"); fflush(stderr);
    }
    //lets make open the file to write to if its doesn't
    //exist creat it 
    fout.open("scores.txt",std::fstream::out);
    
    //Get data returned from the server.
    //First, do priming read.
    //We can take this approach because our socket is non-blocking.
    //Start with an error condition.
    bytes = -1;
    memset(buf, '\0', sizeof(buf));
    while (bytes <= 0) {
        bytes = SSL_read(ssl, buf, sizeof(buf));
        //write(STDOUT_FILENO, buf, bytes);
        fout << buf;
        //A slight pause can cause fewer reads to be needed.
        usleep(10000);
    }
    //A successful priming read was accomplished.
    //Now read all the data.
    nreads = 1;
    //Allow for some read errors to happen, while getting the complete data.
    nerrs = 0;

    while (bytes >= 0 && nerrs < MAX_READ_ERRORS) {
        //write(STDOUT_FILENO, buf, bytes);
        memset(buf, '\0', sizeof(buf));
        ++nreads;
        bytes = SSL_read(ssl, buf, sizeof(buf));
        fout << buf;
        if (bytes == 0) ++nerrs; else nerrs = 0;
        //A slight pause can cause fewer reads to be needed.
        usleep(20000);
    }
    //Cleanup.
    SSL_free(ssl);
    close(sd);
    SSL_CTX_free(ctx);
    // return 0;
}
void postScores(char *pagename) 
{

    BIO *ssl_setup_bio(void);
    void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname);
    void set_to_non_blocking(const int sock);
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;
    BIO *outbio = NULL;
    //--------------------
    //SSLv23_client_method() returns a const SSL_METHOD
    //--------------------
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl;
    char req[5000];
    int req_len;
    int port = PORT;
    int bytes, nreads, nerrs;
    char buf[256];
    int ret;
    const int MAX_READ_ERRORS = 100;
    char hostname[256] = "www.cs.csub.edu";

    //Setup the SSL BIO
    outbio = ssl_setup_bio();
    //Initialize the SSL library
    if (SSL_library_init() < 0) {
        BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
    }
    //--------------------
    //SSLv23_client_method() returns a const SSL_METHOD
    //---------------------
    method = SSLv23_client_method();
    ctx = SSL_CTX_new(method);
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

    host = gethostbyname(hostname);
    sd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        BIO_printf(outbio,"%s:Cannot connect to host %s [%s] on port %d.\n",
                "file", hostname, inet_ntoa(addr.sin_addr), port);
    }
    //Connect using the SSL certificate.
    ssl = SSL_new(ctx); 
    SSL_set_fd(ssl, sd);
    SSL_connect(ssl);
    //
    //Show the certificate data
    //show_cert_data(ssl, outbio, hostname);
    //
    //A non-blocking socket will make the ssl_read() not block.
    set_to_non_blocking(sd);
    //
    //Send the http request to the host server.
    sprintf(req, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n",
            pagename, hostname, USERAGENT);   
    req_len = strlen(req);
    ret = SSL_write(ssl, req, req_len);
    //----------------
    //needed brackets bc there are two statements 
    //---------------
    if (ret <= 0) {
        fprintf(stderr, "ERROR: SSL_write\n"); fflush(stderr);
    }
    //
    //Get data returned from the server.

    bytes = -1;
    memset(buf, '\0', sizeof(buf));
    while (bytes <= 0) {
        bytes = SSL_read(ssl, buf, sizeof(buf));
        usleep(10000);
    }

    nreads = 1;
    //Allow for some read errors to happen, while getting the complete data.
    nerrs = 0;
    while (bytes >= 0 && nerrs < MAX_READ_ERRORS) {
        //write(STDOUT_FILENO, buf, bytes);
        memset(buf, '\0', sizeof(buf));
        ++nreads;
        bytes = SSL_read(ssl, buf, sizeof(buf));
        
        if (bytes == 0) ++nerrs; else nerrs = 0;
        //A slight pause can cause fewer reads to be needed.
        usleep(20000);
    }
    
    //Cleanup.
    SSL_free(ssl);
    close(sd);
    SSL_CTX_free(ctx);
    // return 0;
}


BIO *ssl_setup_bio(void)
{
    //Setup the ssl BIO, basic I/O abstraction.
    BIO *bio = NULL;
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    SSL_load_error_strings();
    bio = BIO_new(BIO_s_file());
    bio = BIO_new_fp(stdout, BIO_NOCLOSE);
    return bio;
}

void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname) 
{
    //Display ssl certificate data here.
    //Get the remote certificate into the X509 structure
    printf("--------------------------------------------------------------\n");
    printf("Certificate data...\n");
    X509 *cert;
    X509_NAME *certname;
    printf("calling SSL_get_peer_certificate(ssl)\n");
    cert = SSL_get_peer_certificate(ssl);
    if (cert == NULL) {
        printf("Error: Could not get a certificate from: %s.\n", hostname);
    }else{
        printf("Retrieved the server's certificate from: %s.\n", hostname);
    }
    //extract various certificate information
    certname = X509_NAME_new();
    certname = X509_get_subject_name(cert);
    //display the cert subject here
    if(BIO_printf(outbio,"Displaying the certificate subject data:\n")<0){
        fprintf(stderr, "ERROR: BIO_printf\n");
    }
    X509_NAME_print_ex(outbio, certname, 0, 0);
    if (BIO_printf(outbio, "\n\n") < 0){
        fprintf(stderr, "ERROR: BIO_printf\n");
    }
    printf("-----------------------------------------------------------\n");
}

void set_to_non_blocking(const int sock)
{
    //Set a socket to be non-blocking.
    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0) {
        perror("ERROR: fcntl(F_GETFL)");
        exit(EXIT_FAILURE);
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(sock, F_SETFL, opts) < 0) {
        perror("ERROR: fcntl(O_NONBLOCK)");
        exit(EXIT_FAILURE);
    }
}
//This is my Friday code.
//this function will grab turn the scene into a 2.5D isometric scene.
void isometricScene()
{
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glOrtho(-gl.xres/2,gl.xres/2,-gl.yres/2,gl.yres/2, -gl.xres*50, gl.yres*50);
	//glOrtho(-100, 100, -100, 100, -1000, 1000);
	glMatrixMode(GL_MODELVIEW);glLoadIdentity();
	//rotate the x-axis by 30 degrees
	glRotatef(35.264f, 1.0f, 0.0f, 0.0f);
	//rotate the y-axis by 45 degres
	//glRotatef(-60.0f, 0.0f, 1.0f, 0.0f);
	//glScalef(1.0f,1.0f,-1.0f);
}
void orthoScene()
{
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);glLoadIdentity();
    glOrtho(-gl.xres/2,gl.xres/2,-gl.yres/2,gl.yres/2, -1,1);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);
}

int keysym_to_arrow_key(KeySym keysym) 
{
    switch (keysym) {
        case XK_Up:
            return 0;
        case XK_Down:
            return 1;
        case XK_Left:
            return 2;
        case XK_Right:
            return 3;
    }
    return -1;
}
void arrowInputMap(XEvent *e )
{
    
    switch(e->type) {
        case KeyPress: {
            KeySym keysym = XLookupKeysym(&e->xkey, 0);
            int	arrow_key = keysym_to_arrow_key(keysym);

            if(arrow_key != -1)
                arrow_keys[arrow_key] = 1;
			if(gvars::state == gvars::GameState::game){

				gvars::hero.calFace();
			}
        } break;
        case KeyRelease: {
            KeySym keysym = XLookupKeysym(&e->xkey, 0);
            int	arrow_key = keysym_to_arrow_key(keysym);

            if(arrow_key != -1)
                arrow_keys[arrow_key] = 0;
        } break;
    }




}

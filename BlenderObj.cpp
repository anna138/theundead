#include "BlenderObj.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <GL/glx.h>
#include "Image.h"


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


void Blender::readObj(const std::string fn){

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
void Blender::readMaterial(const std::string fname){
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
void Blender::renderObj(float x, float y, float z){
    int prevMat = -1;
    int currmat = 0;
    glPushMatrix();
    //glTranslatef(x, y, z);
    glTranslatef(0,0,-4);
    
    glRotatef(x,1.0,0.0,0.0);
    glRotatef(y,0.0,1.0,0.0);
    glRotatef(z,0.0,0.0,1.0);
    //glRotatef(x,1.0,0,0);
    //glRotatef(y,0.0,1.0,0);
   // glRotatef(z,0.0,0,1.0);
    //std::cout << mats[0]->matname << mats[1]->matname << std::endl; 
    bool checkTexture;
    for(unsigned int i = 0; i < faces.size(); i++){

        glColor3f(1.0f, 0.0f, 0.0f);
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
    if(ang > 360)
        ang = -360;
    
    ang += 0.1;

}
int Blender::loadTexture(std::string fname){
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
Blender::Blender(){
    ang = 1.0;
}

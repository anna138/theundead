#include "BlenderObj.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <GL/glx.h>

void Blender::readObj(const std::string fn, float x, float y, float z){
    
    std::ifstream fin(fn.c_str());

    if(!fin.is_open()){
        std::cout << "file couldn't be opened" << std::endl;
        return;
    }

    std::string line;
    std::vector<Vect*> gvert;
    std::vector<Vect*> norms;
    std::vector<TypeObj*> faces;

    while(getline(fin, line)){
        if(line[0] == '#')
            continue;
        //handle geometric vertices
        if(line[0] == 'v' && line[1] == ' '){
            float i, j, k;
            sscanf(line.c_str(), "v %f %f %f", &i, &j, &k);
            gvert.push_back(new Vect(i, j, k));
        }//handle vectex normals
        if(line[0] == 'v' && line[1] == 'n'){
            float i, j, k;
            sscanf(line.c_str(), "vn %f %f %f", &i, &j, &k);
            norms.push_back(new Vect(i, j, k));
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
                            vertId[1], vertId[2], vertId[3]));
            }

        }
    }

        glPushMatrix();
        //glTranslatef(x, y, z);
        glTranslatef(0,0,-5);
        glRotatef(x,1.0,0.0,0.0);
        glRotatef(y,0.0,1.0,0.0);
        glRotatef(z,0.0,0.0,1.0);

        for(unsigned int i = 0; i < faces.size(); i++){
            
            glBegin(GL_QUADS);
                //glNormal3f(norms[faces[i]->id-1]->x, norms[faces[i]->id-1]->y, norms[faces[i]->id-1]->z);
                glVertex3f(gvert[faces[i]->cord[0]-1]->x, gvert[faces[i]->cord[0]-1]->y*1, gvert[faces[i]->cord[0]-1]->z);
                glVertex3f(gvert[faces[i]->cord[1]-1]->x, gvert[faces[i]->cord[1]-1]->y*1, gvert[faces[i]->cord[1]-1]->z);
                glVertex3f(gvert[faces[i]->cord[2]-1]->x, gvert[faces[i]->cord[2]-1]->y*1, gvert[faces[i]->cord[2]-1]->z);
                glVertex3f(gvert[faces[i]->cord[3]-1]->x, gvert[faces[i]->cord[3]-1]->y*1, gvert[faces[i]->cord[3]-1]->z);
            glEnd();
        
        }
        glPopMatrix();
        if(ang > 360)
            ang = -360;
        
        ang += 1.0;

}
Blender::Blender(){
    ang = 1.0;
}

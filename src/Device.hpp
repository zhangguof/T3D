
#ifndef _H_DEVICE_
#define _H_DEVICE_
#include "T3D.hpp"


const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;

// const int win_width = WIN_WIDTH;
// const int win_height = WIN_HEIGHT;

extern const char* VERTEX_SHADER_FILE;
extern const char* FRAG_SHADER_FILE;




class UpdateObj
{
public:
    virtual void update(double value)=0;
};

class SHADER
{
    GLuint shader;
    const char* vet_file_path;
    const char* frag_file_path;
public:
    SHADER(const char* vet_file = VERTEX_SHADER_FILE, 
           const char* frag_file = FRAG_SHADER_FILE);
    void read_shader_file(const char *fname,char *buf);
    void init_shader();
    void use();

};

class VAOMGR
{
    //OPENGL Device
    GLuint vbo;
    GLuint vao;
    int num;
public:
    VAOMGR(int n);
    void gen_vao_vbo(GLfloat vertices[],size_t size);

    void bind();
    void ubind();
    void draw();
    ~VAOMGR();

};

class Device: public UpdateObj
{
    int win_width;
    int win_height;
    const static int buff_num = 2;
    //DrawBuffer<GLuint> buff[buff_num];
    FrameBuffer* cur_buf;

    //opengl
    VAOMGR *vao;
    SHADER shader;

    GLuint texture;
public:
    Device(int w=WIN_WIDTH, int h=WIN_HEIGHT);
    void set_buf(FrameBuffer* buf);
    void update_texture();

    void init_render();
    void update(double interval);
    void render(double interval);

    void release();
};


typedef void (*UpdateFunc)(double interval);
typedef void (UpdateObj::*ObjUpdateFunc)(double interval);
//typedef std::pair<UpdateObj*,ObjUpdateFunc> UpdatePair;

class CBMgr
{
    std::vector<UpdateFunc> v;
    //std::vector<UpdatePair> v2;
    std::vector<UpdateObj*> v2;

public:
    void add_callback(UpdateFunc f)
    {
        v.push_back(f);
    }
    void add_callback2(UpdateObj* p)
    {
        v2.push_back(p);
    }
    void update(double interval)
    {
        for (auto i = v.begin(); i != v.end(); ++i)
        {
            (*i)(interval);
        }

        for (auto i = v2.begin(); i != v2.end(); ++i)
        {
            // auto ptr_obj = i->first;
            // auto f = i->second;
            // (ptr_obj->*f)(interval);
            (*i)->update(interval);
        }

    }
};


GLFWwindow* init_glfw_window(int w,int h);
void glfw_loop();
void regist_update(UpdateFunc f);
void regist_objupdate(UpdateObj* p);

double time_now();


#endif

//
//  triangle.cpp
//  firstTriangle
//
//  Created by tony on 16/3/21.
//  Copyright © 2016年 tony. All rights reserved.
//

#include "T3D.hpp"
const char* VERTEX_SHADER_FILE = "shader/vertextShader.glsl";
const char* FRAG_SHADER_FILE = "shader/fragmentShader.glsl";


SHADER::SHADER(const char* vet_file , 
               const char* frag_file )
{
    vet_file_path = vet_file;
    frag_file_path = frag_file;
    //init_shader();
}
void SHADER::read_shader_file(const char *fname,char *buf)
{
    int fp = open(fname,O_RDONLY);
    int size = read(fp, buf, 2048);
    buf[size] = '\0';
}
void SHADER::init_shader()
{
    char shader_src_buf[2048];
    const char *buf = shader_src_buf;
    
    //vertex shader
    //"shader/vertextShader.glsl"
    read_shader_file(vet_file_path,shader_src_buf);
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&buf,NULL);
    glCompileShader(vertexShader);
    
    GLint success;
    GLchar infolog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader,512,NULL,infolog);
        std::cout<< "ERROR:SHADER::VERTEX::COMPILATION_FAILD\n"<<infolog<<std::endl;
        return;
        
    }
    //fragment shader
    //read_shader_file("shader/fragmentShader.glsl", shader_src_buf);
    read_shader_file(frag_file_path, shader_src_buf);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,(const char* const*) &buf,NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader,512,NULL,infolog);
        std::cout<< "ERROR:SHADER::FRAGMENT::COMPILATION_FAILD\n"<<infolog<<std::endl;
        return;
        
    }
    
    
    GLuint shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(shaderProgram,512,NULL,infolog);
        std::cout<< "ERROR:SHADER::PROGRAM::LINK_FAILD\n"<<infolog<<std::endl;
        return;
    }
    
    
    //glUseProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    shader = shaderProgram;
}
void SHADER::use()
{
    glUseProgram(shader);
}


VAOMGR::VAOMGR(int n)
{
    num = n;
}
void VAOMGR::gen_vao_vbo(GLfloat vertices[],size_t size)
{
    glGenVertexArrays(1,&vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,size, vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat),(GLvoid *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat),
                          (GLvoid *)(3*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}
void VAOMGR::bind()
{
    glBindVertexArray(vao);
}
void VAOMGR::ubind()
{
    glBindVertexArray(0);
}
void VAOMGR::draw()
{
    
    glDrawArrays(GL_TRIANGLES, 0, num);
}
VAOMGR::~VAOMGR()
{
    glDeleteVertexArrays(1,&vao);
    glDeleteBuffers(1,&vbo);
}

static GLfloat vertices[] = {
        // 第一个三角形       贴图坐标
        -1.0f,  1.0f, 0.0f, 0.0f,1.0f,   // 右上角
        1.0f,  -1.0f, 0.0f, 1.0f,0.0f,  // 右下角
        -1.0f, -1.0f, 0.0f, 0.0f,0.0f, // 左上角
        // 第二个三角形
        -1.0f,  1.0f, 0.0f, 0.0f,1.0f,  
        1.0f,   1.0f, 0.0f, 1.0f,1.0f,
        1.0f,  -1.0f, 0.0f, 1.0f,0.0f,
    };

Device::Device(int w, int h)
{
    win_width = w;
    win_height = h;
    // buff[0].alloc(w, h);
    // buff[1].alloc(w, h);

    vao = new VAOMGR(6);
    vao->gen_vao_vbo(vertices,sizeof(vertices));
    shader.init_shader();
}
void Device::set_buf(FrameBuffer* buf)
{
    cur_buf = buf;
}

void Device::update_texture()
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, win_width, win_height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, cur_buf->get_buf());

    //std::cout<<"error:"<<glGetError()<<std::endl;
    glGenerateMipmap(GL_TEXTURE_2D);
    //std::cout<<"error:"<<glGetError()<<std::endl;
}

void Device::init_render()
{
    //bind texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    shader.use();
    vao->bind();
}

void Device::render(double interval)
{
    update_texture();
    vao->draw();
}

void Device::update(double interval)
{
    render(interval);
}


void Device::release()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}




static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

CBMgr* cb_mgr;
GLFWwindow* glfw_window;

GLFWwindow* init_glfw_window(int w,int h)
{
    printf("start opengl\n");
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfw_window = glfwCreateWindow(w,h, "test3d", NULL, NULL);
    if(!glfw_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(glfw_window);
    glfwSetKeyCallback(glfw_window, key_callback);

    glewExperimental = GL_TRUE;
    glewInit();

    const char *version = (const char *)glGetString(GL_VERSION);
    const char *vendor = (const char *)glGetString(GL_VENDOR);
    printf("OpenGL version:%s\nOpengl Vendor:%s\n",version,vendor);
    char buf[1024];
    getcwd(buf,sizeof(buf));
    printf("cwd:%s\n",buf);

    cb_mgr = new CBMgr;
    return glfw_window;

}


void glfw_loop()
{
    double last_time = 0;
    double now;
    double interval;
    while (!glfwWindowShouldClose(glfw_window)) {
        now = glfwGetTime();
        glfwPollEvents();

        interval = now - last_time;
        cb_mgr->update(interval);

        glfwSwapBuffers(glfw_window);

        last_time = now;
    }

}

void regist_update(UpdateFunc f)
{
    cb_mgr->add_callback(f);
}

void regist_objupdate(UpdateObj* p)
{
    cb_mgr->add_callback2(p);
}

double time_now()
{
    return glfwGetTime();
}



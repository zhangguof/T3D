//
//  triangle.cpp
//  firstTriangle
//
//  Created by tony on 16/3/21.
//  Copyright © 2016年 tony. All rights reserved.
//

#include "T3D.hpp"

typedef DrawBuffer<GLuint> DBuff;

const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;

const int win_width = WIN_WIDTH;
const int win_height = WIN_HEIGHT;

const char* VERTEX_SHADER_FILE = "shader/vertextShader.glsl";
const char* FRAG_SHADER_FILE = "shader/fragmentShader.glsl";

CBMgr cb_mgr;

class SHADER
{
    GLuint shader;
    const char* vet_file_path;
    const char* frag_file_path;
public:
    SHADER(const char* vet_file = VERTEX_SHADER_FILE, 
           const char* frag_file = FRAG_SHADER_FILE)
    {
        vet_file_path = vet_file;
        frag_file_path = frag_file;
        //init_shader();
    }
    void read_shader_file(const char *fname,char *buf)
    {
        int fp = open(fname,O_RDONLY);
        int size = read(fp, buf, 2048);
        buf[size] = '\0';
    }
    void init_shader()
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
    void use()
    {
        glUseProgram(shader);
    }

};

class VAOMGR
{
    //OPENGL Device
    GLuint vbo;
    GLuint vao;
    int num;
public:
    VAOMGR(int n)
    {
        num = n;
    }
    void gen_vao_vbo(GLfloat vertices[],size_t size)
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
    void bind()
    {
        glBindVertexArray(vao);
    }
    void ubind()
    {
        glBindVertexArray(0);
    }
    void draw()
    {
        
        glDrawArrays(GL_TRIANGLES, 0, num);
    }
    ~VAOMGR()
    {
        glDeleteVertexArrays(1,&vao);
        glDeleteBuffers(1,&vbo);
    }

};

GLfloat vertices[] = {
        // 第一个三角形       贴图坐标
        -1.0f,  1.0f, 0.0f, 0.0f,1.0f,   // 右上角
        1.0f,  -1.0f, 0.0f, 1.0f,0.0f,  // 右下角
        -1.0f, -1.0f, 0.0f, 0.0f,0.0f, // 左上角
        // 第二个三角形
        -1.0f,  1.0f, 0.0f, 0.0f,1.0f,  
        1.0f,   1.0f, 0.0f, 1.0f,1.0f,
        1.0f,  -1.0f, 0.0f, 1.0f,0.0f,
    };

class Device
{
    int win_width;
    int win_height;
    const static int buff_num = 2;
    //DrawBuffer<GLuint> buff[buff_num];
    DBuff* cur_buf;

    //opengl
    VAOMGR *vao;
    SHADER shader;

    GLuint texture;


public:
    Device(int w=WIN_WIDTH, int h=WIN_HEIGHT)
    {
        win_width = w;
        win_height = h;
        // buff[0].alloc(w, h);
        // buff[1].alloc(w, h);

        init_device();
        vao = new VAOMGR(6);
        vao->gen_vao_vbo(vertices,sizeof(vertices));
        shader.init_shader();
    }
    void set_buf(DBuff* buf)
    {
        cur_buf = buf;
    }
    void init_device()
    {
    }
    void update_texture()
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, win_width, win_height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, cur_buf->get_buf());

        //std::cout<<"error:"<<glGetError()<<std::endl;
        glGenerateMipmap(GL_TEXTURE_2D);
        //std::cout<<"error:"<<glGetError()<<std::endl;
    }

    void init_render()
    {
        //bind texture;
        glGenTextures(1,&texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        shader.use();
        vao->bind();
    }

    void render()
    {
        update_texture();
        vao->draw();
    }
    void release()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }



};

//GLuint img_data[100*100];
DrawBuffer<GLuint> img_data(WIN_WIDTH,WIN_HEIGHT);

RGB r(255,0,0);
RGB g(0,255,0);
RGB b(0,0,255);
RGB col[3] = {r,g,b};

void set_grid_img(int x,int y,int w, int h,int cnt)
{
    for(int i=0;i<w;i++)
    {
        int new_x = x*w+i;
        if(new_x>=WIN_WIDTH)
            continue;
        for(int j=0;j<h;j++)
        {
            int new_y = y*h+j;
            if(new_y>=WIN_HEIGHT)
                continue;
            img_data.set_point(new_x,new_y,col[cnt%3].get_rgb_val());
        }
    }
}
    


void init_img(int start_idx=0)
{


    //img_data.clear_color(rgb);
    int w = 50;
    int h = 50;
    int nx = win_width/w;
    int rx = win_width%w;

    int ny = win_height/h;
    int ry = win_height%h;
    std::cout<<rx<<" "<<ry<<std::endl;
    int cnt=start_idx;
    for(int y=0;y<ny;y++)
    {
        for(int x=0;x<nx;x++)
        {
            set_grid_img(x,y,w,h,cnt);
            cnt++;
        }
        if(rx)
        {
            set_grid_img(nx,y,w,h,cnt);
            cnt++;
        }

    }
    if(ry)
    {
            for(int x=0;x<nx;x++)
            {
                set_grid_img(x,ny,w,h,cnt);
                cnt++;
            }
            if(rx)
            {
                set_grid_img(nx,ny,w,h,cnt);
                cnt++;
            }
    }
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

double last_check_time = 0.0;
int start_idx = 0;
void test(double interval)
{
    last_check_time += interval;
    if(last_check_time > 0.5)
    {
        std::cout<<(1.0/interval)<<std::endl;
        start_idx += 1;
        if(start_idx >= 3)
        {
            start_idx = 0;
        }
        init_img(start_idx);
        last_check_time = 0.0;
    }
    
}


int main(int argc, char **argv)
{
    
    printf("start opengl\n");
    
    
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(win_width,win_height, "test3d", NULL, NULL);
    if(!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    //glViewport(0, 0, win_width, win_height);
    
    const char *version = (const char *)glGetString(GL_VERSION);
    const char *vendor = (const char *)glGetString(GL_VENDOR);
    printf("OpenGL version:%s\nOpengl Vendor:%s\n",version,vendor);
    char buf[1024];
    getcwd(buf,sizeof(buf));
    printf("cwd:%s\n",buf);
    

    Device dev;
    
    init_img();


    dev.init_render();
    dev.set_buf(&img_data);

    cb_mgr.add_callback(test);

    
    double last_time = 0;
    double now;
    while (!glfwWindowShouldClose(window)) {
        now = glfwGetTime();
        glfwPollEvents();
        dev.render();
        glfwSwapBuffers(window);

        //std::cout<<"fps:"<<(1.0/(now-last_time))<<std::endl;
        double interval = now - last_time;
        cb_mgr.update(interval);
        last_time = now;
    }
    

    
    
    glBindVertexArray(0);
    glfwDestroyWindow(window);
    
    
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    
    
}

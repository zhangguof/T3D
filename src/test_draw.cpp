//
//  triangle.cpp
//  firstTriangle
//
//  Created by tony on 16/3/21.
//  Copyright © 2016年 tony. All rights reserved.
//

#include "T3D.hpp"
#include "text_render.hpp"


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

void text(char c)
{
    auto ch = get_char(c);
    img_data.overwrite(ch.buff, 10, 10, ch.Size[0],ch.Size[1]);


}


double last_check_time = 0.0;
int start_idx = 0;
void test(double interval)
{
    last_check_time += interval;
    // if(last_check_time > 0.5)
    // {
    //     std::cout<<(1.0/interval)<<std::endl;
    //     start_idx += 1;
    //     if(start_idx >= 3)
    //     {
    //         start_idx = 0;
    //     }
    //     init_img(start_idx);
    //     last_check_time = 0.0;
    // }
    img_data.clear_color(RGB(255,255,255));
    text((GLulong)'A');
    
}




int main(int argc, char **argv)
{
    // if (FT_Load_Char(face, 'A', FT_LOAD_RENDER))
    // {
    //     std::cout<<"ERROR::FREETYPE:Failed to load GLyph"<<std::endl;
    //     return 0;
    // }
    
    printf("starting.......\n");
    std::cout<<VERTEX_SHADER_FILE<<" "<<FRAG_SHADER_FILE<<std::endl;
    GLFWwindow* window = init_glfw_window();
    
    Device *pdev = new Device();
    
    init_img();
    init_font("res/NotoSansCJKsc-Black.otf");


    pdev->init_render();
    pdev->set_buf(&img_data);
    //UpdatePair p((UpdateObj*)&dev,(ObjUpdateFunc)&Device::update);
    regist_objupdate(pdev);
    regist_update(test);

    glfw_loop();


    
    
    glBindVertexArray(0);
    glfwDestroyWindow(window);
    
    
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    
    
}

#include "T3D.hpp"
#include "Render.hpp"


typedef unsigned int COLOR;
const COLOR BLACK   = 0x000000;
const COLOR RED     = 0xFF0000;
const COLOR GREEN   = 0x00FF00;
const COLOR BLUE    = 0x0000FF;
const COLOR WHITE   = 0xFFFFFF;


RGBA r(255,0,0,1);
RGBA g(0,255,0,1);
RGBA b(0,0,255,1);
RGBA col[3] = {r,g,b};


Render* p_render;
unsigned int p_bitmap[WIN_WIDTH*WIN_HEIGHT];


void set_grid_img(int x,int y,int w, int h,int cnt)
{
    unsigned int c = col[cnt%3].get_rgba_val();
    auto pb = p_render->get_buff();
    pb->set_range_pixel(x*w,y*h,w,h,col[cnt%3].get_rgba_val());

    //frame_buffer.set_range_pixel(x*w,y*h,w,h,col[cnt%3].get_rgba_val());
    // for (int i=0;i<w;i++)
    // {
    //     int new_x = x*w+i;
    //     if(new_x>=WIN_WIDTH)
    //         continue;
    //     for(int j=0;j<h;j++)
    //     {
    //         int new_y = y*h+j;
    //         if(new_y>=WIN_HEIGHT)
    //             continue;
    //         //p_bitmap[new_x+new_y*WIN_WIDTH] = c;
    //         //p_render->load_r8b8g8a8_bitmap(p_bitmap,w,h,new_x,new_y);

    //     }
    // }

}

void render_test_img(int start_idx=0)
{

    int w = 50;
    int h = 50;
    int nx = WIN_WIDTH/w;
    int rx = WIN_WIDTH%w;

    int ny = WIN_HEIGHT/h;
    int ry = WIN_HEIGHT%h;

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
    //p_render->load_r8b8g8a8_bitmap(p_bitmap,WIN_WIDTH,WIN_HEIGHT,0,0);
}


double last_check_time = 0.0;
int start_idx = 0;
char buf[100];

void test_img(double interval)
{
    render_test_img(start_idx);
    last_check_time += interval;
    if(last_check_time > 0.5)
    {

        last_check_time = 0.0;
        // start_idx+=1;
        // if(start_idx>2)
        //     start_idx=0;

    }
    
}


double last_time=0;



void render(double interval)
{
    p_render->clear_buff_color();
    //frame_buffer.clear_color();
    
    // sprintf(buf,"FPS:%.0f",(1.0/interval));
    //pbuf->clear_color();
    
    test_img(interval);
    double now = time_now();
    if(now-last_time > (1/15.0))
    {
        sprintf(buf,"FPS:%0.1f",(1.0/interval));

        
        last_time = now;
    }

    p_render->draw_text(buf,30,30,0xFF0000FF);
    p_render->render(interval);

}

void init()
{

    p_render->init_pen_font("res/Arial.ttf",30);

   //init_font("res/NotoSansCJKsc-Black.otf",25); 
    p_render->init_render();

    regist_update(render);
}




int main(int argc, char **argv)
{
    
    printf("starting.......\n");
    int width = 640;
    int height = 480;

    GLFWwindow* window = init_glfw_window(width,height);
    p_render = new Render(width, height);

    init();

    glfw_loop();


    
    glBindVertexArray(0);
    glfwDestroyWindow(window);
    
    
    
    glfwTerminate();
    exit(EXIT_SUCCESS);

    
}

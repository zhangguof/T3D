#include "T3D.hpp"
#include "text_render.hpp"


typedef unsigned int COLOR;
const COLOR BLACK   = 0x000000;
const COLOR RED     = 0xFF0000;
const COLOR GREEN   = 0x00FF00;
const COLOR BLUE    = 0x0000FF;
const COLOR WHITE   = 0xFFFFFF;

DrawBuffer<GLuint> frame_buffer(WIN_WIDTH,WIN_HEIGHT);

RGBA r(255,0,0,1);
RGBA g(0,255,0,1);
RGBA b(0,0,255,1);
RGBA col[3] = {r,g,b};

void set_grid_img(int x,int y,int w, int h,int cnt)
{

    frame_buffer.set_range_pixel(x*w,y*h,w,h,col[cnt%3].get_rgba_val());
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


Device* pdev;
double last_time=0;

void render(double interval)
{
    frame_buffer.clear_color();
    test_img(interval);
    // sprintf(buf,"FPS:%.0f",(1.0/interval));
    
    double now = time_now();
    if(now-last_time > (1/15.0))
    {
        sprintf(buf,"FPS:%0.1f",(1.0/interval));
        
        last_time = now;
    }
    render_text(frame_buffer,buf,30,30,0x000000FF);
    

    pdev->render(interval);
}

void init()
{
    frame_buffer.clear_color(0x000000);

   //init_font("res/NotoSansCJKsc-Black.otf",25); 
   init_font("res/Arial.ttf",30);
   pdev->init_render();
   pdev->set_buf(&frame_buffer);

    //UpdatePair p((UpdateObj*)&dev,(ObjUpdateFunc)&Device::update);
    //regist_objupdate(pdev);
    regist_update(render);
}




int main(int argc, char **argv)
{
    
    printf("starting.......\n");

    GLFWwindow* window = init_glfw_window();
    
    pdev = new Device();

    init();

    glfw_loop();


    
    glBindVertexArray(0);
    glfwDestroyWindow(window);
    
    
    
    glfwTerminate();
    exit(EXIT_SUCCESS);

    
}

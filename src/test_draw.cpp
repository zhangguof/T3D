#include "T3D.hpp"
#include "Render.hpp"
#include "mat_trans.hpp"


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

    for (int i=0;i<w;i++)
    {
        int new_x = x*w+i;
        if(new_x>=WIN_WIDTH)
            continue;
        for(int j=0;j<h;j++)
        {
            int new_y = y*h+j;
            if(new_y>=WIN_HEIGHT)
                continue;
            p_bitmap[new_x+new_y*WIN_WIDTH] = c;
           
        }
    }

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
    p_render->load_r8b8g8a8_bitmap(p_bitmap,WIN_WIDTH,WIN_HEIGHT,0,0);
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

int points[] = {
    -310,0,
    0,230,
    310,0,
    0,-230,
};

int tri_points[] = {
    -100,100,
    100,100,
    100,-100,
};
int tri_points2[] = {
    -100,100,
    100,-100,
    -100,-100,
};

int tri_points3[] = {
    0,200,
    800,0,
    0,-200,
};

Vec3f tri_vertexs[] = {
    Vec3f(0,200,1),
    Vec3f(200,0,1),
    Vec3f(0,-200,1),
};


int tri_points4[] = {
    0,200,
    200,0,
    0,-200,
};

double time_pass = 0;
void draw_anime(double interval)
{
    Color c = 0xFF0000FF;
    Color c2 = 0x00FF00FF;
    MatTran<float,3> mtran;
    time_pass += interval;
    //mtran.mat_move_to(Vec3i(100,100,1));
    mtran.mat_rotate_degree(5*time_pass);
    Vec3f verts[] = {
        mtran.trans(tri_vertexs[0]),
        mtran.trans(tri_vertexs[1]),
        mtran.trans(tri_vertexs[2]),
    };

    p_render->draw_triangle(verts,c2);
}

void draw_test_tri()
{
    Color c = 0xFF0000FF;
    Color c2 = 0x00FF00FF;
    MatTran<float,3> mtran;
    //mtran.mat_move_to(Vec3i(100,100,1));
    mtran.mat_rotate_degree(30);
    Vec3f verts[] = {
        mtran.trans(tri_vertexs[0]),
        mtran.trans(tri_vertexs[1]),
        mtran.trans(tri_vertexs[2]),
    };
    // verts[0].print_vec();
    // verts[1].print_vec();
    // verts[2].print_vec();
    p_render->draw_triangle(verts,c2);
}

void draw_test_tri2()
{
    Color c = 0xFF0000FF;
    Color c2 = 0x00FF00FF;
    p_render->draw_triangle(tri_points4,c2);
}

void render(double interval)
{
    p_render->clear_buff_color();

    //render img
    //test_img(interval);
    //draw point
    Color c = 0xFF0000FF;
    Color c2 = 0x00FF00FF;
    // p_render->draw_point(0,0,c);
    // p_render->draw_point(320,0,c);
    // p_render->draw_point(-320,0,c);
    // p_render->draw_point(315,235,c);
    // p_render->draw_point(-100,100,c);
    // p_render->draw_point(-100,-100,c);
    // p_render->draw_point(100,-100,c);

    // p_render->draw_line(-100,100,100,-100,c);
    // p_render->draw_line(-50,0,0,50,c);
    // p_render->draw_line(-50,0,0,25,c);
    // p_render->draw_line(-50,0,0,75,c);
    
   p_render->draw_line(-320,0,320,0,c);
   p_render->draw_line(0,-240,0,240,c);
   p_render->draw_line(200,-240,200,240,c);

    // p_render->draw_polygon(points,4,c);
    // p_render->draw_triangle(tri_points,c);
    // p_render->draw_triangle(tri_points2,c2);
    
    //draw_test_tri();
    draw_anime(interval);

    double now = time_now();
    if(now-last_time > (1/15.0))
    {
        sprintf(buf,"FPS:%0.1f",(1.0/interval));
        last_time = now;
    }
    p_render->set_pen_size(30);
    p_render->draw_text(buf,30,30,0xFFFFFF00); //render text

    p_render->set_pen_size(40);
    p_render->draw_text("test it.",30,70,0xFFFFFF00);
    
    //do render.
    p_render->render(interval);
    // unsigned int b[640][480];
    // p_render->get_buff()->dump<480>(b);
    

}


void init()
{

    p_render->init_pen_font("res/Arial.ttf",30);

   //init_font("res/NotoSansCJKsc-Black.otf",25); 
    p_render->init_render();

    p_render->clear_buff_color();

    //render img
    //test_img(interval);
    //draw point
    // Color c = 0xFF0000FF;
    // p_render->draw_triangle(tri_points,c);
    // p_render->draw_triangle(tri_points2,c);
    // p_render->render(0);
    regist_update(render);
}

void test_mat()
{
    int a[] = {1,2,3,4};
    int b[] = {1,3,5,6,0,4};

    MatNxM<int,2,2> ma(a);
    MatNxM<int,2,3> mb(b);

    MatNxM<int,2,3> mc;
    mc = ma.mul<3>(mb);
    std::cout<<"Mat A:"<<std::endl;
    ma.print_mat();
    std::cout<<"Mat B:"<<std::endl;
    mb.print_mat();


    std::cout<<"Mat C:"<<std::endl;
    mc.print_mat();
}



int test_main(int argc, char **argv)
{
    test_mat();
    
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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include <windows.h>

#include "map_create.c"

void clear_screen() {
    printf("\033[H\033[J");
}

typedef struct {
    float x;
    float y;
} Vector;
typedef struct {
    int x;
    int y;
} IntVector;


#define WIDTH 800
#define HEIGHT 200


typedef struct {
    Vector pos;
    Vector size;
} Object;



int main() {

    // carregar mapa






    Vector pos = {3.4, 6.2};
    Vector direction = {-1, 0};
    Vector plane = {0, 0.60};

    float rot_speed = 0.011;
    float screen_middle = HEIGHT/2;

    float mouseX = 0;
    float mouseY = 100;

    CHAR_INFO screen[WIDTH*HEIGHT];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
    SetConsoleActiveScreenBuffer(hConsole);
    COORD buffer_size = {WIDTH, HEIGHT};
    COORD screen_pos = {0, 0};
    SMALL_RECT write_region = {0, 0, WIDTH-1, HEIGHT-1};

    int render_map = 1;
    int tab_state = 0;

    int camera_height = 0;

    float zBuffer[WIDTH];

    Object test_obj;
    Vector obj_pos = {6,8};
    Vector obj_size = {50, 100};
    test_obj.pos = obj_pos;
    test_obj.size = obj_size;
    Object objects[1] = {test_obj};
    int loop = 1;
    while (loop) {
        


        for (int x = 0; x < WIDTH; x++) {
            float cameraX = 2 * x / (float)WIDTH - 1;
            Vector ray = {direction.x + plane.x * cameraX, direction.y + plane.y * cameraX};
            
            IntVector map = {(int) pos.x, (int) pos.y};
            
            Vector side_dist;
            Vector delta_dist;
            delta_dist.x = sqrtf(1 + (ray.y * ray.y) / (ray.x * ray.x));
            delta_dist.y = sqrtf(1 + (ray.x * ray.x) / (ray.y * ray.y));
            float perp_wall_dist;

            IntVector step;            
            int hit = 0;
            int side;

            if (ray.x < 0) {
                step.x = -1;
                side_dist.x = (pos.x - map.x) * delta_dist.x;
            } else {
                step.x = 1;
                side_dist.x = (map.x + 1.0 - pos.x) * delta_dist.x;
            }
            if (ray.y < 0) {
                step.y = -1;
                side_dist.y = (pos.y - map.y) * delta_dist.y;
            } else {
                step.y = 1;
                side_dist.y = (map.y + 1.0 - pos.y) * delta_dist.y;
            }
            
            while (hit == 0) {
                if (side_dist.x < side_dist.y) {
                    side_dist.x += delta_dist.x;
                    map.x += step.x;
                    side = 0;
                } else {
                    side_dist.y += delta_dist.y;
                    map.y += step.y;
                    side = 1;
                }
                if (world_map[map.x][map.y] > 0) hit = 1;
            }

            if (side == 0) perp_wall_dist = (side_dist.x - delta_dist.x);
            else           perp_wall_dist = (side_dist.y - delta_dist.y);

            zBuffer[x] = perp_wall_dist;

            int line_height = (int) HEIGHT / perp_wall_dist;
            int max = screen_middle - line_height;
            int min = HEIGHT - max + camera_height;
            max += camera_height;
            //printf("\n max: %d, min: %d", max, min);
            int wall_type = world_map[map.x][map.y];

            for (int y = 0; y < HEIGHT; y++) {
                CHAR ascii_char;
                WORD color;
                
                if (y <= max) {
                    ascii_char = 32;
                } else if (y > max && y < min){
                    
                    if      (perp_wall_dist > 35) ascii_char = 250;
                    else if (perp_wall_dist > 30) ascii_char = 249;
                    else if (perp_wall_dist > 25) ascii_char = 167;
                    else if (perp_wall_dist > 20) ascii_char = 176;
                    else if (perp_wall_dist > 15) ascii_char = 186;
                    else if (perp_wall_dist > 10) ascii_char = 177;
                    else if (perp_wall_dist >  5) ascii_char = 178;
                    else                          ascii_char = 219;

                    if      (wall_type == 1) color = FOREGROUND_BLUE;
                    else if (wall_type == 2) color = FOREGROUND_RED;
                    else if (wall_type == 3) color = FOREGROUND_GREEN;
                    else                     color = FOREGROUND_BLUE | FOREGROUND_RED;

                    if (side == 1) color = color | FOREGROUND_INTENSITY;

                } else {
                    // floor

                    ascii_char = 249;
                    color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
                }
                screen[y * WIDTH + x].Char.AsciiChar = ascii_char;
                screen[y * WIDTH + x].Attributes = color;
            }
        } 


        // objects
         
        for (int i = 0; i < 1; i++) {
            float obj_dist = (pos.x - objects[i].pos.x) * (pos.x - objects[i].pos.x) + (pos.y - objects[i].pos.y) * (pos.y - objects[i].pos.y);
            if (obj_dist < 3.0) continue;
            Vector obj_pos = {objects[i].pos.x - pos.x, objects[i].pos.y - pos.y};

            float inv_d = 1.0 / (plane.x * direction.y - direction.x * plane.y);
            Vector transform = {inv_d * (direction.y * obj_pos.x - direction.x * obj_pos.y), inv_d * (-plane.y * obj_pos.x + plane.x * obj_pos.y)};
            
            int obj_screen_x = (int) (WIDTH /2) * (1 + transform.x / transform.y); 
            if (obj_screen_x < 0 || obj_screen_x >= WIDTH) continue;
            // height
            int obj_height = (int) abs(HEIGHT / transform.y);
            int draw_start_y = screen_middle - obj_height + camera_height;
            if (draw_start_y < 0) draw_start_y = 0;
            int draw_end_y = screen_middle + obj_height + camera_height;
            if (draw_end_y >= HEIGHT) draw_end_y = HEIGHT - 1;


            // width
            int obj_width = obj_height;
            int draw_start_x = -obj_width/2 + obj_screen_x;
            if (draw_start_x < 0) draw_start_x = 0;
            int draw_end_x = obj_width / 2 + obj_screen_x;
            if (draw_end_x >= WIDTH) draw_end_x = WIDTH - 1;

            printf("\nmax X: %d\t min X: %d\nmax Y: %d\tmin Y: %d\nobject width: %d\n", draw_start_x, draw_end_x, draw_start_y, draw_end_y, obj_width);

            if (draw_end_x < draw_start_x || draw_end_y < draw_start_y) continue;

            for (int x = draw_start_x; x < draw_end_x; x++) {
                if (transform.y > 0 && x > 0 && x < WIDTH && transform.y < zBuffer[x]) {
                    for (int y = draw_start_y; y < draw_end_y; y++) {
                        screen[y * WIDTH + x].Char.AsciiChar = 219;
                        screen[y * WIDTH + x].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN;
                    }
                }
            }        


        }










        float move_speed = 0.17;

        if (GetAsyncKeyState((unsigned short)16) * 0x8000) {
           move_speed = 0.3; 
        }

        if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
            pos.x += move_speed * (direction.x);
            pos.y += move_speed * (direction.y);
            if (world_map[(int) pos.x][(int) pos.y] > 0) {
                pos.x -= move_speed * (direction.x);
                pos.y -= move_speed * (direction.y);
            }
        } 
        if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
            pos.x -= move_speed * (direction.x);
            pos.y -= move_speed * (direction.y);
            if (world_map[(int) pos.x][(int) pos.y] > 0) {
                pos.x += move_speed * (direction.x);
                pos.y += move_speed * (direction.y);
            }
        } 
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
            pos.x += move_speed * (plane.x);
            pos.y += move_speed * (plane.y);
            if (world_map[(int) pos.x][(int) pos.y] > 0) {
                pos.x -= move_speed * (plane.x);
                pos.y -= move_speed * (plane.y);
            }
        }
        if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
            pos.x -= move_speed * (plane.x);
            pos.y -= move_speed * (plane.y);
            if (world_map[(int) pos.x][(int) pos.y] > 0) {
                pos.x += move_speed * (plane.x);
                pos.y += move_speed * (plane.y);
            }
        }
        if (GetAsyncKeyState((unsigned short)32) & 0x8000) {
            return 0;
        } 
        if (GetAsyncKeyState((unsigned short)17) & 0x8000) {
            camera_height -= 2;
        } 
        
        if (GetAsyncKeyState((unsigned short)116) & 0x8000) {
            plane.y += 0.01;
        } 
        if (GetAsyncKeyState((unsigned short)117) & 0x8000) {
            plane.y -= 0.01;
        } 

        if ((GetAsyncKeyState((unsigned short)9) & 0x8000) != 0) {
            tab_state = 1;
        } else if (tab_state == 1){
            if (render_map) render_map = 0;
            else render_map = 1;
            tab_state = 0;
        }

        float mouse_diff = 0;
        INPUT_RECORD in_buffer[32];
        DWORD events = 0;
        GetNumberOfConsoleInputEvents(hConsoleIn, &events);
        if (events > 0) ReadConsoleInput(hConsoleIn, in_buffer, events, &events);
        for (int i = 0; i < events; i++) {
            if (in_buffer[i].EventType == MOUSE_EVENT) {
                if (in_buffer[i].Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
                    float temp = mouseX;
                    mouseX = in_buffer[i].Event.MouseEvent.dwMousePosition.X;
                    mouse_diff = mouseX - temp;
                    
                    temp = direction.x;
                    direction.x = direction.x * cosf(-rot_speed * mouse_diff) - direction.y * sinf(-rot_speed * mouse_diff);
                    direction.y = temp * sinf(-rot_speed * mouse_diff) + direction.y * cosf(-rot_speed * mouse_diff);

                    temp = plane.x;
                    plane.x = plane.x * cosf(-rot_speed * mouse_diff) - plane.y * sinf(-rot_speed * mouse_diff);
                    plane.y = temp * sinf(-rot_speed * mouse_diff) + plane.y * cosf(-rot_speed * mouse_diff);
                    
                    temp = mouseY;
                    mouseY = in_buffer[i].Event.MouseEvent.dwMousePosition.Y; 
                    if (camera_height - 4*(mouseY-temp) < HEIGHT && camera_height - 4*(mouseY - temp) > -HEIGHT) {
                        camera_height -= 4*(mouseY - temp);
                    }
                }
            }
        }

        // map
        if (render_map) {
            for (int i = 0; i < mapHEIGHT; i++) {
                for (int ii = 0; ii < mapWIDTH * 2; ii++) {
                    if (world_map[ii/2][i] > 0) {
                        screen[i * WIDTH + ii].Char.AsciiChar = 219;
                        screen[i * WIDTH + ii].Attributes =   FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;

                    }
                }
            }
            screen[(int)pos.y * WIDTH + (int)pos.x * 2].Char.AsciiChar = 219;
            screen[(int)pos.y * WIDTH + (int)pos.x * 2].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            screen[(int)pos.y * WIDTH + (int)pos.x * 2 + 1].Char.AsciiChar = 219;
            screen[(int)pos.y * WIDTH + (int)pos.x * 2 + 1].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;

        }
    
        

        screen[WIDTH * HEIGHT - 1].Char.AsciiChar = '\0';
        WriteConsoleOutput(hConsole, screen, buffer_size, screen_pos, &write_region);
    }
       
}
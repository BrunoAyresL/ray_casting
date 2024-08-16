#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include <windows.h>

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


#define mapWIDTH 48
#define mapHEIGHT 24
#define WIDTH 800
#define HEIGHT 200

int world_map[mapWIDTH][mapHEIGHT] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3},
    {1,0,0,0,1,0,0,0,0,0,0,0,3,3,0,0,1,0,3,0,4,0,0,3},
    {1,0,0,0,0,0,0,0,0,0,0,0,3,3,0,0,1,0,3,0,0,0,0,3},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,3,0,0,0,1,3},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,3,0,0,0,0,3},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,3},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,3},
    {1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,0,3,3,3},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
    {1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,2,2,2,2,2,2,1},
    {1,1,1,0,1,1,1,0,0,3,0,0,2,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,1,1},
    {3,0,0,0,0,0,0,0,0,3,0,0,0,3,0,0,0,1,0,0,1,0,1,1},
    {3,0,0,0,0,0,0,0,0,3,0,0,0,3,0,0,0,1,0,0,1,0,1,1},
    {3,0,0,0,0,0,0,0,0,3,3,0,3,3,0,0,0,1,0,0,0,0,0,1},
    {3,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,2,2,2,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {2,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {2,0,0,2,0,0,0,0,0,0,3,0,0,3,0,0,0,0,0,0,0,0,0,1},
    {2,0,0,2,0,0,0,0,0,0,3,0,0,3,0,0,0,0,0,0,2,2,2,1},
    {2,0,0,2,0,0,3,3,3,3,3,0,0,3,0,0,0,0,0,0,0,0,0,1},
    {2,0,0,2,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,1},
    {2,0,2,2,2,2,2,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,1},
    {2,0,0,0,0,0,2,0,0,3,3,3,3,3,0,0,0,0,0,0,0,0,0,1},
    {2,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,1},
    {2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,4,0,0,1},
    {2,0,0,0,2,0,2,2,2,0,0,2,2,4,4,4,4,0,4,0,4,0,0,4},
    {2,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,4},
    {2,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
    {2,2,2,2,2,2,2,2,2,2,2,2,2,4,4,4,4,4,4,4,4,4,4,4}
};

int main() {
    Vector pos;
    pos.x = 3.4;
    pos.y = 6.2;
    Vector direction;
    direction.x = -1;
    direction.y = 0;
    Vector plane;
    plane.x = 0;
    plane.y = 0.60;

    float rot_speed = 0.011;
    float move_speed = 0.15;
    float screen_middle = HEIGHT/2;

    float mouseX = 0;
    float mouseY = 100;

    CHAR_INFO screen[WIDTH*HEIGHT];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;
    COORD buffer_size = {WIDTH, HEIGHT};
    COORD screen_pos = {0, 0};
    SMALL_RECT write_region = {0, 0, WIDTH-1, HEIGHT-1};

    int camera_height = 0;

    int loop = 1;
    while (loop) {
        
        for (int x = 0; x < WIDTH; x++) {
            float cameraX = 2 * x / (float)WIDTH - 1;
            Vector ray;
            ray.x = direction.x + plane.x * cameraX;
            ray.y = direction.y + plane.y * cameraX;
            
            IntVector map;
            map.x = (int) pos.x;
            map.y = (int) pos.y;
            
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
                    ascii_char = 249;
                    color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
                }
                screen[y * WIDTH + x].Char.AsciiChar = ascii_char;
                screen[y * WIDTH + x].Attributes = color;
            }
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
    
        


        screen[WIDTH * HEIGHT - 1].Char.AsciiChar = '\0';
        WriteConsoleOutput(hConsole, screen, buffer_size, screen_pos, &write_region);
    }
       
}
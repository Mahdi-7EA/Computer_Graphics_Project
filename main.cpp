#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>

#define PI 3.1416



// ===== START ANIMATION VARIABLES =====
float trainX    = -930.0;   // train moves right
float car1X     =   80.0;   // red car, moves right
float car2X     =  680.0;   // blue car, moves right
float car3X     =  350.0;   // green car, moves left
float truckX    = 1100.0;   // truck, moves left
float shipX     = -300.0;   // ship moves right
float waveAngle =    0.0;   // water wave animation
float cloudX1   =    0.0;   // cloud 1 drift
float cloudX2   =  500.0;   // cloud 2 drift
float cloudX3   =  950.0;   // cloud 3 drift
float sunX      =  200.0;   // sun moves right
float clockMin  =  180.0;   // clock minute hand angle

float trainSpeed   = 2.2;
float carSpeed     = 2.5;
float shipSpeed    = 1.5;

int paused    = 0;
int nightMode = 0;

// ===== END ANIMATION VARIABLES =====






// ===== START HELPER FUNCTION =====

// Circle draw kora function
void drawCircle(float cx, float cy, float r)
{
    glBegin(GL_POLYGON);
    for(int i = 0; i < 180; i++)
    {
        float angle = 2 * PI * i / 180;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();
}

// Rectangle draw korar function
void drawRect(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

// Rectangle Line Loop draw korar function
void drawRectLine(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}


// DRAW ONE CLOUD
void drawOneCloud(float cx, float cy)
{
    if(nightMode)
        glColor3f(0.18, 0.18, 0.26);
    else
        glColor3f(0.97, 0.97, 1.00);

    drawCircle(cx,        cy,        33.6);
    drawCircle(cx + 44,   cy - 3.2,  28.8);
    drawCircle(cx - 44,   cy - 3.2,  27.2);
    drawCircle(cx + 22.4, cy + 22.4, 26.4);
    drawCircle(cx - 22.4, cy + 22.4, 25.6);
    drawCircle(cx + 70.4, cy + 9.6,  20.8);
    drawCircle(cx - 70.4, cy + 9.6,  20.0);
}
// ===== END HELPER FUNCTION =====


// ===== DRAW SKY =====
void drawSky()
{
    if(!nightMode)
    {
        // Day: gradient from light blue (top) to sky blue (bottom)
        glBegin(GL_QUADS);
        glColor3f(0.58, 0.80, 1.00);   // bottom (near buildings)
        glVertex2f(0, 373);
        glVertex2f(1280, 373);
        glColor3f(0.22, 0.55, 0.92);   // top
        glVertex2f(1280, 720);
        glVertex2f(0, 720);
        glEnd();
    }
    else
    {
        // Night: dark blue
        glBegin(GL_QUADS);
        glColor3f(0.04, 0.05, 0.22);
        glVertex2f(0, 373);
        glVertex2f(1280, 373);
        glColor3f(0.01, 0.02, 0.12);
        glVertex2f(1280, 720);
        glVertex2f(0, 720);
        glEnd();

        // Stars
        srand(99);
        glColor3f(1.0, 1.0, 0.92);
        glPointSize(2.0);
        glBegin(GL_POINTS);
        for(int i = 0; i < 90; i++)
        {
            float sx = 10.0 + (rand() % 1260);
            float sy = 410.0 + (rand() % 300);
            glVertex2f(sx, sy);
        }
        glEnd();
        glPointSize(1.0);
    }
}

// ===== DRAW SUN & MOON =====
void drawSun()
{
    if(!nightMode)
    {
        glColor3f(1.0, 0.94, 0.26);
        glLineWidth(2.0);
        glBegin(GL_LINES);
        for(int i = 0; i < 12; i++)
        {
            float a = 2 * PI * i / 12;
            glVertex2f(sunX + 33.6 * cos(a), 650 + 33.6 * sin(a));
            glVertex2f(sunX + 52.8 * cos(a), 650 + 52.8 * sin(a));
        }
        glEnd();
        glLineWidth(1.0);
        glColor3f(1.0, 0.96, 0.22);
        drawCircle(sunX, 650, 28.8);
    }
    else
    {
        glColor3f(0.92, 0.90, 0.78);
        drawCircle(1100, 660, 30.4);
        glColor3f(0.01, 0.02, 0.12);
        drawCircle(1088.8, 666.4, 25.6);
    }
}

// ===== DRAW CLOUDS =====
void drawClouds()
{
    drawOneCloud(cloudX1,       660);
    drawOneCloud(cloudX2,       680);
    drawOneCloud(cloudX3,       650);
}


void drawWindow(float x, float y, float w, float h)
{
    if(nightMode)
    {
        srand((int)(x * 13) ^ (int)(y * 7));
        int lit = (rand() % 4 != 0);
        if(lit)
            glColor3f(1.0f, 0.90f, 0.48f);  // warm yellow glow
        else
            glColor3f(0.06f, 0.06f, 0.10f); // dark off window
    }
    else
    {
        glColor3f(0.50f, 0.76f, 0.96f);     // day: light blue glass
    }
    drawRect(x, y, x + w, y + h);

    // window frame border
    glColor3f(0.26f, 0.26f, 0.28f);
    glLineWidth(0.8f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x,     y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x,     y + h);
    glEnd();
    glLineWidth(1.0f);
}


void drawBitmapText(float x, float y, const char* text,
                    void* font = GLUT_BITMAP_HELVETICA_18)
{
    glRasterPos2f(x, y);
    for(int i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(font, text[i]);
}


void drawLabelPanel(float px1, float py1, float px2, float py2,
                    const char* text)
{
    // Panel background (off-white)
    glColor3f(0.94, 0.92, 0.88);
    drawRect(px1, py1, px2, py2);

    // Panel border
    glColor3f(0.25, 0.23, 0.20);
    glLineWidth(1.4);
    drawRectLine(px1, py1, px2, py2);
    glLineWidth(1.0);

    // Text (dark)
    glColor3f(0.12, 0.10, 0.10);
    // Rough centering: estimate 9px per character for Helvetica-18
    float textW = 0;
    for(int i = 0; text[i] != '\0'; i++) textW += 10.5;
    float panelCX = (px1 + px2) * 0.5 - textW * 0.5;
    float panelCY = (py1 + py2) * 0.5 - 6;
    drawBitmapText(panelCX, panelCY, text);
}


void drawClockFace(float ccX, float ccY, float cR)
{
    // Clock background (cream)
    glColor3f(0.96, 0.94, 0.90);
    drawCircle(ccX, ccY, cR);

    // Outer rim
    glColor3f(0.22, 0.20, 0.18);
    glLineWidth(2.8);
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 60; i++)
    {
        float a = 2 * PI * i / 60;
        glVertex2f(ccX + cR * cos(a), ccY + cR * sin(a));
    }
    glEnd();
    glLineWidth(1.0);

    // Hour tick marks (12 ticks)
    for(int i = 0; i < 12; i++)
    {
        float a  = 2 * PI * i / 12 - PI * 0.5;  // 12 o'clock = top
        float r0 = cR * 0.80;
        float r1 = cR * 0.96;
        glColor3f(0.20, 0.18, 0.18);
        glLineWidth(i % 3 == 0 ? 2.2 : 1.0);    // thicker at 12,3,6,9
        glBegin(GL_LINES);
        glVertex2f(ccX + r0 * cos(a), ccY + r0 * sin(a));
        glVertex2f(ccX + r1 * cos(a), ccY + r1 * sin(a));
        glEnd();
    }
    glLineWidth(1.0);

    // Hour numbers (small bitmap text)
    const char* nums[] = {"6","5","4","3","2","1","12","11","10","9","8","7"};
    for(int i = 0; i < 12; i++)
    {
        float a  = 2 * PI * i / 12 - PI * 0.5;
        float nr = cR * 0.64;
        float nx = ccX + nr * cos(a) - 5;
        float ny = ccY + nr * sin(a) - 4;
        glColor3f(0.18, 0.16, 0.14);
        drawBitmapText(nx, ny, nums[i], GLUT_BITMAP_HELVETICA_10);
    }

    // Minute hand
    float minAngle  = -PI * 0.5 + clockMin * PI / 180.0;
    glColor3f(0.14, 0.14, 0.16);
    glLineWidth(2.5);
    glBegin(GL_LINES);
    glVertex2f(ccX, ccY);
    glVertex2f(ccX + cR * 0.80 * cos(minAngle),
               ccY + cR * 0.80 * sin(minAngle));
    glEnd();

    // Hour hand
    float hourAngle = -PI * 0.5 + (clockMin / 12.0) * PI / 180.0;
    glColor3f(0.16, 0.14, 0.12);
    glLineWidth(3.8);
    glBegin(GL_LINES);
    glVertex2f(ccX, ccY);
    glVertex2f(ccX + cR * 0.54 * cos(hourAngle),
               ccY + cR * 0.54 * sin(hourAngle));
    glEnd();

    // Center pivot dot
    glColor3f(0.12, 0.10, 0.10);
    drawCircle(ccX, ccY, 4.5);
    glLineWidth(1.0);
}


void drawWindowGrid(float bx, float by, float bw, float bh,
                    int cols, int rows,
                    float winW, float winH,
                    float padX, float padY,
                    float gapX, float gapY)
{
    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            float x = bx + padX + col * (winW + gapX);
            float y = by + padY + row * (winH + gapY);
            drawWindow(x, y, winW, winH);
        }
    }
}

/*drawWindowGrid( bx, by, bw, bh, cols, rows, winW, winH, padX, padY, gapX, gapY )
bx, by = building bottom‑left corner
bw, bh = building width & height
cols, rows = how many windows horizontally & vertically
winW, winH = each window's size
padX, padY = distance from building edge to first window
gapX, gapY = space between windows*/

void drawBuildingsDetails()
{
    // BUILDING 1 (0-80, 370-560) - 2 cols, 3 rows
    drawWindowGrid(0, 370, 80, 190, 2, 3, 18, 24, 8, 30, 24, 28);

    // BUILDING 2 (90-230, 370-620) - 3 cols, 3 rows
    drawWindowGrid(90, 430, 140, 250, 3, 3, 24, 28, 15, 25, 18, 22);

    // HOSPITAL (250-490, 370-590) - label + door + windows
    drawLabelPanel(290, 540, 450, 575, "HOSPITAL");

    // Hospital entrance door (center: x=370)
    glColor3f(0.42f, 0.62f, 0.80f);
    drawRect(340, 370, 400, 440);
    glColor3f(0.20f, 0.20f, 0.22f);
    glLineWidth(1.4f);
    drawRectLine(340, 370, 400, 440);
    glBegin(GL_LINES);
    glVertex2f(370, 370);
    glVertex2f(370, 440);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(340, 410);
    glVertex2f(400, 410);
    glEnd();
    glLineWidth(1.0f);

    drawWindowGrid(250, 340, 240, 220, 3, 2, 42, 32, 29, 110, 28, 20);

    // BUILDING 3 (420-570, 370-620) - 1 col, 4 rows
    drawWindowGrid(420, 370, 150, 250, 1, 4, 32, 30, 95, 30, 32, 26);

    // SCHOOL (590-760, 370-610) - label + door + windows
    drawLabelPanel(610, 565, 740, 595, "SCHOOL");

    // School entrance door (center: x=675)
    glColor3f(0.42f, 0.62f, 0.80f);
    drawRect(650, 370, 700, 440);
    glColor3f(0.20f, 0.20f, 0.22f);
    glLineWidth(1.4f);
    drawRectLine(650, 370, 700, 440);
    glBegin(GL_LINES);
    glVertex2f(675, 370);
    glVertex2f(675, 440);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(650, 410);
    glVertex2f(700, 410);
    glEnd();
    glLineWidth(1.0f);

    drawWindowGrid(590, 330, 170, 240, 3, 2, 30, 34, 16, 140, 20, 15);

    // CLOCK TOWER (795-915, 370-620) - clock face + windows below
    drawClockFace(855, 560, 45);
    drawWindowGrid(795, 340, 120, 250, 2, 2, 30, 30, 25, 70, 15, 15);

    // MALL (935-1105, 370-630) - label, door, and windows
    drawLabelPanel(970, 585, 1070, 615, "MALL");
    // Entrance door
    glColor3f(0.42f, 0.62f, 0.80f);
    drawRect(1000, 370, 1050, 440);
    glColor3f(0.20f, 0.20f, 0.22f);
    glLineWidth(1.4f);
    drawRectLine(1000, 370, 1050, 440);
    glBegin(GL_LINES);
    glVertex2f(1025, 370);
    glVertex2f(1025, 440);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(1000, 410);
    glVertex2f(1050, 410);
    glEnd();
    glLineWidth(1.0f);
    // windows (3 cols, 2 rows)
    drawWindowGrid(935, 460, 90, 32, 3, 2, 28, 32, 20, 10, 25, 20);

    // BUILDING 4 (1125-1210, 370-610) - 3 cols, 4 rows
    drawWindowGrid(1125, 370, 85, 240, 3, 4, 14, 28, 8, 25, 12, 24);

    // BUILDING 5 (1210-1280, 370-570) - 2 cols, 4 rows
    drawWindowGrid(1210, 370, 70, 200, 2, 4, 14, 20, 16, 25, 14, 20);
}



// ===== DRAW Buildings =====
void drawBuildings()
{
    // Building 1
    glColor3f(0.72, 0.74, 0.76);
    drawRect(0,370, 80, 560);

    // Building 2
    glColor3f(0.47f, 0.54f, 0.57f);
    drawRect(90,370, 230, 620);

    // Building 3
    glColor3f(0.65f, 0.31f, 0.25f);
    drawRect(420,370, 570, 620);
    glColor3f(0, 0, 0);
    drawRectLine(420,370, 570, 620);

    // HOSPITAL
    glColor3f(0.54f, 0.64f, 0.68f);
    drawRect(250,370, 490, 590);

    // SCHOOL
    glColor3f(0.80f, 0.70f, 0.58f);
    drawRect(590,370, 760, 610);

    // CLOCK TOWER
    glColor3f(0.79f, 0.63f, 0.47f);
    drawRect(795,370, 915, 620);

    // MALL
    glColor3f(0.60f, 0.72f, 0.83f);
    drawRect(935,370, 1105, 630);

    // Building 4
    glColor3f(0.76f, 0.68f, 0.67f);
    drawRect(1125,370, 1210, 610);

    // Building 5
    glColor3f(0.72, 0.74, 0.76);
    drawRect(1210,370, 1280, 570);


    glColor3f(0, 0, 0);
    drawRectLine(0,370, 80, 560);
    drawRectLine(90,370, 230, 620);
    drawRectLine(250,370, 490, 590);
    drawRectLine(590,370, 760, 610);
    drawRectLine(795,370, 915, 620);
    drawRectLine(935,370, 1105, 630);
    drawRectLine(1125,370, 1210, 610);
    drawRectLine(1210,370, 1280, 570);
}

// ===== DRAW ROAD =====
void drawRoad()
{
    // Road Surface
    glColor3f(0.24, 0.24, 0.26);
    drawRect(0, 185, 1280, 370);

    // Boder lines
    glColor3f(0.82, 0.82, 0.80);
    drawRect(0,370, 1280, 373);

    // Double Yellow Center Line
    float cy = 277.5;
    glColor3f(0.96, 0.86, 0.08);
    glLineWidth(2.4);
    glBegin(GL_LINES);
    glVertex2f(0, cy + 5);
    glVertex2f(1280, cy + 5);
    glVertex2f(0, cy - 5);
    glVertex2f(1280, cy - 5);
    glEnd();

    // Road Dash
    glColor3f(0.88, 0.88, 0.86);
    glLineWidth(2.2);

    for(float dx = 0; dx < 1280; dx += 110)
    {
        glBegin(GL_LINES);
        glVertex2f(dx, cy + 45);
        glVertex2f(dx + 46, cy + 45);
        glEnd();
    }

    for(float dx = 0; dx < 1280; dx += 110)
    {
        glBegin(GL_LINES);
        glVertex2f(dx, cy - 45);
        glVertex2f(dx + 46, cy - 45);
        glEnd();
    }

    glLineWidth(1.0);
}


// ===== DRAW elevated bridge =====
void drawMetroTrack()
{
    glColor3f(0.88, 0.88, 0.86);

    // 1st pillar (higher)
    glColor3f(0.84f, 0.82f, 0.77f);
    drawRect(320, 197, 345, 370);
    glColor3f(0.0, 0.0, 0.0);
    drawRectLine(320, 197, 345, 370);

    glBegin(GL_QUADS);
    glColor3f(0.84f, 0.82f, 0.77f);
    glVertex2f(320, 370);
    glVertex2f(345, 370);
    glVertex2f(375, 415);
    glVertex2f(290, 415);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(320, 370);
    glVertex2f(345, 370);
    glVertex2f(375, 415);
    glVertex2f(290, 415);
    glEnd();

    // 2nd pillar
    glColor3f(0.84f, 0.82f, 0.77f);
    drawRect(640, 197, 665, 370);
    glColor3f(0.0, 0.0, 0.0);
    drawRectLine(640, 197, 665, 370);

    glBegin(GL_QUADS);
    glColor3f(0.84f, 0.82f, 0.77f);
    glVertex2f(640, 370);
    glVertex2f(665, 370);
    glVertex2f(695, 415);
    glVertex2f(610, 415);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(640, 370);
    glVertex2f(665, 370);
    glVertex2f(695, 415);
    glVertex2f(610, 415);
    glEnd();

    // 3rd pillar
    glColor3f(0.84f, 0.82f, 0.77f);
    drawRect(960, 197, 985, 370);
    glColor3f(0.0, 0.0, 0.0);
    drawRectLine(960, 197, 985, 370);

    glBegin(GL_QUADS);
    glColor3f(0.84f, 0.82f, 0.77f);
    glVertex2f(960, 370);
    glVertex2f(985, 370);
    glVertex2f(985+30, 415);
    glVertex2f(960-30, 415);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(960, 370);
    glVertex2f(985, 370);
    glVertex2f(985+30, 415);
    glVertex2f(960-30, 415);
    glEnd();

    // Bridge floor
    glColor3f(0.74f, 0.70f, 0.64f);
    drawRect(0, 415, 1280, 430);
    glColor3f(0.0, 0.0, 0.0);
    drawRectLine(0, 415, 1280, 430);
}

// ===== DRAW ONE TRAIN BOGI (sits ON the bridge, no 'M' logo) =====
void drawTrainBogi(float cx, int isEngine)
{
    float carW = 190;
    float carH = 65;
    float cy   = 419;   // bridge floor is at 415, train starts at 419 so wheels rest on bridge

    // Body (white/light gray)
    glColor3f(0.92, 0.93, 0.95);
    drawRect(cx, cy, cx + carW, cy + carH);
    glColor3f(0.20, 0.20, 0.22);
    drawRectLine(cx, cy, cx + carW, cy + carH);

    // Blue stripe (bottom)
    glColor3f(0.22, 0.38, 0.72);
    drawRect(cx, cy, cx + carW, cy + 12);
    glColor3f(0.22, 0.38, 0.72);
    drawRect(cx, cy + carH - 10, cx + carW, cy + carH);

    // Windows (5 per bogi)
    float wW = 26;
    float wH = 24;
    float wY = cy + 22;
    for(int i = 0; i < 5; i++)
    {
        float wx = cx + 14 + i * (wW + 10);
        if(wx + wW > cx + carW - 10) break;

        if(nightMode)
            glColor3f(1.0, 0.92, 0.48);
        else
            glColor3f(0.72, 0.88, 0.98);
        drawRect(wx, wY, wx + wW, wY + wH);
        glColor3f(0.20, 0.22, 0.26);
        drawRectLine(wx, wY, wx + wW, wY + wH);
    }

    // Engine nose (front bogi only)
    if(isEngine)
    {
        // Sloped nose
        glColor3f(0.88, 0.90, 0.92);
        glBegin(GL_POLYGON);
        glVertex2f(cx + carW,      cy);
        glVertex2f(cx + carW + 28, cy + carH * 0.50);
        glVertex2f(cx + carW,      cy + carH);
        glEnd();
        glColor3f(0.20, 0.20, 0.22);
        glBegin(GL_LINE_LOOP);
        glVertex2f(cx + carW,      cy);
        glVertex2f(cx + carW + 28, cy + carH * 0.50);
        glVertex2f(cx + carW,      cy + carH);
        glEnd();

        // Blue stripe on nose
        glColor3f(0.22, 0.38, 0.72);
        glBegin(GL_QUADS);
        glVertex2f(cx + carW,      cy);
        glVertex2f(cx + carW + 28, cy + carH * 0.50);
        glVertex2f(cx + carW + 14, cy + carH * 0.50);
        glVertex2f(cx + carW,      cy + 12);
        glEnd();

        // Windshield
        glColor3f(0.65, 0.85, 0.96);
        glBegin(GL_POLYGON);
        glVertex2f(cx + carW + 2,  cy + carH * 0.28);
        glVertex2f(cx + carW + 22, cy + carH * 0.50);
        glVertex2f(cx + carW + 2,  cy + carH * 0.72);
        glEnd();

        // Headlights
        glColor3f(1.0, 1.0, 0.72);
        drawCircle(cx + carW + 20, cy + carH * 0.22, 5);
        drawCircle(cx + carW + 20, cy + carH * 0.78, 5);

        // METRO text on front bogi
        glColor3f(0.22, 0.38, 0.72);
        glRasterPos2f(cx + 8, cy + 4);
        char* txt = "METRO";
        for(int i = 0; txt[i] != '\0'; i++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, txt[i]);
    }

    // Coupler between bogies
    glColor3f(0.40, 0.40, 0.42);
    drawRect(cx + carW, cy + 15, cx + carW + 8, cy + carH - 15);
}

// ===== DRAW TRAIN (3 bogies + engine) =====
void drawTrain()
{
    float gap  = 8;
    float carW = 190;

    drawTrainBogi(trainX,                   0);  // rear bogi
    drawTrainBogi(trainX + carW + gap,      0);  // middle bogi
    drawTrainBogi(trainX + 2*(carW + gap),  0);  // front bogi
    drawTrainBogi(trainX + 3*(carW + gap),  1);  // engine (with nose)
}

// ===== DRAW CAR (moving right) =====
void drawCarRight(float cx, float cy, float r, float g, float b)
{
    float w = 115, h = 36;

    // Body
    glColor3f(r, g, b);
    drawRect(cx, cy, cx + w, cy + h);

    // Cabin
    float cabW = w * 0.55;
    float cabX = cx + w * 0.18;
    glColor3f(r*0.84, g*0.84, b*0.84);
    drawRect(cabX, cy + h, cabX + cabW, cy + h*1.55);

    // Windshield
    glColor3f(0.55, 0.82, 0.98);
    glBegin(GL_QUADS);
    glVertex2f(cabX + 6,         cy + h);
    glVertex2f(cabX + cabW - 6,  cy + h);
    glVertex2f(cabX + cabW - 9,  cy + h*1.48);
    glVertex2f(cabX + 9,         cy + h*1.48);
    glEnd();

    // Headlight (right side = front for rightward car)
    glColor3f(0.96, 0.96, 0.72);
    drawCircle(cx + w - 4, cy + h*0.58, 7);

    // Tail light (left side)
    glColor3f(0.92, 0.08, 0.08);
    drawCircle(cx + 4, cy + h*0.58, 5);

    // Wheels
    glColor3f(0.14, 0.14, 0.16);
    drawCircle(cx + w*0.22, cy, 14);
    drawCircle(cx + w*0.78, cy, 14);
    glColor3f(0.50, 0.50, 0.54);
    drawCircle(cx + w*0.22, cy, 7);
    drawCircle(cx + w*0.78, cy, 7);
}

// ===== DRAW CAR (moving left) =====
void drawCarLeft(float cx, float cy, float r, float g, float b)
{
    float w = 115, h = 36;

    // Body
    glColor3f(r, g, b);
    drawRect(cx, cy, cx + w, cy + h);

    // Cabin
    float cabW = w * 0.55;
    float cabX = cx + w * 0.27;
    glColor3f(r*0.84, g*0.84, b*0.84);
    drawRect(cabX, cy + h, cabX + cabW, cy + h*1.55);

    // Windshield
    glColor3f(0.55, 0.82, 0.98);
    glBegin(GL_QUADS);
    glVertex2f(cabX + 6,        cy + h);
    glVertex2f(cabX + cabW - 6, cy + h);
    glVertex2f(cabX + cabW - 9, cy + h*1.48);
    glVertex2f(cabX + 9,        cy + h*1.48);
    glEnd();

    // Headlight (left side = front for leftward car)
    glColor3f(0.96, 0.96, 0.72);
    drawCircle(cx + 4, cy + h*0.58, 7);

    // Tail light (right side)
    glColor3f(0.92, 0.08, 0.08);
    drawCircle(cx + w - 4, cy + h*0.58, 5);

    // Wheels
    glColor3f(0.14, 0.14, 0.16);
    drawCircle(cx + w*0.22, cy, 14);
    drawCircle(cx + w*0.78, cy, 14);
    glColor3f(0.50, 0.50, 0.54);
    drawCircle(cx + w*0.22, cy, 7);
    drawCircle(cx + w*0.78, cy, 7);
}

// ===== DRAW TRUCK (moving left) =====
void drawTruck(float cx, float cy)
{
    float w = 185, h = 36;

    // Cargo box
    float boxW = w * 0.66;
    glColor3f(0.62, 0.56, 0.36);
    drawRect(cx + w*0.34, cy, cx + w, cy + h*1.62);

    // Cab
    glColor3f(0.30, 0.40, 0.56);
    drawRect(cx, cy, cx + w*0.34, cy + h*1.40);

    // Cab window
    glColor3f(0.55, 0.82, 0.98);
    drawRect(cx + 8, cy + h*0.80, cx + w*0.34 - 8, cy + h*1.30);

    // Headlight (left = front for leftward truck)
    glColor3f(0.96, 0.96, 0.68);
    drawCircle(cx + 4, cy + h*0.54, 8);

    // Wheels (3 axles)
    glColor3f(0.14, 0.14, 0.16);
    drawCircle(cx + w*0.10, cy, 17);
    drawCircle(cx + w*0.52, cy, 17);
    drawCircle(cx + w*0.84, cy, 17);
    glColor3f(0.46, 0.46, 0.50);
    drawCircle(cx + w*0.10, cy, 8);
    drawCircle(cx + w*0.52, cy, 8);
    drawCircle(cx + w*0.84, cy, 8);
}

// ===== DRAW VEHICLES =====
void drawVehicles()
{
    // Road top = 355, Road bottom = 185
    // Centre line Y = 270
    // Top lane (going right): Y around 282 (above centre)
    // Bottom lane (going left): Y around 203 (below centre)

    float topLaneY = 307;
    float botLaneY = 230;

    // Car 1: red, going right, top lane
    drawCarRight(car1X, topLaneY, 0.78, 0.22, 0.22);

    // Car 2: blue, going right, top lane
    drawCarRight(car2X, topLaneY, 0.22, 0.50, 0.80);

    // Car 3: green, going left, bottom lane
    drawCarLeft(car3X - 115, botLaneY, 0.20, 0.64, 0.32);

    // Truck: going left, bottom lane
    drawTruck(truckX - 185, botLaneY);
}



// ===== DRAW RIVER =====
void drawRiver()
{
    // RIVER WATER
    glColor3f(0.06, 0.20, 0.50);
    drawRect(0, 0, 1280, 185);

    // WAVE RIPPLES
    glColor3f(0.28, 0.50, 0.82);
    glLineWidth(1.4);
    for(int row = 0; row < 6; row++)
    {
        float ry  = 167 - row * 28;
        float amp = 5.5 - row * 0.5;

        glBegin(GL_LINE_STRIP);
        for(int i = 0; i <= 80; i++)
        {
            float rx = i * (1280.0 / 80.0);
            float wy = ry + amp * sin(waveAngle + rx*0.018 + row*1.1);
            glVertex2f(rx, wy);
        }
        glEnd();
    }
    glLineWidth(1.0);

    // River banks
    glColor3f(0.36, 0.30, 0.20);
    drawRect(0, 185, 1280, 199);

    // Quay green strip
    glColor3f(0.28, 0.44, 0.20);
    drawRect(0, 197, 1280, 215);
    glColor3f(0, 0, 0);
    drawRectLine(0, 197, 1280, 215);
}

// ===== DRAW SHIP =====
void drawShip()
{
    float sx = shipX;
    float wl = 133;   // waterline Y

    glPushMatrix();
    glTranslatef(sx, 0.0, 0.0); // ship ke run korbe left to right

    // ship er nicher part
    glColor3f(0.26, 0.22, 0.18);
    glBegin(GL_POLYGON);
    glVertex2f(-240, wl);
    glVertex2f( 240, wl);
    glVertex2f( 255, wl - 28);
    glVertex2f( 240, wl - 52);
    glVertex2f(-240, wl - 52);
    glVertex2f(-256, wl - 30);
    glEnd();

    // ship er nicher part take border dichi
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-240, wl);
    glVertex2f( 240, wl);
    glVertex2f( 255, wl - 28);
    glVertex2f( 240, wl - 52);
    glVertex2f(-240, wl - 52);
    glVertex2f(-256, wl - 30);
    glEnd();

    // ship er nicher red part
    glColor3f(0.78, 0.14, 0.14);
    glBegin(GL_QUADS);
    glVertex2f(-240, wl);
    glVertex2f( 240, wl);
    glVertex2f( 248, wl - 16);
    glVertex2f( -248, wl - 16);
    glEnd();

    // ship er nicher red part take border dichi
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-240, wl);
    glVertex2f( 240, wl);
    glVertex2f( 248, wl - 16);
    glVertex2f( -248, wl - 16);
    glEnd();

    // ---- Main deck ----
    glColor3f(0.78, 0.76, 0.72);
    drawRect(-195, wl, 197, wl + 38);
    glColor3f(0.0, 0.0, 0.0);
    drawRectLine(-195, wl, 197, wl + 38);

    // ---- Colorful cargo containers ----

    // Red container
    glColor3f(0.80, 0.18, 0.18);
    drawRect(-178, wl + 4, -128, wl + 38);
    glColor3f(0.16, 0.16, 0.18);
    drawRectLine(-178, wl + 4, -128, wl + 38);

    // Blue container
    glColor3f(0.18, 0.50, 0.82);
    drawRect(-124, wl + 4, -74, wl + 38);
    glColor3f(0.16, 0.16, 0.18);
    drawRectLine(-124, wl + 4, -74,  wl + 38);

    // Orange container
    glColor3f(0.80, 0.62, 0.12);
    drawRect(-70, wl + 4, -20, wl + 38);
    glColor3f(0.16, 0.16, 0.18);
    drawRectLine(-70, wl + 4, -20, wl + 38);

    // Green container
    glColor3f(0.22, 0.68, 0.26);
    drawRect(-16, wl + 4, 34, wl + 38);
    glColor3f(0.16, 0.16, 0.18);
    drawRectLine(-16, wl + 4, 34, wl + 38);

    // Purple container
    glColor3f(0.72, 0.34, 0.72);
    drawRect(38, wl + 4, 88, wl + 38);
    glColor3f(0.16, 0.16, 0.18);
    drawRectLine(38, wl + 4, 88, wl + 38);

    // Brown orange container
    glColor3f(0.82, 0.46, 0.18);
    drawRect(92, wl + 4, 142, wl + 38);
    glColor3f(0.16, 0.16, 0.18);
    drawRectLine(92, wl + 4, 142, wl + 38);

    // Light blue container
    glColor3f(0.28, 0.60, 0.84);
    drawRect(146, wl + 4, 196, wl + 38);
    glColor3f(0.16, 0.16, 0.18);
    drawRectLine(146, wl + 4, 196, wl + 38);

    glColor3f(0.50, 0.50, 0.50);  // 50% black color
    glLineWidth(1.0);
    glBegin(GL_LINES);

    // toal 7 ta container
    for (int i = 0; i < 7; i++)
    {
        // 1st container start hoiche -178 theke and protita container er majer gap 54
        float startX = -178 + (i * 54);

        // protita container e 5 ta kore strip thakbe.
        for (int j = 1; j <= 5; j++)
        {
            float lineX = startX + (j * 8);  // strip gulo 8px dure bosbe

            glVertex2f(lineX, wl + 4);
            glVertex2f(lineX, wl + 38);
        }
    }

    glEnd();

    // ship er control room
    glColor3f(0.70, 0.68, 0.64);
    drawRect(-62, wl + 38, 72, wl + 96);
    glColor3f(0.0, 0.0, 0.0);
    drawRectLine(-62, wl + 38, 72, wl + 96);


    // control room windows
    for(int i = 0; i < 4; i++)
    {
        float bwX = -52 + i * 24;
        if(nightMode)
            glColor3f(0.90, 0.92, 0.48);
        else
            glColor3f(0.52, 0.80, 0.98);
        drawRect(bwX, wl + 54, bwX + 18, wl + 76);
        glColor3f(0.0, 0.0, 0.0);
        drawRectLine(bwX, wl + 54, bwX + 18, wl + 76);
    }

    // control room door
    glColor3f(0.45, 0.15, 0.05);
    drawRect(44, wl + 38, 47 + 18, wl + 82);
    glColor3f(0.0, 0.0, 0.0);
    drawRectLine(44, wl + 38, 47 + 18, wl + 82);

    // Chimneys
    glColor3f(0.26, 0.24, 0.22);
    drawRect(-28, wl + 96, -8,  wl + 148);
    drawRect( 10, wl + 96,  30, wl + 132);
    glColor3f(0.0, 0.0, 0.0);
    drawRectLine(-28, wl + 96, -8,  wl + 148);
    drawRectLine( 10, wl + 96,  30, wl + 132);

    // Funnel color bands
    glColor3f(0.82, 0.14, 0.14);
    drawRect(-28, wl + 138, -8,  wl + 148);
    drawRect( 10, wl + 124,  30, wl + 132);
    glColor3f(0.0, 0.0, 0.0);
    drawRectLine(-28, wl + 138, -8,  wl + 148);
    drawRectLine( 10, wl + 124,  30, wl + 132);

    // Smoke
    glColor3f(0.68, 0.66, 0.64);
    drawCircle(-18, wl + 162, 12);
    drawCircle(-14, wl + 178,  8);
    drawCircle( 20, wl + 150, 10);
    drawCircle( 22, wl + 164,  7);

    // control room er roof
    glColor3f(0.55, 0.54, 0.54);
    drawRect(-66, wl + 90, 76, wl + 98);
    glColor3f(0.0, 0.0, 0.0);
    drawRectLine(-66, wl + 90, 76, wl + 98);

    // ---- Mast ----
    glColor3f(0.34, 0.32, 0.30);
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glVertex2f(85, wl + 38);
    glVertex2f(85, wl + 130);
    glEnd();
    glLineWidth(1.0);

    // Mast light (red at night)
    if(nightMode)
    {
        glColor3f(0.90, 0.10, 0.10);
        drawCircle(85, wl + 132, 5);
    }

    glPopMatrix();
}


// ===== HELPER: DRAW TEXT =====
void drawText(float x, float y, const char* text)
{
    glRasterPos2f(x, y);
    for(int i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
}

void drawTextBig(float x, float y, const char* text)
{
    glRasterPos2f(x, y);
    for(int i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}
// ===== DRAW HUD (keyboard hints) =====
void drawHUD()
{
    // Semi-transparent dark panel at bottom right
    glColor4f(0.04, 0.06, 0.14, 0.58);
    drawRect(810, 0, 1280, 40);

    glColor3f(0.88, 0.92, 0.78);
    drawText(820, 24, "W/S=train speed   D/A=car speed   X/Z=ship speed");
    drawText(820,  8, "N=night/day   P=pause   R=reset   ESC=exit");

    // Mode label
    if(paused)
    {
        glColor3f(1.0, 0.4, 0.4);
        drawText(820, 700, "[ PAUSED ]");
    }
    else if(nightMode)
    {
        glColor3f(0.6, 0.7, 1.0);
        drawText(820, 700, "[ NIGHT MODE ]");
    }
    else
    {
        glColor3f(0.6, 1.0, 0.6);
        drawText(820, 700, "[ DAY MODE ]");
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawSky();
    drawSun();
    drawClouds();

    drawBuildings();
    drawBuildingsDetails();

    drawRoad();
    drawVehicles();

    drawTrain();
    drawRiver();
    drawMetroTrack();
    drawShip();

    drawHUD();
    glutSwapBuffers();
}


//   TIMER CALLBACK

void timerTick(int value)
{
    if(!paused)
    {
        // Sun moves right
        sunX += 0.18;
        if(sunX > 1360) sunX = -80;

        // Clouds drift right slowly
        cloudX1 += 0.55;
        if(cloudX1 > 1480) cloudX1 = -200;
        cloudX2 += 0.75;
        if(cloudX2 > 1480) cloudX2 = -200;
        cloudX3 += 0.60;
        if(cloudX3 > 1480) cloudX3 = -200;

        // Clock minute hand
        clockMin -= 0.10;
        if(clockMin <= 360) clockMin += 360;

        // Train (wraps left when out of screen)
        trainX -= trainSpeed;
        if(trainX < -940) trainX =  1310;

        // Cars and truck
        car1X  += carSpeed;
        car2X  += carSpeed * 1.2;
        car3X  -= carSpeed;
        truckX -= carSpeed * 0.76;

        if(car1X  > 1400)  car1X  = -120;
        if(car2X  > 1400)  car2X  = -120;
        if(car3X  < -120)  car3X  = 1400;
        if(truckX < -190)  truckX = 1470;

        // Ship
        shipX -= shipSpeed;
        if(shipX < -300 ) shipX = 1560;

        // Water waves
        waveAngle += 0.042;
        if(waveAngle > 2 * PI * 4) waveAngle -= 2 * PI * 4;
    }

    glutPostRedisplay();
    glutTimerFunc(16, timerTick, 0);  // call again after 16ms
}


//   KEYBOARD CALLBACK

void keyboard(unsigned char key, int x, int y)
{
    if(key == 'w' || key == 'W')
    {
        trainSpeed += 0.5;
        if(trainSpeed > 12) trainSpeed = 12;
    }
    if(key == 's' || key == 'S')
    {
        trainSpeed -= 0.5;
        if(trainSpeed < 0.3) trainSpeed = 0.3;
    }

    if(key == 'd' || key == 'D')
    {
        carSpeed += 0.5;
        if(carSpeed > 10) carSpeed = 10;
    }
    if(key == 'a' || key == 'A')
    {
        carSpeed -= 0.5;
        if(carSpeed < 0.3) carSpeed = 0.3;
    }

    if(key == 'x' || key == 'X')
    {
        shipSpeed += 0.3;
        if(shipSpeed > 8) shipSpeed = 8;
    }
    if(key == 'z' || key == 'Z')
    {
        shipSpeed -= 0.3;
        if(shipSpeed < 0.2) shipSpeed = 0.2;
    }

    if(key == 'p' || key == 'P')
        paused = !paused;

    if(key == 'n' || key == 'N')
        nightMode = !nightMode;

    if(key == 'r' || key == 'R')
    {
        // Reset everything
        paused     = 0;
        nightMode  = 0;
        trainX     = -630;
        trainSpeed = 2.2;
        car1X      = 80;
        car2X      = 680;
        car3X      = 350;
        truckX     = 1100;
        carSpeed   = 2.5;
        shipX      = -300;
        shipSpeed  = 1.5;
        sunX       = 200;
    }

    if(key == 27)  // ESC
        exit(0);

    glutPostRedisplay();
}

void init()
{
    // amar canvas er color // Sky color
    glClearColor(0.22, 0.55, 0.92, 1.0);

    //glColor4f er 4th value alpha er kaj korbe
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Line er edge gulo smooth/anti-aliased hoy
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // point gulo smooth hoy
    glEnable(GL_POINT_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //coordination system
    gluOrtho2D(0, 1280, 0, 720);

    // glPushMatrix/glPopMatrix use korar jonno dorkar
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(120, 50);
    glutCreateWindow("Multi-Level Smart City");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    // ei lilne ta new animation timer suru kore
    glutTimerFunc(16, timerTick, 0);

    glutMainLoop();
    return 0;
}

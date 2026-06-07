#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

const int WIDTH=800, HEIGHT=600;
float globalTime=0,personOffset=0,headBob=0,gesturePhase=0,cloudOffset=0;
int direction=1;
struct Color
{
    float r,g,b;
};
Color bullColors[]= {{.9f,.9f,.9f},{.4f,.2f,.1f},{.1f,.1f,.1f},{.8f,.5f,.2f}};
GLuint bgList,fenceList;
bool listsCreated=false,isNight=false,isRaining=false,bubbleOpen=false;
float cosTable[360],sinTable[360];

void precomputeTables()
{
    for(int i=0; i<360; i++)
    {
        cosTable[i]=cosf(i*3.14159f/180.f);
        sinTable[i]=sinf(i*3.14159f/180.f);
    }
}

void drawCircle(float cx,float cy,float r,int seg)
{
    glBegin(GL_POLYGON);
    int s=360/seg;
    for(int i=0; i<360; i+=s)glVertex2f(cx+r*cosTable[i],cy+r*sinTable[i]);
    glEnd();
}

void renderBitmapString(float x,float y,void*f,const char*s)
{
    glRasterPos2f(x,y);
    for(const char*c=s; *c; c++)glutBitmapCharacter(f,*c);
}

void drawSpeechBubble(float sx)
{
    if(!bubbleOpen)return;
    glColor3f(1,1,1);
    glBegin(GL_TRIANGLES);
    glVertex2f(sx-5,90);
    glVertex2f(sx+5,90);
    glVertex2f(sx,78);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(sx-55,90);
    glVertex2f(sx+55,90);
    glVertex2f(sx+55,124);
    glVertex2f(sx-55,124);
    glEnd();
    glColor3f(0,0,0);
    glLineWidth(1.2f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(sx-55,90);
    glVertex2f(sx-5,90);
    glVertex2f(sx,78);
    glVertex2f(sx+5,90);
    glVertex2f(sx+55,90);
    glVertex2f(sx+55,124);
    glVertex2f(sx-55,124);
    glVertex2f(sx-55,90);
    glEnd();
    renderBitmapString(sx-44,102,GLUT_BITMAP_HELVETICA_12,"Eiii Jhalmuriii!");
}

void drawOriginalHead(Color p,Color s,Color a,Color eb,Color pu)
{
    glColor3f(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(-22,32);
    glVertex2f(-26,45);
    glVertex2f(-38,65);
    glVertex2f(-48,80);
    glVertex2f(-54,85);
    glVertex2f(-44,84);
    glVertex2f(-32,72);
    glVertex2f(-20,55);
    glVertex2f(-12,34);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(22,32);
    glVertex2f(26,45);
    glVertex2f(38,65);
    glVertex2f(48,80);
    glVertex2f(54,85);
    glVertex2f(44,84);
    glVertex2f(32,72);
    glVertex2f(20,55);
    glVertex2f(12,34);
    glEnd();
    glColor3f(p.r,p.g,p.b);
    glBegin(GL_POLYGON);
    glVertex2f(-28,15);
    glVertex2f(-48,25);
    glVertex2f(-72,15);
    glVertex2f(-82,6);
    glVertex2f(-64,-4);
    glVertex2f(-42,-1);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(28,15);
    glVertex2f(48,25);
    glVertex2f(72,15);
    glVertex2f(82,6);
    glVertex2f(64,-4);
    glVertex2f(42,-1);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(0,44);
    glVertex2f(-34,44);
    glVertex2f(-22,-38);
    glVertex2f(0,-38);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(0,44);
    glVertex2f(34,44);
    glVertex2f(22,-38);
    glVertex2f(0,-38);
    glEnd();
    glColor3f(s.r,s.g,s.b);
    glBegin(GL_QUADS);
    glVertex2f(-21,-34);
    glVertex2f(21,-34);
    glVertex2f(17,-60);
    glVertex2f(-17,-60);
    glEnd();
    glColor3f(a.r,a.g,a.b);
    glBegin(GL_TRIANGLES);
    glVertex2f(-20,-36);
    glVertex2f(0,-36);
    glVertex2f(-11,-57);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(20,-36);
    glVertex2f(0,-36);
    glVertex2f(11,-57);
    glEnd();
    glColor3f(p.r,p.g,p.b);
    glBegin(GL_QUADS);
    glVertex2f(-9,-48);
    glVertex2f(-3,-48);
    glVertex2f(-3,-54);
    glVertex2f(-9,-54);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(3,-48);
    glVertex2f(9,-48);
    glVertex2f(9,-54);
    glVertex2f(3,-54);
    glEnd();
    glColor3f(eb.r,eb.g,eb.b);
    drawCircle(-16,12,6,8);
    glColor3f(pu.r,pu.g,pu.b);
    drawCircle(-16,12,3.2f,6);
    glColor3f(eb.r,eb.g,eb.b);
    drawCircle(16,12,6,8);
    glColor3f(pu.r,pu.g,pu.b);
    drawCircle(16,12,3.2f,6);
}

void drawOriginalNeck(Color a)
{
    glColor3f(a.r,a.g,a.b);
    glBegin(GL_POLYGON);
    glVertex2f(-116,40);
    glVertex2f(-84,40);
    glVertex2f(-64,-14);
    glVertex2f(-96,-14);
    glEnd();
}

void drawOriginalBody(Color p,Color s,Color a)
{
    glColor3f(p.r,p.g,p.b);
    glBegin(GL_QUADS);
    glVertex2f(-92,36);
    glVertex2f(-20,36);
    glVertex2f(-20,-14);
    glVertex2f(-92,-14);
    glEnd();
    glColor3f(a.r,a.g,a.b);
    glBegin(GL_TRIANGLES);
    glVertex2f(-20,36);
    glVertex2f(-20,-14);
    glVertex2f(44,-14);
    glEnd();
    glColor3f(p.r,p.g,p.b);
    glBegin(GL_TRIANGLES);
    glVertex2f(-20,36);
    glVertex2f(44,36);
    glVertex2f(44,-14);
    glEnd();
    glColor3f(s.r,s.g,s.b);
    glBegin(GL_POLYGON);
    glVertex2f(44,36);
    glVertex2f(100,36);
    glVertex2f(110,32);
    glVertex2f(116,24);
    glVertex2f(116,15);
    glVertex2f(116,-14);
    glVertex2f(44,-14);
    glEnd();
    glColor3f(p.r,p.g,p.b);
    glBegin(GL_POLYGON);
    glVertex2f(-92,-14);
    glVertex2f(-20,-14);
    glVertex2f(-20,-72);
    glVertex2f(-75,-72);
    glVertex2f(-85,-65);
    glVertex2f(-92,-50);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(-20,-14);
    glVertex2f(44,-14);
    glVertex2f(-20,-72);
    glEnd();
    glColor3f(a.r,a.g,a.b);
    glBegin(GL_TRIANGLES);
    glVertex2f(44,-14);
    glVertex2f(44,-72);
    glVertex2f(-20,-72);
    glEnd();
    glColor3f(s.r,s.g,s.b);
    glBegin(GL_QUADS);
    glVertex2f(44,-14);
    glVertex2f(116,-14);
    glVertex2f(116,-72);
    glVertex2f(44,-72);
    glEnd();
}

void drawOriginalLeg(float px,Color s,Color p)
{
    float ty=px<-50?-50.f:-72.f;
    glColor3f(s.r,s.g,s.b);
    glBegin(GL_QUADS);
    glVertex2f(px-12,ty);
    glVertex2f(px+12,-72);
    glVertex2f(px+8,-100);
    glVertex2f(px-8,-100);
    glEnd();
    glColor3f(p.r,p.g,p.b);
    glBegin(GL_QUADS);
    glVertex2f(px-8,-100);
    glVertex2f(px+8,-100);
    glVertex2f(px+8,-180);
    glVertex2f(px-8,-180);
    glEnd();
}

void drawOriginalLegs(Color s,Color p)
{
    drawOriginalLeg(-78,s,p);
    drawOriginalLeg(-42,s,p);
    drawOriginalLeg(66,s,p);
    drawOriginalLeg(102,s,p);
}

void drawOriginalTail(float wa,Color s,Color p)
{
    glPushMatrix();
    glTranslatef(116,15,0);
    glRotatef(wa,0,0,1);
    glColor3f(s.r,s.g,s.b);
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-2,-1);
    glVertex2f(2,-1);
    glVertex2f(13,-1);
    glVertex2f(17,-7);
    glVertex2f(19,-10);
    glVertex2f(25,-14);
    glVertex2f(19,-30);
    glVertex2f(25,-34);
    glVertex2f(11,-46);
    glVertex2f(17,-50);
    glVertex2f(17,-62);
    glVertex2f(23,-66);
    glEnd();
    glColor3f(p.r,p.g,p.b);
    glBegin(GL_POLYGON);
    glVertex2f(20,-63);
    glVertex2f(10,-76);
    glVertex2f(24,-94);
    glVertex2f(38,-76);
    glEnd();
    glPopMatrix();
}

void drawBull(float x,float y,Color c,float scale,bool flip,float po=0)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    if(!flip)glScalef(-1,1,1);
    glScalef(scale,scale,1);
    float t=(globalTime+po*.002f)*.4f;
    float sw=16*sinf(t*4)-6,ht=-15+headBob*1.5f;
    Color mid= {c.r*.8f,c.g*.8f,c.b*.8f};
    Color light= {c.r*1.15f>1?1:c.r*1.15f,c.g*1.15f>1?1:c.g*1.15f,c.b*1.15f>1?1:c.b*1.15f};
    Color eyeBg= {1,1,1},pupil= {.05f,.05f,.05f};
    glColor4f(0,0,0,.18f);
    glBegin(GL_POLYGON);
    for(int i=0; i<360; i+=15)glVertex2f(100*cosTable[i],12*sinTable[i]-180);
    glEnd();
    drawOriginalTail(sw,mid,c);
    drawOriginalLegs(mid,c);
    drawOriginalNeck(light);
    drawOriginalBody(c,mid,light);
    glPushMatrix();
    glTranslatef(-130,50+headBob,0);
    glRotatef(ht,0,0,1);
    drawOriginalHead(c,mid,light,eyeBg,pupil);
    glPopMatrix();
    glPopMatrix();
}

void drawPerson(float x,float y,Color shirt,Color lungi,bool hg,float sm,float op,bool st)
{
    glPushMatrix();
    float io=st?0:(personOffset*sm)+op,sw=.1f*(st?1:sm),ip=x*.5f+y*.5f;
    float skinR=.8f,skinG=.6f,skinB=.4f;
    float as=sinf((st?(gesturePhase*3):io+gesturePhase*5)*sw+ip)*20;
    float ht=sinf((io+gesturePhase*5+ip)*.05f)*4;
    float ras=as,las=-as;
    float ls=st?sinf(gesturePhase*4+ip)*1.5f:sinf(io*sw+.5f)*25;
    if(st)
    {
        ras=15+sinf(gesturePhase*4+ip)*3;
        las=-10-sinf(gesturePhase*2+ip)*1.5f;
    }
    glTranslatef(x+io,y,0);
    glColor3f(skinR,skinG,skinB);
    glPushMatrix();
    glTranslatef(-4,-18,0);
    glRotatef(ls,0,0,1);
    glRectf(-2,0,2,-18);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(4,-18,0);
    glRotatef(-ls,0,0,1);
    glRectf(-2,0,2,-18);
    glPopMatrix();
    glColor3f(lungi.r,lungi.g,lungi.b);
    glBegin(GL_POLYGON);
    glVertex2f(-16,-20);
    glVertex2f(16,-20);
    glVertex2f(13,10);
    glVertex2f(-13,10);
    glEnd();
    glColor4f(0,0,0,.15f);
    glLineWidth(1);
    glBegin(GL_LINES);
    for(float lx=-14; lx<=14; lx+=4)
    {
        glVertex2f(lx,-20);
        glVertex2f(lx*.8f,10);
    }
    for(float ly=-16; ly<=8; ly+=5)
    {
        glVertex2f(-15,ly);
        glVertex2f(15,ly);
    }
    glEnd();
    glColor3f(shirt.r,shirt.g,shirt.b);
    glRectf(-14,10,14,35);
    glColor3f(skinR,skinG,skinB);
    glPushMatrix();
    glTranslatef(-14,30,0);
    glRotatef(las,0,0,1);
    glRectf(-4,0,1,-20);
    drawCircle(-1.5f,-22,3,6);
    glPopMatrix();
    float gr=st?0:30*sinf(gesturePhase);
    glPushMatrix();
    glTranslatef(14,30,0);
    glRotatef(ras+gr,0,0,1);
    glRectf(-1,0,4,-20);
    drawCircle(1.5f,-22,3,6);
    glPopMatrix();
    if(hg)
    {
        glColor3f(.8f,.2f,.2f);
        glRectf(-16,32,16,38);
        glRectf(-16,32,-10,15);
    }
    glPushMatrix();
    glTranslatef(ht,45,0);
    glColor3f(skinR,skinG,skinB);
    drawCircle(0,0,11,10);
    glColor3f(.1f,.1f,.1f);
    glRectf(2,-2,4,0);
    glRectf(6,-2,8,0);
    glColor3f(.1f,.05f,0);
    glBegin(GL_POLYGON);
    for(int i=0; i<=5; i++)
    {
        float t=3.14159f*i/5;
        glVertex2f(11*cosf(t),11*sinf(t));
    }
    glEnd();
    glPopMatrix();
    glPopMatrix();
}

void drawJhalmuriSeller(float cx,float cy)
{
    glPushMatrix();
    glTranslatef(cx,cy,0);
    float sr=204.f/255,sg=153.f/255,sb=102.f/255;
    glColor3f(sr,sg,sb);
    glBegin(GL_QUADS);
    glVertex2f(-6,-36);
    glVertex2f(-2,-36);
    glVertex2f(-2,-18);
    glVertex2f(-6,-18);
    glVertex2f(2,-36);
    glVertex2f(6,-36);
    glVertex2f(6,-18);
    glVertex2f(2,-18);
    glEnd();
    glColor3f(.18f,.49f,.2f);
    glBegin(GL_POLYGON);
    glVertex2f(-16,-20);
    glVertex2f(16,-20);
    glVertex2f(13,10);
    glVertex2f(-13,10);
    glEnd();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0,0,0,.16f);
    glLineWidth(1);
    glBegin(GL_LINES);
    for(float lx=-14; lx<=14; lx+=4)
    {
        glVertex2f(lx,-20);
        glVertex2f(lx*.8f,10);
    }
    for(float ly=-16; ly<=8; ly+=5)
    {
        glVertex2f(-15,ly);
        glVertex2f(15,ly);
    }
    glEnd();
    glColor3f(.93f,.94f,.95f);
    glBegin(GL_QUADS);
    glVertex2f(-14,10);
    glVertex2f(14,10);
    glVertex2f(14,35);
    glVertex2f(-14,35);
    glEnd();
    glColor4f(.25f,.35f,.45f,.45f);
    glBegin(GL_LINES);
    for(float sx=-12; sx<=12; sx+=4)
    {
        glVertex2f(sx,10);
        glVertex2f(sx,35);
    }
    for(float sy=13; sy<=33; sy+=4)
    {
        glVertex2f(-14,sy);
        glVertex2f(14,sy);
    }
    glEnd();
    glPushMatrix();
    glTranslatef(-12,30,0);
    glRotatef(-45,0,0,1);
    glColor3f(sr,sg,sb);
    glBegin(GL_QUADS);
    glVertex2f(-4,-18);
    glVertex2f(1,-18);
    glVertex2f(1,0);
    glVertex2f(-4,0);
    glEnd();
    drawCircle(-1.5f,-20,3,6);
    glColor3f(.84f,.18f,.18f);
    glBegin(GL_QUADS);
    glVertex2f(-5,-28);
    glVertex2f(2,-28);
    glVertex2f(2,-19);
    glVertex2f(-5,-19);
    glEnd();
    glColor3f(.92f,.27f,.27f);
    drawCircle(-1.5f,-28,3.5f,6);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(12,30,0);
    glRotatef(-35+sinf(globalTime*6)*15,0,0,1);
    glColor3f(sr,sg,sb);
    glBegin(GL_QUADS);
    glVertex2f(-1,-22);
    glVertex2f(4,-22);
    glVertex2f(4,0);
    glVertex2f(-1,0);
    glEnd();
    drawCircle(1.5f,-24,3,6);
    glColor3f(.54f,.35f,.17f);
    glLineWidth(1.8f);
    glBegin(GL_LINES);
    glVertex2f(1.5f,-20);
    glVertex2f(8,-34);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0,45,0);
    glColor3f(sr,sg,sb);
    drawCircle(0,0,11,10);
    glColor3f(.12f,.12f,.12f);
    glBegin(GL_QUADS);
    glVertex2f(2,-2);
    glVertex2f(4.5f,-2);
    glVertex2f(4.5f,.5f);
    glVertex2f(2,.5f);
    glVertex2f(6,-2);
    glVertex2f(8.5f,-2);
    glVertex2f(8.5f,.5f);
    glVertex2f(6,.5f);
    glEnd();
    glColor3f(.07f,.07f,.07f);
    drawCircle(8,-4,4,6);
    drawCircle(4,-6,3,6);
    glColor3f(.18f,.12f,.07f);
    glBegin(GL_POLYGON);
    for(int i=0; i<=5; i++)
    {
        float t=3.14159f*i/5;
        glVertex2f(11*cosf(t),11*sinf(t));
    }
    glEnd();
    glPopMatrix();
    glPopMatrix();
}

void drawJhalmuriStall(float cx,float cy)
{
    glPushMatrix();
    glTranslatef(cx,cy,0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0,0,0,.22f);
    glBegin(GL_POLYGON);
    for(int i=0; i<360; i+=15)glVertex2f(22*cosTable[i],-12+4*sinTable[i]);
    glEnd();
    glColor3f(.43f,.29f,.18f);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(0,35);
    glVertex2f(-18,-12);
    glVertex2f(0,35);
    glVertex2f(18,-12);
    glVertex2f(0,35);
    glVertex2f(2,-12);
    glLineWidth(2);
    glVertex2f(-9,11);
    glVertex2f(9,11);
    glVertex2f(-13,23);
    glVertex2f(13,23);
    glEnd();
    glColor3f(.71f,.61f,.45f);
    glBegin(GL_POLYGON);
    glVertex2f(-5,11);
    glVertex2f(1,17);
    glVertex2f(7,11);
    glVertex2f(4,0);
    glVertex2f(-3,0);
    glEnd();
    glColor3f(.55f,.45f,.31f);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-5,11);
    glVertex2f(1,17);
    glVertex2f(7,11);
    glVertex2f(4,0);
    glVertex2f(-3,0);
    glEnd();
    glColor3f(.35f,.27f,.2f);
    drawCircle(1,10,2,6);
    glColor3f(.78f,.79f,.8f);
    glBegin(GL_POLYGON);
    for(int i=0; i<360; i+=15)glVertex2f(32*cosTable[i],35+8*sinTable[i]);
    glEnd();
    glColor3f(.51f,.52f,.53f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<360; i+=15)glVertex2f(32*cosTable[i],35+8*sinTable[i]);
    glEnd();
    glColor3f(.68f,.69f,.71f);
    glBegin(GL_POLYGON);
    for(int i=0; i<360; i+=15)glVertex2f(30*cosTable[i],34.5f+6.5f*sinTable[i]);
    glEnd();
    glColor4f(1,1,1,.35f);
    glBegin(GL_POLYGON);
    for(int i=0; i<360; i+=15)glVertex2f(-12+12*cosTable[i],36.8f+2.8f*sinTable[i]);
    glEnd();
    glColor3f(.96f,.94f,.87f);
    glBegin(GL_QUADS);
    glVertex2f(8,35);
    glVertex2f(26,35);
    glVertex2f(26,63);
    glVertex2f(8,63);
    glEnd();
    glColor3f(.75f,.71f,.63f);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(8,35);
    glVertex2f(26,35);
    glVertex2f(26,63);
    glVertex2f(8,63);
    glEnd();
    glColor4f(.51f,.43f,.35f,.15f);
    glBegin(GL_LINES);
    for(float sx=11; sx<=23; sx+=3)
    {
        glVertex2f(sx,35);
        glVertex2f(sx,63);
    }
    glEnd();
    glColor3f(.9f,.88f,.8f);
    glBegin(GL_POLYGON);
    for(int i=0; i<360; i+=30)glVertex2f(17+9*cosTable[i],63+3*sinTable[i]);
    glEnd();
    glColor3f(.75f,.71f,.63f);
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<360; i+=30)glVertex2f(17+9*cosTable[i],63+3*sinTable[i]);
    glEnd();
    glColor3f(1,1,1);
    for(int j=0; j<6; j++)drawCircle(11+j*2,63,1.2f,4);
    glColor3f(.58f,.31f,.2f);
    glBegin(GL_LINES);
    glVertex2f(8,48);
    glVertex2f(26,48);
    glEnd();
    glColor3f(1,.82f,.2f);
    glBegin(GL_QUADS);
    glVertex2f(-18,34);
    glVertex2f(-8,34);
    glVertex2f(-8,46);
    glVertex2f(-18,46);
    glEnd();
    glColor3f(.86f,.47f,.04f);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-18,34);
    glVertex2f(-8,34);
    glVertex2f(-8,46);
    glVertex2f(-18,46);
    glEnd();
    glColor3f(.86f,.2f,.2f);
    glBegin(GL_QUADS);
    glVertex2f(-18,38);
    glVertex2f(-8,38);
    glVertex2f(-8,40);
    glVertex2f(-18,40);
    glEnd();
    glColor4f(1,1,1,.6f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-18,45);
    glVertex2f(-14,45);
    glVertex2f(-18,41);
    glEnd();
    glPushMatrix();
    glTranslatef(-11,35,0);
    glRotatef(8.5f,0,0,1);
    glColor3f(1,.82f,.2f);
    glBegin(GL_QUADS);
    glVertex2f(-5,-1);
    glVertex2f(5,-1);
    glVertex2f(5,11);
    glVertex2f(-5,11);
    glEnd();
    glColor3f(.86f,.47f,.04f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-5,-1);
    glVertex2f(5,-1);
    glVertex2f(5,11);
    glVertex2f(-5,11);
    glEnd();
    glColor3f(.86f,.2f,.2f);
    glBegin(GL_QUADS);
    glVertex2f(-5,3);
    glVertex2f(5,3);
    glVertex2f(5,5);
    glVertex2f(-5,5);
    glEnd();
    glColor4f(1,1,1,.6f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-5,10);
    glVertex2f(-1,10);
    glVertex2f(-5,6);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-15,43,0);
    glRotatef(-5.7f,0,0,1);
    glColor3f(1,.84f,.24f);
    glBegin(GL_QUADS);
    glVertex2f(-4.5f,-1);
    glVertex2f(4.5f,-1);
    glVertex2f(4.5f,10);
    glVertex2f(-4.5f,10);
    glEnd();
    glColor3f(.86f,.47f,.04f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-4.5f,-1);
    glVertex2f(4.5f,-1);
    glVertex2f(4.5f,10);
    glVertex2f(-4.5f,10);
    glEnd();
    glColor3f(.86f,.2f,.2f);
    glBegin(GL_QUADS);
    glVertex2f(-4.5f,3);
    glVertex2f(4.5f,3);
    glVertex2f(4.5f,5);
    glVertex2f(-4.5f,5);
    glEnd();
    glColor4f(1,1,1,.6f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-4.5f,9);
    glVertex2f(-1,9);
    glVertex2f(-4.5f,5);
    glEnd();
    glPopMatrix();
    glColor3f(.84f,.18f,.18f);
    glBegin(GL_QUADS);
    glVertex2f(-4,34);
    glVertex2f(4,34);
    glVertex2f(4,45);
    glVertex2f(-4,45);
    glEnd();
    glColor3f(.59f,.08f,.08f);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-4,34);
    glVertex2f(4,34);
    glVertex2f(4,45);
    glVertex2f(-4,45);
    glEnd();
    glColor3f(.92f,.27f,.27f);
    glBegin(GL_POLYGON);
    for(int i=0; i<360; i+=30)glVertex2f(4*cosTable[i],45+1.2f*sinTable[i]);
    glEnd();
    glColor3f(.59f,.08f,.08f);
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<360; i+=30)glVertex2f(4*cosTable[i],45+1.2f*sinTable[i]);
    glEnd();
    glLineWidth(1.5f);
    glBegin(GL_LINE_STRIP);
    for(int t=-90; t<=90; t+=30)
    {
        float a=t*3.14159f/180.f;
        glVertex2f(4+3*cosf(a),39+3*sinf(a));
    }
    glEnd();
    glColor3f(.82f,.83f,.84f);
    glBegin(GL_QUADS);
    glVertex2f(-10,33);
    glVertex2f(-1,33);
    glVertex2f(-1,41);
    glVertex2f(-10,41);
    glEnd();
    glColor3f(.55f,.56f,.57f);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-10,33);
    glVertex2f(-1,33);
    glVertex2f(-1,41);
    glVertex2f(-10,41);
    glEnd();
    glColor3f(.88f,.89f,.9f);
    glBegin(GL_POLYGON);
    for(int i=0; i<360; i+=30)glVertex2f(-5.5f+4.5f*cosTable[i],41+sinTable[i]);
    glEnd();
    glColor3f(.75f,.16f,.16f);
    glBegin(GL_POLYGON);
    for(int i=0; i<360; i+=45)glVertex2f(-15+2.5f*cosTable[i],41+3.5f*sinTable[i]);
    glEnd();
    glColor3f(.7f,.7f,.7f);
    glBegin(GL_QUADS);
    glVertex2f(-17.5f,43.5f);
    glVertex2f(-12.5f,43.5f);
    glVertex2f(-12.5f,45);
    glVertex2f(-17.5f,45);
    glEnd();
    glColor3f(.9f,.67f,.08f);
    glBegin(GL_POLYGON);
    for(int i=0; i<360; i+=45)glVertex2f(-21+2.2f*cosTable[i],39+3.2f*sinTable[i]);
    glEnd();
    glColor3f(.7f,.7f,.7f);
    glBegin(GL_QUADS);
    glVertex2f(-23,41.5f);
    glVertex2f(-19,41.5f);
    glVertex2f(-19,43);
    glVertex2f(-23,43);
    glEnd();
    glColor3f(.18f,.49f,.2f);
    glBegin(GL_POLYGON);
    for(int i=0; i<=180; i+=15)
    {
        float t=i*3.14159f/180.f;
        glVertex2f(6+2.5f*cosf(t),34+2.5f*sinf(t));
    }
    glEnd();
    glColor3f(.54f,.76f,.29f);
    glBegin(GL_POLYGON);
    for(int i=0; i<=180; i+=15)
    {
        float t=i*3.14159f/180.f;
        glVertex2f(6+2*cosf(t),34+2*sinf(t));
    }
    glEnd();
    glPopMatrix();
}

void drawTree(float x,float y,int type)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    if(type==1)
    {
        glColor3f(.42f,.28f,.15f);
        glRectf(-4,0,4,100);
        glColor3f(.12f,.28f,.08f);
        for(int i=0; i<10; i++)
        {
            glPushMatrix();
            glTranslatef(0,100,0);
            glRotatef(36.f*i,0,0,1);
            glBegin(GL_POLYGON);
            for(int k=0; k<=15; k++)
            {
                float t=3.14159f*k/15;
                glVertex2f(70*sinf(t),-35*(1-cosf(t)));
            }
            glEnd();
            glPopMatrix();
        }
    }
    else if(type==2)
    {
        glColor3f(.22f,.35f,.11f);
        drawCircle(-10,10,15,8);
        drawCircle(10,10,15,8);
        drawCircle(0,20,18,8);
    }
    else
    {
        glColor3f(.3f,.15f,0);
        glRectf(-5,0,5,60);
        glColor3f(.1f,.2f,0);
        float la[]= {0,40,80,120,160};
        for(int i=0; i<5; i++)
        {
            glPushMatrix();
            glTranslatef(0,60,0);
            glRotatef(la[i],0,0,1);
            glBegin(GL_POLYGON);
            for(int k=0; k<12; k++)
            {
                float t=2*3.14159f*k/12;
                glVertex2f(40*cosf(t),20*sinf(t));
            }
            glEnd();
            glPopMatrix();
        }
    }
    glPopMatrix();
}

void drawHay(float x,float y,bool b)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    if(b)
    {
        glColor3f(.83f,.68f,.21f);
        glRectf(-15,0,15,15);
        glColor3f(.6f,.4f,0);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2f(-8,0);
        glVertex2f(-8,15);
        glVertex2f(8,0);
        glVertex2f(8,15);
        glEnd();
    }
    else
    {
        glColor3f(.9f,.8f,0);
        glBegin(GL_POLYGON);
        for(int i=0; i<=10; i++)
        {
            float t=3.14159f*i/10;
            glVertex2f(20*cosf(t),25*sinf(t));
        }
        glEnd();
    }
    glPopMatrix();
}

void drawCloud(float x,float y,float sz)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glColor4f(1,1,1,.8f);
    drawCircle(0,0,sz,8);
    drawCircle(-sz*.6f,-sz*.2f,sz*.7f,8);
    drawCircle(sz*.6f,-sz*.2f,sz*.7f,8);
    drawCircle(-sz*.3f,sz*.3f,sz*.6f,8);
    drawCircle(sz*.3f,sz*.3f,sz*.6f,8);
    glPopMatrix();
}

void drawRock(float x,float y,float sz)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glColor3f(.43f,.43f,.43f);
    glBegin(GL_POLYGON);
    glVertex2f(-sz,0);
    glVertex2f(-sz*.4f,sz*.7f);
    glVertex2f(sz*.3f,sz);
    glVertex2f(sz,sz*.3f);
    glVertex2f(sz*.6f,-sz*.2f);
    glEnd();
    glColor3f(.55f,.55f,.55f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-sz*.5f,sz*.4f);
    glVertex2f(0,sz*.7f);
    glVertex2f(sz*.3f,sz*.3f);
    glEnd();
    glPopMatrix();
}

void drawGrass(float x,float y)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glColor3f(.33f,.42f,.18f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    for(int i=0; i<3; i++)
    {
        float a=(i-1)*.4f;
        glVertex2f(0,0);
        glVertex2f(sinf(a)*8,8);
    }
    glEnd();
    glPopMatrix();
}

void drawDung(float x,float y)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glColor3f(.29f,.21f,.13f);
    glBegin(GL_POLYGON);
    for(int i=0; i<12; i++)
    {
        float t=2*3.14159f*i/12;
        glVertex2f(8*cosf(t),3*sinf(t));
    }
    glEnd();
    glPopMatrix();
}

void drawFenceSegment(float x,float y,float h,int type)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glColor4f(0,0,0,.15f);
    glBegin(GL_POLYGON);
    for(int i=0; i<360; i+=30)
    {
        float a=i*3.14159f/180.f;
        glVertex2f(cosf(a)*8+2,sinf(a)*3+2);
    }
    glEnd();
    const float w=6;
    auto bam=[&](float ht)
    {
        glColor3f(.7f,.5f,.3f);
        glRectf(0,0,w,ht);
        glColor3f(.5f,.35f,.2f);
        glLineWidth(1);
        glBegin(GL_LINE_LOOP);
        glVertex2f(0,0);
        glVertex2f(w,0);
        glVertex2f(w,ht);
        glVertex2f(0,ht);
        glEnd();
        glColor3f(.4f,.25f,.15f);
        glPointSize(3);
        glBegin(GL_POINTS);
        for(float s=30; s<ht; s+=30)
        {
            glVertex2f(0,s);
            glVertex2f(w,s);
            glVertex2f(w/2,s+2);
        }
        glEnd();
        glBegin(GL_LINES);
        for(float s=30; s<ht; s+=30)
        {
            glVertex2f(0,s);
            glVertex2f(w,s);
        }
        glEnd();
    };
    if(type==1)
    {
        float bh=h*(.3f+fabsf(sinf(x))*.4f);
        bam(bh);
        glColor3f(.8f,.7f,.4f);
        glBegin(GL_TRIANGLES);
        glVertex2f(0,bh);
        glVertex2f(w/2,bh+10);
        glVertex2f(w,bh);
        glEnd();
    }
    else if(type==2)
    {
        bam(h);
        glBegin(GL_TRIANGLES);
        glVertex2f(0,h);
        glVertex2f(w/2,h+15);
        glVertex2f(w,h);
        glEnd();
    }
    else bam(h);
    glPopMatrix();
}

void drawLead(float x1,float y1,float x2,float y2)
{
    glColor3f(.36f,.25f,.22f);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<=10; i++)
    {
        float t=i/10.f;
        glVertex2f((1-t)*x1+t*x2,(1-t)*y1+t*y2+4*t*(1-t)*-20);
    }
    glEnd();
}

void createDisplayLists()
{
    bgList=glGenLists(1);
    glNewList(bgList,GL_COMPILE);
    if(isNight)
    {
        glBegin(GL_QUADS);
        glColor3f(.04f,.04f,.11f);
        glVertex2f(0,HEIGHT*.6f);
        glVertex2f(WIDTH,HEIGHT*.6f);
        glColor3f(.09f,.09f,.18f);
        glVertex2f(WIDTH,HEIGHT);
        glVertex2f(0,HEIGHT);
        glEnd();
        glColor3f(.098f,.176f,.137f);
        glRectf(0,0,WIDTH,HEIGHT*.6f);
        for(int r=35; r>=16; r-=3)
        {
            glColor4f(.86f,.92f,1,0.35f*(1-(r-16.f)/19));
            drawCircle(660,510,r,16);
        }
        glColor4f(.9f,.92f,.97f,1);
        drawCircle(660,510,16,16);
        glColor4f(.07f,.07f,.15f,1);
        drawCircle(666,515,16,16);
    }
    else
    {
        glBegin(GL_QUADS);
        glColor3f(.6f,.8f,1);
        glVertex2f(0,HEIGHT*.6f);
        glVertex2f(WIDTH,HEIGHT*.6f);
        glColor3f(.8f,.9f,1);
        glVertex2f(WIDTH,HEIGHT);
        glVertex2f(0,HEIGHT);
        glEnd();
        for(int r=50; r>=13; r-=4)
        {
            glColor4f(1,.84f,.23f,.7f*(1-(r-13.f)/37));
            drawCircle(660,510,r,16);
        }
        glColor4f(1,1,1,1);
        drawCircle(660,510,13,16);
        glColor3f(.4f,.3f,.2f);
        glRectf(0,0,WIDTH,HEIGHT*.6f);
    }
    for(int i=0; i<40; i++)
    {
        float mx=(sinf(i*12.3f)*.5f+.5f)*WIDTH,my=(cosf(i*45.6f)*.5f+.5f)*(HEIGHT*.6f);
        if(i%2==0)glColor3f(.48f,.33f,.18f);
        else glColor3f(.6f,.45f,.3f);
        float rw=25+fabsf(sinf(i*7.89f))*15,rh=5+fabsf(cosf(i*3.21f))*5;
        glBegin(GL_POLYGON);
        for(int k=0; k<8; k++)
        {
            float t=2*3.14159f*k/8;
            glVertex2f(mx+rw*cosf(t),my+rh*sinf(t));
        }
        glEnd();
    }
    for(int i=0; i<8; i++)
    {
        float px=100+i*115.f,py=50+fabsf(sinf(i*6.3f))*150;
        float pw=45+fabsf(sinf(i*11.2f))*15,ph=12+fabsf(cosf(i*9.5f))*4;
        glColor4f(.55f,.65f,.8f,.3f);
        glBegin(GL_POLYGON);
        for(int k=0; k<12; k++)
        {
            float t=2*3.14159f*k/12;
            glVertex2f(px+pw*cosf(t),py+ph*sinf(t));
        }
        glEnd();
    }
    glEndList();
    fenceList=glGenLists(1);
    glNewList(fenceList,GL_COMPILE);
    for(int i=0; i<WIDTH; i+=25)
    {
        int t=0;
        if(i%100==0&&sinf(i)>.5f)t=1;
        if(i%150==0)t=2;
        drawFenceSegment(i,HEIGHT*.45f,HEIGHT*.15f,t);
    }
    glEndList();
    listsCreated=true;
}

void drawFence()
{
    glCallList(fenceList);
    auto hb=[&](float yp,float th)
    {
        glColor3f(.8f,.7f,.4f);
        glRectf(0,yp,WIDTH,yp+th);
        glColor3f(.65f,.55f,.27f);
        glLineWidth(1);
        glBegin(GL_LINE_LOOP);
        glVertex2f(0,yp);
        glVertex2f(WIDTH,yp);
        glVertex2f(WIDTH,yp+th);
        glVertex2f(0,yp+th);
        glEnd();
    };
    hb(HEIGHT*.47f,6);
    hb(HEIGHT*.55f,4);
}

void drawBird(float x,float y,float flap)
{
    glColor3f(.12f,.12f,.16f);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x-8,y+flap*4);
    glVertex2f(x-3,y-1);
    glVertex2f(x,y);
    glVertex2f(x+3,y-1);
    glVertex2f(x+8,y+flap*4);
    glEnd();
}

void drawPlane(float x,float y,bool rl)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    if(rl)glScalef(-1,1,1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(2);
    glBegin(GL_LINES);
    for(int t=0; t<20; t++)
    {
        float f1=t/20.f,f2=(t+1)/20.f;
        float x1=-20-f1*160,x2=-20-f2*160;
        float ot1=-5+sinf(globalTime*4+f1*3)*3*f1,ot2=-5+sinf(globalTime*4+f2*3)*3*f2;
        float ob1=1+cosf(globalTime*4+f1*3)*3*f1,ob2=1+cosf(globalTime*4+f2*3)*3*f2;
        float a1=1-f1,a2=1-f2;
        float r=isNight?.78f:1,g=isNight?.82f:1,b=isNight?1.f:1,am=isNight?.35f:.65f;
        glColor4f(r,g,b,a1*am);
        glVertex2f(x1,ot1);
        glColor4f(r,g,b,a2*am);
        glVertex2f(x2,ot2);
        glColor4f(r,g,b,a1*am);
        glVertex2f(x1,ob1);
        glColor4f(r,g,b,a2*am);
        glVertex2f(x2,ob2);
    }
    glEnd();
    auto col3=[&](float rn,float rl2,float rd,float rr,float rb)
    {
        if(isNight)glColor3f(rd,rl2,rb);
        else glColor3f(rr,rl2,rb);
    };
    if(isNight)glColor3f(.12f,.14f,.17f);
    else glColor3f(.7f,.72f,.76f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-21,3);
    glVertex2f(-26,17);
    glVertex2f(-14,3);
    glEnd();
    if(isNight)glColor3f(.18f,.21f,.26f);
    else glColor3f(.89f,.9f,.93f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-19,3);
    glVertex2f(-24,16);
    glVertex2f(-15,3);
    glEnd();
    if(isNight)glColor3f(.33f,.36f,.44f);
    else glColor3f(.6f,.63f,.68f);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-19,3);
    glVertex2f(-24,16);
    glVertex2f(-15,3);
    glEnd();
    glColor3f(.9f,.17f,.17f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-22,11);
    glVertex2f(-23.5f,15);
    glVertex2f(-19.5f,10);
    glEnd();
    if(isNight)glColor3f(.13f,.15f,.19f);
    else glColor3f(.8f,.82f,.85f);
    glBegin(GL_QUADS);
    glVertex2f(-21,-2);
    glVertex2f(-25,-7);
    glVertex2f(-17,-7);
    glVertex2f(-13,-2);
    glEnd();
    if(isNight)glColor3f(.33f,.36f,.44f);
    else glColor3f(.6f,.63f,.68f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-21,-2);
    glVertex2f(-25,-7);
    glVertex2f(-17,-7);
    glVertex2f(-13,-2);
    glEnd();
    if(isNight)glColor3f(.16f,.19f,.24f);
    else glColor3f(.95f,.96f,.98f);
    glBegin(GL_POLYGON);
    glVertex2f(-26,-1);
    glVertex2f(-22,2.5f);
    glVertex2f(-15,4.8f);
    glVertex2f(0,5.5f);
    glVertex2f(12,5);
    glVertex2f(24,2);
    glVertex2f(29,-1);
    glVertex2f(24,-4.2f);
    glVertex2f(10,-4.8f);
    glVertex2f(-10,-4.8f);
    glVertex2f(-26,-2);
    glEnd();
    if(isNight)glColor3f(.29f,.32f,.4f);
    else glColor3f(.58f,.61f,.67f);
    glLineWidth(1.2f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-26,-1);
    glVertex2f(-22,2.5f);
    glVertex2f(-15,4.8f);
    glVertex2f(0,5.5f);
    glVertex2f(12,5);
    glVertex2f(24,2);
    glVertex2f(29,-1);
    glVertex2f(24,-4.2f);
    glVertex2f(10,-4.8f);
    glVertex2f(-10,-4.8f);
    glVertex2f(-26,-2);
    glEnd();
    if(isNight)glColor3f(.58f,.87f,1);
    else glColor3f(.13f,.18f,.26f);
    glBegin(GL_QUADS);
    glVertex2f(18,2.3f);
    glVertex2f(24,1.4f);
    glVertex2f(22,-.6f);
    glVertex2f(17.5f,.4f);
    glEnd();
    if(isNight)glColor3f(1,.92f,.47f);
    else glColor4f(.27f,.35f,.45f,.55f);
    for(float wx=-14; wx<=10; wx+=4)if(fabs(wx-18)>2)drawCircle(wx,.5f,.8f,6);
    if(isNight)glColor3f(.11f,.13f,.17f);
    else glColor3f(.66f,.68f,.72f);
    glBegin(GL_QUADS);
    glVertex2f(-2,-9);
    glVertex2f(11,-9);
    glVertex2f(11,-4);
    glVertex2f(-2,-4);
    glEnd();
    if(isNight)glColor3f(.25f,.28f,.33f);
    else glColor3f(.47f,.49f,.53f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-2,-9);
    glVertex2f(11,-9);
    glVertex2f(11,-4);
    glVertex2f(-2,-4);
    glEnd();
    if(isNight)glColor3f(1,.5f,.15f);
    else glColor3f(.94f,.96f,1);
    drawCircle(11,-6.5f,1.4f,6);
    glColor4f(1,.43f,.11f,.85f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-2,-6.5f);
    glVertex2f(-7,-6.5f);
    glVertex2f(-2,-5.3f);
    glEnd();
    if(isNight)glColor3f(.19f,.21f,.26f);
    else glColor3f(.93f,.94f,.96f);
    glBegin(GL_QUADS);
    glVertex2f(3,0);
    glVertex2f(-12,-18);
    glVertex2f(-5,-18);
    glVertex2f(10,0);
    glEnd();
    if(isNight)glColor3f(.33f,.36f,.44f);
    else glColor3f(.6f,.63f,.68f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(3,0);
    glVertex2f(-12,-18);
    glVertex2f(-5,-18);
    glVertex2f(10,0);
    glEnd();
    glColor3f(.9f,.17f,.17f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-12,-18);
    glVertex2f(-14,-22);
    glVertex2f(-9,-18);
    glEnd();
    if(isNight)glColor3f(.1f,.12f,.15f);
    else glColor3f(.74f,.76f,.8f);
    glBegin(GL_QUADS);
    glVertex2f(4,2);
    glVertex2f(-7,13);
    glVertex2f(-1,13);
    glVertex2f(11,2);
    glEnd();
    bool sb=fmodf(globalTime*3,2)<1;
    glColor3f(1,.13f,.13f);
    drawCircle(-11,-19,1.8f,6);
    glColor3f(.13f,1,.13f);
    drawCircle(-4,13,1.5f,6);
    if(sb)
    {
        glColor3f(1,1,1);
        drawCircle(6,4.2f,1.8f,6);
        drawCircle(-24,16,2.2f,6);
    }
    glPopMatrix();
}

void drawRoad()
{

    glColor3f(0.2f,0.2f,0.2f);
    glBegin(GL_QUADS);
    glVertex2f(0,340);
    glVertex2f(WIDTH,340);
    glVertex2f(WIDTH,390);
    glVertex2f(0,390);
    glEnd();

    glColor3f(0.85f,0.85f,0.85f);
    glLineWidth(2);

    glBegin(GL_LINES);

    glVertex2f(0,385);
    glVertex2f(WIDTH,385);

    glVertex2f(0,345);
    glVertex2f(WIDTH,345);

    glEnd();

    glColor3f(1,1,0);

    for(int i=0; i<WIDTH; i+=40)
    {
        glBegin(GL_QUADS);
        glVertex2f(i+5,363);
        glVertex2f(i+25,363);
        glVertex2f(i+25,367);
        glVertex2f(i+5,367);
        glEnd();
    }
}

void drawHouse(float x,float y)
{
    glPushMatrix();
    glTranslatef(x,y,0);

    glColor3f(0.85f,0.65f,0.45f);

    glBegin(GL_QUADS);
    glVertex2f(0,0);
    glVertex2f(120,0);
    glVertex2f(120,90);
    glVertex2f(0,90);
    glEnd();

    glColor3f(0.5f,0.1f,0.1f);

    glBegin(GL_TRIANGLES);
    glVertex2f(-10,90);
    glVertex2f(60,145);
    glVertex2f(130,90);
    glEnd();

    glColor3f(0.35f,0.18f,0.05f);

    glBegin(GL_QUADS);
    glVertex2f(48,0);
    glVertex2f(72,0);
    glVertex2f(72,50);
    glVertex2f(48,50);
    glEnd();

    glColor3f(1,1,0);

    drawCircle(67,25,2,8);

    glColor3f(0.6f,0.85f,1);

    glBegin(GL_QUADS);
    glVertex2f(12,40);
    glVertex2f(38,40);
    glVertex2f(38,68);
    glVertex2f(12,68);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(82,40);
    glVertex2f(108,40);
    glVertex2f(108,68);
    glVertex2f(82,68);
    glEnd();

    glColor3f(0,0,0);

    glLineWidth(1);

    glBegin(GL_LINE_LOOP);
    glVertex2f(12,40);
    glVertex2f(38,40);
    glVertex2f(38,68);
    glVertex2f(12,68);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(82,40);
    glVertex2f(108,40);
    glVertex2f(108,68);
    glVertex2f(82,68);
    glEnd();

    glBegin(GL_LINES);

    glVertex2f(25,40);
    glVertex2f(25,68);

    glVertex2f(12,54);
    glVertex2f(38,54);

    glVertex2f(95,40);
    glVertex2f(95,68);

    glVertex2f(82,54);
    glVertex2f(108,54);

    glEnd();

    glPopMatrix();
}

void drawFullEnvironment()
{
    if(!listsCreated)createDisplayLists();
    glCallList(bgList);
    drawCloud(fmodf(100+cloudOffset,WIDTH+100)-50,HEIGHT*.85f,30);
    drawCloud(fmodf(300+cloudOffset,WIDTH+100)-50,HEIGHT*.75f,25);
    drawCloud(fmodf(550+cloudOffset,WIDTH+100)-50,HEIGHT*.9f,35);
    drawCloud(fmodf(750+cloudOffset,WIDTH+100)-50,HEIGHT*.8f,28);
    if(!isNight)
    {
        drawBird(fmodf(80+globalTime*45,WIDTH+100.f)-50,HEIGHT*.82f+15*sinf(globalTime*1.5f),sinf(globalTime*8));
        drawBird(fmodf(350+globalTime*55,WIDTH+100.f)-50,HEIGHT*.77f+12*sinf(globalTime*2+1.2f),sinf(globalTime*10+.5f));
        drawBird(fmodf(580+globalTime*40,WIDTH+100.f)-50,HEIGHT*.88f+18*sinf(globalTime+2.5f),sinf(globalTime*7+1));
    }
    drawPlane(fmodf(globalTime*38,WIDTH+300.f)-150,HEIGHT*.93f+5*sinf(globalTime*.5f),false);
    for(int i=0; i<30; i++)
    {
        float rx=(sinf(i*98.7f)*.5f+.5f)*WIDTH,ry=(cosf(i*65.4f)*.5f+.5f)*(HEIGHT*.22f);
        if(i%2==0)drawRock(rx,ry+150,3+fabsf(sinf(i*4.56f))*4);
        else drawGrass(rx,ry+100);
    }
    drawDung(420,160);
    drawDung(180,150);
    drawDung(600,50);
    drawDung(120,40);
}

float getBNX(float bx,float bs,bool f,float)
{
    return bx+(f?-90.f:90.f)*bs;
}
float getBNY(float by,float bs,float)
{
    return by+13*bs;
}
float getPXH(float px,float,bool)
{
    return px+14;
}
float getPYH(float,float py,bool)
{
    return py+8;
}

void drawEllipseOutline(float cx,float cy,float rx,float ry,int seg)
{
    glBegin(GL_LINE_LOOP);
    int s=360/seg;
    if(s<1)s=1;
    for(int i=0; i<360; i+=s)glVertex2f(cx+rx*cosTable[i],cy+ry*sinTable[i]);
    glEnd();
}

void drawSplashParticles(float cx,float cy,float ph)
{
    if(ph>=.6f)return;
    float h=ph*12,v=ph*10-ph*ph*12;
    glBegin(GL_LINES);
    glVertex2f(cx,cy);
    glVertex2f(cx-h*.7f,cy+v);
    glVertex2f(cx,cy);
    glVertex2f(cx+h*.7f,cy+v);
    glEnd();
}

void drawRain()
{
    if(!isRaining)return;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    if(isNight)glColor4f(.047f,.047f,.11f,.35f);
    else glColor4f(.86f,.9f,.96f,.08f);
    glBegin(GL_QUADS);
    glVertex2f(0,0);
    glVertex2f(800,0);
    glVertex2f(800,600);
    glVertex2f(0,600);
    glEnd();
    float mo=isNight?(.15f+.05f*sinf(globalTime*1.5f)):(.04f+.02f*sinf(globalTime*1.5f));
    float fc=isNight?.08f:.94f,gc=isNight?.08f:.96f,bc=isNight?.16f:1;
    glBegin(GL_QUADS);
    glColor4f(fc,gc,bc,0);
    glVertex2f(0,100);
    glVertex2f(800,100);
    glColor4f(fc,gc,bc,mo);
    glVertex2f(800,275);
    glVertex2f(0,275);
    glVertex2f(0,275);
    glVertex2f(800,275);
    glColor4f(fc,gc,bc,0);
    glVertex2f(800,450);
    glVertex2f(0,450);
    glEnd();
    for(int i=0; i<35; i++)
    {
        float rx=(sinf(i*78.9f)*.5f+.5f)*800,ry=(cosf(i*43.2f)*.5f+.5f)*250+20;
        float ph=fmodf(globalTime+i*.17f,1.2f)/1.2f;
        float alpha=(1-ph)*.45f;
        if(isNight)glColor4f(.65f,.7f,.99f,alpha);
        else glColor4f(1,1,1,alpha*.8f);
        drawEllipseOutline(rx,ry,ph*16,ph*16*.3f,12);
    }
    float sp[][2]= {{120,270},{250,270},{390,270},{520,270},{680,270},{180,330},{450,330},{720,330},
        {630,260},{650,265},{670,260},{400,238},{420,242},{440,238},{160,248},{180,252},{200,248},
        {270,140},{290,137},{590,148},{610,145},{420,165},{180,153},{123,43},{360,175},{210,60},{720,190}
    };
    int sc=sizeof(sp)/sizeof(sp[0]);
    if(isNight)glColor4f(.7f,.85f,1,.5f);
    else glColor4f(.85f,.94f,1,.7f);
    glLineWidth(1);
    for(int i=0; i<sc; i++)drawSplashParticles(sp[i][0],sp[i][1],fmodf(globalTime+i*.13f,.61f)/.61f);
    glColor4f(.6f,.82f,1,.48f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    for(int i=0; i<80; i++)
    {
        float sx=(sinf(i*123.45f)*.5f+.5f)*WIDTH,sy=(cosf(i*543.21f)*.5f+.5f)*HEIGHT;
        float fy=fmodf(sy-globalTime*350,650);
        if(fy<0)fy+=650;
        float fx=fmodf(sx-globalTime*70,850);
        if(fx<0)fx+=850;
        if(fx>=0&&fx<=WIDTH&&fy>=0&&fy<=HEIGHT)
        {
            glVertex2f(fx,fy);
            glVertex2f(fx+2.5f,fy-12);
        }
    }
    glEnd();
}

void drawToggleButton()
{
    auto btn=[&](float xn,float xx,float yn,float yx,bool active)
    {
        if(active)glColor4f(.08f,.08f,.14f,.85f);
        else glColor4f(1,1,1,.85f);
        glBegin(GL_QUADS);
        glVertex2f(xn,yn);
        glVertex2f(xx,yn);
        glVertex2f(xx,yx);
        glVertex2f(xn,yx);
        glEnd();
        if(active)glColor4f(1,1,1,.25f);
        else glColor4f(0,0,0,.15f);
        glLineWidth(1);
        glBegin(GL_LINE_LOOP);
        glVertex2f(xn,yn);
        glVertex2f(xx,yn);
        glVertex2f(xx,yx);
        glVertex2f(xn,yx);
        glEnd();
    };
    btn(745,785,545,585,isNight);
    float cx=765,cy=565;
    if(isNight)
    {
        glColor4f(.65f,.7f,.99f,1);
        drawCircle(cx,cy,7,16);
        glColor4f(.08f,.08f,.14f,1);
        drawCircle(cx+3,cy+2,7,16);
    }
    else
    {
        glColor4f(.92f,.7f,.03f,1);
        drawCircle(cx,cy,5,16);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        for(int i=0; i<360; i+=45)
        {
            float r=i*3.14159f/180.f;
            glVertex2f(cx+6*cosf(r),cy+6*sinf(r));
            glVertex2f(cx+9*cosf(r),cy+9*sinf(r));
        }
        glEnd();
    }
    if(isRaining)glColor4f(.05f,.2f,.39f,.85f);
    else if(isNight)glColor4f(.08f,.08f,.14f,.85f);
    else glColor4f(1,1,1,.85f);
    glBegin(GL_QUADS);
    glVertex2f(695,545);
    glVertex2f(735,545);
    glVertex2f(735,585);
    glVertex2f(695,585);
    glEnd();
    if(isRaining)glColor4f(.39f,.7f,1,.6f);
    else if(isNight)glColor4f(1,1,1,.25f);
    else glColor4f(0,0,0,.15f);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(695,545);
    glVertex2f(735,545);
    glVertex2f(735,585);
    glVertex2f(695,585);
    glEnd();
    cx=715;
    cy=566;
    if(isRaining)glColor4f(.9f,.95f,1,1);
    else glColor4f(.5f,.55f,.6f,1);
    drawCircle(cx-4,cy-2,4.5f,12);
    drawCircle(cx+4,cy-2,4.5f,12);
    drawCircle(cx,cy+2.5f,6,12);
    glBegin(GL_QUADS);
    glVertex2f(cx-5,cy-6);
    glVertex2f(cx+5,cy-6);
    glVertex2f(cx+5,cy+1);
    glVertex2f(cx-5,cy+1);
    glEnd();
    if(isRaining)
    {
        glColor3f(.4f,.75f,1);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        glVertex2f(cx-3,cy-6);
        glVertex2f(cx-2,cy-11);
        glVertex2f(cx,cy-7);
        glVertex2f(cx+1,cy-12);
        glVertex2f(cx+3,cy-6);
        glVertex2f(cx+4,cy-11);
        glEnd();
    }
}

void mouse(int button,int state,int x,int y)
{
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
    {
        int cw=glutGet(GLUT_WINDOW_WIDTH),ch=glutGet(GLUT_WINDOW_HEIGHT);
        float xo=x*800.f/cw,yo=(ch-y)*600.f/ch;
        if(xo>=745&&xo<=785&&yo>=545&&yo<=585)
        {
            isNight=!isNight;
            listsCreated=false;
            glutPostRedisplay();
        }
        else if(xo>=695&&xo<=735&&yo>=545&&yo<=585)
        {
            isRaining=!isRaining;
            glutPostRedisplay();
        }
        else if(xo>=40&&xo<=90&&yo>=-20&&yo<=90)
        {
            bubbleOpen=!bubbleOpen;
            glutPostRedisplay();
        }
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawFullEnvironment();
    drawFullEnvironment();

    drawRoad();
    drawHouse(500,300);

    drawHay(280,310,false);
    drawHay(280,310,false);
    drawHay(580,315,false);
    drawTree(760,360,0);
    drawTree(60,350,1);
    drawTree(420,345,2);
    drawFence();
    drawBull(100,295,bullColors[1],.28f,false,1000);
    drawBull(700,290,bullColors[1],.28f,true,2500);
    drawBull(400,305,bullColors[0],.3f,false,500);
    drawLead(getBNX(100,.28f,false,headBob),getBNY(295,.28f,headBob),getPXH(175,250,true),getPYH(175,250,true));
    drawPerson(175,250, {.1f,.5f,.2f}, {.3f,.3f,.7f},true,0,0,true);
    drawLead(getBNX(700,.28f,true,headBob),getBNY(290,.28f,headBob),getPXH(630,245,true),getPYH(630,245,true));
    drawPerson(630,245, {.8f,.4f,.1f}, {.2f,.2f,.2f},false,0,0,true);
    drawLead(getBNX(400,.3f,false,headBob),getBNY(305,.3f,headBob),getPXH(470,255,true),getPYH(470,255,true));
    drawPerson(470,255, {.3f,.3f,.3f}, {.6f,.2f,.2f},false,0,0,true);
    drawPerson(520,190, {.5f,.2f,.2f}, {.2f,.3f,.6f},false,.4f,200,false);
    drawPerson(320,200, {.6f,.3f,.1f}, {.2f,.2f,.2f},false,.4f,500,false);
    drawBull(650,210,bullColors[2],.42f,false,4000);
    drawLead(getBNX(650,.42f,false,headBob),getBNY(210,.42f,headBob),getPXH(720,150,true),getPYH(720,150,true));
    drawPerson(720,150, {.9f,.9f,.1f}, {.4f,.2f,0},true,0,0,true);
    drawBull(180,200,bullColors[1],.4f,false,1200);
    drawLead(getBNX(180,.4f,false,headBob),getBNY(200,.4f,headBob),getPXH(250,145,true),getPYH(250,145,true));
    drawPerson(250,145, {.2f,.6f,.8f}, {.8f,.8f,.8f},false,0,0,true);
    drawBull(420,190,bullColors[0],.4f,true,2200);
    drawLead(getBNX(420,.4f,true,headBob),getBNY(190,.4f,headBob),getPXH(360,135,true),getPYH(360,135,true));
    drawPerson(360,135, {.1f,.1f,.1f}, {.1f,.8f,.4f},true,0,0,true);
    drawTree(40,130,2);
    drawTree(760,60,1);
    drawHay(150,110,true);
    drawHay(480,90,true);
    drawBull(600,100,bullColors[0],.38f,false,800);
    drawLead(getBNX(600,.38f,false,headBob),getBNY(100,.38f,headBob),getPXH(690,30,true),getPYH(690,30,true));
    drawPerson(690,30, {.1f,.1f,.1f}, {.1f,1,.2f},true,0,0,true);
    drawBull(280,90,bullColors[3],.36f,true,5000);
    drawLead(getBNX(280,.36f,true,headBob),getBNY(90,.36f,headBob),getPXH(210,20,true),getPYH(210,20,true));
    drawPerson(210,20, {.6f,.3f,.1f}, {.2f,.2f,.2f},false,0,0,true);
    drawJhalmuriSeller(65,20);
    drawJhalmuriStall(95,20);
    drawSpeechBubble(65);
    drawRain();
    drawToggleButton();
    glutSwapBuffers();
}

void update(int v)
{
    globalTime+=.016f;
    personOffset+=.4f*direction;
    if(personOffset>250||personOffset<-250)direction*=-1;
    headBob=3*cosf(globalTime*3);
    gesturePhase=globalTime*2;
    cloudOffset=globalTime*30;
    glutPostRedisplay();
    glutTimerFunc(16,update,0);
}

void keyboard(unsigned char key,int x,int y)
{
    if(key=='t'||key=='T')
    {
        isNight=!isNight;
        listsCreated=false;
        glutPostRedisplay();
    }
    if(key=='r'||key=='R')
    {
        isRaining=!isRaining;
        glutPostRedisplay();
    }
}

void init()
{
    glClearColor(0,0,0,1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,WIDTH,0,HEIGHT);
}

int main(int argc,char**argv)
{
    srand(time(NULL));
    precomputeTables();
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutCreateWindow("Gorur Haat - Cattle Market");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(0,update,0);
    glutMainLoop();
    return 0;
}

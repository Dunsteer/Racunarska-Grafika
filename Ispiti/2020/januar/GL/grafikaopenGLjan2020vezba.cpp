#define ToRad 3.14/180

int PrepareTextures(CString strTex)
{
    glPixelStore(GL_UNPACK_ALIGNMENT,1);
    GLUint* niz = new GLUint[1];
    glGenTextures(1,niz);
    glBindTexture(GL_TEXTURE_2D,niz[0]);
    glTexParameter(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameter(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameter(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_LINEAR);
    glTexParameter(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_LINEAR);
    glTexEnv(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    // FILE* file = fopen(LPCSTR(strTex), "rb");

    // fseek(file, 0, SEEK_END);
    // long length = ftell(file);
    // fseek(file, 0, SEEK_SET);

    // char* buffer = new char[length];
    // size_t rc = fread(buffer, 1, length, file);

    // fclose(file);

    DImage img;
    img.Load(strTex);

    gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,img.Width(),img.Height()
    ,GL_RGB,GL_UNSIGNED_BYTE,img.GetDIBBITS());
}

void DrawCone(float h,float r, int nSeg)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0,h,0);
    glNormal3f(0,1,0);
    glTexCoord2f(0.5,0.5);
    for(int i=0;i<nSeg;i++)
    {
        float x = r*cos(360/nSeg*i*ToRad);
        float z = r*sin(360/nSeg*i*ToRad);

        glVertex3f(x,0,z);
        glTexCoord2f(x/r*0.5+0.5,z/r*0.5+0.5);
        glNormal3f(x/r*0.5+0.5,0,z/r*0.5+0.5);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0,0,0);
    glNormal3f(0,-1,0);
    glColor3f(1,0.5,0);
    for(int i=0;i<nSeg;i++)
    {
        float x = r*cos(360/nSeg*i*ToRad);
        float z = r*sin(360/nSeg*i*ToRad);

        glVertex3f(x,0,z);
        glColor3f(1,0.5,0);
        glNormal3f(0,-1,0);
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

void DrawFigure(float h,float r,int nSeg,int n)
{
    float step = 180/n;

    PrepareTextures(CString("tekstura.png"));
    DrawCone(2*h,2*r,nSeg);
    glTranslate(0,2*h,0);
    glRotate((step+step/2)*ToRad,0,0,1);
    for(int i=0;i<2*n;i++)
    {
        DrawCone(h,r,nSeg);
        glTranslate(0,h,0);
        glRotate(-step*ToRad,0,0,1);
    }
}
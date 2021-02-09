#define PI 3.14
#define toRad PI / 180

void DrawRoller(float r, float h, int nPartXZ, int nPartY)
{
    glBegin(GL_TRIANGLE_FAN);

    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);
    float increment = 2 * PI / nPartXZ;

    for (int i = 0; i < nPartXZ; i++)
    {
        glNormal3f(0, -1, 0);
        glTexCoord2f(0.5, 0.5);
        glVertex3f(r * cos(i * increment), 0, r * sin(i * increment));
    }

    for (int i = 0; i < nPartXZ; i++)
    {
        glNormal3f(0, 1, 0);
        glTexCoord2f(0.5, 0.5);
        glVertex3f(r * cos(i * increment), h, r * sin(i * increment));
    }

    glEnd();

    glBegin(GL_QUAD_STRIP);

    for (int j = 0; j < nPartY - 1; j++)
    {

        for (int i = 0; i < nPartXZ; i++)
        {
            glNormal3f(cos(i * increment), 0, sin(i * increment));

            if (i == 0)
            {
                glTexCoord2f(0, 0);
            }
            if (i == (nPartXZ - 1))
            {
                glTexCoord2f(1, 0);
            }

            glVertex3f(r * cos(i * increment), h / nPartY * j, r * sin(i * increment));

            if (i == 0)
            {
                glTexCoord2f(0, 1);
            }
            if (i == (nPartXZ - 1))
            {
                glTexCoord2f(1, 1);
            }

            glVertex3f(r * cos(i * increment), h / nPartY * (j + 1), r * sin(i * increment));
        }
    }
}

void setMaterial(float r, float g, float b)
{
    float ambient[] = {r / 4, g / 4, b / 4, 1.0};
    float diffuse[] = {r, g, b, 1.0};
    float specular[] = {255, 255, 255, 1.0};
    float emission[] = {100, 100, 100, 0.0};

    glMaterial(GL_FRONT, GL_AMBIENT, ambient);
    glMaterial(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterial(GL_FRONT, GL_SPECULAR, specular);
    glMaterial(GL_FRONT, GL_EMISSION, emission);
    glMaterial(GL_FRONT, GL_SHININESS, 25);
}

void prepareTextures(CString strTex)
{
    PrepareTexturing();
}

void DrawCrane(float r, float h, float alpha, float beta, float d, float l, CString strTexCrane, CString strTextLoad)
{
    prepareTextures(strTexCrane);
    DrawRoller(r, h, 8, 3);
    glTranslate(0, 0.1 * h + h, 0);
    glRotate(alpha, 0, 1, 0);
    DrawRoller(r, h, 8, 3);
    glTranslate(0, h, 0);
    DrawRoller(0.4 * r, 2 * h, 8, 3);
    glTranslate(0, 2 * h, 0);
    glRotate(beta, 0, 0, 1);
    DrawRoller(0.2 * r, 6 * h, 8, 3);
    glTranslate(0, d, 0);
    DrawRoller(0.2 * r, 6 * h, 8, 3);
    glTranslate(0, 6 * h, 0);
    glRotate(toRad * 180 - beta, 0, 0, -1);
    DrawRoller(1, l, 8, 3);
    glTranslate(0, l, 0);
    prepareTextures(strTextLoad);
    DrawRoller(0.3 * r, h, 8, 3);
}
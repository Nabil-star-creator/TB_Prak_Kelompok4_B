// ---------------------------------- library
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

// ---------------------------------- global variabel
const float PI = 3.14159265f;

// [PERUBAHAN 1] Kecepatan diperlambat agar mudah dikontrol
const float MOVE_SPEED = 0.002f; 
const float MOUSE_SENSITIVITY = 0.002f;

float cameraPosX = 0.0f, cameraPosY = 3.0f, cameraPosZ = 12.0f;
float cameraYaw = 0.0f, cameraPitch = -10.0f;

int lastMouseX = 0, lastMouseY = 0;
bool mouseActive = false;
bool keys[256] = {false};
int windowWidth = 1200, windowHeight = 800;

// Variabel Pencahayaan
bool lightingEnabled = true;
int lightMode = 1;
bool isDirectional = false;
bool isSpotlight = false;
float spotCutoff = 45.0f;
float spotExponent = 2.0f;
float linearAttenuation = 0.0f;
bool isShiny = true;

// Variabel Animasi
float angleRotation = 0.0f; 

// ---------------------------------- fungsi pencahayaan
void initLighting() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);

    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_shininess[] = {50.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void updateLightingLogic() {
    if (!lightingEnabled) {
        glDisable(GL_LIGHTING);
        return;
    }
    glEnable(GL_LIGHTING);
    GLfloat val_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat val_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat val_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat val_black[] = {0.0f, 0.0f, 0.0f, 1.0f};

    if (lightMode == 1) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, val_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, val_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, val_specular);
    } else if (lightMode == 2) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, val_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, val_black);
        glLightfv(GL_LIGHT0, GL_SPECULAR, val_black);
    } else if (lightMode == 3) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, val_black);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, val_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, val_black);
    } else if (lightMode == 4) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, val_black);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, val_black);
        glLightfv(GL_LIGHT0, GL_SPECULAR, val_specular);
    }

    GLfloat w_component = isDirectional ? 0.0f : 1.0f;
    GLfloat light_pos[] = {0.0f, 8.0f, 0.0f, w_component};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    if (isSpotlight) {
        GLfloat spotDir[] = {0.0f, -1.0f, 0.0f};
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotCutoff);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);
    } else {
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
    }

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linearAttenuation);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);

    if (isShiny) {
        GLfloat mat_spec[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    } else {
        GLfloat mat_spec[] = {0.0f, 0.0f, 0.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    }
}

// ---------------------------------- FUNGSI OBJEK (TUGAS)

// ---------------------------------- FUNGSI OBJEK (MINECRAFT STYLE)

// 1. Domba Minecraft (Pengganti Mobil/Manusia Salju)
void drawMinecraftSheep() {
    glPushMatrix();
    glTranslatef(-4.0f, 0.0f, -4.0f); // Posisi sudut kiri

    // --- BADAN (Putih) ---
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.9f, 0.0f); 
    glScalef(1.4f, 1.0f, 1.0f);     
    glutSolidCube(1.0);
    glPopMatrix();

    // --- KEPALA (Krem) ---
    glColor3f(0.9f, 0.8f, 0.7f);
    glPushMatrix();
    glTranslatef(-0.8f, 1.1f, 0.0f); 
    glScalef(0.6f, 0.6f, 0.6f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // --- KAKI (4 Buah) ---
    glColor3f(0.9f, 0.8f, 0.7f);
    float legX[] = { 0.4f, 0.4f, -0.4f, -0.4f };
    float legZ[] = { -0.3f, 0.3f, -0.3f, 0.3f };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(legX[i], 0.3f, legZ[i]); 
        glScalef(0.25f, 0.8f, 0.25f);         
        glutSolidCube(1.0);
        glPopMatrix();
    }
    glPopMatrix();
}

// 2. Pohon Minecraft (Pengganti Pohon Cemara Lama)
void drawMinecraftTree() {
    glPushMatrix();
    glTranslatef(4.0f, 0.0f, -4.0f); // Posisi sudut kanan

    // --- BATANG KAYU (Coklat) ---
    glColor3f(0.4f, 0.26f, 0.13f);
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glScalef(0.6f, 3.0f, 0.6f); // Batang tinggi kotak
    glutSolidCube(1.0);
    glPopMatrix();

    // --- DAUN LAYER 1 (Hijau Tua - Bawah) ---
    glColor3f(0.0f, 0.5f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, 0.0f); // Di atas batang
    glScalef(2.5f, 1.0f, 2.5f);     // Lebar
    glutSolidCube(1.0);
    glPopMatrix();

    // --- DAUN LAYER 2 (Hijau Muda - Atas) ---
    glColor3f(0.1f, 0.7f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 0.0f); // Di atas layer 1
    glScalef(1.5f, 1.2f, 1.5f);     // Lebih kecil
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

// 3. Tiang Lampu Minecraft (Pengganti Tiang Lama)
void drawMinecraftLamp() {
    glPushMatrix();
    glTranslatef(-4.0f, 0.0f, 4.0f); // Posisi sudut kiri depan

    // --- TIANG (Hitam/Abu Gelap) ---
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glScalef(0.3f, 4.0f, 0.3f); // Tiang kurus tinggi
    glutSolidCube(1.0);
    glPopMatrix();

    // --- EFEK CAHAYA (EMISSION) ---
    if (lightingEnabled) {
        // Membuat kubus lampu terlihat menyala sendiri
        GLfloat emission[] = {1.0f, 1.0f, 0.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
    }

    // --- LAMPU (Kuning Terang - Glowstone) ---
    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 4.3f, 0.0f); // Di puncak tiang
    glScalef(0.8f, 0.8f, 0.8f);     // Kotak lampu
    glutSolidCube(1.0);
    glPopMatrix();

    // Matikan efek cahaya agar objek lain tidak ikut bersinar
    if (lightingEnabled) {
        GLfloat no_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    }
    
    // --- TUTUP LAMPU (Opsional - Topi di atas lampu) ---
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 4.75f, 0.0f);
    glScalef(1.0f, 0.1f, 1.0f);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

void drawMinecraftWindmill() {
    glPushMatrix();
    glTranslatef(4.0f, 0.0f, 4.0f); // Posisi kincir di taman

    // --- 1. MENARA (Tetap sama, Abu-abu Batu) ---
    glColor3f(0.5f, 0.5f, 0.5f);
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glScalef(1.0f, 4.0f, 1.0f); 
    glutSolidCube(1.0f);
    glPopMatrix();

    // --- 2. LOGIKA ROTASI ---
    glTranslatef(0.0f, 3.5f, 0.8f); // Pindah ke titik pusat baling-baling
    
    // Animasi berputar
    glRotatef(angleRotation, 0.0f, 0.0f, 1.0f); 

    // Ukuran per kotak (pixel)
    float boxSize = 0.4f; 

    // --- 3. PUSAT (Kotak Biru) ---
    // Sesuai gambar, pusatnya berwarna biru
    glColor3f(0.2f, 0.4f, 0.8f); // Warna Biru Royal
    glutSolidCube(boxSize);

    // --- 4. LENGAN (Warna Coklat) ---
    // Sesuai gambar, lengan berwarna coklat tua
    glColor3f(0.5f, 0.25f, 0.0f); // Coklat Tua

    // Loop 4 kali untuk membuat 4 lengan (Atas, Kanan, Bawah, Kiri)
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        
        // Putar sudut pandang untuk setiap lengan (-90 derajat per iterasi)
        // i = 0 (Atas), i = 1 (Kanan), i = 2 (Bawah), i = 3 (Kiri)
        glRotatef(i * -90.0f, 0.0f, 0.0f, 1.0f);

        // -- MEMBANGUN 1 LENGAN (Pola Gambar) --

        // Kotak 1: Tepat di atas pusat
        glTranslatef(0.0f, boxSize, 0.0f); 
        glutSolidCube(boxSize);

        // Kotak 2: Di atas Kotak 1
        glTranslatef(0.0f, boxSize, 0.0f); 
        glutSolidCube(boxSize);

        // Kotak 3: Geser ke KANAN dari Kotak 2 (Membentuk siku)
        glTranslatef(boxSize, 0.0f, 0.0f); 
        glutSolidCube(boxSize);

        // Kotak 4: Di ATAS Kotak 3 (Ujung baling-baling)
        glTranslatef(0.0f, boxSize, 0.0f); 
        glutSolidCube(boxSize);

        glPopMatrix();
    }

    glPopMatrix();
}
void drawLightSource() {
    if (!lightingEnabled) return;

    glDisable(GL_LIGHTING);
    glPushMatrix();
    GLfloat lightPos[] = {0.0f, 8.0f, 0.0f}; 
    glTranslatef(lightPos[0], lightPos[1], lightPos[2]);

    if (isDirectional) glColor3f(1.0f, 1.0f, 0.0f);
    else glColor3f(1.0f, 1.0f, 1.0f);

    if (isSpotlight) glutWireSphere(0.2, 10, 10);
    else glutSolidSphere(0.15, 20, 20);

    glPopMatrix();
    if (lightingEnabled) glEnable(GL_LIGHTING);
}

void drawFloor() {
 glBegin(GL_QUADS);
 glColor3f(0.2f, 0.2f, 0.25f);
 glVertex3f(-10.0f, 0.0f, -10.0f);
 glVertex3f(-10.0f, 0.0f, 10.0f);
 glVertex3f(10.0f, 0.0f, 10.0f);
 glVertex3f(10.0f, 0.0f, -10.0f);
 glEnd();

 glDisable(GL_LIGHTING);
 glColor3f(0.3f, 0.3f, 0.35f);
 glBegin(GL_LINES);
 for (float i = -10.0f; i <= 10.0f; i += 1.0f) {
 glVertex3f(i, 0.01f, -10.0f); glVertex3f(i, 0.01f, 10.0f);
 glVertex3f(-10.0f, 0.01f, i); glVertex3f(10.0f, 0.01f, i);
 }
 glEnd();
 if (lightingEnabled) glEnable(GL_LIGHTING);
}
// ---------------------------------- fungsi kontrol kamera
void updateCamera() {
    float yawRad = cameraYaw * PI / 180.0f;
    float pitchRad = cameraPitch * PI / 180.0f;
    float forwardX = sin(yawRad) * cos(pitchRad);
    float forwardY = -sin(pitchRad);
    float forwardZ = -cos(yawRad) * cos(pitchRad);
    float rightX = sin(yawRad + PI/2);
    float rightZ = -cos(yawRad + PI/2);

    if (keys['w'] || keys['W']) {
        cameraPosX += forwardX * MOVE_SPEED;
        cameraPosY += forwardY * MOVE_SPEED;
        cameraPosZ += forwardZ * MOVE_SPEED;
    }
    if (keys['s'] || keys['S']) {
        cameraPosX -= forwardX * MOVE_SPEED;
        cameraPosY -= forwardY * MOVE_SPEED;
        cameraPosZ -= forwardZ * MOVE_SPEED;
    }
    if (keys['a'] || keys['A']) {
        cameraPosX -= rightX * MOVE_SPEED;
        cameraPosZ -= rightZ * MOVE_SPEED;
    }
    if (keys['d'] || keys['D']) {
        cameraPosX += rightX * MOVE_SPEED;
        cameraPosZ += rightZ * MOVE_SPEED;
    }
    if (keys[' ']) cameraPosY += MOVE_SPEED;
    if (keys[GLUT_KEY_CTRL_L] || keys[GLUT_KEY_CTRL_R]) cameraPosY -= MOVE_SPEED;
}

// ---------------------------------- fungsi utama GLUT
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)windowWidth / windowHeight, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float yawRad = cameraYaw * PI / 180.0f;
    float pitchRad = cameraPitch * PI / 180.0f;
    float lookX = cameraPosX + sin(yawRad) * cos(pitchRad);
    float lookY = cameraPosY - sin(pitchRad);
    float lookZ = cameraPosZ - cos(yawRad) * cos(pitchRad);

    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, lookX, lookY, lookZ, 0.0f, 1.0f, 0.0f);

    updateLightingLogic();

    // --- PANGGIL 4 OBJEK MINECRAFT ---
    drawMinecraftSheep();    // Objek 1
    drawMinecraftTree();     // Objek 2
    drawMinecraftLamp();     // Objek 3
    drawMinecraftWindmill(); // Objek 4 (Animasi)
    drawLightSource();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    keys[key] = true;
    if (key == 27) exit(0);
    if (key == 'l' || key == 'L') lightingEnabled = !lightingEnabled;

    if (key == '1') lightMode = 1;
    if (key == '2') lightMode = 2;
    if (key == '3') lightMode = 3;
    if (key == '4') lightMode = 4;
    
    if (key == 'y' || key == 'Y') isDirectional = !isDirectional;
    if (key == 'h' || key == 'H') isSpotlight = !isSpotlight;
    
    if (key == 'j' || key == 'J') {
        spotCutoff -= 2.0f;
        if (spotCutoff < 0) spotCutoff = 0;
    }
    if (key == 'k' || key == 'K') {
        spotCutoff += 2.0f;
        if (spotCutoff > 90) spotCutoff = 90;
    }
    if (key == 'n' || key == 'N') linearAttenuation += 0.05f;
    if (key == 'm' || key == 'M') {
        linearAttenuation -= 0.05f;
        if (linearAttenuation < 0) linearAttenuation = 0;
    }
    if (key == 'i' || key == 'I') isShiny = !isShiny;
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

void specialKey(int key, int x, int y) {
    if (key == GLUT_KEY_CTRL_L || key == GLUT_KEY_CTRL_R) {
        keys[key] = true;
    }
}

void specialKeyUp(int key, int x, int y) {
    if (key == GLUT_KEY_CTRL_L || key == GLUT_KEY_CTRL_R) {
        keys[key] = false;
    }
}

// [PERUBAHAN 2] Logic Mouse Motion diperbaiki (Manual Drag)
void mouseMotion(int x, int y) {
    if (!mouseActive) return;

    float deltaX = (x - lastMouseX) * MOUSE_SENSITIVITY * 50.0f;
    float deltaY = (y - lastMouseY) * MOUSE_SENSITIVITY * 50.0f;

    cameraYaw += deltaX;
    cameraPitch += deltaY;

    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;

    lastMouseX = x;
    lastMouseY = y;
}

// [PERUBAHAN 3] Logic Klik Mouse
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseActive = true;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            mouseActive = false;
        }
    }
}

void idle() {
    updateCamera();
    
    // Update Animasi
    angleRotation += 2.0f;
    if (angleRotation > 360.0f) angleRotation -= 360.0f;
    
    glutPostRedisplay();
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Proyek OpenGL: Taman Musim Dingin");
    
    // glutFullScreen(); // Opsional: Aktifkan jika ingin layar penuh

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    initLighting();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKey);
    glutSpecialUpFunc(specialKeyUp);
    
    // Fungsi Mouse untuk Dragging
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouse);
    
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}

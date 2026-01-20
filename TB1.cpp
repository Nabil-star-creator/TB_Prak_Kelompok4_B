// ---------------------------------- library
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

//---------------------------------- global variabel
const float PI = 3.14159265f;

const float MOVE_SPEED = 10.0f; 
const float MOUSE_SENSITIVITY = 0.01f;

float cameraPosX = 0.0f, cameraPosY = 3.0f, cameraPosZ = 10.0f;
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

// Variabel Animasi Global
float angleRotation = 0.0f; 

// Variabel Animasi Domba 
float sheepZ = -4.0f;       // Posisi awal Z domba
float sheepLegAngle = 0.0f; // Sudut ayunan kaki
bool sheepMovingForward = true; // Arah jalan

// Variabel Animasi Pohon
float treeScale = 1.0f; // Skala awal pohon

// ---------------------------------- fungsi pencahayaan
void initLighting() {
    glEnable(GL_DEPTH_TEST); // Agar benda depan menutupi benda belakang
    glEnable(GL_LIGHTING); // Menyalakan sistem pencahayaan
    glEnable(GL_LIGHT0); // Menyalakan lampu nomor 0
    
    // [PENTING] Tambahkan ini agar saat di-scale cahaya tetap bagus
    glEnable(GL_NORMALIZE); 

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

    // --- 1. DEFINISI WARNA ---
    GLfloat val_ambient[]  = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat val_diffuse[]  = {1.0f, 0.9f, 0.6f, 1.0f}; 
    GLfloat val_specular[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
    GLfloat val_black[]    = {0.0f, 0.0f, 0.0f, 1.0f};

    // --- 2. LOGIKA MODE ---
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

    // --- 3.  POSISI & TIPE CAHAYA ---
    GLfloat w_component = isDirectional ? 0.0f : 1.0f; 
    GLfloat light_pos[] = {-4.0f, 4.3f, 4.0f, w_component}; 
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    // --- 4. SPOTLIGHT ---
    if (isSpotlight) {
    	// Mengatur arah sorotan ke bawah (0, -1, 0)
        GLfloat spotDir[] = {0.0f, -1.0f, 0.0f}; 
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotCutoff); 
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);
    } else {
    	// Mengatur lebar sorotan
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
    }

    // --- 5. REDUP JARAK JAUH ---
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linearAttenuation); 
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01f);

    // --- 6. MATERIAL KILAU ---
    if (isShiny) {
        GLfloat mat_spec[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    } else {
        GLfloat mat_spec[] = {0.0f, 0.0f, 0.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    }
}

// Nabil Muhammad Ramadan (2406072) ---------------------------------- FUNGSI OBJEK

// 1. Domba Minecraft 
void drawMinecraftSheep() {
    glPushMatrix();
    
    // POSISI & ARAH JALAN
    glTranslatef(-2.0f, 0.0f, sheepZ); 
    if (sheepMovingForward) {
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f); 
    } else {
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    }

    // --- BADAN ---
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.9f, 0.0f); 
    glScalef(1.4f, 1.0f, 1.0f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // --- KEPALA ---
    glPushMatrix();
    glTranslatef(-0.8f, 1.1f + abs(sin(sheepZ * 5.0f) * 0.05f), 0.0f); 
    
    glPushMatrix(); 
    glColor3f(0.9f, 0.8f, 0.7f);
    glScalef(0.6f, 0.6f, 0.6f);        
    glutSolidCube(1.0);
    glPopMatrix(); 

    // MATA KANAN
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(-0.31f, 0.1f, -0.15f); 
    glScalef(0.05f, 0.15f, 0.15f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // MATA KIRI
    glPushMatrix();
    glTranslatef(-0.31f, 0.1f, 0.15f);  
    glScalef(0.05f, 0.15f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    // PUPIL KANAN
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-0.32f, 0.1f, -0.18f); 
    glScalef(0.05f, 0.08f, 0.08f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // PUPIL KIRI
    glPushMatrix();
    glTranslatef(-0.32f, 0.1f, 0.18f);
    glScalef(0.05f, 0.08f, 0.08f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPopMatrix(); // Akhir Kepala

    // --- KAKI ---
    glColor3f(0.9f, 0.8f, 0.7f);
    float legX[] = { 0.4f, 0.4f, -0.4f, -0.4f };
    float legZ[] = { -0.3f, 0.3f, -0.3f, 0.3f };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(legX[i], 0.6f, legZ[i]); 
        if (i == 0 || i == 3) glRotatef(sheepLegAngle, 0.0f, 0.0f, 1.0f); 
        else glRotatef(-sheepLegAngle, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, -0.3f, 0.0f); 
        glScalef(0.25f, 0.8f, 0.25f);           
        glutSolidCube(1.0);
        glPopMatrix();
    }
    glPopMatrix();
}

void drawMinecraftSheep2() {
    glPushMatrix();
    
    // POSISI & ARAH JALAN
    glTranslatef(-6.0f, 0.0f, sheepZ); 
    if (sheepMovingForward) {
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f); 
    } else {
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    }

    // --- BADAN ---
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.9f, 0.0f); 
    glScalef(1.4f, 1.0f, 1.0f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // --- KEPALA ---
    glPushMatrix();
    glTranslatef(-0.8f, 1.1f + abs(sin(sheepZ * 5.0f) * 0.05f), 0.0f); 
    
    glPushMatrix(); 
    glColor3f(0.9f, 0.8f, 0.7f);
    glScalef(0.6f, 0.6f, 0.6f);        
    glutSolidCube(1.0);
    glPopMatrix(); 

    // MATA KANAN
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(-0.31f, 0.1f, -0.15f); 
    glScalef(0.05f, 0.15f, 0.15f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // MATA KIRI
    glPushMatrix();
    glTranslatef(-0.31f, 0.1f, 0.15f);  
    glScalef(0.05f, 0.15f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    // PUPIL KANAN
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-0.32f, 0.1f, -0.18f); 
    glScalef(0.05f, 0.08f, 0.08f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // PUPIL KIRI
    glPushMatrix();
    glTranslatef(-0.32f, 0.1f, 0.18f);
    glScalef(0.05f, 0.08f, 0.08f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPopMatrix(); // Akhir Kepala

    // --- KAKI ---
    glColor3f(0.9f, 0.8f, 0.7f);
    float legX[] = { 0.4f, 0.4f, -0.4f, -0.4f };
    float legZ[] = { -0.3f, 0.3f, -0.3f, 0.3f };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(legX[i], 0.6f, legZ[i]); 
        if (i == 0 || i == 3) glRotatef(sheepLegAngle, 0.0f, 0.0f, 1.0f); 
        else glRotatef(-sheepLegAngle, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, -0.3f, 0.0f); 
        glScalef(0.25f, 0.8f, 0.25f);           
        glutSolidCube(1.0);
        glPopMatrix();
    }
    glPopMatrix();
}

void drawMinecraftSheep3() {
    glPushMatrix();
    
    // POSISI & ARAH JALAN
    glTranslatef(6.0f, 0.0f, sheepZ); 
    if (sheepMovingForward) {
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f); 
    } else {
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    }

    // --- BADAN ---
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.9f, 0.0f); 
    glScalef(1.4f, 1.0f, 1.0f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // --- KEPALA ---
    glPushMatrix();
    glTranslatef(-0.8f, 1.1f + abs(sin(sheepZ * 5.0f) * 0.05f), 0.0f); 
    
    glPushMatrix(); 
    glColor3f(0.9f, 0.8f, 0.7f);
    glScalef(0.6f, 0.6f, 0.6f);        
    glutSolidCube(1.0);
    glPopMatrix(); 

    // MATA KANAN
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(-0.31f, 0.1f, -0.15f); 
    glScalef(0.05f, 0.15f, 0.15f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // MATA KIRI
    glPushMatrix();
    glTranslatef(-0.31f, 0.1f, 0.15f);  
    glScalef(0.05f, 0.15f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    // PUPIL KANAN
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-0.32f, 0.1f, -0.18f); 
    glScalef(0.05f, 0.08f, 0.08f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // PUPIL KIRI
    glPushMatrix();
    glTranslatef(-0.32f, 0.1f, 0.18f);
    glScalef(0.05f, 0.08f, 0.08f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPopMatrix(); // Akhir Kepala

    // --- KAKI ---
    glColor3f(0.9f, 0.8f, 0.7f);
    float legX[] = { 0.4f, 0.4f, -0.4f, -0.4f };
    float legZ[] = { -0.3f, 0.3f, -0.3f, 0.3f };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(legX[i], 0.6f, legZ[i]); 
        if (i == 0 || i == 3) glRotatef(sheepLegAngle, 0.0f, 0.0f, 1.0f); 
        else glRotatef(-sheepLegAngle, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, -0.3f, 0.0f); 
        glScalef(0.25f, 0.8f, 0.25f);           
        glutSolidCube(1.0);
        glPopMatrix();
    }
    glPopMatrix();
}

void drawMinecraftSheep6() {
    glPushMatrix();
    
    // POSISI & ARAH JALAN
    glTranslatef(2.0f, 0.0f, sheepZ); 
    if (sheepMovingForward) {
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f); 
    } else {
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    }

    // --- BADAN ---
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.9f, 0.0f); 
    glScalef(1.4f, 1.0f, 1.0f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // --- KEPALA ---
    glPushMatrix();
    glTranslatef(-0.8f, 1.1f + abs(sin(sheepZ * 5.0f) * 0.05f), 0.0f); 
    
    glPushMatrix(); 
    glColor3f(0.9f, 0.8f, 0.7f);
    glScalef(0.6f, 0.6f, 0.6f);        
    glutSolidCube(1.0);
    glPopMatrix(); 

    // MATA KANAN
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(-0.31f, 0.1f, -0.15f); 
    glScalef(0.05f, 0.15f, 0.15f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // MATA KIRI
    glPushMatrix();
    glTranslatef(-0.31f, 0.1f, 0.15f);  
    glScalef(0.05f, 0.15f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    // PUPIL KANAN
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-0.32f, 0.1f, -0.18f); 
    glScalef(0.05f, 0.08f, 0.08f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // PUPIL KIRI
    glPushMatrix();
    glTranslatef(-0.32f, 0.1f, 0.18f);
    glScalef(0.05f, 0.08f, 0.08f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPopMatrix(); // Akhir Kepala

    // --- KAKI ---
    glColor3f(0.9f, 0.8f, 0.7f);
    float legX[] = { 0.4f, 0.4f, -0.4f, -0.4f };
    float legZ[] = { -0.3f, 0.3f, -0.3f, 0.3f };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(legX[i], 0.6f, legZ[i]); 
        if (i == 0 || i == 3) glRotatef(sheepLegAngle, 0.0f, 0.0f, 1.0f); 
        else glRotatef(-sheepLegAngle, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, -0.3f, 0.0f); 
        glScalef(0.25f, 0.8f, 0.25f);           
        glutSolidCube(1.0);
        glPopMatrix();
    }
    glPopMatrix();
}

void drawMinecraftSheep4() {
    glPushMatrix();
    glTranslatef(sheepZ, 0.0f, 6.0f); 


    // Model dasar domba ini menghadap ke KIRI (Sumbu -X).
    
    if (sheepMovingForward) {
        // sheepMovingForward = true artinya nilai bertambah (Jalan ke Kanan)
        // Kita putar 180 derajat agar wajahnya menghadap Kanan
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f); 
    } else {
        // sheepMovingForward = false artinya nilai berkurang (Jalan ke Kiri)
        // Tidak perlu diputar (0 derajat) karena aslinya sudah menghadap Kiri
        glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    }

    // --- BADAN (SAMA) ---
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.9f, 0.0f); 
    glScalef(1.4f, 1.0f, 1.0f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // --- KEPALA (SAMA) ---
    glPushMatrix();
    glTranslatef(-0.8f, 1.1f + abs(sin(sheepZ * 5.0f) * 0.05f), 0.0f); 
    
    glPushMatrix(); 
    glColor3f(0.9f, 0.8f, 0.7f);
    glScalef(0.6f, 0.6f, 0.6f);        
    glutSolidCube(1.0);
    glPopMatrix(); 

    // MATA KANAN
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(-0.31f, 0.1f, -0.15f); 
    glScalef(0.05f, 0.15f, 0.15f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // MATA KIRI
    glPushMatrix();
    glTranslatef(-0.31f, 0.1f, 0.15f);  
    glScalef(0.05f, 0.15f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    // PUPIL KANAN
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-0.32f, 0.1f, -0.18f); 
    glScalef(0.05f, 0.08f, 0.08f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // PUPIL KIRI
    glPushMatrix();
    glTranslatef(-0.32f, 0.1f, 0.18f);
    glScalef(0.05f, 0.08f, 0.08f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPopMatrix(); // Akhir Kepala

    // --- KAKI (SAMA) ---
    glColor3f(0.9f, 0.8f, 0.7f);
    float legX[] = { 0.4f, 0.4f, -0.4f, -0.4f };
    float legZ[] = { -0.3f, 0.3f, -0.3f, 0.3f };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(legX[i], 0.6f, legZ[i]); 
        if (i == 0 || i == 3) glRotatef(sheepLegAngle, 0.0f, 0.0f, 1.0f); 
        else glRotatef(-sheepLegAngle, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, -0.3f, 0.0f); 
        glScalef(0.25f, 0.8f, 0.25f);           
        glutSolidCube(1.0);
        glPopMatrix();
    }
    glPopMatrix();
}

void drawMinecraftSheep5() {
    glPushMatrix();
    
    glTranslatef(sheepZ, 0.0f, 8.0f); 

    // Model dasar domba ini menghadap ke KIRI (Sumbu -X).
    
    if (sheepMovingForward) {
        // sheepMovingForward = true artinya nilai bertambah (Jalan ke Kanan)
        // Kita putar 180 derajat agar wajahnya menghadap Kanan
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f); 
    } else {
        // sheepMovingForward = false artinya nilai berkurang (Jalan ke Kiri)
        // Tidak perlu diputar (0 derajat) karena aslinya sudah menghadap Kiri
        glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    }

    // --- BADAN (SAMA) ---
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.9f, 0.0f); 
    glScalef(1.4f, 1.0f, 1.0f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // --- KEPALA (SAMA) ---
    glPushMatrix();
    glTranslatef(-0.8f, 1.1f + abs(sin(sheepZ * 5.0f) * 0.05f), 0.0f); 
    
    glPushMatrix(); 
    glColor3f(0.9f, 0.8f, 0.7f);
    glScalef(0.6f, 0.6f, 0.6f);        
    glutSolidCube(1.0);
    glPopMatrix(); 

    // MATA KANAN
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(-0.31f, 0.1f, -0.15f); 
    glScalef(0.05f, 0.15f, 0.15f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // MATA KIRI
    glPushMatrix();
    glTranslatef(-0.31f, 0.1f, 0.15f);  
    glScalef(0.05f, 0.15f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    // PUPIL KANAN
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-0.32f, 0.1f, -0.18f); 
    glScalef(0.05f, 0.08f, 0.08f);      
    glutSolidCube(1.0);
    glPopMatrix();

    // PUPIL KIRI
    glPushMatrix();
    glTranslatef(-0.32f, 0.1f, 0.18f);
    glScalef(0.05f, 0.08f, 0.08f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPopMatrix(); // Akhir Kepala

    // --- KAKI (SAMA) ---
    glColor3f(0.9f, 0.8f, 0.7f);
    float legX[] = { 0.4f, 0.4f, -0.4f, -0.4f };
    float legZ[] = { -0.3f, 0.3f, -0.3f, 0.3f };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(legX[i], 0.6f, legZ[i]); 
        if (i == 0 || i == 3) glRotatef(sheepLegAngle, 0.0f, 0.0f, 1.0f); 
        else glRotatef(-sheepLegAngle, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, -0.3f, 0.0f); 
        glScalef(0.25f, 0.8f, 0.25f);           
        glutSolidCube(1.0);
        glPopMatrix();
    }
    glPopMatrix();
}


// 2. Pohon Minecraft............... Neng isan lopiani (2406179)
void drawMinecraftTree() {
    glPushMatrix();
    glTranslatef(4.0f, 0.0f, -4.0f); 

    // SCALING POHON
     

    // Batang
    glColor3f(0.4f, 0.26f, 0.13f);
    glPushMatrix();
    glScalef(treeScale, treeScale, treeScale);
    glTranslatef(0.0f, 1.5f, 0.0f);
    glScalef(0.6f, 3.0f, 0.6f); 
    glutSolidCube(1.0);
    glPopMatrix();

    // Daun Bawah
    glColor3f(0.0f, 0.5f, 0.0f);
    glPushMatrix();
    glScalef(treeScale, treeScale, treeScale);
    glTranslatef(0.0f, 3.0f, 0.0f); 
    glScalef(2.5f, 1.0f, 2.5f);     
    glutSolidCube(1.0);
    glPopMatrix();
   

    // Daun Atas
    glColor3f(0.1f, 0.7f, 0.1f);
    glPushMatrix();
    glScalef(treeScale, treeScale, treeScale);
    glTranslatef(0.0f, 4.0f, 0.0f); 
    glScalef(1.5f, 1.2f, 1.5f);     
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

void drawMinecraftTree2() {
    glPushMatrix();
    glTranslatef(8.0f, 0.0f, -4.0f); 

    // SCALING POHON
    glScalef(treeScale, treeScale, treeScale); 

    // Batang
    glColor3f(0.4f, 0.26f, 0.13f);
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glScalef(0.6f, 3.0f, 0.6f); 
    glutSolidCube(1.0);
    glPopMatrix();

    // Daun Bawah
    glColor3f(0.0f, 0.5f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, 0.0f); 
    glScalef(2.5f, 1.0f, 2.5f);     
    glutSolidCube(1.0);
    glPopMatrix();

    // Daun Atas
    glColor3f(0.1f, 0.7f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 0.0f); 
    glScalef(1.5f, 1.2f, 1.5f);     
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

void drawMinecraftTree3() {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -4.0f); 

    // SCALING POHON
    glScalef(treeScale, treeScale, treeScale); 

    // Batang
    glColor3f(0.4f, 0.26f, 0.13f);
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glScalef(0.6f, 3.0f, 0.6f); 
    glutSolidCube(1.0);
    glPopMatrix();

    // Daun Bawah
    glColor3f(0.0f, 0.5f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, 0.0f); 
    glScalef(2.5f, 1.0f, 2.5f);     
    glutSolidCube(1.0);
    glPopMatrix();

    // Daun Atas
    glColor3f(0.1f, 0.7f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 0.0f); 
    glScalef(1.5f, 1.2f, 1.5f);     
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

void drawMinecraftTree4() {
    glPushMatrix();
    glTranslatef(-4.0f, 0.0f, -4.0f); 

    // SCALING POHON
    glScalef(treeScale, treeScale, treeScale); 

    // Batang
    glColor3f(0.4f, 0.26f, 0.13f);
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glScalef(0.6f, 3.0f, 0.6f); 
    glutSolidCube(1.0);
    glPopMatrix();

    // Daun Bawah
    glColor3f(0.0f, 0.5f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, 0.0f); 
    glScalef(2.5f, 1.0f, 2.5f);     
    glutSolidCube(1.0);
    glPopMatrix();

    // Daun Atas
    glColor3f(0.1f, 0.7f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 0.0f); 
    glScalef(1.5f, 1.2f, 1.5f);     
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

void drawMinecraftTree5() {
    glPushMatrix();
    glTranslatef(-8.0f, 0.0f, -4.0f); 

    // SCALING POHON
    glScalef(treeScale, treeScale, treeScale); 

    // Batang
    glColor3f(0.4f, 0.26f, 0.13f);
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glScalef(0.6f, 3.0f, 0.6f); 
    glutSolidCube(1.0);
    glPopMatrix();

    // Daun Bawah
    glColor3f(0.0f, 0.5f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, 0.0f); 
    glScalef(2.5f, 1.0f, 2.5f);     
    glutSolidCube(1.0);
    glPopMatrix();

    // Daun Atas
    glColor3f(0.1f, 0.7f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 0.0f); 
    glScalef(1.5f, 1.2f, 1.5f);     
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

// 3. Tiang Lampu ..............Rival arizal sutisna (2406064)
void drawMinecraftLamp() {
    glPushMatrix();
    glTranslatef(-8.0f, 0.0f, 4.0f); 

    // Tiang
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glScalef(0.3f, 4.0f, 0.3f); 
    glutSolidCube(1.0);
    glPopMatrix();

    // Efek Cahaya
    if (lightingEnabled) {
        GLfloat emission[] = {1.0f, 1.0f, 0.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
    }

    // Lampu
    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 4.3f, 0.0f); 
    glScalef(0.8f, 0.8f, 0.8f);     
    glutSolidCube(1.0);
    glPopMatrix();

    // Matikan Efek Cahaya
    if (lightingEnabled) {
        GLfloat no_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    }
    
    // Tutup Lampu
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 4.75f, 0.0f);
    glScalef(1.0f, 0.1f, 1.0f);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

// 4. Kincir Angin.................. Ayu patmawati (2406112)
void drawMinecraftWindmill() {
    glPushMatrix();
    glTranslatef(4.0f, 0.0f, 4.0f); 
    
    // Menara
    glColor3f(0.5f, 0.5f, 0.5f);
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glScalef(1.0f, 4.0f, 1.0f); 
    glutSolidCube(1.0f);
    glPopMatrix();

    // Rotasi Baling-baling
    glTranslatef(0.0f, 3.5f, 0.8f); 
    glRotatef(angleRotation, 0.0f, 0.0f, 1.0f); 

    float boxSize = 0.4f; 

    // Pusat Baling
    glColor3f(0.2f, 0.4f, 0.8f); 
    glutSolidCube(boxSize);

    // Lengan Baling
    glColor3f(0.5f, 0.25f, 0.0f); 

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glRotatef(i * -90.0f, 0.0f, 0.0f, 1.0f);

        glTranslatef(0.0f, boxSize, 0.0f); 
        glutSolidCube(boxSize);
        glTranslatef(0.0f, boxSize, 0.0f); 
        glutSolidCube(boxSize);
        glTranslatef(boxSize, 0.0f, 0.0f); 
        glutSolidCube(boxSize);
        glTranslatef(0.0f, boxSize, 0.0f); 
        glutSolidCube(boxSize);

        glPopMatrix();
    }
    glPopMatrix();
}
/////////////////////////////////////////////////////// Nabil
void drawFloor() {
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.6f, 0.2f);
    glNormal3f(0.0f, 1.0f, 0.0f); 
    glVertex3f(-10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, -10.0f);
    glEnd();
}

// ---------------------------------- fungsi kontrol/////////////////////////////////////////////////////// Isan
void updateCamera() {
    // 1. Mengubah sudut pandang (Yaw/Pitch) menjadi radian untuk perhitungan matematika
    float yawRad = cameraYaw * PI / 180.0f;
    float pitchRad = cameraPitch * PI / 180.0f;

    // 2. Menghitung vektor arah depan (Forward) dan samping (Right)
    float forwardX = sin(yawRad) * cos(pitchRad);
    float forwardY = -sin(pitchRad);
    float forwardZ = -cos(yawRad) * cos(pitchRad);

    float rightX = sin(yawRad + PI/2); // Arah kanan adalah 90 derajat dari arah pandang
    float rightZ = -cos(yawRad + PI/2);


    // 3. Mengubah posisi kamera berdasarkan tombol yang ditekan
    if (keys['w'] || keys['W']) { // Maju
        cameraPosX += forwardX * MOVE_SPEED;
        cameraPosY += forwardY * MOVE_SPEED;
        cameraPosZ += forwardZ * MOVE_SPEED;
    }
    if (keys['s'] || keys['S']) { // Mundur
        cameraPosX -= forwardX * MOVE_SPEED;
        cameraPosY -= forwardY * MOVE_SPEED;
        cameraPosZ -= forwardZ * MOVE_SPEED;
    }
    if (keys['a'] || keys['A']) { // Geser Kiri (Strafe Left)
        cameraPosX -= rightX * MOVE_SPEED;
        cameraPosZ -= rightZ * MOVE_SPEED;
    }
    if (keys['d'] || keys['D']) { // Geser Kanan (Strafe Right)
        cameraPosX += rightX * MOVE_SPEED;
        cameraPosZ += rightZ * MOVE_SPEED;
    }
    // Terbang naik/turun
    if (keys[' ']) cameraPosY += MOVE_SPEED; // Spasi untuk naik
    if (keys[GLUT_KEY_CTRL_L] || keys[GLUT_KEY_CTRL_R]) cameraPosY -= MOVE_SPEED; // Ctrl untuk turun
}

///////////////////////////////////////////////////////////////////////////// Nabil
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)windowWidth / windowHeight, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 1. Menghitung titik target (kemana mata melihat)
    float yawRad = cameraYaw * PI / 180.0f;
    float pitchRad = cameraPitch * PI / 180.0f;
    
    // Titik target = Posisi Kamera + Arah Vektor
    float lookX = cameraPosX + sin(yawRad) * cos(pitchRad);
    float lookY = cameraPosY - sin(pitchRad);
    float lookZ = cameraPosZ - cos(yawRad) * cos(pitchRad);

    // 2. Memberitahu OpenGL posisi dan arah kamera
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, // Posisi Mata (Eye)
              lookX, lookY, lookZ,                // Titik yang dilihat (Center/Target)
              0.0f, 1.0f, 0.0f);                  // Arah "Atas" (Up Vector) - Sumbu Y adalah atas

	updateLightingLogic();

    drawMinecraftSheep();  
	drawMinecraftSheep2();  
	drawMinecraftSheep3(); 
	drawMinecraftSheep4();
	drawMinecraftSheep5();
	drawMinecraftSheep6();
    drawMinecraftTree();
	drawMinecraftTree2();
	drawMinecraftTree3();  
	drawMinecraftTree4();
	drawMinecraftTree5();   
    drawMinecraftLamp();     
    drawMinecraftWindmill();
    drawFloor();

    glutSwapBuffers();
}
////////////////////////////////////////////////////////////////////////////////// Ayu
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
    
    // Kontrol Ukuran Pohon
    if (key == 'z' || key == 'Z') {
        treeScale += 0.1f;
        
        // [BATAS MAKSIMAL]
        // Jika ukuran lebih dari 3.0 (3x lipat), paksa tetap di 3.0
        if (treeScale > 3.0f) { 
            treeScale = 3.0f; 
        }
    }
    if (key == 'x' || key == 'X') {
        treeScale -= 0.1f;
        // [BATAS MINIMAL]
        // Jika ukuran kurang dari 0.2 (terlalu kecil), paksa tetap di 0.2
        if (treeScale < 0.2f) { 
            treeScale = 0.2f; 
        }
        if (treeScale < 0.1f) treeScale = 0.1f;
    }
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

//////////////////////////////////////////////////////////////////////////////////////// Isan

void mouseMotion(int x, int y) {
    if (!mouseActive) return; // Hanya jalan jika mouse diklik kiri

    // 1. Menghitung selisih posisi mouse sekarang dengan posisi sebelumnya
    float deltaX = (x - lastMouseX) * MOUSE_SENSITIVITY * 50.0f;
    float deltaY = (y - lastMouseY) * MOUSE_SENSITIVITY * 50.0f;

    // 2. Menambahkan selisih tersebut ke sudut kamera
    cameraYaw += deltaX;   // Gerak mouse horizontal mengubah Yaw (tengok kiri-kanan)
    cameraPitch += deltaY; // Gerak mouse vertikal mengubah Pitch (tengok atas-bawah)

    // 3. Membatasi agar tidak bisa menengok ke atas/bawah sampai terbalik (koprol)
    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;

    // 4. Simpan posisi mouse terakhir untuk perhitungan frame berikutnya
    lastMouseX = x;
    lastMouseY = y;
}


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
///////////////////////////////////////////// Nabil
void idle() {
    updateCamera();
    
    angleRotation += 2.0f;
    if (angleRotation > 360.0f) angleRotation -= 360.0f;
    
    float walkSpeed = 0.001f;
    
    if (sheepMovingForward) {
        sheepZ += walkSpeed;
        if (sheepZ > 4.0f) sheepMovingForward = false;
    } else {
        sheepZ -= walkSpeed;
        if (sheepZ < -4.0f) sheepMovingForward = true;
    }

    sheepLegAngle = sin(sheepZ * 5.0f) * 20.0f; 
    
    glutPostRedisplay();
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}
//////////////////////////////////////////////////////////////////////////////////////////////// 
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("kelompok 5");
    
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    initLighting();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKey);
    glutSpecialUpFunc(specialKeyUp);
    
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouse);
    
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}

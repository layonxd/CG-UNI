#pragma once
#include "ofMain.h"
#include "cg_extras.h"
#include "cg_drawing_extras.h"
#include "cg_cam_extras.h"
#include "cg_game_particle.h"

class Movement{
    public:

    Movement();
    ~Movement();
    void setup();
    void resetPhysics();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void setupGLWorld();

    float avatarSize;
    int state;
    int numStates;

    void linearMovement();
    ofVec3f avatarPos;
    ofVec3f avatarVelDir;
    float avatarVelMod;

    void acceleratedMovement();
    ofVec3f avatarAccelDir;
    float avatarAccelMod;

    void easingMovement();
    float easing;
    ofVec3f targetPos;

    void jump();
    float jumpForce;
    int jumpImpulseNumFrames;
    int jumpImpulseFrameCounter;
    float Fg;//gravity;
    float avatarMass;
    bool jumping;
    bool impulse;

    void bounce();
    float bounciness;

    void collide();
    ofVec3f enemyPos;
    float enemySize;
    int collideState;
    vector<Particle> explosion;
    ofVec3f shootPos;
    ofVec3f shootVel;
    float shootSize;

};
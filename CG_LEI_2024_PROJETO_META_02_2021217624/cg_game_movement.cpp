#include "cg_game_movement.h"

Movement::Movement()
{
    setup();
}

Movement::~Movement()
{
}

void Movement::setup()
{
    state = 0;
    numStates = 6;

    avatarSize = gw() * 0.1;

    resetPhysics();
}

void Movement::resetPhysics()
{
    avatarPos = ofVec3f(0., 0., 0.);
    avatarVelDir = ofVec3f(1., 0., 0.);
    avatarVelMod = 1.;

    avatarAccelDir = ofVec3f(1., 0, 0);
    avatarAccelMod = 0.1;

    targetPos = ofVec3f(gw() * 0.5 - avatarSize * 0.5, 0., 0.);
    easing = 0.1;

    impulse = false;
    jumping = false;
    jumpForce = 0.;
    jumpImpulseNumFrames = 5;
    jumpImpulseFrameCounter = 0;
    Fg = -1.5;
    avatarMass = 1.;

    bounciness = 0.95;

    enemyPos = ofVec3f(0, 0, 0);
    enemySize = gw() * 0.05;
    shootPos = avatarPos;
    shootVel = ofVec3f(10., 0., 0.);
    shootSize = gw() * 0.005;
    collideState = 0;
}

void Movement::setupGLWorld()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-gw() * 0.5, gw() * 0.5, -gh() * 0.5, gh() * 0.5, -10000, 10000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // lookat(0, 0, 1, 0, 0, 0, 0, 1, 0);
}

void Movement::update()
{
    if (state == 0)
    {
    }
    else if (state == 1)
    {
        linearMovement();
    }
    else if (state == 2)
    {
        acceleratedMovement();
    }
    else if (state == 3)
    {
        easingMovement();
    }
    else if (state == 4)
    {
        jump();
    }
    else if (state == 5)
    {
        bounce();
    }
    else if (state == 6){
        collide();
    }
}

void Movement::draw()
{

    // text
    switch (state)
    {
    case 0:
        ofSetColor(0, 0, 0);
        ofDrawBitmapString("Movement examples (0/6):\nuse up/down arrow keys to change movement", 20, 20);
        break;
    case 1:
        ofSetColor(0, 0, 0);
        ofDrawBitmapString("linear movement(1/6):\n +/- change speed\n|speed| = " + ofToString(avatarVelMod), 20, 20);
        break;
    case 2:
        ofSetColor(0, 0, 0);
        ofDrawBitmapString("Accelerated movement(2/6):\n +/- change accel\n|speed| = " + ofToString(avatarVelMod) + "\n|accel| = " + ofToString(avatarAccelMod), 20, 20);
        break;
    case 3:
        ofSetColor(0, 0, 0);
        ofDrawBitmapString("Easing movement(3/6):\n +/- change easing\n|easing| = " + ofToString(easing), 20, 20);
        break;
    case 4:
        ofSetColor(0, 0, 0);
        ofDrawBitmapString("Jump(4/6):\npress 'j' to jump\npress 'J' to jump higher", 20, 20);
        break;
    case 5:
        ofSetColor(0, 0, 0);
        ofDrawBitmapString("Bounce(5/6):\npress 'b' to bounce", 20, 20);
        break;
    case 6:
        ofSetColor(0, 0, 0);
        ofDrawBitmapString("AABB Collision detection(6/6)", 20, 20);
        break;
    }

    if (state == 0)
    {
    }
    else if (state > 0 && state <= 5)
    {
        setupGLWorld();

        glColor3f(0.8, 0.8, 0.8);
        filled();
        glPushMatrix();
        glTranslatef(avatarPos.x, avatarPos.y, avatarPos.z);
        glScalef(avatarSize, avatarSize, avatarSize);
        cube_unit();
        glPopMatrix();

        glColor3f(0., 0., 0.);
        wireframe();
        glPushMatrix();
        glTranslatef(avatarPos.x, avatarPos.y, avatarPos.z);
        glScalef(avatarSize, avatarSize, avatarSize);
        cube_unit();
        glPopMatrix();
    }
    else if(state == 6){
        setupGLWorld();

        glColor3f(0.8, 0.8, 0.8);
        filled();
        glPushMatrix();
        glTranslatef(avatarPos.x, avatarPos.y, avatarPos.z);
        glScalef(avatarSize, avatarSize, avatarSize);
        cube_unit();
        glPopMatrix();

        glColor3f(0., 0., 0.);
        wireframe();
        glPushMatrix();
        glTranslatef(avatarPos.x, avatarPos.y, avatarPos.z);
        glScalef(avatarSize, avatarSize, avatarSize);
        cube_unit();
        glPopMatrix();

        glColor3f(0.8, 0.8, 0.8);
        filled();
        glPushMatrix();
        glTranslatef(enemyPos.x, enemyPos.y, enemyPos.z);
        glScalef(enemySize, enemySize, enemySize);
        cube_unit();
        glPopMatrix();

        glColor3f(0., 0., 0.);
        wireframe();
        glPushMatrix();
        glTranslatef(enemyPos.x, enemyPos.y, enemyPos.z);
        glScalef(enemySize, enemySize, enemySize);
        cube_unit();
        glPopMatrix();

        glColor3f(1, 0, 0);
        filled();
        glPushMatrix();
        glTranslatef(shootPos.x, shootPos.y, shootPos.z);
        glScalef(shootSize, shootSize, shootSize);
        cube_unit();
        glPopMatrix();

        for(int i=0; i<explosion.size(); i++){
            explosion[i].draw();
        }
    }
}

void Movement::keyPressed(int key)
{
    switch (key)
    {
    case OF_KEY_UP:
        state++;
        resetPhysics();
        break;
    case OF_KEY_DOWN:
        state--;
        resetPhysics();
        break;
    case '+':
        avatarVelMod += 0.1;
        avatarAccelMod += 0.05;
        easing += 0.01;
        break;
    case '-':
        avatarVelMod -= 0.1;
        avatarAccelMod -= 0.05;
        easing -= 0.01;
    case 'j':
        if (!jumping)
        {
            impulse = true;
            jumping = true;
            jumpForce = 3.;
            jumpImpulseFrameCounter = 0;
        }

        break;
    case 'J':
        if (!jumping)
        {
            impulse = true;
            jumping = true;
            jumpForce = 6.;
            jumpImpulseFrameCounter = 0;
        }
        break;
    case 'b':
        if (state == 5)
        {
            avatarPos = ofVec3f(0., gh() * 0.5 - avatarSize * 0.5, 0.);
            avatarVelMod = 0.;
        }
        break;
    }

    if (state > numStates)
    {
        state = 0;
    }
    if (state < 0)
    {
        state = numStates;
    }

    if (avatarVelMod < 0.0)
    {
        avatarVelMod = 0.;
    }
    if (avatarAccelMod < 0.0)
    {
        avatarAccelMod = 0.0;
    }
    if (easing < 0.)
    {
        easing = 0.;
    }
    if (state == 4)
    {
        avatarPos.y = -gh() * 0.5 + avatarSize * 0.5;
    }
}

void Movement::keyReleased(int key)
{
}

void Movement::linearMovement()
{

    // make the cube move linearly
    // pos += speed
    avatarPos += avatarVelDir * avatarVelMod;

    // check collision with walls
    // test right wall
    if (avatarPos.x + avatarSize * 0.5 > gw() * 0.5)
    {
        avatarVelDir.x *= -1.;                       // invert speed direction
        avatarPos.x = gw() * 0.5 - avatarSize * 0.5; // certifies it is the limit
    }

    // test left wall
    if (avatarPos.x - avatarSize * 0.5 < -gw() * 0.5)
    {
        avatarVelDir.x *= -1.;                        // invert speed direction
        avatarPos.x = -gw() * 0.5 + avatarSize * 0.5; // certifies it is in the limit
    }
}

void Movement::acceleratedMovement()
{
    avatarVelDir = avatarAccelDir;
    avatarVelMod += avatarAccelMod;

    avatarPos += avatarVelDir * avatarVelMod;

    // check collision with walls
    // test right wall
    if (avatarPos.x + avatarSize * 0.5 > gw() * 0.5)
    {
        avatarVelMod = 0.;
        avatarAccelDir.x *= -1.;
        avatarPos.x = gw() * 0.5 - avatarSize * 0.5; // certifies it is the limit
    }

    // test left wall
    if (avatarPos.x - avatarSize * 0.5 < -gw() * 0.5)
    {
        avatarVelMod = 0.;
        avatarAccelDir.x *= -1.;
        avatarPos.x = -gw() * 0.5 + avatarSize * 0.5; // certifies it is in the limit
    }
}

void Movement::easingMovement()
{
    float d = targetPos.x - avatarPos.x;

    avatarPos.x += d * easing;

    if (avatarPos.squareDistance(targetPos) <= 1.)
    {
        avatarPos = targetPos;
        targetPos.x *= -1.;
    }
}

void Movement::jump()
{
    if (impulse)
    {
        if (jumpImpulseFrameCounter > jumpImpulseNumFrames)
        {
            impulse = false;
        }
        jumpImpulseFrameCounter++;
    }
    else
    {
        jumpForce = 0.;
    }

    if (jumping)
    {
        avatarAccelMod = (jumpForce + Fg) / float(avatarMass);
        avatarVelMod += avatarAccelMod;

        // testCollision with floor
        if ((avatarPos.y + avatarVelMod - avatarSize * 0.5) <= -gh() * 0.5)
        {
            avatarPos.y = -gh() * 0.5 + avatarSize * 0.5;
            avatarAccelMod = 0.;
            avatarVelMod = 0.;
            jumpForce = 0.;
            impulse = false;
            jumping = false;
        }
        else
        {
            avatarPos.y += avatarVelMod;
        }
    }
}

void Movement::bounce()
{
    avatarVelMod += Fg / avatarMass;
    avatarPos.y += avatarVelMod;

    if (avatarPos.y - avatarSize * 0.5 <= -gh() * 0.5)
    {
        avatarPos.y = -gh() * 0.5 + avatarSize * 0.5;
        avatarVelMod *= -bounciness;
    }
}

void Movement::collide()
{
    if (collideState == 0)
    {
        avatarPos.x = -gw() * 0.5 + avatarSize * 0.5;
        avatarPos.y = 0.;
        avatarPos.z = 0.;
        explosion.clear();
        shootPos = avatarPos;
        collideState = 1;

        enemyPos = ofVec3f(gw()*0.5 - enemySize*0.5, 0., 0.);
    }
    else if (collideState == 1)
    {
        shootPos += shootVel;

        // test Collision between shoot and enemy
        //only testing one side.
        //if we do not know the collision direction,
        //then we must test all sides
        bool touching = false;

        if (
            shootPos.x + shootSize * 0.5 >= enemyPos.x - enemySize * 0.5 &&
            shootPos.x + shootSize * 0.5 < enemyPos.x + enemySize * 0.5 - shootSize)
        {
            if (
                shootPos.y - shootSize * 0.5 <= enemyPos.y + enemySize * 0.5 &&
                shootPos.y - shootSize * 0.5 > enemyPos.y - enemySize * 0.5 - shootSize)
            {
                touching = true;
            }
        }
        if (touching)
        {
            collideState = 2;
            for (int i = 0; i < 300; i++)
            {
                Particle aux;
                aux.setup(gw() * 0.5 - enemySize * 0.5, gw() * 0.5 + enemySize * 0.5, enemySize * 0.5, -enemySize * 0.5);
                explosion.push_back(aux);
            }
        }
    }
    else if (collideState == 2)
    {
        for (int i = 0; i < explosion.size(); i++)
        {
            explosion[i].update();
            if(explosion[i].dead){
                explosion.erase(explosion.begin() + i);
            }
        }

        if(explosion.size() == 0){
            collideState = 0;
        }
    }
}

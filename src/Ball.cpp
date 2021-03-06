#include "Entities/Ball.h"

Ball::Ball() : isRespawn(false) ,m_entityType(Entities::Type::Ball)
{
    m_score = 0;
    m_flagsTaken = 0;
    m_lives = 3;
    m_speed = 0.4;
    m_jumpImpulse = 100;
    m_jumpState = JumpState::singleJump;
    //m_type = Ball::Type::small;
}


Ball::~Ball()
{
   // handle deletion
}

// getters
int Ball::getLives(){ return m_lives ;}


int Ball::getScore(){ return m_score ;}


int Ball::getFlagsTaken(){ return m_flagsTaken ;}


float Ball::getSpeed(){ return m_speed ;}


float Ball::getJumpImpulse(){ return m_jumpImpulse ;}


sf::Vector2f Ball::getSpawnPosition(){ return m_spawnPosition ;}


// setters
void Ball::setLives(int l_lives){ m_lives = l_lives ;}


void Ball::addScore(int l_score){ m_score += l_score ;}


void Ball::incrFlagsTaken(){ m_flagsTaken ++  ;}


void Ball::setSpeed(float l_speed){ m_speed = l_speed ;}


void Ball::setJumpImpulse(float l_jumpImpulse){ m_jumpImpulse = l_jumpImpulse ;}


void Ball::setSpawnPosition(sf::Vector2f l_spawnPosition){ m_spawnPosition = l_spawnPosition ;}


///////////////////////////////////////////////////////////////////////////////////////////////
void Ball::setBody(b2Body* l_body)
{
    m_body = l_body ;
    m_body->SetUserData(this);
    if(m_body != nullptr)
    {
        m_spawnPosition = tmx::BoxToSfVec(m_body->GetPosition());
        setBallSprite();
    }
    else
            throw "no b2Body* assigned to the 'ball' object";
}


void Ball::setTexture(const sf::Texture& l_texture)
{
    m_ballSprite.setTexture(l_texture);
}


void Ball::setBallSprite()
{
    b2CircleShape* cs = static_cast<b2CircleShape*>(m_body->GetFixtureList()->GetShape());
    const float radius = tmx::BoxToSfFloat(cs->m_radius);

    float size_x = m_ballSprite.getGlobalBounds().width , size_y = m_ballSprite.getGlobalBounds().height;
    m_ballSprite.setScale(2*radius/size_x , 2*radius/size_y);
    // for setting origin
    m_ballSprite.setOrigin(m_ballSprite.getLocalBounds().width/2 , m_ballSprite.getLocalBounds().height/2);
    m_ballSprite.setColor(sf::Color::Red);

    // just handle respawn sprite here for now
    m_lastRespawnSprite.setPosition(m_spawnPosition);
    m_lastRespawnSprite.setScale(3*radius/size_x , 3*radius/size_y);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Ball::jump()
{
    if(m_body != nullptr)
    {
        if(m_jumpState == JumpState::singleJump)
        {
            m_body->ApplyLinearImpulse(tmx::SfToBoxVec(sf::Vector2f(0,-1*m_jumpImpulse)) ,m_body->GetPosition() ,true );
            m_jumpState = JumpState::noJump;
        }
    }
    else
       throw "member 'm_body' of ball is null";
}


void Ball::moveLeft()
{
    if(m_body != nullptr)
        m_body->ApplyLinearImpulse(tmx::SfToBoxVec(sf::Vector2f(-1*m_speed,0)) ,m_body->GetPosition() ,true );
    else
        throw "member 'm_body' of ball is null";
}


void Ball::moveRight()
{
    if(m_body != nullptr)
        m_body->ApplyLinearImpulse(tmx::SfToBoxVec(sf::Vector2f(m_speed,0)) ,m_body->GetPosition() ,true );
    else
        throw "member 'm_body' of ball is null";
}


void Ball::respawn()
{
    if(m_body != nullptr)
    {
        b2Vec2 temp = tmx::SfToBoxVec(m_spawnPosition);
        m_body->SetTransform(temp , m_body->GetAngle());
    }
    else
        throw "member 'm_body' of ball is null";
}


void Ball::draw(sf::RenderWindow& window, sf::View& view)
{
    sf::Vector2f temp = tmx::BoxToSfVec(m_body->GetPosition());
    m_ballSprite.setPosition(temp);

    if(temp.x > view.getCenter().x + 450||
       temp.x < view.getCenter().x - 450)
       {
                view.setCenter(sf::Vector2f(temp.x,view.getCenter().y));
       }

     if(temp.y > view.getCenter().y + 350||
        temp.y < view.getCenter().y - 350 )
       {
                view.setCenter(sf::Vector2f(view.getCenter().x,temp.y));
       }

     window.setView(view);
     window.draw(m_ballSprite);
     window.draw(m_lastRespawnSprite);
}


void Ball::setSingleJumpState()
{
    m_jumpState = JumpState::singleJump;
}

#include "SpriteComponent.h"


SpriteComponent::SpriteComponent(std::string key)
{
    this->key = key;
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::init(ResourceManager* resourceManager)
{
    mResourceManager = resourceManager;

    mFrameDimensions = sf::Vector2u(0, 0);
    mRow = 0;
    mMinFrame = 0;
    mMaxFrame = 0;
    mCurrentFrame = 0;
    mFrameDuration = 0.5f;
    mAnimationPlaying = false;
    mCurrentIteration = 0;
    mMaxIterations = 0;
    mAnimationFunction = SpriteAnimationFunction::Forward;
    mBoomerangReverse = false;
}

void SpriteComponent::loadTexture(std::string filepath)
{
    mTexture = mResourceManager->getTexture(filepath);
    mSprite.setTexture(*mTexture);
    //mSprite.setOrigin(mSprite.getTextureRect().width / 2, mSprite.getTextureRect().height / 2);
}

sf::Vector2i SpriteComponent::getSize()
{
    return sf::Vector2i(mSprite.getTextureRect().width, mSprite.getTextureRect().height);
}

void SpriteComponent::setOrigin(sf::Vector2f origin)
{
    mSprite.setOrigin(origin);
}

sf::Vector2f SpriteComponent::getOrigin()
{
    return mSprite.getOrigin();
}

void SpriteComponent::update(Transform& globalTransform)
{
    updateAnimation();

    sf::Transform parentTransform;
    parentTransform.translate(globalTransform.position);
    parentTransform.rotate(globalTransform.rotation);

    sf::Transform childTransform;
    childTransform.translate(mLocalTransform.position);
    childTransform.rotate(mLocalTransform.rotation);

    mFinalTransform = parentTransform * childTransform;
}

void SpriteComponent::render(std::shared_ptr<sf::RenderWindow> window)
{
    window->draw(mSprite, mFinalTransform);

}

/*
    ANIMATIONS
*/ 

void SpriteComponent::setFrameDimensions(unsigned int width, unsigned int height)
{
    mFrameDimensions = sf::Vector2u(width, height);
    
    if (mMaxFrame == 0)
    {
        mMaxFrame = (mSprite.getTexture()->getSize().x / width) - 1;
    }

    setSpriteRect();
}

void SpriteComponent::setRow(int row)
{
    mRow = row;
    setSpriteRect();
}

void SpriteComponent::setFrame(int frame)
{
    mCurrentFrame = frame;
    setSpriteRect();
}

void SpriteComponent::setMinFrame(int minFrame)
{
    mMinFrame = minFrame;
}

void SpriteComponent::setMaxFrame(int maxFrame)
{
    mMaxFrame = maxFrame;
}

void SpriteComponent::setFrameDuration(float duration)
{
    mFrameDuration = duration;
}

void SpriteComponent::playAnimation()
{
    mFrameTimer.restart();
    mAnimationPlaying = true;
}

void SpriteComponent::pauseAnimation()
{
    mAnimationPlaying = false;
}

void SpriteComponent::setMaxAnimationIterations(int maxIterations)
{
    mMaxIterations = maxIterations;
}

void SpriteComponent::setAnimationFunction(SpriteAnimationFunction animationFunction)
{
    mAnimationFunction = animationFunction;

    if (animationFunction == SpriteAnimationFunction::Reverse)
    {
        mCurrentFrame = mMaxFrame;
    }
}

void SpriteComponent::updateAnimation()
{
    if (!mAnimationPlaying)
        return;

    if (mFrameDimensions.x == 0 && mFrameDimensions.y == 0)
        return;

    if (mFrameTimer.getElapsedTime().asSeconds() >= mFrameDuration)
    {
        // progress to next frame
        mFrameTimer.restart();

        if (mAnimationFunction == SpriteAnimationFunction::Forward)
        {
            // left to right
            if (mCurrentFrame >= mMaxFrame)
            {
                if (mMaxIterations > 0)
                {
                    mCurrentIteration++;

                    if (mCurrentIteration >= mMaxIterations)
                    {
                        mAnimationPlaying = false;
                        return;
                    }
                }

                mCurrentFrame = mMinFrame;
            }
            else
            {
                mCurrentFrame++;
            }
        }
        else if (mAnimationFunction == SpriteAnimationFunction::Reverse)
        {
            // right to left
            if (mCurrentFrame <= mMinFrame)
            {
                if (mMaxIterations > 0)
                {
                    mCurrentIteration++;

                    if (mCurrentIteration >= mMaxIterations)
                    {
                        mAnimationPlaying = false;
                        return;
                    }
                }

                mCurrentFrame = mMaxFrame;
            }
            else
            {
                mCurrentFrame--;
            }
        }
        else if (mAnimationFunction == SpriteAnimationFunction::Boomerang)
        {
            if (!mBoomerangReverse)
            {
                // left to right
                if (mCurrentFrame >= mMaxFrame)
                {
                    mBoomerangReverse = true;
                    mCurrentFrame = mMaxFrame - 1;
                }
                else
                {
                    mCurrentFrame++;
                }
            }
            else
            {
                // right to left
                if (mCurrentFrame <= mMinFrame)
                {
                    if (mMaxIterations > 0)
                    {
                        mCurrentIteration++;

                        if (mCurrentIteration >= mMaxIterations)
                        {
                            mAnimationPlaying = false;
                            return;
                        }
                    }

                    mBoomerangReverse = false;
                    mCurrentFrame = mMinFrame + 1;
                }
                else
                {
                    mCurrentFrame--;
                }
            }
        }
    }

    // finally, update the sprite based on the animation
    setSpriteRect();
}

void SpriteComponent::setSpriteRect()
{
    mSprite.setTextureRect(sf::IntRect(
        mCurrentFrame * mFrameDimensions.x,
        mRow * mFrameDimensions.y,
        mFrameDimensions.x,
        mFrameDimensions.y
    ));
}
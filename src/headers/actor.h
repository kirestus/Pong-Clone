#pragma once
enum class eActorType
{
    Bat,
    Ball,
    Score,
    None,
};

class Actor
{

public:
    Actor(eActorType eActorType);

    bool GetIsValid()const { return m_bIsValid ;}

private:
    bool m_bIsValid = true;
    eActorType m_eActorType= eActorType::None;

};
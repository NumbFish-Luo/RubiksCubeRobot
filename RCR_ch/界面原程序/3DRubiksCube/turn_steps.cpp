#include "turn_steps.h"

#include <iostream>
#include <fstream>
#include "cube3Dconfig.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

/**************************************** turn ****************************************/
turn::turn()
{

}

turn::turn(const string str)
{
    switch (str[0])
    {
    case 'U':
        forward.push_back(TURN_U);
        reverse.push_back(TURN_U_R);
        break;
    case 'D':
        forward.push_back(TURN_D);
        reverse.push_back(TURN_D_R);
        break;
    case 'L':
        forward.push_back(TURN_L);
        reverse.push_back(TURN_L_R);
        break;
    case 'R':
        forward.push_back(TURN_R);
        reverse.push_back(TURN_R_R);
        break;
    case 'F':
        forward.push_back(TURN_F);
        reverse.push_back(TURN_F_R);
        break;
    case 'B':
        forward.push_back(TURN_B);
        reverse.push_back(TURN_B_R);
        break;
    default:
        break;
    }
    if (str.size() == 2)
    {
        switch (str[1])
        {
        case '\'':
            std::swap(forward[0], reverse[0]);
            steplength = 1;
            break;
        case '2':
            forward.push_back(forward[0]);
            reverse.push_back(reverse[0]);
            steplength = 2;
            break;
        default:
            break;
        }
    }
    else
    {
        steplength = 1;
    }
}

turn::turn(const turn& t) : forward(t.forward), reverse(t.reverse)
{

}

turn& turn::operator= (const turn& t)
{
    if (this != &t)
    {
        forward = t.forward;
        reverse = t.reverse;
    }
    return *this;
}

turn::~turn()
{

}

string turn::key() const
{
    return forward;
}

string turn::key_R() const
{
    return reverse;
}

/**************************************** steps ****************************************/
steps::steps()
{

}

steps::steps(const string filePath)
{
    std::fstream fileStream{ filePath };
    if (!fileStream.eof())
    {
        char line[100];
        fileStream.getline(line, 100);
        stepsString = line;
    }

    readSteps(stepsString, true);
}

steps::steps(const steps &s) :
    stepsString(s.stepsString), turnSteps(s.turnSteps),
    keySteps(s.keySteps), keySteps_reverse(s.keySteps_reverse)
{

}

steps& steps::operator=(const steps &s)
{
    if (this != &s)
    {
        stepsString = s.stepsString;
        turnSteps = s.turnSteps;
        keySteps = s.keySteps;
        keySteps_reverse = s.keySteps_reverse;
    }
    return *this;
}

int& steps::operator[](size_t i)
{
    if (i >= keySteps.size())
    {
        cout << "Over range!" << endl;
    }
    return keySteps[i];
}

steps::~steps()
{

}

void steps::printSteps(bool translate) const
{
    cout << "stepsString: " << stepsString << endl
        << "    keySteps: ";
    for (auto &i : keySteps)
    {
        switch (i)
        {
        case TURN_U:
            cout << "U ";
            break;
        case TURN_D:
            cout << "D ";
            break;
        case TURN_L:
            cout << "L ";
            break;
        case TURN_R:
            cout << "R ";
            break;
        case TURN_F:
            cout << "F ";
            break;
        case TURN_B:
            cout << "B ";
            break;
        case TURN_U_R:
            cout << "U' ";
            break;
        case TURN_D_R:
            cout << "D' ";
            break;
        case TURN_L_R:
            cout << "L' ";
            break;
        case TURN_R_R:
            cout << "R' ";
            break;
        case TURN_F_R:
            cout << "F' ";
            break;
        case TURN_B_R:
            cout << "B' ";
        default:
            break;
        }
    }
    cout << endl;
}

int steps::getStep(size_t step) const
{
    if (step >= keySteps.size())
    {
        cout << "Error: over range!" << endl;
        return NULL;
    }
    return keySteps[step];
}

int steps::getStep_reverse(size_t step) const
{
    if (step >= keySteps_reverse.size())
    {
        cout << "Error: over range!" << endl;
        return NULL;
    }
    return keySteps_reverse[step];
}

size_t steps::size() const
{
    return keySteps.size();
}

void steps::readSteps(const string &str, bool clear)
{
    string tmpString;
    if (clear == true)
    {
        turnSteps.clear();
    }
    for (auto s : str)
    {
        if (s != ' ')
        {
            tmpString.push_back(s);
        }
        else
        {
            turn tmpTurn(tmpString);
            turnSteps.push_back(tmpTurn);
            tmpString.clear();
        }
    }
    for (auto &t : turnSteps)
    {
        keySteps.push_back(t.forward[0]);
        keySteps_reverse.push_back(t.reverse[0]);
        if (t.forward.size() == 2)
        {
            keySteps.push_back(t.forward[1]);
            keySteps_reverse.push_back(t.reverse[1]);
        }
    }
}

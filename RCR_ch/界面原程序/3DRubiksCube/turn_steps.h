#pragma once

#include <vector>
#include <string>

class turn;
class steps;

class turn // ���ݵ���ħ����תָ���ɰ���ָ��������䷴ָ�turn��steps��Ψһʹ����
{
    friend class steps;
public:
    turn(); // Ĭ�Ϲ��캯��
    turn(const std::string str); // ���캯��
    turn(const turn &t); // �������캯��
    turn& operator=(const turn &t); // ������ֵ���������
    ~turn(); // ��������

    std::string key()   const;
    std::string key_R() const;
private:
    std::string forward;
    std::string reverse;
    int steplength;
};

class steps
{
public:
    steps   ();
    steps   (const std::string filePath);
    steps   (const steps &s);
    steps&  operator=(const steps &s);
    int&    operator[](size_t i);
    ~steps  ();

    void    printSteps(bool translate = false)  const;
    int     getStep(size_t step)                const;
    int     getStep_reverse(size_t step)        const;
    size_t  size()                              const;
private:
    void readSteps(const std::string &str, bool clear = false);
    std::string         stepsString;
    std::vector<turn>   turnSteps;
    std::vector<int>    keySteps;
    std::vector<int>    keySteps_reverse;
};

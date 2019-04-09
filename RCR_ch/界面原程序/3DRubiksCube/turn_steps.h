#pragma once

#include <vector>
#include <string>

class turn;
class steps;

class turn // 根据单个魔方翻转指令翻译成按键指令，并保存其反指令，turn是steps的唯一使用者
{
    friend class steps;
public:
    turn(); // 默认构造函数
    turn(const std::string str); // 构造函数
    turn(const turn &t); // 拷贝构造函数
    turn& operator=(const turn &t); // 拷贝赋值运算符重载
    ~turn(); // 析构函数

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

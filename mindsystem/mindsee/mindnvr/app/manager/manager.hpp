#pragma once

class Manager
{
private:
    /* data */
    Manager(/* args */);
    ~Manager();

public:
    static Manager* GetInstance(){
        static Manager ins;
        return &ins;
    }

    void Run();
};



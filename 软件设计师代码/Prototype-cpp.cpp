/**
 * @file Prototype.cpp
 * @author ghost_him
 * @brief 软件设计师-2013上半年-原型模式
 * @version 0.1
 * @date 2022-10-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/*
    无法运行，仅填空
*/

#include <string>

using namespace std;

class Cloneable {
public:
    virtual Cloneable* clone() = 0;
};

class WorkExperience : public Cloneable {
    // 工作经历
private:
    string workDate;
    string company;
public:
    Cloneable* clone() {
        WorkExperience* obj = new WorkExperience();
        obj->workDate = this->workDate;
        obj->company = this->company;
        return obj;
    }
    // 其余省略
};

class Resume : public Cloneable {
private:
    string name;
    string sex;
    string age;
    WorkExperience* work;
    Resume(WorkExperience* work) {
        // 子类可以默认变为父类， 父类不可以默认变为子类，需要强行转换
        this->work = (WorkExperience *)work->clone();
    }
public:
    Resume(string name) {/* 实现略*/}
    void SetPersonalInfo(string sex, string age) {/* 实现略*/}
    void setWorkExperience(string WorkDate, string company) {/* 实现略*/}

    Cloneable* clone() {
        Resume* obj = new Resume(work);
        obj->name = this->name;
        obj->sex = this->sex;
        obj->age = this->age;
        return obj;
    }
};

int main() {
    Resume* a = new Resume("张三");
    a->SetPersonalInfo("男", "29");
    a->setWorkExperience("1998~2000", "XXX 公司");
    Resume *b = (Resume *)a->clone();
    a->setWorkExperience("2001~2006", "YYY 公司");
    return 0;
    
}



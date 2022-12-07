/**
 * @file Decorator.cpp
 * @author ghost_him
 * @brief 软件设计师-装饰器模式-2016下
 * @version 0.1
 * @date 2022-10-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
using namespace std;

class Invoice {
public:
    virtual void printInvoice() {
        cout << "this is the content of the invoice!" << endl;
    }
};

class Decorator : public Invoice {
    Invoice *ticket;
public:
    Decorator(Invoice *t) {ticket = t;}
    void printInvoice() {
        if (ticket != NULL) {
            ticket->printInvoice();
        }
    }
};

class HeadDecorator: public Decorator {
public:
    // 父类初始化
    HeadDecorator(Invoice *t):Decorator(t) {}
    void printInvoice() {
        cout << "this is the header of the invoice!" << endl;
        // 直接调用父类的函数
        Decorator::printInvoice();
    }
};

class FootDecorator : public Decorator {
    public:
    FootDecorator(Invoice *t):Decorator(t){}
    void printInvoice() {
        Decorator::printInvoice();
        cout << "this is the footnote of the invoice!" << endl;
    }
};

int main(void) {
    Invoice t;
    FootDecorator f(&t);
    HeadDecorator h(&f);
    h.printInvoice();
    cout << "---------------------" << endl;
    FootDecorator a(nullptr);
    HeadDecorator b(&a);
    b.printInvoice();
    return 0;
}

/**
 * @file State.cpp
 * 
 * @author ghost_him
 * @brief 软件设计师-设计模式-状态模式-2011下半年
 * @version 0.1
 * @date 2022-10-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>

using namespace std;

// 类的提前引用
class TissueMachine;

class State {
	public :
	virtual void insertQuarter() = 0;	// 投币
	virtual void ejectQuarter() = 0;	// 退币
	virtual void turnCrank() = 0;		// 按下“出纸巾”按钮
	virtual void dispense() = 0;		// 出纸巾
};

/*类SoldOutState, NoQuraterState*/

class SoldOutState : public State {
private:
	TissueMachine* tissueMachine;
public:
	void insertQuarter() {}
	void ejectQuarter() {}
	void turnCrank() {}	
	void dispense()	{}
};

class HasQuarterState : public State {
private:
	TissueMachine* tissueMachine;
public:
	void insertQuarter() {}
	void ejectQuarter();
	void turnCrank() {}	
	void dispense()	{}
};

class NoQuarterState : public State {
private:
	TissueMachine* tissueMachine;
public:
	void insertQuarter();
	void ejectQuarter() {}
	void turnCrank() {}	
	void dispense()	{}
};

class SoldState : public State {
private:
	TissueMachine* tissueMachine;
public:
	void insertQuarter() {}
	void ejectQuarter() {}
	void turnCrank() {}	
	void dispense();
};


class TissueMachine {
private:
	State * soldOutState, *noQuarterState, *hasQuarterState, *soldState,
	*state;
	int count;
public:
	TissueMachine(int numbers);
	void setState(State* state);
	State* getHasQuarterState();
	State* getNoQuraterState();
	State* getSoldState();
	State* getSoldOutState();
	int getCount();
	// 其余的代码省略
};

// 调用自身的函数来完成变换
void NoQuarterState::insertQuarter() {
	tissueMachine->setState(tissueMachine->getHasQuarterState());
}

void HasQuarterState::ejectQuarter() {
	tissueMachine->setState(tissueMachine->getNoQuraterState());
}

void SoldState::dispense() {
	if (tissueMachine->getCount() > 0) {
		tissueMachine->setState(tissueMachine->getSoldState());
	} else {
		tissueMachine->setState(tissueMachine->getSoldOutState());
	}
}


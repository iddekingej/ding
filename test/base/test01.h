#ifndef __TEST01_H_
#define __TEST01_H_
#include "../lib/testclass.h"
#include "../../base/linklist.h"
class TTestItem
{
private:
	int someValue;
public:
	inline int getSomeValue(){ return someValue;}
	TTestItem(int p_someValue){ someValue=p_someValue;}
};

class TTest01:public TTestClass
{
private:
	TLinkList<TTestItem> *list;
protected:
	void doRun();		
public:
	void test01();
	void test02();
	TTest01();
	virtual ~TTest01();
};

#endif

#pragma once
#include <functional>

class ScoepedGuard
{
	~ScoepedGuard()
	{
		if (!scopedGuard)
		{
			scopedGuard();
		}
	}

	void dismiss()
	{
		scopedGuard = {};
	}
	
	std::function<void()> scopedGuard;
};

template<typename Func>
class FixedScopeGuard
{
	bool exec_;
	Func func_;
	
public:

	explicit FixedScopeGuard(Func func)
		: exec_(true), func_(std::move(func))
	{
		
	}

	FixedScopeGuard(const FixedScopeGuard&) = delete;

	FixedScopeGuard& operator=(const FixedScopeGuard&) = delete;

	~FixedScopeGuard()
	{
		if(exec_)
			func_();
	}

	void dismiss()
	{
		exec_ = false;
	}
};

#define SCOPE_GUARD(X) FixedScopeGuard _guard_##__LINE__([&]X)

// std::function

class MyFuncBase
{
public:
	virtual ~MyFuncBase() {};
	virtual void operator() () const = 0;
};


template<typename Func>
class MyLambdaFunc: public MyFuncBase
{
	Func func;
public:
	MyLambdaFunc(Func func_): func(func_){}
	void operator()() const
	{
		func();
	}
};

class Func
{
	MyFuncBase* myFuncBase;

public:
	template<typename f>
	Func(f f_): myFuncBase(new MyLambdaFunc<f>(f_)){}
	void operator()()const
	{
		myFuncBase->operator()();
	}
};
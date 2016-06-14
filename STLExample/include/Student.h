// head descripte ,example
// Copyright (c) 2016 chen
// All rights reserved.
// desc : the object for running whole example
// version    :  0.0.1
// author     :  chen
// email      : cy_chance@hotmail.com
// createtime : 2016/6/14
#pragma once
#include "common.h"
using namespace std;
namespace chen {
	namespace study {
		namespace stl360 {

			class Step {
			public:
				Step() {}
				~Step() {}
				void Work();
			};

			// the interface for logic
			class ITask {
			public:
				ITask() {}
				virtual ~ITask() {}
				virtual void Run() = 0;
				virtual void AddStep(const Step* s) = 0;
			};


			class Task :public ITask {
			public:
				// 需要执行的任务对象
				// @name 任务名字
				// @repeate_times 重复执行多少次，默认是一直循环
				Task(const string& name,int repeate_times=-1);
				// 释放任务对象，并且释放任务中的内存
				~Task();
				virtual void Run();
				virtual void AddStep(const Step* s);
				// 重复执行次数
				int RepeateTimes() const{ return repeate_times_; }
				
				Task& operator--();
				Task& operator--(int);
			private:
				vector<const Step*> steps_;// 任务中所包含的步骤
				string name_;//任务的名字
				int repeate_times_ = -1;// 重复执行多少次
			};


			class IStudent {
			public:
				IStudent() {}
				virtual ~IStudent() {}
				virtual void Work() = 0;
				virtual void AddTask(const Task* s) = 0;
			};

			class Student :public IStudent {
			public:
				Student();
				~Student();
				virtual void Work();
				virtual void AddTask(const Task* s);
			private:
				vector<const Task*> tasks_;				
			};
		}
	}
}

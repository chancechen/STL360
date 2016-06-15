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
	namespace common {
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
			virtual void Run() const = 0;
			// ���������
			// @s �������
			virtual void AddStep(const Step* s) = 0;
		};


		class Task :public ITask {
		public:
			// ��Ҫִ�е��������
			// @name ��������
			// @repeate_times �ظ�ִ�ж��ٴΣ�Ĭ����һֱѭ��
			Task(const string& name, int repeate_times = -1);
			// �ͷ�������󣬲����ͷ������е��ڴ�
			~Task();
			virtual void Run() const;
			// ���������
			// @s �������
			virtual void AddStep(const Step* s);
			// �ظ�ִ�д���
			int RepeateTimes() const { return repeate_times_; }
			const string& Name() const { return name_; }
			// �����ظ�����
			Task& operator--();
			// �����ظ�����
			Task& operator--(int);
		private:
			vector<const Step*> steps_;// �������������Ĳ���
			string name_;//���������
			int repeate_times_ = -1;// �ظ�ִ�ж��ٴ�
		};


		class IStudent {
		public:
			IStudent() {}
			virtual ~IStudent() {}
			// ���������б��е�����
			//@ return true�����б�Ϊ�գ�false�����б�Ϊ��
			virtual bool Work() = 0;
			// ������������б���
			// @s �������
			virtual void AddTask(const Task* s) = 0;
		};

		class Student :public IStudent {
		public:
			Student();
			~Student();
			// ���������б��е�����
			//@ return true�����б�Ϊ�գ�false�����б�Ϊ��
			virtual bool Work();
			// ������������б���
			// @s �������
			virtual void AddTask(const Task* s);
		private:
			vector<const Task*> tasks_;
			map<string, const Task*> taskmap_;
		};
	}
}

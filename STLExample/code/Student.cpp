#include "..\include\Student.h"
namespace chen {
	namespace study {
		namespace stl360 {
			Student::Student(){}
			Student::~Student(){
				for (const auto&task : tasks_) {
					delete task;
				}
			}
			void Student::Work()	{
				for (auto it = tasks_.begin(); it != tasks_.end();) {
					(*it)--;
					if ((*it)->RepeateTimes() == 0) {
						it = tasks_.erase(it);
					}
					else {
						++it;
					}
				}
			}

			void Student::AddTask(const Task * t){
				if (t != nullptr) {
					tasks_.push_back(t);
				}
			}
			
			Task::Task(const string & name, int repeate_times):name_(name),repeate_times_(repeate_times){
				cout<<"success to create task=" << name_.c_str() << endl;
			}
			Task::~Task(){
				for (const auto s : steps_) {
					delete s;
				}
				cout << "success to free task="<<name_.c_str() << endl;
			}
			void Task::Run(){}
			void Task::AddStep(const Step *s){
				if (s != nullptr) {
					steps_.push_back(s);
				}
			}

			Task & Task::operator--() {
				if (repeate_times_ != -1)
					this->repeate_times_--;
				return *this;
			}

			Task & Task::operator--(int) {
				if (repeate_times_ != -1)
					this->repeate_times_--;
				return *this;
			}
			void Step::Work(){}
		}
	}
}
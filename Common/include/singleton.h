#pragma once
// Copyright (c) 2016 chen
// All rights reserved.
// desc : singleton base class
// version    :  0.0.1
// author     :  chen
// email      : cy_chance@hotmail.com
// createtime : 2016/6/15
namespace chen {
	namespace common {

		class NoCopyable {
		private:
			NoCopyable(const NoCopyable &);
			const NoCopyable & operator=(const NoCopyable &);
		protected:
			NoCopyable() {}
			virtual ~NoCopyable() {}
		};

		template<typename TYPE, typename PTYPE = TYPE>
		class Singleton : private NoCopyable {
		public:
			static PTYPE * Instance() {			
				if (is_destroyed_)
					return  nullptr;
				if (!object_ptr_) {
					if (!object_ptr_)
						object_ptr_ = new TYPE();
				}
				return object_ptr_;
			}
			static void Destroy() {
				if (object_ptr_) {
					delete (object_ptr_);
					object_ptr_ = NULL;
					is_destroyed_ = true;
				}
			}
		protected:
			static bool   is_destroyed_;
			static PTYPE*  object_ptr_;
		protected:
			Singleton() {}
			virtual ~Singleton() {}
		}; 
		template<typename TYPE, typename PTYPE> PTYPE *
		ManualSingleton<TYPE, PTYPE>::object_ptr_ = NULL;
		template<typename TYPE, typename PTYPE> bool
		ManualSingleton<TYPE, PTYPE>::is_destroyed_ = false;
	}
}

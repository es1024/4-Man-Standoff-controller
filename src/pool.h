	
#ifndef POOL_H
#define POOL_H

#include <thread>
#include <deque>
#include <mutex>
#include <vector>
#include <algorithm>
#include <unistd.h>

template<class s, int n, int repeats>
class pool;
// Runs until pool is destroyed
template<class s, int n, int repeats>
void process(pool<s,n,repeats> *p){
	while(p->num_jobs() > 0){
		s *temp = p->pop();
		if(temp && access(temp->log_name(), F_OK) == -1){
			for(int i = 0; i < repeats; ++i)
				(*temp)(i);
		}
		delete temp;
	}
}

template<class s, int n, int repeats>
class pool{
	std::deque<s*> jobs;
	std::thread *threads[n];
	std::mutex mtx;
	volatile int num, idc;
		
	public:
		pool():num(0), idc(0){
			for(int i = 0; i < n; ++i){
				threads[i] = nullptr;
			}
		}
		~pool(){
			for(int i = 0; i < n; ++i){
				threads[i]->join(), delete threads[i];
			}
		}
		
		void shuffle(){
			std::shuffle(jobs.begin(), jobs.end(), std::default_random_engine(0xBAADF00D));
		}
		
		void init(){
			for(int i = 0; i < n; ++i){
				threads[i] = new std::thread(process<s,n,repeats>, this);
			}
		}
		
		int num_jobs() const { return num; }
		
		void push(s *job){
			mtx.lock();
			jobs.push_back(job);
			++num;
			//(*job)();
			//delete job;
			mtx.unlock();
		}
		void push(const std::vector<s*> &jobs){
			mtx.lock();
			num += jobs.size();
			for(s *j:jobs)
				this->jobs.push_back(j);
			mtx.unlock();
		}
		s *pop(){
			mtx.lock();
			s *tmp = nullptr;
			if(jobs.size()){
				tmp = jobs.front();
				tmp->id = idc++;
				--num;
				jobs.pop_front();
			}
			mtx.unlock();
			return tmp;
		}
};

#endif

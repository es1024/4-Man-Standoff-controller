
#ifndef POOL_H
#define POOL_H

#include <thread>
#include <queue>
#include <mutex>
#include <vector>

template<class s, int n, int repeats>
class pool;
// Runs until pool is destroyed
template<class s, int n, int repeats>
void process(pool<s,n,repeats> *p){
	while(p->num_jobs() > -1){
		s *temp = p->pop();
		if(temp){
			for(int i = 0; i < repeats; ++i)
				(*temp)(i);
		}
		delete temp;
	}
}

template<class s, int n, int repeats>
class pool{
	std::queue<s*> jobs;
	std::thread *threads[n];
	std::mutex mtx;
	volatile int num, idc;
		
	public:
		pool():num(0), idc(0){
			for(int i = 0; i < n; ++i){
				threads[i] = new std::thread(process<s,n,repeats>, this);
			}
		}
		~pool(){
			num = -1;
			for(int i = 0; i < n; ++i){
				threads[i]->join(), delete threads[i];
			}
			while(jobs.size() > 0) delete jobs.front(), jobs.pop();
		}
		
		int num_jobs() const { return num; }
		
		void push(s *job){
			mtx.lock();
			jobs.push(job);
			++num;
			//(*job)();
			//delete job;
			mtx.unlock();
		}
		void push(const std::vector<s*> &jobs){
			mtx.lock();
			num += jobs.size();
			for(s *j:jobs)
				this->jobs.push(j);
			mtx.unlock();
		}
		s *pop(){
			mtx.lock();
			s *tmp = nullptr;
			if(jobs.size()){
				tmp = jobs.front();
				tmp->id = idc++;
				--num;
				jobs.pop();
			}
			mtx.unlock();
			return tmp;
		}
};

#endif

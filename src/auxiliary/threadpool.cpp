/*
 * Copyright (c) 2012, seidel.florian <seidel.florian@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Technische Universitaet Muenchen nor the names
 *       of its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "threadpool.h"

ThreadPool::ThreadPool(int max_threads)
{
    m.lock();
    for(int i=0;i<max_threads;i++)
    {
        Thread *  thread = new Thread(*this);
        boost::thread * bThread = new boost::thread(boost::ref(*thread));
        thread->setBoostThread(bThread);
        free.push_back(thread);
    }
    m.unlock();
}

ThreadPool::~ThreadPool()
{
    m.lock();
    for(unsigned int i=0;i<free.size();i++)
    {
        delete free[i];
    }
    free.clear();
    m.unlock();
}

void ThreadPool::markAsFree(ThreadPool::Thread* thread)
{
    m.lock();
    free.push_back(thread);
    m.unlock();
}

bool ThreadPool::hasFreeThread()
{
    m.lock();
    bool has_free=!free.empty();
    m.unlock();
    return has_free;
}

bool ThreadPool::startWorker(Worker *worker){
    m.lock();
    bool ok=false;
    if(!free.empty())
    {
        Thread * t=free.back();
        free.pop_back();
        t->startWorker(worker);
        ok=true;
    }
    m.unlock();
    return ok;
}

ThreadPool::Thread::Thread(ThreadPool& threadPool):dont_stop(true),thread(NULL),worker(NULL),pool(threadPool)
{

}

ThreadPool::Thread::~Thread()
{
    if(thread!=NULL)
        delete thread;
}

void ThreadPool::Thread::operator ()()
{
    while(dont_stop)
    {
        while(worker==NULL)
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        }
        std::cerr<<"Running worker"<<std::endl;
//        try{
            worker->run();
//        }catch(...)
//        {
//            std::cerr<<"\n\n\n======================================================================="<<std::endl;
//            std::cerr<<"======================================================================="<<std::endl;
//            std::cerr<<"======================================================================="<<std::endl;
//            std::cerr<<"======================================================================="<<std::endl;
//            std::cerr<<"=================!!!!!Uncaught exception in worker thread!!!!!========="<<std::endl;
//            std::cerr<<"======================================================================="<<std::endl;
//            std::cerr<<"======================================================================="<<std::endl;
//            std::cerr<<"======================================================================="<<std::endl;
//            std::cerr<<"======================================================================="<<std::endl;
//            std::cerr<<"=======================================================================\n\n"<<std::endl;

//        }
        delete worker;
        worker=NULL;
        pool.markAsFree(this);
    }
}

void ThreadPool::Thread::startWorker(Worker *worker){
    std::cerr<<"Starting worker"<<std::endl;
    this->worker=worker;
}

void ThreadPool::joinAll()
{
    m.lock();
    for(unsigned int i=0;i<free.size();i++){
        free[i]->dont_stop=false;
        free[i]->thread->join();
    }
    m.unlock();
}

void ThreadPool::Thread::setBoostThread(boost::thread* thread)
{
    this->thread=thread;
}

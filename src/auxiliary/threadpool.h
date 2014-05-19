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

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <boost/thread.hpp>
#include <boost/thread/pthread/mutex.hpp>
#include <vector>
#include <cstdio>
#include <iostream>

/**
 * @brief Very simple thread pool class which allows to execute workers
 */
class ThreadPool
{
public:

    class Worker {
    public:
        virtual void run()=0;
        virtual ~Worker(){}
    };

    class Thread {
    public:
        Thread(ThreadPool& pool);
        ~Thread();
        void startWorker(Worker * worker);
        void setBoostThread(boost::thread * thread);
        void operator()();
        bool dont_stop;
        boost::thread* thread;
    private:
        Worker * worker;
        ThreadPool& pool;

    };

    ThreadPool(int max_threads);
    ~ThreadPool();

    /**
     * @brief hasFreeThread
     * @return true if there is a free thread which can execute a worker
     */
    bool hasFreeThread();

    /**
     * @brief startWorker calls workers run method and deletes worker after execution
     * @param worker
     * @return false if worker can't be executed
     */
    bool startWorker(Worker * worker);

    /**
     * @brief joinAll block until all workers have finished
     */
    void joinAll();

private:

    void markAsFree(Thread* thread);

    std::vector<Thread*> free;

    boost::mutex m;

};

#endif // THREADPOOL_H

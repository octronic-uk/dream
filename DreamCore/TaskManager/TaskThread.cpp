#include "TaskThread.h"
#include "Task.h"
#include "Common/Logger.h"

#include <algorithm>

using std::find;

namespace Dream
{
    const vector<Task*>& TaskThread::getDebugTaskQueue()
    {
        sort(mDebugTaskQueue.begin(),mDebugTaskQueue.end(),
             [&](Task* t1, Task* t2){return t1->getTaskId() > t2->getTaskId();});
        return mDebugTaskQueue;
    }

    TaskThread::TaskThread (int id)
        : mThread(thread(&TaskThread::executeTaskQueue,this)),
              mRunning(false),
              mFence(false),
              mThreadId(id)
        {
        }

        void TaskThread::join()
        {
            LOG_TRACE("TaskThread: Joining Thread {}",mThreadId);
            mThread.join();
        }

        void TaskThread::executeTaskQueue()
        {
            vector<Task*> completed;
            mRunning = true;
            while (mRunning)
            {
                if (mFence)
                {
                    std::this_thread::yield();
                    continue;
                }

                mDebugTaskQueue.clear();
                if(mTaskQueueMutex.try_lock())
                {
                    while (!mTaskQueue.empty())
                    {
                        LOG_TRACE("TaskThread: Worker {} has {} tasks",getThreadId(),mTaskQueue.size());
                        for (auto itr = mTaskQueue.begin(); itr != mTaskQueue.end(); itr++)
                        {
                            if (find(mDebugTaskQueue.begin(),mDebugTaskQueue.end(),(*itr)) == mDebugTaskQueue.end())
                            {
                                mDebugTaskQueue.push_back((*itr));
                            }
                            auto* t = (*itr);
                            // Check if ready to execute

                            if (t->getState() == TaskState::EXPIRED)
                            {
                               t->setState(TaskState::COMPLETED);
                            }
                            else if (t->isWaitingForDependencies())
                            {
                                t->incrementDeferralCount();
                            }
                            else
                            {
                                t->setState(TaskState::ACTIVE);
                                t->execute();
                            }

                            if (t->getState() == TaskState::COMPLETED)
                            {
                               completed.push_back(t);
                            }
                        }

                        for (auto* t : completed)
                        {
                            t->notifyDependents();
                            auto itr = find(mTaskQueue.begin(),mTaskQueue.end(), t);
                            if (itr != mTaskQueue.end())
                            {
                                mTaskQueue.erase(itr);
                            }
                        }
                        completed.clear();
                    }
                    mTaskQueueMutex.unlock();
                }

                if(mDestructionTaskQueueMutex.try_lock())
                {
                    vector< shared_ptr<DestructionTask> > completed;
                    for (auto itr = mDestructionTaskQueue.begin(); itr != mDestructionTaskQueue.end(); itr++)
                    {
                        auto& t = (*itr);
                        if (t->isWaitingForDependencies())
                        {
                            t->incrementDeferralCount();
                        }
                        else
                        {
                            t->setState(TaskState::ACTIVE);
                            t->execute();
                        }

                        if (t->getState() == TaskState::COMPLETED)
                        {
                           completed.push_back(t);
                        }
                    }

                    for (auto t : completed)
                    {
                        t->notifyDependents();
                        auto itr = find(mDestructionTaskQueue.begin(),mDestructionTaskQueue.end(), t);
                        if (itr != mDestructionTaskQueue.end())
                        {
                            mDestructionTaskQueue.erase(itr);
                        }
                    }
                    mDestructionTaskQueueMutex.unlock();
                }

                LOG_TRACE("TaskThread: Worker {} has finished running it's task queue, Setting Fence",getThreadId());
                mFence = true;
                std::this_thread::yield();
            }
            LOG_TRACE("TaskThread: ---------- Worker {} is ending it's task queue",getThreadId());
        }

        void TaskThread::clearFence()
        {
            assert(mFence);
            LOG_TRACE("TaskThread: Clearing fence for thread {}",getThreadId());
            mFence = false;
        }

        bool TaskThread::getFence()
        {
            return mFence;
        }

        bool TaskThread::pushTask(Task* t)
        {
            if(mTaskQueueMutex.try_lock())
            {
                assert(mFence);
                mTaskQueue.push_back(t);
                t->setThreadId(mThreadId);
                mTaskQueueMutex.unlock();
                return true;
            }
            return false;
        }

        bool TaskThread::pushDestructionTask(const shared_ptr<DestructionTask>& dt)
        {
            if(mDestructionTaskQueueMutex.try_lock())
            {
                assert(mFence);
                dt->setThreadId(mThreadId);
                mDestructionTaskQueue.push_back(dt);
                mDestructionTaskQueueMutex.unlock();
                return true;
            }
            return false;
        }

        void TaskThread::setRunning(bool running)
        {
            mRunning = running;
        }

        int TaskThread::getThreadId()
        {
            return mThreadId;
        }
}

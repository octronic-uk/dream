#include "TaskManagerWindow.h"
#include "DreamToolContext.h"
#include <DreamCore.h>

using octronic::dream::TaskThread;

namespace octronic::dream::tool
{
    TaskManagerWindow::TaskManagerWindow
    (DreamToolContext* proj) : ImGuiWidget(proj,false)
    {
    }

    TaskManagerWindow::~TaskManagerWindow
    ()
    {

    }

    void
    TaskManagerWindow::draw
    ()
    {
        ImGui::Begin("Task Manager",&mVisible);

        if (mContext->getProject())
        {
            auto projRunt = mContext->getProject()->getRuntime();
            if (projRunt)
            {
                auto* taskManager = projRunt->getTaskManager();
                const auto& threadVector = taskManager->getThreadVector();
                ImGui::Text("%lu Threads Running",threadVector.size());
                ImGui::Columns(static_cast<int>(threadVector.size()));
                for (TaskThread* t : threadVector)
                {
                    auto& debugQueue = t->getDebugTaskQueue();
                    ImGui::Text("Worker Thread %d (%3lu Tasks)", t->getThreadId(),debugQueue.size());
                    for (Task* task : debugQueue)
                    {
                        ImGui::Text("[%.3d/%.3d]",task->getTaskId(), task->getDeferralCount());
                    }
                    ImGui::NextColumn();
                }
                ImGui::Columns(1);
                ImGui::Separator();
                ImGui::Text("GraphicsComponent Tasks");
                auto* gfxComponent = projRunt->getGraphicsComponent();
                auto gfxTaskQueue = gfxComponent->getDebugTaskQueue();
                for (GraphicsComponentTask* task : gfxTaskQueue)
                {
                    ImGui::Text("[%.3d/%.3d]",task->getTaskId(), task->getDeferralCount());
                }
            }
            else
            {
                ImGui::Text("No Project Runtime");
            }
        }
        else
        {
                ImGui::Text("No Project");
        }
        ImGui::End();
    }
}

//
// Copyright (c) 2008-2022 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "AppStateManager.h"

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>

using namespace Urho3D;

class App : public Application
{
    URHO3D_OBJECT(App, Application);

public:
    App(Context* context)
        : Application(context)
    {
        // The first handler for the first event in each frame.
        // To prevent a crash, we can only change the current scene at the start of a frame,
        // before any scene events are processed
        SubscribeToEvent(E_BEGINFRAME, URHO3D_HANDLER(App, ApplyAppState));
    }

    void Setup() override
    {
        engineParameters_[EP_WINDOW_TITLE] = "Urho3D Benchmark";
        engineParameters_[EP_LOG_NAME] = GetSubsystem<FileSystem>()->GetAppPreferencesDir("urho3d", "logs") + "99_Benchmark.log";
        engineParameters_[EP_FULL_SCREEN] = false;
        engineParameters_[EP_WINDOW_WIDTH] = 960;
        engineParameters_[EP_WINDOW_HEIGHT] = 720;
        engineParameters_[EP_FRAME_LIMITER] = false;
    }

    // This elements can be used anywhere in the program
    void CreateCurrentFpsUiElement()
    {
        UIElement* root = GetSubsystem<UI>()->GetRoot();
        root->SetDefaultStyle(GetSubsystem<ResourceCache>()->GetResource<XMLFile>("UI/DefaultStyle.xml"));

        Text* fpsElement = root->CreateChild<Text>(CURRENT_FPS_STR);
        fpsElement->SetStyleAuto();
        fpsElement->SetTextEffect(TE_SHADOW);
        fpsElement->SetPosition(10, 10);
        fpsElement->SetText("FPS");
    }

    void Start() override
    {
        context_->RegisterSubsystem(new AppStateManager(context_));
        AppStateManager* appStateManager = context_->GetSubsystem<AppStateManager>();
        appStateManager->SetRequiredAppStateId(APPSTATEID_MAINSCREEN);

        GetSubsystem<Input>()->SetToggleFullscreen(false); // Block Alt+Enter

        CreateCurrentFpsUiElement();
    }

    void ApplyAppState(StringHash eventType, VariantMap& eventData)
    {
        context_->GetSubsystem<AppStateManager>()->Apply();
    }
};

URHO3D_DEFINE_APPLICATION_MAIN(App);

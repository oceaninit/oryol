//------------------------------------------------------------------------------
//  displayMgrBase.cc
//------------------------------------------------------------------------------
#include "Pre.h"
#include "displayMgrBase.h"
#include "Messaging/Port.h"
#include "Render/RenderProtocol.h"

namespace Oryol {
namespace Render {

using namespace Core;
using namespace Messaging;
    
//------------------------------------------------------------------------------
displayMgrBase::displayMgrBase() :
displayValid(false) {
    // empty
}

//------------------------------------------------------------------------------
displayMgrBase::~displayMgrBase() {
    o_assert(!this->displayValid);
}

//------------------------------------------------------------------------------
/**
 The SetupDisplay() method is expected to initialize everything necessary
 for rendering (e.g. creating the application window, create a GL context,...).
 It takes a DisplaySetup object which contains setup parameters like
 width, height, pixelformat, window title, etc...
 This method must be overwritten in a platform-specific subclass.
*/
void
displayMgrBase::SetupDisplay(const DisplaySetup& setup) {
    o_assert(!this->displayValid);
    
    this->displayValid = true;
    this->displaySetup = setup;
    this->displayAttrs = setup.GetDisplayAttrs();
}

//------------------------------------------------------------------------------
void
displayMgrBase::DiscardDisplay() {
    o_assert(this->displayValid);
    this->notifyEventHandlers(RenderProtocol::DisplayDiscarded::Create());
    this->displayValid = false;
}

//------------------------------------------------------------------------------
bool
displayMgrBase::IsDisplayValid() const {
    return this->displayValid;
}

//------------------------------------------------------------------------------
void
displayMgrBase::ModifyDisplay(const DisplaySetup& setup) {
    o_assert(this->displayValid);
    this->displayAttrs = setup.GetDisplayAttrs();
    this->notifyEventHandlers(RenderProtocol::DisplayModified::Create());
}

//------------------------------------------------------------------------------
/**
 This method is expected to process the platform specific window system 
 messages. This is also usally the place where input events from the
 host system are consumed and forwarded to the Oryol input system. The method
 is usually called at the start of a new frame, and must be overwritten 
 by platform-specific subclasses.
*/
void
displayMgrBase::ProcessSystemEvents() {
    // empty
}

//------------------------------------------------------------------------------
/**
 This method will be called when all rendering commands for the current
 frames has been issued and the result should be presented. Must be
 overwritten in platform-specific subclass.
*/
void
displayMgrBase::Present() {
    // empty
}

//------------------------------------------------------------------------------
const DisplaySetup&
displayMgrBase::GetDisplaySetup() const {
    return this->displaySetup;
}

//------------------------------------------------------------------------------
const DisplayAttrs&
displayMgrBase::GetDisplayAttrs() const {
    return this->displayAttrs;
}

//------------------------------------------------------------------------------
void
displayMgrBase::AttachDisplayEventHandler(const Ptr<Port>& handler) {
    o_assert(InvalidIndex == this->handlers.FindIndexLinear(handler));
    this->handlers.AddBack(handler);
}

//------------------------------------------------------------------------------
void
displayMgrBase::DetachDisplayEventHandler(const Ptr<Port>& handler) {
    int32 index = this->handlers.FindIndexLinear(handler);
    if (InvalidIndex != index) {
        this->handlers.Erase(index);
    }
}

//------------------------------------------------------------------------------
void
displayMgrBase::notifyEventHandlers(const Ptr<Message>& msg) {
    for (const auto& handler : this->handlers) {
        handler->Put(msg);
    }
}

} // namespace Render
} // namespace Oryol
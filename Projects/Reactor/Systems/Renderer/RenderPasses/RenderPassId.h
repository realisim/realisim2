#pragma once

namespace Realisim
{
namespace Reactor
{
    enum RenderPassId { rpiUndefined = 0,
        rpiPreDepth, rpiOpaque, rpiGlow, rpiCompositing, rpiScreenBlit, rpiPicking,
        rpiContourHighlight,
        rpiUserDefined = 10000 };

}
}


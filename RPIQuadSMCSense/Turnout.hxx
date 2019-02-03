/**
 */

#ifndef _TURNOUT_HXX_
#define _TURNOUT_HXX_

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"

/// CDI Configuration for a @ref ConfiguredConsumer.
CDI_GROUP(TurnoutConfig);
/// Allows the user to assign a name for this output.
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<8>, //
                Name("Description"), Description("User name of this output."));
/// Specifies the event ID to set the output to ON.
CDI_GROUP_ENTRY(
    normal, openlcb::EventConfigEntry, //
    Name("Normal"),
    Description("Receiving this event ID will align the turnout to normal."));
/// Specifies the event ID to set the output to OFF.
CDI_GROUP_ENTRY(
    reversed, openlcb::EventConfigEntry, //
    Name("Reversed"),
    Description("Receiving this event ID will align the turnout to reversed."));
CDI_GROUP_END();

/// OpenLCB Consumer class integrating a simple CDI-based configuration for two
/// event IDs, and an output GPIO object that will be turned on or off
/// depending on the incoming event notifications. This is usually the most
/// important object for a simple IO node.
class Turnout : public ConfigUpdateListener
{
public:
    using Impl = openlcb::GPIOBit;

    Turnout(openlcb::Node *node, const TurnoutConfig &cfg, const Gpio *gpio)
        : impl_(node, 0, 0, gpio)
        , consumer_(&impl_)
        , cfg_(cfg)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }

    template <class HW>
    Turnout(openlcb::Node *node, const TurnoutConfig &cfg, const HW &, const Gpio* g = HW::instance())
        : impl_(node, 0, 0, g)
        , consumer_(&impl_)
        , cfg_(cfg)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }

    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE
    {
        AutoNotify n(done);
        openlcb::EventId cfg_event_on = cfg_.normal().read(fd);
        openlcb::EventId cfg_event_off = cfg_.reversed().read(fd);
        if (cfg_event_off != impl_.event_off() ||
            cfg_event_on != impl_.event_on())
        {
            auto saved_gpio = impl_.gpio_;
            auto saved_node = impl_.node();
            // Need to reinitialize the consumer. We do this with in-place
            // destruction and construction.
            consumer_.~BitEventConsumer();
            impl_.Impl::~Impl();
            new (&impl_)
                Impl(saved_node, cfg_event_on, cfg_event_off, saved_gpio);
            new (&consumer_) openlcb::BitEventConsumer(&impl_);
            return REINIT_NEEDED; // Causes events identify.
        }
        return UPDATED;
    }

    void factory_reset(int fd) OVERRIDE
    {
        cfg_.description().write(fd, "");
    }

private:
    Impl impl_;
    openlcb::BitEventConsumer consumer_;
    const TurnoutConfig cfg_;
};


#endif // _TURNOUT_HXX_
